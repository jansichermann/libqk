// Copyright 2013 George King.
// Permission to use this file is granted in libqk/license.txt.


#import "qk-macros.h"


typedef CGFloat (^BlockRowHeight)(NSIndexPath*, id); // takes an index path and row; returns a height.
typedef NSString* (^BlockRowIdentifier)(NSIndexPath*, id); // takes and index path and row; returns cell reuse identifier.
// NOTE: cell is untyped so that block literals can specify whatever type is appropriate for each case.
typedef void (^BlockConfigureCell)(id, NSIndexPath*, id); // takes a cell, index path and row; configures cell.
typedef NSIndexPath* (^BlockWillSelect)(NSIndexPath*, id); // takes an index path and a row; returns index path to select or nil.
typedef void (^BlockRowDo)(NSIndexPath*, id); // takes an index path and row; performs an action.

@interface QKUITableView : UITableView <UITableViewDataSource, UITableViewDelegate>

@property (nonatomic) NSArray* rows;
@property (nonatomic, copy) BlockRowHeight blockRowHeight;
@property (nonatomic, copy) BlockRowIdentifier blockRowIdentifier;
@property (nonatomic, copy) BlockConfigureCell blockConfigureCell;
@property (nonatomic, copy) BlockWillSelect blockWillSelect;
@property (nonatomic, copy) BlockRowDo blockDidSelect; // BlockRowDo


DEC_INIT(Frame:(CGRect)frame rows:(NSArray *)rows cellTypes:(id)cellTypes);

- (void)setRows:(NSArray *)rows reload:(BOOL)reload;
- (id)rowAtIndexPath:(NSIndexPath*)indexPath;
- (UITableViewCell*)selectedCell;
- (void)deselectAnimated:(BOOL)animated;

@end