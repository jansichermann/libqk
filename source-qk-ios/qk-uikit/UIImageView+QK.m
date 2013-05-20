// Copyright 2013 George King.
// Permission to use this file is granted in libqk/license.txt.



#import "UIImage+QK.h"
#import "UIImageView+QK.h"


@implementation UIImageView (QK)

@dynamic highlighted;


+ (id)withImage:(UIImage *)image {
    return [[self alloc] initWithImage:image];
}


+ (id)withImageNamed:(NSString *)name {
    return [[self alloc] initWithImage:[UIImage imageNamed:name]];
}


@end
