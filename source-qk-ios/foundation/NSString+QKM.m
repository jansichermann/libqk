// Copyright 2013 George King.
// Permission to use this file is granted in libqk/license.txt.


#import "NSString+QKM.h"


@implementation NSString (QKM)


- (CGSize)sizeForFont:(UIFont*)font
                 size:(CGSize)size
            lineBreak:(UILineBreakMode)lineBreak
              lineMin:(int)lineMin {
  
  CGSize s = [self sizeWithFont:font constrainedToSize:size lineBreakMode:lineBreak];
  s.height = MAX(font.lineHeight * lineMin, s.height);
  return s;
}


- (CGFloat)heightForFont:(UIFont*)font
                       w:(CGFloat)w
                       h:(CGFloat)h
               lineBreak:(UILineBreakMode)lineBreak
                 lineMin:(int)lineMin {
  
  CGSize s = [self sizeWithFont:font constrainedToSize:CGSizeMake(w, h) lineBreakMode:lineBreak];
  return MAX(font.lineHeight * lineMin, s.height);
}


@end
