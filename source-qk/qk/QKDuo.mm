// Copyright 2013 George King.
// Permission to use this file is granted in libqk/license.txt.


#import "QKDuo.h"


@implementation QKDuo

+ (QKDuo*)a:(id)a b:(id)b {
  QKDuo* d = [self new];
  d.a = a;
  d.b = b;
  return d;
}

@end
