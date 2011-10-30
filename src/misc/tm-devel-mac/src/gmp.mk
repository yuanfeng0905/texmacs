# This file is part of tm-devel-mac.
# Adapted from the mingw-cross-env project.
# See doc/index.html for further information.

# GMP
PKG             := gmp
$(PKG)_IGNORE   :=
$(PKG)_VERSION  := 5.0.2
$(PKG)_CHECKSUM := 2968220e1988eabb61f921d11e5d2db5431e0a35
$(PKG)_SUBDIR   := gmp-$($(PKG)_VERSION)
$(PKG)_FILE     := gmp-$($(PKG)_VERSION).tar.bz2
$(PKG)_WEBSITE  := http://www.gmplib.org/
$(PKG)_URL      := ftp://ftp.gmplib.org/pub/gmp-$($(PKG)_VERSION)/$($(PKG)_FILE)
$(PKG)_URL_2    := ftp://ftp.cs.tu-berlin.de/pub/gnu/$(PKG)/$($(PKG)_FILE)
$(PKG)_DEPS     := 

GMP_ABI_i386    := 32
GMP_ABI_ppc     := 32
GMP_ABI_x86_64  := 64

define $(PKG)_UPDATE
    curl -s -L 'http://www.gmplib.org/' | \
    grep '<a href="' | \
    $(SED) -n 's,.*gmp-\([0-9][^>]*\)\.tar.*,\1,p' | \
    grep -v '^4\.' | \
    head -1
endef


define $(PKG)_BUILD
   $(foreach BUILD_ARCH,$(BUILD_ARCHS),
     $(call $(3)_BUILD_ARCH,$(1),$(2),$(BUILD_ARCH)))
endef


define $(PKG)_BUILD_ARCH
    [ -d '$(1)/../$(3)' ] || mkdir -p '$(1)/../$(3)'
    cd '$(1)/../$(3)' && '$(1)'/configure\
        --host=$(TARGET_$(3)) \
        NM='/usr/bin/nm -p' \
        CC="gcc-4.2 -arch $(3) -mmacosx-version-min=10.5 -isysroot /Developer/SDKs/MacOSX10.5.sdk "\
        CXX="g++-4.2 -arch $(3) -mmacosx-version-min=10.5 -isysroot /Developer/SDKs/MacOSX10.5.sdk "\
        CPP="cpp-4.2"\
        CXXCPP="cpp-4.2" \
        CPPFLAGS=" -I$(PREFIX)/$(3)/include -I$(PREFIX)/include"\
        CFLAGS=" -I$(PREFIX)/$(3)/include -I$(PREFIX)/include"\
        CXXFLAGS="-I$(PREFIX)/$(3)/include -I$(PREFIX)/include"\
        LDFLAGS=" -Wl,-L$(PREFIX)/$(3)/lib  -Wl,-L$(PREFIX)/lib   -Wl,-syslibroot /Developer/SDKs/MacOSX10.5.sdk/ " \
        --prefix='$(PREFIX)/$(3)' \
        --disable-shared \
        --without-readline \
        ABI=$(GMP_ABI_$(3)) 
    $(MAKE) -C '$(1)/../$(3)' -j '$(JOBS)'
    $(MAKE) -C '$(1)/../$(3)' -j 1 install
endef
