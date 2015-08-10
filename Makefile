#XMAKE edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
.PHONY:all
all:xmake_makefile_check HashCompare 
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:xmake_makefile_check
xmake_makefile_check:
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mxmake_makefile_check[0m']"
	@if [ "XMAKE" -nt "Makefile" ] ; then echo "[ERROR]PLEASE UPDATE 'Makefile' BY 'xmake'" ; exit 250 ;  fi

.PHONY:clean
clean:
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf HashCompare
	rm -rf ./output/bin/HashCompare
	rm -rf src/HashCompare_compare.o

.PHONY:dist
dist:clean
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	cd ..;tar czvf ThreadSafeHash.tar.gz ThreadSafeHash;cd ThreadSafeHash

.PHONY:love
love:
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

HashCompare:src/HashCompare_compare.o
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mHashCompare[0m']"
	g++ src/HashCompare_compare.o -Xlinker "-("  ../../Publish/Common/Gsl/lib/libgsl.a -L"/search/wangkun/local/lib" \
  -lpthread \
  -lcrypto \
  -lrt -Xlinker "-)" -o HashCompare
	mkdir -p ./output/bin
	cp -f --link HashCompare ./output/bin

src/HashCompare_compare.o:src/compare.cpp \
  src/compare.cpp \
  include/ThreadSafeHash.h
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40msrc/HashCompare_compare.o[0m']"
	g++ -c -I. \
  -I./include \
  -I../../Common/Gsl/include \
  -I/search/wangkun/local/include -I../../Publish/Common/Gsl \
  -I../../Publish/Common/Gsl/include \
  -I../../Publish/Common/Gsl/output \
  -I../../Publish/Common/Gsl/output/include    -o src/HashCompare_compare.o src/compare.cpp

endif #ifeq ($(shell uname -m),x86_64)


