#!/bin/sh

ANDROID_ROOT="/mnt/data/android" \
ANDROID_NDK="/opt/android-ndk" \
TOOLCHAIN_VER="4.4.3"  \
PLATFORM_VER="5" \
CROSS_COMPILE=arm-eabi- \
PATH=$ANDROID_ROOT/prebuilt/linux-x86/toolchain/arm-eabi-$TOOLCHAIN_VER/bin:$PATH  \
CPPFLAGS="-DANDROID -I $ANDROID_ROOT/system/core/include -I$ANDROID_NDK/platforms/android-$PLATFORM_VER/arch-arm/usr/include -I$ANDROID_ROOT/bionic/libc/include -I$ANDROID_ROOT/bionic/libc/kernel/common -I$ANDROID_ROOT/bionic/libc/kernel/arch-arm -L$ANDROID_NDK/platforms/android-$PLATFORM_VER/arch-arm/usr/lib -L$ANDROID_ROOT/out/target/product/generic/system/lib " \
CFLAGS="-fno-exceptions -Wno-multichar -mthumb-interwork -mthumb -nostdlib -lc -ldl "  \
./configure CC=arm-eabi-gcc --host=arm-linux --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-proxy --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smtp --disable-gopher --disable-manual --disable-ipv6 --disable-sspi --without-random --with-ssl=$ANDROID_ROOT/external/openssl ac_cv_header_x509_h=yes ac_cv_header_crypto_h=yes ac_cv_header_ssl_h=yes --without-ca-bundle --without-ca-path --with-zlib=$ANDROID_ROOT/external/zlib
