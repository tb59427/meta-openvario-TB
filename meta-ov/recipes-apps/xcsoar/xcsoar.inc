DESCRIPTION = "XCSoar glide computer"
HOMEPAGE = "www.xcsoar.org"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=393a5ca445f6965873eca0259a17f833"
SECTION = "base/app"

DEPENDS = "	\
		curl \
		pkgconfig-native \
		libxslt-native \
		librsvg-native \
		imagemagick-native \
		libinput \
		lua \
		udev \
		ttf-dejavu \
		jpeg \
		freetype \
		libpng \
		glm \
		virtual/egl \
		virtual/mesa \
		virtual/libgles2 \
		alsa-lib \
		libsodium \
		c-ares \
"

RDEPENDS:${PN} = "\
        ttf-dejavu-sans-condensed \
"

S = "${WORKDIR}/git"

LC_LOCALE_PATH = "${datadir}/locale"

SRC_URI += " \
	file://0007-Disable-touch-screen-auto-detection.patch \
	https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.bz2;sha256sum=8681f175d4bdb26c52222665793eef08490d7758529330f98d3b29dd0735bccc;unpack=0 \
	https://boostorg.jfrog.io/artifactory/main/release/1.79.0/source/boost_1_79_0.tar.bz2;sha256sum=475d589d51a7f8b3ba2ba4eda022b170e562ca3b760ee922c146b6c65856ef39;unpack=0 \
"

inherit pkgconfig

addtask do_package_write_ipk after do_package

EXTRA_OEMAKE = " \
	HOSTCC='${BUILD_CC}' \
	HOSTCXX='${BUILD_CXX}' \
	AR='${AR}' \
	RANLIB='${RANLIB}' \
	CXX='${CXX}' \
	CC='${CC}' \
	AS='${AS}' \
	LD='${LD}' \
	STRIP='${STRIP}' \
	CCACHE='' \
	\
	DEBUG=n DEBUG_GLIBCXX=n \
	ENABLE_MESA_KMS=y GLES2=y \
	GEOTIFF=n \
"

do_configure() {
	install -d ${B}/output/download
	# TODO: remove Boost 1.78.0 after XCSoar 7.24 has been released
	cp ${WORKDIR}/boost_1_78_0.tar.bz2 ${B}/output/download/
	cp ${WORKDIR}/boost_1_79_0.tar.bz2 ${B}/output/download/
	oe_runmake boost
}

do_compile() {
	export PATH=$PATH:/usr/bin
	ln -sf convert.im7 ${STAGING_DIR_NATIVE}/usr/bin/convert
	export FONTCONFIG_PATH=/etc/fonts
	oe_runmake
}

do_install() {
	oe_runmake install-bin install-mo DESTDIR=${D}
}

FILES:${PN} = " \
	${bindir}/xcsoar \
	${bindir}/vali-xcs \
	${LC_LOCALE_PATH}/*/LC_MESSAGES/xcsoar.mo \
"
