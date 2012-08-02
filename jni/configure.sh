#!/bin/sh

SOURCE=/home/sources

# path of android ndk
ANDROID_NDK="/opt/android-ndk"
# toolchain version
# android-ndk r8b supports 4.4.3 and 4.6
TOOLCHAIN_VERSION="4.4.3"
# platform version, i.e. api level
PLATFORM_VERSION=8
# target
TARGET=arm-linux-androideabi
# path
PATH=$ANDROID_NDK/toolchains/$TARGET-$TOOLCHAIN_VERSION/prebuilt/linux-x86/bin:$PATH
# the fullpath of libgcc.a
LIBGCCA=`ls $ANDROID_NDK/toolchains/$TARGET-$TOOLCHAIN_VERSION/prebuilt/linux-x86/lib/gcc/$TARGET/*/thumb/libgcc.a`

# the path of openssl
OPENSSL_PREFIX=/mnt/data/android/external/openssl
# the path of libcrypto.so libssl.so, can get it from /system/lib
OPENSSL_LIBDIR=/mnt/data/android/out/target/product/generic/system/lib

CURL_VERSION=7.27.0
C_ARES_VERSION=1.9.1
CURL_EXTRA="--disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-proxy --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smtp --disable-gopher --disable-sspi"

pushd `dirname $0`

rm -rf curl curl-$CURL_VERSION
tar xf $SOURCE/curl-$CURL_VERSION.tar.*
mv curl-$CURL_VERSION curl
mkdir -p curl/ares

rm -rf ares c-ares-$C_ARES_VERSION
tar xf $SOURCE/c-ares-$C_ARES_VERSION.tar.*
mv c-ares-$C_ARES_VERSION ares

pushd curl
./configure CC=$TARGET-gcc --host=arm-linux \
	CPPFLAGS="-DANDROID -I$ANDROID_NDK/platforms/android-$PLATFORM_VERSION/arch-arm/usr/include " \
	CFLAGS="-fno-exceptions -Wno-multichar -mthumb-interwork -mthumb -nostdlib " \
	LIBS="-lc -ldl -lz $LIBGCCA " \
	LDFLAGS="-L$ANDROID_NDK/platforms/android-$PLATFORM_VERSION/arch-arm/usr/lib -L$OPENSSL_LIBDIR " \
	--enable-ipv6 --disable-manual --with-random=/dev/urandom \
	--with-ssl=$OPENSSL_PREFIX --without-ca-bundle --without-ca-path \
	--with-zlib --enable-ares $CURL_EXTRA || exit 1
popd

pushd ares
./configure CC=$TARGET-gcc --host=arm-linux \
	CPPFLAGS="-DANDROID -I$ANDROID_NDK/platforms/android-$PLATFORM_VERSION/arch-arm/usr/include " \
	CFLAGS="-fno-exceptions -Wno-multichar -mthumb-interwork -mthumb -nostdlib " \
	LIBS="-lc -ldl " \
	LDFLAGS="-L$ANDROID_NDK/platforms/android-$PLATFORM_VERSION/arch-arm/usr/lib " \
	--with-random=/dev/urandom || exit 1
popd

popd
