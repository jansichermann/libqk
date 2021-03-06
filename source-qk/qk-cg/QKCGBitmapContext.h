// Copyright 2013 George King.
// Permission to use this file is granted in libqk/license.txt.


#import "qk-vec.h"
#import "QKData.h"
#import "QKSubData.h"
#import "QKImage.h"
#import "QKCGColorSpace.h"
#import "QKCGContext.h"


@interface QKCGBitmapContext : QKCGContext <QKData>

@property (nonatomic, readonly) QKPixFmt format;
@property (nonatomic, readonly) V2I32 size;



@property (nonatomic, readonly) QKSubData* subdata;

- (Int)area;
- (Int)length;
- (void*)mutableBytes;


+ (id)withFormat:(QKPixFmt)format size:(V2I32)size;

#if TARGET_OS_IPHONE
+ (id)withFormat:(QKPixFmt)format image:(UIImage*)image flipY:(BOOL)flipY;
- (void)fillWithImage:(UIImage*)image flipY:(BOOL)flipY;
#endif

// remove alpha bytes; we do this in place and require user to act appropriately.
// note that this is useful both for RGBX and RGBA formats.
- (QKPixFmt)exciseAlphaChannel;
- (QKImage*)imageByExcisingAlphaChannel;

@end

