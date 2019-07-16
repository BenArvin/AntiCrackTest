//
//  ViewController.m
//  AntiCrackTest
//
//  Created by BenArvin on 2019/7/16.
//  Copyright © 2019 BenArvin. All rights reserved.
//

#import "ViewController.h"
#import "AntiCrack.hpp"

@interface ViewController ()

@property (nonatomic) UILabel *titleLabel;
@property (nonatomic) UIActivityIndicatorView *indicatorView;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    if (!_titleLabel) {
        [self.view addSubview:self.titleLabel];
        [self.view addSubview:self.indicatorView];
        
        self.titleLabel.frame = CGRectMake(0, 200, self.view.bounds.size.width, 20);
        self.indicatorView.frame = CGRectMake(0, 0, 50, 50);
        self.indicatorView.center = CGPointMake(self.titleLabel.center.x, CGRectGetMaxY(self.titleLabel.frame) + 50);
        
        [self.indicatorView startAnimating];
        
//        antiDebug();
//        antiCrack();
    }
}

- (UILabel *)titleLabel {
    if (!_titleLabel) {
        _titleLabel = [[UILabel alloc] init];
        _titleLabel.textAlignment = NSTextAlignmentCenter;
        _titleLabel.font = [UIFont boldSystemFontOfSize:20];
        _titleLabel.textColor = [UIColor blackColor];
        _titleLabel.text = @"Hello world!";
    }
    return _titleLabel;
}

- (UIActivityIndicatorView *)indicatorView {
    if (!_indicatorView) {
        _indicatorView = [[UIActivityIndicatorView alloc] init];
        _indicatorView.activityIndicatorViewStyle = UIActivityIndicatorViewStyleGray;
    }
    return _indicatorView;
}

@end
