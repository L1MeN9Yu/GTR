#!/bin/bash

echo "Remove build folder"
rm -rf build

echo "Build for Brotli/cJSON maccatalyst"
xmake f -p macosx -a x86_64 --target_minver=10.15 --cxflags="-fembed-bitcode -target x86_64-apple-ios13.0-macabi" --mxflags=-fembed-bitcode --asflags=-fembed-bitcode
xmake
mv build/macosx build/maccatalyst

echo "Build Brotli/cJSON for iOS"
xmake m package -p iphoneos -f "--target_minver=9 --cxflags=\"-fembed-bitcode\" --mxflags=\"-fembed-bitcode\" --asflags=\"-fembed-bitcode\""

echo "Build Brotli/cJSON for macOS"
xmake m package -p macosx -f "--target_minver=10.12 --cxflags=\"-fembed-bitcode\" --mxflags=\"-fembed-bitcode\" --asflags=\"-fembed-bitcode\""

echo "Create folders"
mkdir -p \
build/iOS/libbrotli/lib build/macOS/libbrotli/lib \
build/iOS/libcurl/lib build/macOS/libcurl/lib \
build/iOS/libcJSON/lib build/macOS/libcJSON/lib \
build/iOS/libcJSON/include/cJSON build/macOS/libcJSON/include/cJSON \
build/maccatalyst/libcJSON/include/cJSON \

echo "Copy Headers"
mv build/brotli.pkg build/brotli
cp -r brotli/c/include build/iOS/libbrotli/include 
cp -r brotli/c/include build/macOS/libbrotli/include 

mv build/cJSON.pkg build/cJSON
cp -r cJSON/cJSON.h build/iOS/libcJSON/include/cJSON
cp -r cJSON/cJSON.h build/macOS/libcJSON/include/cJSON

echo "Copy Libs"
cp build/brotli/iphoneos/universal/lib/release/libbrotli.a build/iOS/libbrotli/lib/libbrotli.a
cp build/brotli/iphoneos/universal/lib/release/libbrotli.a build/iOS/libbrotli/lib/libbrotlidec.a
cp build/cJSON/iphoneos/universal/lib/release/libcJSON.a build/iOS/libcJSON/lib/libcJSON.a
cp build/brotli/macosx/x86_64/lib/release/libbrotli.a build/macOS/libbrotli/lib/libbrotli.a
cp build/macOS/libbrotli/lib/libbrotli.a build/macOS/libbrotli/lib/libbrotlidec.a
cp build/cJSON/macosx/x86_64/lib/release/libcJSON.a build/macOS/libcJSON/lib/libcJSON.a

mkdir -p build/maccatalyst/libbrotli/lib
mkdir -p build/maccatalyst/libcJSON/lib
mkdir -p build/maccatalyst/libcurl/lib

cp build/maccatalyst/x86_64/release/libbrotli.a build/maccatalyst/libbrotli/lib
cp build/maccatalyst/x86_64/release/libcJSON.a build/maccatalyst/libcJSON/lib
cp build/maccatalyst/libbrotli/lib/libbrotli.a build/maccatalyst/libbrotli/lib/libbrotlidec.a
mv build/maccatalyst/libbrotli/lib/libbrotli.a build/maccatalyst/libbrotli/lib/libbrotli.maccatalyst.a
mv build/maccatalyst/libcJSON/lib/libcJSON.a build/maccatalyst/libcJSON/lib/libcJSON.maccatalyst.a
cp -r brotli/c/include build/maccatalyst/libbrotli/include 
cp -r cJSON/cJSON.h build/maccatalyst/libcJSON/include/cJSON

echo "clean brotli files"
rm -rf .xmake build/.deps build/.objs build/brotli build/cJSON build/iphoneos build/macosx build/maccatalyst/x86_64

realpath() {
    [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}

XCODE=$(xcode-select -p)
if [ ! -d "$XCODE" ]; then
	echo "You have to install Xcode and the command line tools first"
	exit 1
fi

REL_SCRIPT_PATH="$(dirname "$0")"
SCRIPTPATH=$(realpath "$REL_SCRIPT_PATH")
CURLPATH="$SCRIPTPATH/curl"

PWD=$(pwd)
cd "$CURLPATH" || exit

if [ ! -x "$CURLPATH/configure" ]; then
	echo "Curl needs external tools to be compiled"
	echo "Make sure you have autoconf, automake and libtool installed"

	./buildconf

	EXITCODE=$?
	if [ $EXITCODE -ne 0 ]; then
		echo "Error running the buildconf program"
		cd "$PWD" || exit
		exit $EXITCODE
	fi
fi


export CC="$XCODE/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
DESTDIR="$SCRIPTPATH/build"

echo "Build libcurl for maccatalyst"

export MACCATALYST_DEPLOYMENT_TARGET="10.15"
ARCHS=(x86_64)
HOSTS=(x86_64)
PLATFORMS=(MacOSX)
SDK=(MacOSX)
BROTLI="${PWD}/../build/maccatalyst/libbrotli"

for (( i=0; i<${#ARCHS[@]}; i++ )); do

	ARCH=${ARCHS[$i]}
	export CFLAGS="-arch $ARCH -pipe -Os -gdwarf-2 -mmacosx-version-min=${MACCATALYST_DEPLOYMENT_TARGET} -fembed-bitcode -Werror=partial-availability -target x86_64-apple-ios13.0-macabi"
	export LDFLAGS="-arch $ARCH"
	export CPPFLAGS="-I${PWD}/../include"

	cd "$CURLPATH" || exit
	./configure	--host="${HOSTS[$i]}-apple-darwin" \
			--with-darwinssl \
			--with-zlib \
			--with-brotli="${BROTLI}" \
			--without-libidn2 \
			--enable-static \
			--disable-shared \
			--enable-threaded-resolver \
			--disable-verbose \
			--enable-ipv6
	EXITCODE=$?
	if [ $EXITCODE -ne 0 ]; then
		echo "Error running the cURL configure program"
		cd "$PWD" || exit
		exit $EXITCODE
	fi

	make -j $(sysctl -n hw.logicalcpu_max)
	EXITCODE=$?
	if [ $EXITCODE -ne 0 ]; then
		echo "Error running the make program"
		cd "$PWD" || exit
		exit $EXITCODE
	fi
	mkdir -p "$DESTDIR/$ARCH"
	cp "$CURLPATH/lib/.libs/libcurl.a" "$DESTDIR/$ARCH/"
	cp "$CURLPATH/lib/.libs/libcurl.a" "$DESTDIR/libcurl-$ARCH.a"
	make clean
done

cd "$DESTDIR"
lipo -create -output libcurl.a libcurl-*.a
rm libcurl-*.a
rm -rf x86_64

#Copying cURL headers
if [ -d "$DESTDIR/include" ]; then
	echo "Cleaning headers"
	rm -rf "$DESTDIR/include"
fi
cp -R "$CURLPATH/include" "$DESTDIR/"
rm include/Makefile include/Makefile.am include/Makefile.in include/README include/curl/Makefile include/curl/Makefile.in include/curl/Makefile.am

cp -r include maccatalyst/libcurl/include
cp libcurl.a maccatalyst/libcurl/lib/libcurl.maccatalyst.a

echo "Build libcurl for macOSX"

export MACOSX_DEPLOYMENT_TARGET="10.12"
ARCHS=(x86_64)
HOSTS=(x86_64)
PLATFORMS=(MacOSX)
SDK=(MacOSX)
BROTLI="${PWD}/../build/macOS/libbrotli"

for (( i=0; i<${#ARCHS[@]}; i++ )); do

	ARCH=${ARCHS[$i]}
	export CFLAGS="-arch $ARCH -pipe -Os -gdwarf-2 -mmacosx-version-min=${MACOSX_DEPLOYMENT_TARGET} -fembed-bitcode -Werror=partial-availability"
	export LDFLAGS="-arch $ARCH"
	export CPPFLAGS="-I${PWD}/../include"

	cd "$CURLPATH" || exit
	./configure	--host="${HOSTS[$i]}-apple-darwin" \
			--with-darwinssl \
			--with-zlib \
			--with-brotli="${BROTLI}" \
			--without-libidn2 \
			--enable-static \
			--disable-shared \
			--enable-threaded-resolver \
			--disable-verbose \
			--enable-ipv6
	EXITCODE=$?
	if [ $EXITCODE -ne 0 ]; then
		echo "Error running the cURL configure program"
		cd "$PWD" || exit
		exit $EXITCODE
	fi

	make -j $(sysctl -n hw.logicalcpu_max)
	EXITCODE=$?
	if [ $EXITCODE -ne 0 ]; then
		echo "Error running the make program"
		cd "$PWD" || exit
		exit $EXITCODE
	fi
	mkdir -p "$DESTDIR/$ARCH"
	cp "$CURLPATH/lib/.libs/libcurl.a" "$DESTDIR/$ARCH/"
	cp "$CURLPATH/lib/.libs/libcurl.a" "$DESTDIR/libcurl-$ARCH.a"
	make clean
done

cd "$DESTDIR"
lipo -create -output libcurl.a libcurl-*.a
rm libcurl-*.a
rm -rf i386 x86_64

#Copying cURL headers
if [ -d "$DESTDIR/include" ]; then
	echo "Cleaning headers"
	rm -rf "$DESTDIR/include"
fi
cp -R "$CURLPATH/include" "$DESTDIR/"
rm include/Makefile include/Makefile.am include/Makefile.in include/README include/curl/Makefile include/curl/Makefile.in include/curl/Makefile.am

cp -r include macOS/libcurl/include
cp libcurl.a macOS/libcurl/lib/libcurl.a

echo "Build libcurl for iOS"

export IPHONEOS_DEPLOYMENT_TARGET="9"
ARCHS=(armv7 armv7s arm64 i386 x86_64)
HOSTS=(armv7 armv7s arm i386 x86_64)
PLATFORMS=(iPhoneOS iPhoneOS iPhoneOS iPhoneSimulator iPhoneSimulator)
SDK=(iPhoneOS iPhoneOS iPhoneOS iPhoneSimulator iPhoneSimulator)
BROTLI="${PWD}/../build/iOS/libbrotli"

for (( i=0; i<${#ARCHS[@]}; i++ )); do
	ARCH=${ARCHS[$i]}
	export CFLAGS="-arch $ARCH -pipe -Os -gdwarf-2 -isysroot $XCODE/Platforms/${PLATFORMS[$i]}.platform/Developer/SDKs/${SDK[$i]}.sdk -miphoneos-version-min=${IPHONEOS_DEPLOYMENT_TARGET} -fembed-bitcode -Werror=partial-availability"
	export LDFLAGS="-arch $ARCH -isysroot $XCODE/Platforms/${PLATFORMS[$i]}.platform/Developer/SDKs/${SDK[$i]}.sdk -L${BROTLI}/lib"
	if [ "${PLATFORMS[$i]}" = "iPhoneSimulator" ]; then
		export CPPFLAGS="-D__IPHONE_OS_VERSION_MIN_REQUIRED=${IPHONEOS_DEPLOYMENT_TARGET%%.*}0000"
	fi
	cd "$CURLPATH"
	./configure	--host="${HOSTS[$i]}-apple-darwin" \
			--with-darwinssl \
			--with-zlib \
			--with-brotli="${BROTLI}" \
			--enable-static \
			--disable-shared \
			--enable-threaded-resolver \
			--disable-verbose \
			--enable-ipv6 
	EXITCODE=$?
	if [ $EXITCODE -ne 0 ]; then
		echo "Error running the cURL configure program"
		cd "$PWD" || exit
		exit $EXITCODE
	fi

	make -j $(sysctl -n hw.logicalcpu_max)
	EXITCODE=$?
	if [ $EXITCODE -ne 0 ]; then
		echo "Error running the make program"
		cd "$PWD" || exit
		exit $EXITCODE
	fi
	mkdir -p "$DESTDIR/$ARCH"
	cp "$CURLPATH/lib/.libs/libcurl.a" "$DESTDIR/$ARCH/"
	cp "$CURLPATH/lib/.libs/libcurl.a" "$DESTDIR/libcurl-$ARCH.a"
	make clean
done

#Build a single static lib with all the archs in it
cd "$DESTDIR"
lipo -create -output libcurl.a libcurl-*.a
rm libcurl-*.a
rm -rf i386 x86_64 armv7 armv7s arm64

#Copying cURL headers
if [ -d "$DESTDIR/include" ]; then
	echo "Cleaning headers"
	rm -rf "$DESTDIR/include"
fi
cp -R "$CURLPATH/include" "$DESTDIR/"
rm include/Makefile include/Makefile.am include/Makefile.in include/README include/curl/Makefile include/curl/Makefile.in include/curl/Makefile.am include/curl/Makefile.am

cp -r include iOS/libcurl/include
cp libcurl.a iOS/libcurl/lib/libcurl.a

rm iOS/libbrotli/lib/libbrotlidec.a macOS/libbrotli/lib/libbrotlidec.a maccatalyst/libbrotli/lib/libbrotlidec.a libcurl.a 
rm -rf include
