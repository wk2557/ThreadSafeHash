#XMAKE edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
.PHONY:all
all:xmake_makefile_check test_ThreadSafe_Hash 
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:xmake_makefile_check
xmake_makefile_check:
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mxmake_makefile_check[0m']"
	@if [ "XMAKE" -nt "Makefile" ] ; then echo "[ERROR]PLEASE UPDATE 'Makefile' BY 'xmake'" ; exit 250 ;  fi

.PHONY:clean
clean:
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf test_ThreadSafe_Hash
	rm -rf ./output/bin/test_ThreadSafe_Hash
	rm -rf src/test_ThreadSafe_Hash_test_ThreadSafe_Hash.o

.PHONY:dist
dist:clean
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	cd ..;tar czvf test.tar.gz test;cd test

.PHONY:love
love:
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

test_ThreadSafe_Hash:src/test_ThreadSafe_Hash_test_ThreadSafe_Hash.o
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40mtest_ThreadSafe_Hash[0m']"
	g++ src/test_ThreadSafe_Hash_test_ThreadSafe_Hash.o -Xlinker "-("  ../../../Common/ThirdParty/Gtest/lib/libgtest.a -lpthread \
  -lcrypto \
  -lrt \
  -lm \
  -lssl -Xlinker "-)" -o test_ThreadSafe_Hash
	mkdir -p ./output/bin
	cp -f test_ThreadSafe_Hash ./output/bin

src/test_ThreadSafe_Hash_test_ThreadSafe_Hash.o:src/test_ThreadSafe_Hash.cpp \
  src/test_ThreadSafe_Hash.cpp
	@echo "[[1;32;40mXMAKE:BUILD[0m][Target:'[1;32;40msrc/test_ThreadSafe_Hash_test_ThreadSafe_Hash.o[0m']"
	g++ -c -I. \
  -I../include -I../../../Common/ThirdParty/Gtest \
  -I../../../Common/ThirdParty/Gtest/include \
  -I../../../Common/ThirdParty/Gtest/output \
  -I../../../Common/ThirdParty/Gtest/output/include -D_GNU_SOURCE \
  -D__STDC_LIMIT_MACROS \
  -DVERSION=\"1.0.1.0\" \
  -DNDEBUG \
  -DNDEMO \
  -D__64BIT__ \
  -D_FILE_OFFSET_BITS=64 -g \
  -pipe \
  -W \
  -Wall \
  -Werror \
  -fPIC \
  -Wpointer-arith \
  -Wconversion \
  -rdynamic  -o src/test_ThreadSafe_Hash_test_ThreadSafe_Hash.o src/test_ThreadSafe_Hash.cpp

endif #ifeq ($(shell uname -m),x86_64)


