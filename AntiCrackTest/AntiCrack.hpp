//
//  AntiCrack.hpp
//  AntiCrackTest
//
//  Created by BenArvin on 2019/7/16.
//  Copyright © 2019 BenArvin. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <sys/sysctl.h>
#include <dispatch/dispatch.h>
#include <CoreFoundation/CFBundle.h>

char * CFStringToCharString(CFStringRef aString) {
    if (aString == NULL) {
        return NULL;
    }
    CFIndex length = CFStringGetLength(aString);
    CFIndex maxSize =
    CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
    char *buffer = (char *)malloc(maxSize);
    if (CFStringGetCString(aString, buffer, maxSize, kCFStringEncodingUTF8)) {
        return buffer;
    }
    free(buffer);
    return NULL;
}

static __attribute__((always_inline)) void asm_exit() {
#ifdef __arm64__
    __asm__("mov X0, #0\n"
            "mov w16, #1\n"
            "svc #0x80\n"
            
            "mov x1, #0\n"
            "mov sp, x1\n"
            "mov x29, x1\n"
            "mov x30, x1\n"
            "ret");
#endif
}

static int detectDebug_sysctl(void) __attribute__((always_inline));
int detectDebug_sysctl(void) {
    size_t size = sizeof(struct kinfo_proc);
    struct kinfo_proc info;
    int ret, name[4];
    
    memset(&info, 0, sizeof(struct kinfo_proc));
    
    name[0] = CTL_KERN;
    name[1] = KERN_PROC;
    name[2] = KERN_PROC_PID;
    name[3] = getpid();
    
#if 0
    if ((ret = (sysctl(name, 4, &info, &size, NULL, 0)))) {
        return ret; // sysctl() failed for some reason
    }
#else
    __asm__ volatile("mov x0, %[name_ptr]\n"
                     "mov x1, #4\n"
                     "mov x2, %[info_ptr]\n"
                     "mov x3, %[size_ptr]\n"
                     "mov x4, #0\n"
                     "mov x5, #0\n"
                     "mov w16, #202\n"
                     "svc #0x80"
                     :
                     : [name_ptr] "r"(name), [info_ptr] "r"(&info),
                     [size_ptr] "r"(&size));
#endif
    
    return (info.kp_proc.p_flag & P_TRACED) ? 1 : 0;
}

void antiDebug() {
    if (detectDebug_sysctl()) {
        asm_exit();
    }
}

static dispatch_source_t antiCrackTimer;
static CFStringRef rightBundleID;
void antiCrack() {
    if (!antiCrackTimer) {
        antiCrackTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0));
    }
    if (!rightBundleID) {
        rightBundleID = CFSTR("com.BenArvin.TestApp.VideoTest");
    }
    dispatch_source_set_timer(antiCrackTimer, DISPATCH_TIME_NOW, 300 * NSEC_PER_SEC, 0 * NSEC_PER_SEC);
    dispatch_source_set_event_handler(antiCrackTimer, ^{
        CFStringRef bundleID = CFBundleGetIdentifier(CFBundleGetMainBundle());
        if (CFStringCompare(bundleID, rightBundleID, 0) != 0) {
            asm_exit();
        }
    });
    dispatch_resume(antiCrackTimer);
}
