#!/usr/bin/env bash
# Copyright 2013 George King.
# Permission to use this file is granted in libqk/license.txt.

# build a static lib from autoconf/make based source.
# invoke this from the lib source root (i.e. where the configure script is).

set -e

error() { echo 'error:' "$@" 1>&2; exit 1; }

echo "build-lib-arch.sh: $@"

sdk=$1;   shift
host=$1;  shift
arch=$1;  shift

platform=${sdk%%[0-9]*}
platform_dir=$DEV_DIR/Platforms/$platform.platform
sdk_dir=$platform_dir/Developer/SDKs/$sdk.sdk

libtool=$TOOL_DIR/libtool

cc=$TOOL_DIR/clang
cxx=$TOOL_DIR/clang++
as="$cc"
ld=$TOOL_DIR/ld

cc_flags=''

if   [[ $platform == 'MacOSX' ]]; then
  min_flag='' # TODO
elif [[ $platform == 'iPhoneOS' ]]; then
  min_flag='-miphoneos-version-min=7.0'
  cc_flags='-mfpu=neon' # libpng enables intrinsics when it detects this flag.
  #as="$GAS_PRE $as" # may help with arm neon gas-style assembly.
elif [[ $platform == 'iPhoneSimulator' ]]; then
  min_flag="-mios-simulator-version-min=7.0"
else
  error "unkown platform: $platform"
fi

# i386
# -fexceptions -fasm-blocks -fstrict-aliasing -Wprotocol -fobjc-abi-version=2 -fobjc-legacy-dispatch  

echo "
sdk: $sdk
host: $host
arch: $arch
platform: $platform
libtool: $libtool
cc: $cc
cxx: $cxx
as: $as
ld: $ld
min_flag: $min_flag
cc_flags: $cc_flags
config_args: $config_args
----"


for n in SRC_DIR DEV_DIR TOOL_DIR PLATFORM_TOOL_DIR QK_DIR platform_dir sdk_dir; do
  eval v=\$$n
  echo "$n: $v"
  [[ -d "$v" ]] || error "bad $n"
done

mkdir -p "$BUILD_DIR/$arch"
cd "$BUILD_DIR/$arch"
rm -rf * # clean aggressively; removing the whole directory is disruptive to terminal sessions.
rm -rf .deps .libs # hidden directories

# even with the quiet flag, libtool is verbose, so redirect output.
if [[ -n "$QUIET" ]]; then
  OUT=/dev/null
else
  OUT=/dev/stdout
  set -x # to print entire configure command.
fi

echo "running configure..."
"$SRC_DIR/configure" \
--disable-dependency-tracking \
--prefix="$PWD/install" \
--host=$host \
--enable-static \
--disable-shared \
--with-sysroot="$sdk_dir" \
LIBTOOL="$libtool" \
CC="$cc" \
CXX="$cxx" \
CCAS="$as" \
LD="$ld" \
CFLAGS="$CC_OPT $cc_flags" \
CPPFLAGS="-arch $arch $min_flag -isysroot $sdk_dir -I$sdk_dir/usr/include" \
LDFLAGS=" -arch $arch $min_flag -isysroot $sdk_dir -L$sdk_dir/usr/lib" \
$CONFIG_ARGS \
$QUIET

set +x

echo "running make..."
make $QUIET > $OUT
echo "running make install..."
make install $QUIET > $OUT

echo "
BUILD COMPLETE: $LIB_NAME $sdk $host $arch
--------------
"
