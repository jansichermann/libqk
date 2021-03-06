// Copyright 2010 George King.
// Permission to use this file is granted in libqk/license.txt.

/*
qk_check is an assertion macro intended for production use.
qk_assert is exactly like check, when QK_OPTIMIZE is true, and a no-op otherwise.
qk_fail is identical to check, but is not predicated on a conditional.
all of these macros throw an objective-c exception.

BREAK provides a console-driven break mechanism.
*/


#ifndef QK_OPTIMIZE
# ifdef NDEBUG
#   define QK_OPTIMIZE 1
# else
#   define QK_OPTIMIZE 0
# endif
#endif


#define _qk_fail(expr_str, ...) ({ \
NSString* expr_line = expr_str ? [NSString stringWithFormat:@"!%s\n", expr_str] : @""; \
NSString* file_str = [[NSString stringWithUTF8String:__FILE__] lastPathComponent]; \
NSString* msg = [NSString stringWithFormat:__VA_ARGS__]; \
NSLog(@"ERROR: %@:%d: %s\n%@%@\n", file_str, __LINE__, __PRETTY_FUNCTION__, expr_line, msg); \
[[NSAssertionHandler currentHandler] \
handleFailureInFunction:[NSString stringWithUTF8String:__PRETTY_FUNCTION__] \
file:file_str \
lineNumber:__LINE__ \
description:@"%@%@", expr_line, msg]; \
abort(); \
})

# define qk_check(expr, ...) \
((expr) ? (void)0 : _qk_fail(#expr, __VA_ARGS__))

# define qk_fail(...) _qk_fail((char*)NULL, __VA_ARGS__)

// soft checks simply print an error message
# define qk_check_soft(expr, ...) \
((void)((expr) ? 0 : NSLog(@"ERROR: soft check failed: %@:%d: %s\n! %s\n%@\n", \
[[NSString stringWithUTF8String:__FILE__] lastPathComponent], __LINE__, __PRETTY_FUNCTION__, \
#expr, [NSString stringWithFormat:__VA_ARGS__])))


#if QK_OPTIMIZE // release
# define qk_assert(expr, ...) ((void)0)
# define qk_assert_soft(expr, ...) ((void)0)
#else // debug
# define qk_assert(expr, ...) qk_check((expr), __VA_ARGS__)
# define qk_assert_soft(expr, ...) qk_check_soft((expr), __VA_ARGS__)
#endif // QK_OPTIMIZE

#define ASSERT_WCHAR_IS_UTF32 qk_assert(sizeof(wchar_t) == 4, @"bad wchar_t size: %lu", sizeof(wchar_t))


#define BREAK _pseudo_breakpoint(__FILE__, __LINE__, __func__)


#define CHECK_RET_ERROR(expr, domain_prefix, code_suffix, _desc, ...) \
if (!(expr)) { \
return [NSError withDomain:domain_prefix##ErrorDomain \
code:domain_prefix##ErrorCode##code_suffix \
desc:(_desc) \
info:__VA_ARGS__]; \
}

#define CHECK_SET_ERROR_RET_NIL(expr, domain_prefix, code_suffix, _desc, ...) \
if (!(expr)) { \
NSString* __desc = (_desc); \
NSDictionary* __info = __VA_ARGS__; \
qk_check(errorPtr, @"%@ (no errorPtr provided): %@", _desc, __info); \
*errorPtr = [NSError withDomain:domain_prefix##ErrorDomain \
code:domain_prefix##ErrorCode##code_suffix \
desc:__desc \
info:__info]; \
return nil; \
}



#ifdef __cplusplus
extern "C" {
#endif
  
void _qk_pseudo_breakpoint(const char* file, int line, const char* func);

#ifdef __cplusplus
}
#endif
    
