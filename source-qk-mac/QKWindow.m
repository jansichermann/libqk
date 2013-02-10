// Copyright 2013 George King.
// Permission to use this file is granted in oropendula/license.txt.


#import "QKWindow.h"


@interface QKWindow ()

@property (nonatomic) CGRect normalFrame;
@property (nonatomic) NSUInteger normalStyleMask;
@property (nonatomic) NSInteger normalLevel;
@property (nonatomic) BOOL normalOpaque;
@property (nonatomic) BOOL normalHidesOnDeactivate;
@property (nonatomic) NSButton* screenButton;
@property (nonatomic) NSTimer* timer;

@end


@implementation QKWindow

@dynamic delegate; // redeclaration of super property.


- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}


- (NSString*)description {
  return [NSString withFormat:@"<%@ %p: %@>", self.class, self, NSStringFromCGRect(self.frame)];
}


- (NSView*)contentView {
  return [super contentView];
}


DEF_INIT(View:(NSView *)view
         delegate:(id<QKWindowDelegate>)delegate
         styleMask:(NSUInteger)styleMask
         screenMode:(QKWindowScreenMode)screenMode
         position:(CGPoint)position
         activate:(BOOL)activate) {
  
  if (view.width < 1 || view.height < 1) {
    errFL(@"WARNING: view is degenerate: %@", view);
  }
  INIT(super initWithContentRect:view.frame styleMask:styleMask backing:NSBackingStoreBuffered defer:YES);
  
  _screenMode = screenMode;
  self.contentView = view;
  self.delegate = delegate;
  self.backgroundColor = [NSColor blueColor]; // for debugging blank windows
  self.releasedWhenClosed = NO;
  
  if (_screenMode) {
    [self addScreenButton];
  }
  
  if (activate) {
    [self makeKeyAndOrderFront:nil];
    [self makeMainWindow]; // must come after makeKeyAndOrderFront
  }
  else {
    [self orderBack:nil];
  }
  self.originFromVisibleTopLeft = position;

  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(screenDidChange:)
                                               name:NSWindowDidChangeScreenNotification
                                             object:self];
  
  [self screenDidChange:nil]; // retina resolution factor is not recognized on first frame, so this is necessary.
  return self;
}


DEF_INIT(View:(NSView*)view
         delegate:(id<QKWindowDelegate>)delegate
         closeable:(BOOL)closeable
         miniaturizable:(BOOL)miniaturizable
         resizable:(BOOL)resizable
         screenMode:(BOOL)screenMode
         position:(CGPoint)position
         activate:(BOOL)activate) {
  
  NSUInteger styleMask =
  NSTitledWindowMask
  | (closeable ? NSClosableWindowMask : 0)
  | (miniaturizable ? NSMiniaturizableWindowMask : 0)
  | (resizable ? NSResizableWindowMask : 0);
  
  return [self initWithView:view
                   delegate:delegate
                  styleMask:styleMask
                 screenMode:screenMode
                   position:position
                   activate:activate];
}


- (QKView*)qkView {
  return CAST(QKView, self.contentView);
}


- (void)setFrame:(CGRect)frame {
  [self setFrame:frame display:YES];
}


- (void)setOriginFromVisibleTopLeft:(CGPoint)origin {
  CGRect svf = self.screen.visibleFrame;
  CGFloat vsh = svf.origin.y + svf.size.height; // visible screen height accounts for possible menu bar
  CGFloat wh = self.frame.size.height;
  self.frameOrigin = CGPointMake(origin.x, vsh - (origin.y + wh));
}


// since the title bar gets removed on cover screen, we must add the button on init and every time we uncover.
- (void)addScreenButton {
  // setup cover screen button
  NSView* windowView = self.contentView.superview;
  CGRect wf = windowView.frame;
  NSImage* image = [NSImage imageNamed:NSImageNameEnterFullScreenTemplate];
  CGSize s = image.size;
  CGRect frame = CGRectMake(wf.size.width - s.width - 3, wf.size.height - s.height - 3, s.width, s.height);
  
  if (!_screenButton) {
    _screenButton =  [[NSButton alloc] initWithFrame:frame];
    _screenButton.target = self;
    _screenButton.action = @selector(toggleCoversScreen);
    _screenButton.autoresizingMask = NSViewMinXMargin | NSViewMinYMargin;
    _screenButton.image = image;
    _screenButton.bordered = NO;
    // TODO: fix white in highlight state.
  }
  
  [windowView addSubview:_screenButton];
}


- (void)setCoversScreen:(BOOL)coversScreen {
  if (_coversScreen == coversScreen) return;
  
  _coversScreen = coversScreen;
  
  if (coversScreen) {
    self.normalFrame = self.frame;
    self.normalStyleMask = self.styleMask;
    self.normalLevel = self.level;
    self.normalOpaque = self.isOpaque;
    self.normalHidesOnDeactivate = self.hidesOnDeactivate;
    
    self.styleMask = NSBorderlessWindowMask; // set mask before changing frame
    self.frame = self.screen.frame;
    self.level = NSMainMenuWindowLevel + 1; // set the window to sit above the menu bar
    self.opaque = YES;
    self.hidesOnDeactivate = YES;
    
    [self makeKeyAndOrderFront:nil];
  }
  else { // return to normal
    self.styleMask = self.normalStyleMask; // set mask before changing frame
    self.frame = self.normalFrame;
    self.level = self.normalLevel;
    self.opaque = self.normalOpaque;
    self.hidesOnDeactivate = self.normalHidesOnDeactivate;
    [self addScreenButton];
  }
  [DEL_RESPONDS(windowChangedCoversScreen:) windowChangedCoversScreen:self];
}


- (void)toggleCoversScreen {
  self.coversScreen = !self.coversScreen;
}


- (void)keyUp:(NSEvent *)event {
  if (_coversScreen && [event keyCode] == 53) { // escape key
    [self toggleCoversScreen];
  }
  [DEL_RESPONDS(keyUp:) keyUp:event];
}



- (void)screenDidChange:(NSNotification *)notification {
  errFL(@"window: %p screenDidChange:; scale: %f", self, self.screen.backingScaleFactor);
  self.contentView.layer.contentsScale = self.screen.backingScaleFactor;
  [DEL_RESPONDS(windowChangedScreen:) windowChangedScreen:self];
}


- (void)setContentSizeConstrainingAspect:(CGSize)size {
  self.contentSize = size;
  self.contentAspectRatio = size;
}

@end
