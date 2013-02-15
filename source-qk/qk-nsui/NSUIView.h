// Copyright 2013 George King.
// Permission to use this file is granted in libqk/license.txt.


#import "qk-macros.h"


#if TARGET_OS_IPHONE
# define NSUIView UIView
#else
# define NSUIView NSView
typedef Int UIViewAutoresizing;
#endif


extern const UIViewAutoresizing UIFlexNone;
extern const UIViewAutoresizing UIFlexWidth;
extern const UIViewAutoresizing UIFlexHeight;
extern const UIViewAutoresizing UIFlexLeft;
extern const UIViewAutoresizing UIFlexRight;
extern const UIViewAutoresizing UIFlexTop;
extern const UIViewAutoresizing UIFlexBottom;

extern const UIViewAutoresizing UIFlexSize;
extern const UIViewAutoresizing UIFlexHorizontal;
extern const UIViewAutoresizing UIFlexVertical;


@interface NSUIView (NSUI)


@property (nonatomic) CGPoint origin;
@property (nonatomic) CGSize size;
@property (nonatomic) CGFloat x;
@property (nonatomic) CGFloat y;
@property (nonatomic) CGFloat width;
@property (nonatomic) CGFloat height;
@property (nonatomic) CGFloat centerX;
@property (nonatomic) CGFloat centerY;
@property (nonatomic) CGPoint boundsCenter;


+ (id)withFrame:(CGRect)frame;

DEC_INIT(Frame:(CGRect)frame flex:(UIViewAutoresizing)flex);
DEC_INIT(FlexFrame:(CGRect)frame);
DEC_INIT(FlexFrame);

- (void)inspect:(NSString*)label;
- (void)inspectParents:(NSString*)label;


@end

