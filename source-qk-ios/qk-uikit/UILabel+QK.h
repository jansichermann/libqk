// Copyright 2013 George King.
// Permission to use this file is granted in libqk/license.txt.


#import "NSUIView.h"


@interface UILabel (QK)

+ (id)withFont:(UIFont*)font lines:(int)lines x:(CGFloat)x y:(CGFloat)y width:(CGFloat)width flex:(UIFlex)flex;
+ (id)withFontSize:(CGFloat)fontSize lines:(int)lines x:(CGFloat)x y:(CGFloat)y width:(CGFloat)width flex:(UIFlex)flex;
+ (id)withBoldFontSize:(CGFloat)fontSize lines:(int)lines x:(CGFloat)x y:(CGFloat)y width:(CGFloat)width flex:(UIFlex)flex;

@end

