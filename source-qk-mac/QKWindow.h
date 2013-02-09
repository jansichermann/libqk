// Copyright 2013 George King.
// Permission to use this file is granted in libqk/license.txt.


#import "QKView.h"


typedef enum {
  QKWindowScreenModeNone = 0,
  QKWindowScreenModeCover, // cover a single screen
  QKWindowScreenModeFull, // use OS X full-screen mode, which disables other screens.
} QKWindowScreenMode;


@protocol QKWindowDelegate;


@interface QKWindow : NSWindow

@property (nonatomic) id<QKWindowDelegate> delegate;
@property (nonatomic, readonly) QKWindowScreenMode screenMode;
@property (nonatomic) BOOL coversScreen;

DEC_INIT(View:(NSView *)view
         delegate:(id<QKWindowDelegate>)delegate
         styleMask:(NSUInteger)styleMask
         screenMode:(QKWindowScreenMode)screenMode
         position:(CGPoint)position
         activate:(BOOL)activate);

DEC_INIT(View:(NSView*)view
         delegate:(id<QKWindowDelegate>)delegate
         closeable:(BOOL)closeable
         miniaturizable:(BOOL)miniaturizable
         resizable:(BOOL)resizable
         screenMode:(BOOL)screenMode
         position:(CGPoint)position
         activate:(BOOL)activate);

- (QKView*)qkView;

- (void)setOriginFromVisibleTopLeft:(CGPoint)origin;
- (void)toggleCoversScreen;
- (void)setContentSizeConstrainingAspect:(CGSize)size;

- (NSView*)contentView;

@end


@protocol QKWindowDelegate <NSWindowDelegate>
@optional

- (void)windowChangedCoversScreen:(QKWindow*)window;
- (void)windowChangedScreen:(QKWindow*)window;

@end
