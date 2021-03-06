// Copyright 2013 George King.
// Permission to use this file is granted in libqk/license.txt.



#import "QKCGBitmapContext.h"

#define CTX ((CGContextRef)_ref)

@interface QKCGBitmapContext ()

@end


@implementation QKCGBitmapContext


- (id)initFormat:(QKPixFmt)format size:(V2I32)size {
  int bpc = QKPixFmtBitsPerChannel(format);
  int channels = QKPixFmtChannels(format);
  int bpr = (bpc / 8) * channels * size.v[0]; // bytes per row
  
  INIT(super initWithRetainedRef:
       CGBitmapContextCreate(NULL, // manage data internally
                             size.v[0], size.v[1],
                             bpc, bpr,
                             (CGColorSpaceRef)[[QKCGColorSpace withFormat:format] ref],
                             QKPixFmtBitmapInfo(format)));
  
  _size = size;
  _format = format;
  return self;
}


+ (id)withFormat:(QKPixFmt)format size:(V2I32)size {
  return [[self alloc] initFormat:format size:size];
}


- (const void*)bytes {
  return CGBitmapContextGetData(CTX);
}


- (void*)mutableBytes {
  return CGBitmapContextGetData(CTX);
}


- (Int)area {
  return _size.v[0] * _size.v[1];
}


- (Int)length {
  return QKPixFmtBytesPerPixel(_format) * self.area;
}



- (BOOL)isMutable {
  return YES;
}


#if TARGET_OS_IPHONE


+ (id)withFormat:(QKPixFmt)format image:(UIImage*)image flipY:(BOOL)flipY {
  QKCGBitmapContext* ctx = [self withFormat:format size:V2I32WithCGSize(image.size)];
  [ctx fillWithImage:image flipY:flipY];
  return ctx;
}


- (void)fillWithImage:(UIImage*)image flipY:(BOOL)flipY {
  if (flipY) {
    CGContextScaleCTM(CTX, 1, -1);
    CGContextTranslateCTM(CTX, 0, -_size.v[1]);
  }
  CGContextDrawImage(CTX, CGRectMake(0, 0, _size.v[0], _size.v[1]), image.CGImage);
}


#endif // TARGE_OS_IPHONE


// remove alpha bytes; we do this in place and require user to act appropriately.
// note that this is useful both for RGB and RGBA formats, since RGB formats still have space for the alpha channel.
- (QKPixFmt)exciseAlphaChannel {
  switch (_format) {
    case QKPixFmtRGBXU8:
    case QKPixFmtRGBAU8: {
      V4U8* from = (V4U8*)self.mutableBytes;
      V4U8* end = from + _size.v[0] * _size.v[1];
      V3U8* to = (V3U8*)from;
      while (from < end) {
        *to = *((V3U8*)from);
        to++;
        from++;
      }
    }
    case QKPixFmtRGBU8:
    return QKPixFmtRGBU8;
    default:
      qk_fail(@"unsupported format: 0x%X", _format);
  }
}


- (QKImage*)imageByExcisingAlphaChannel {
  QKPixFmt f = [self exciseAlphaChannel];
  QKSubData* d = [QKSubData withData:self offset:0 length:QKPixFmtBytesPerPixel(f) * self.area];
  return [QKImage withFormat:f size:_size data:d];
}


@end

