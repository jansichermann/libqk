// Copyright 2012 George King.
// Permission to use this file is granted in libqk/license.txt.


#import "qk-types.h"
#import "qk-macros.h"
#import "qk-math.h"


// define vector types as structs that contain an element array named '_' of length 'dim'.
// this is preferable to named fields (e.g. 'x, y') because it makes the vector iterable;
// this makes both generic macro code and application code more regular.
// the naming convention is VDT, where D is the dimension (e.g. '2'), and T is the scalar element type, (e.g. 'F32').
// we make use of C compound literals, which look like type-casts of array literals (e.g, '(V2F32) { { 0, 0 } }').
// this has the advantage that unspecified fields are zero-initialized,
// which allows for generic conversions between dimensions.


#define _TYPEDEF_V(TE, TV, dim) \
\
typedef struct { TE _[dim]; } TV; \
\
static const TV TV##Zero = (TV) {{}}; \
\
\
static inline F64 TV##Aspect(TV v, TE eps) { \
return (v._[0] < eps || v._[1] < eps) ? 0 : (F64)v._[0] / (F64)v._[1]; \
} \
\
static inline TE TV##Measure(TV v) { TE m = 1; for_in(i, dim) m *= v._[i]; return m; } \
\
static inline TV TV##Neg(TV v) { TV r; for_in(i, dim) r._[i] = -v._[i]; return r; } \
\
static inline TV TV##Add(TV a, TV b) { TV r; for_in(i, dim) r._[i] = a._[i] + b._[i]; return r; } \
static inline TV TV##Sub(TV a, TV b) { TV r; for_in(i, dim) r._[i] = a._[i] - b._[i]; return r; } \
\
static inline TV TV##Mul(TV v, TE s) { TV r; for_in(i, dim) r._[i] = v._[i] * s; return r; } \
static inline TV TV##Div(TV v, TE s) { TV r; for_in(i, dim) r._[i] = v._[i] / s; return r; } \
\
static inline TV TV##MulVec(TV a, TV b) { TV r; for_in(i, dim) r._[i] = a._[i] * b._[i]; return r; } \
static inline TV TV##DivVec(TV a, TV b) { TV r; for_in(i, dim) r._[i] = a._[i] / b._[i]; return r; } \
\
static inline TV TV##Lerp(TV a, TV b, F64 t) { \
TV r; for_in(i, dim) r._[i] = lerp(a._[i], b._[i], t); return r; \
}\
\
static inline BOOL TV##LT(TV a, TV b) { \
for_in(i, dim) { if (a._[i] < b._[i]) return YES; if (a._[i] > b._[i]) return NO; } return NO; \
} \
\
static inline BOOL TV##LTRev(TV a, TV b) { \
for_in_rev(i, dim) { if (a._[i] < b._[i]) return YES; if (a._[i] > b._[i]) return NO; } return NO; \
} \
\
static inline TV TV##WithCGPoint(CGPoint p) { return (TV) {{ p.x, p.y }}; } \
\
static inline TV TV##WithCGSize(CGSize s) { return (TV) {{ s.width, s.height }}; } \
\
static inline CGPoint CGPointWith##TV(TV v) { return CGPointMake(v._[0], v._[1]); } \
\
static inline CGSize CGSizeWith##TV(TV v) { return CGSizeMake(v._[0], v._[1]); } \


#define _TYPEDEF_V2_EXP(TE, TV, fmt) \
_TYPEDEF_V(TE, TV, 2) \
\
static const TV TV##Unit = (TV) {{ 1, 1 }}; \
\
static inline TV TV##Make(TE x, TE y) { return (TV) {{ x, y }}; } \
\
static inline NSString* TV##Desc(TV v) { \
return [NSString stringWithFormat:@"(" fmt @" " fmt @")", v._[0], v._[1]]; \
} \
\
static inline TV TV##UpdateRange(TV r, TE v) { \
return (TV) { (v < r._[0] ? v : r._[0]), (v > r._[1] ? v : r._[1]) }; \
}


#define _TYPEDEF_V3_EXP(TE, TV, fmt) \
_TYPEDEF_V(TE, TV, 3) \
static inline TV TV##Make(TE x, TE y, TE z) { return (TV) {{ x, y, z }}; } \
\
static inline NSString* TV##Desc(TV v) { \
return [NSString stringWithFormat:@"(" fmt @" " fmt @" " fmt @")", v._[0], v._[1], v._[2]]; \
} \


#define _TYPEDEF_V4_EXP(TE, TV, fmt) \
_TYPEDEF_V(TE, TV, 4) \
static inline TV TV##Make(TE x, TE y, TE z, TE w) { return (TV) {{ x, y, z, w }}; } \
\
static inline NSString* TV##Desc(TV v) { \
return [NSString stringWithFormat:@"(" fmt @" " fmt @" " fmt @" " fmt @")", v._[0], v._[1], v._[2], v._[3]]; \
} \


#define _TYPEDEF_V2(TE, fmt) _TYPEDEF_V2_EXP(TE, V2##TE, fmt)
#define _TYPEDEF_V3(TE, fmt) _TYPEDEF_V3_EXP(TE, V3##TE, fmt)
#define _TYPEDEF_V4(TE, fmt) _TYPEDEF_V4_EXP(TE, V4##TE, fmt)


#define _DEF_V_F(dim, TV) \
\
static inline TV TV##InvVec(TV v) { TV r; for_in(i, dim) r._[i] = 1.0 / v._[i]; return r; } \
\
static inline F64 TV##Dot(TV a, TV b) { F64 d = 0; for_in(i, dim) d += a._[i] * b._[i]; return d; } \
\
static inline F64 TV##Mag(TV v) { return sqrt(TV##Dot(v, v)); } \
\
static inline F64 TV##Dist(TV a, TV b) { return TV##Mag(TV##Sub(a, b)); } \
\
static inline TV TV##Norm(TV v) { return TV##Div(v, TV##Mag(v)); } \


#define _DEF_V_F_WITH_I_EXP(dim, TF, TI, TVF, TVI) \
static inline TVF TVF##With##TI(TVI v) { \
TVF r; for_in(i ,dim) r._[i] = v._[i]; \
return r; \
} \
\
static inline TVF TVF##With##TI##Scaled(TVI v, F64 scale) { \
  TVF r; for_in(i ,dim) r._[i] = v._[i] / scale; \
  return r; \
}

#define _DEF_V_F_WITH_I(dim, TF, TI) \
_DEF_V_F_WITH_I_EXP(dim, TF, TI, V##dim##TF, V##dim##TI)


#define _DEF_TVA_WITH_TVB(dim, TVA, TVB) \
static inline TVA TVA##With##TVB(TVB v) { \
TVA r; for_in(i ,dim) r._[i] = v._[i]; \
return r; \
}


_TYPEDEF_V2(U8, @"%uc");
_TYPEDEF_V2(U16, @"%hu");
_TYPEDEF_V2(U32, @"%u");
_TYPEDEF_V2(I32, @"%d");
_TYPEDEF_V2(I64, @"%lld");
_TYPEDEF_V2(F32, @"% f");
_TYPEDEF_V2(F64, @"% f");

_TYPEDEF_V3(U8, @"%uc");
_TYPEDEF_V3(U16, @"%hu");
_TYPEDEF_V3(I32, @"%d");
_TYPEDEF_V3(F32, @"% f");
_TYPEDEF_V3(F64, @"% f");

_TYPEDEF_V4(U8, @"%uc");
_TYPEDEF_V4(U16, @"%hu");
_TYPEDEF_V4(F32, @"% f");
_TYPEDEF_V4(F64, @"% f");

_DEF_V_F(2, V2F32);
_DEF_V_F(2, V2F64);
_DEF_V_F(3, V3F32);
_DEF_V_F(3, V3F64);
_DEF_V_F(4, V4F32);
_DEF_V_F(4, V4F64);

_DEF_V_F_WITH_I(2, F32, U16);
_DEF_V_F_WITH_I(2, F32, I32);
_DEF_V_F_WITH_I(2, F32, U32);
_DEF_V_F_WITH_I(2, F64, U16);
_DEF_V_F_WITH_I(2, F64, I32);
_DEF_V_F_WITH_I(2, F64, U32);

_DEF_TVA_WITH_TVB(2, V2F32, V2F64);
_DEF_TVA_WITH_TVB(2, V2F64, V2F32);

typedef V2U16 Seg;
typedef V3U16 Tri;


// perpendicular vector
static inline V2F32 V2F32Perp(V2F32 v) { return V2F32Make(v._[1], -v._[0]); }

static inline V2F32 V2F32NormPerp(V2F32 v) { return V2F32Norm(V2F32Perp(v)); }


// expand a range stored in a vector with a scalar value.
#define EXPAND_RANGE_VEC(range, val) { \
__typeof__(val) __val = (val); \
if (__val < range._[0]) range._[0] = __val; \
if (__val > range._[1]) range._[1] = __val; \
}

// expand a range stored as two vectors with a vector value.
#define EXPAND_VEC_RANGE(dim, a, b, v) { \
__typeof__(v) __v = (v); \
for_in(i, dim) { \
if (__v._[i] < a._[i]) a._[i] = __v._[i]; \
if (__v._[i] > b._[i]) b._[i] = __v._[i]; \
} \
}
