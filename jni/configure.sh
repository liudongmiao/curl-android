#!/bin/sh

SOURCE=/home/sources

ANDROID_ROOT="/mnt/data/android"
ANDROID_NDK="/opt/android-ndk"
TOOLCHAIN_VER="4.4.3"
PLATFORM_VER=5

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
ANDROID_ROOT="$ANDROID_ROOT" \
ANDROID_NDK="$ANDROID_NDK" \
TOOLCHAIN_VER="$TOOLCHAIN_VER"  \
PLATFORM_VER="$PLATFORM_VER" \
CROSS_COMPILE=arm-eabi- \
PATH=$ANDROID_ROOT/prebuilt/linux-x86/toolchain/arm-eabi-$TOOLCHAIN_VER/bin:$PATH  \
CPPFLAGS="-DANDROID -I $ANDROID_ROOT/system/core/include -I$ANDROID_NDK/platforms/android-$PLATFORM_VER/arch-arm/usr/include -I$ANDROID_ROOT/bionic/libc/include -I$ANDROID_ROOT/bionic/libc/kernel/common -I$ANDROID_ROOT/bionic/libc/kernel/arch-arm -L$ANDROID_NDK/platforms/android-$PLATFORM_VER/arch-arm/usr/lib -I$ANDROID_ROOT/external/openssl/include -L$ANDROID_ROOT/out/target/product/generic/system/lib " \
CFLAGS="-fno-exceptions -Wno-multichar -mthumb-interwork -mthumb -nostdlib -lc -ldl " \
./configure CC=arm-eabi-gcc --host=arm-linux --enable-ipv6 --disable-manual --with-random=/dev/urandom --with-ssl ac_cv_header_openssl_x509_h=yes ac_cv_header_openssl_rsa_h=yes ac_cv_header_openssl_crypto_h=yes ac_cv_header_openssl_pem_h=yes ac_cv_header_openssl_ssl_h=yes ac_cv_header_openssl_err_h=yes ac_cv_header_openssl_pkcs12_h=yes ac_cv_header_openssl_engine_h=yes --without-ca-bundle --without-ca-path --with-zlib --enable-ares $CURL_EXTRA || exit 1
popd

pushd ares
ANDROID_ROOT="$ANDROID_ROOT" \
ANDROID_NDK="$ANDROID_NDK" \
TOOLCHAIN_VER="$TOOLCHAIN_VER"  \
PLATFORM_VER="$PLATFORM_VER" \
CROSS_COMPILE=arm-eabi- \
PATH=$ANDROID_ROOT/prebuilt/linux-x86/toolchain/arm-eabi-$TOOLCHAIN_VER/bin:$PATH  \
CPPFLAGS="-DANDROID -I $ANDROID_ROOT/system/core/include -I$ANDROID_NDK/platforms/android-$PLATFORM_VER/arch-arm/usr/include -I$ANDROID_ROOT/bionic/libc/include -I$ANDROID_ROOT/bionic/libc/kernel/common -I$ANDROID_ROOT/bionic/libc/kernel/arch-arm -L$ANDROID_NDK/platforms/android-$PLATFORM_VER/arch-arm/usr/lib -L$ANDROID_ROOT/out/target/product/generic/system/lib " \
CFLAGS="-fno-exceptions -Wno-multichar -mthumb-interwork -mthumb -nostdlib -lc -ldl " \
./configure CC=arm-eabi-gcc --host=arm-linux --with-random=/dev/urandom || exit 1
popd

popd
