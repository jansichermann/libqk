// Copyright George King 2013.
// Permission to use this file is granted in libqk/license.txt.


#import "sqlite3.h"
#import "qk-foundation.h"
#import "qk-basic.h"
#import "qk-sql-util.h"
#import "SqlDatabase.h"


@interface SqlDatabase ()
@end


@implementation SqlDatabase


- (void)dealloc {
  int code = sqlite3_close_v2(_handle);
  check(code == SQLITE_OK, @"SQLite database close failed: %@; %@", sql_code_description(code), _path);
}


- (id)initWithPath:(NSString*)path
         writeable:(BOOL)writeable
            create:(BOOL)create
             mutex:(BOOL)mutex
       sharedCache:(BOOL)sharedCache {
  
  INIT(super init);
  _path = path;
  int flags =
  (writeable ? SQLITE_OPEN_READWRITE : SQLITE_OPEN_READONLY) |
  (create ? SQLITE_OPEN_CREATE : 0) |
  (mutex ? SQLITE_OPEN_FULLMUTEX : SQLITE_OPEN_NOMUTEX) |
  (sharedCache ? SQLITE_OPEN_SHAREDCACHE :  SQLITE_OPEN_PRIVATECACHE);
  
  int code = sqlite3_open_v2(path.asUtf8, &_handle, flags, NULL);
  if (code != SQLITE_OK) {
    sqlite3_close(_handle);// close database even in the event of error to release all resources.
    fail(@"SQLite database open failed: %@; %@", sql_code_description(code), _path);
  }
  return self;
}


+ (id)withPath:(NSString*)path writeable:(BOOL)writeable create:(BOOL)create {
  // choose safe options by default.
  return [[self alloc] initWithPath:path writeable:writeable create:create mutex:YES sharedCache:NO];
}


+ (id)named:(NSString*)resourceName {
  return [self withPath:[[NSBundle mainBundle] pathForResource:resourceName ofType:@"sqlite3"]
              writeable:NO
                 create:NO];
}


- (SqlStatement*)prepare:(NSString*)string {
  return [[SqlStatement alloc] initWithDatabase:self string:string];
}


- (SqlStatement*)prepareInsert:(int)count table:(NSString*)table {
  // count is the number of columns
  NSMutableString* s = [NSMutableString withFormat:@"INSERT INTO %@ VALUES (NULL", table];
  for_imn(i, 1, count) {
    [s appendString:@", ?"];
  }
  [s appendString:@");"];
  return [self prepare:s];
}


- (SqlStatement*)prepareSelect:(NSString*)select table:(NSString*)table {
  return [self prepare:[NSString withFormat:@"SELECT %@ FROM %@;", select, table]];
}


@end

