BUILDDIR=build
#BUILDDIR=build_debug

#change to Debug for debug mode
BUILDTYPE=Release
#BUILDTYPE=Debug

all: cmake
	@@$(MAKE) -C $(BUILDDIR)

mkbuilddir:
	@@[ -d $(BUILDDIR) ] || mkdir $(BUILDDIR)

cmake: mkbuilddir
	@@[ ! -f "$(BUILDDIR)/CMakeCache.txt" ] && cd $(BUILDDIR) && cmake -DCMAKE_BUILD_TYPE=$(BUILDTYPE) .. || true

test:
	@@$(MAKE) -C $(BUILDDIR) test

debdeps:
	apt-get install cmake libprotobuf-dev protobuf-compiler libqt4-dev freeglut3-dev libxi-dev libxmu-dev

clean: cmake
	@@$(MAKE) -C $(BUILDDIR) clean

clean-all:
	cd $(BUILDDIR) && rm -rf *
	
