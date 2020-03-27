CC                 := gcc
CXX                := g++
LD                 := ld

CFLAGS             := \
  -fdata-sections \
  -ffunction-sections \
  -fno-asynchronous-unwind-tables \
  -fvisibility=hidden \
  -pipe \
  -Wall
  
CXXFLAGS           := \
  -std=c++98 \
  -exceptions \
  -fno-rtti \
  -fdata-sections \
  -ffunction-sections \
  -fno-asynchronous-unwind-tables \
  -fvisibility=hidden \
  -pipe \
  -Wall

INCLUDE            :=
LDFLAGS            := -r
EXE_FLAGS          := 
  

TOOL_VARS := \
  CC CXX LD \
  CFLAGS CXXFLAGS LDFLAGS \
  INCLUDE

.PHONY: toolvars
toolvars:
	@echo "List of tool variables:"
	@echo "  CC                        C compiler execution command line"
	@echo "  CFLAGS                    C compiler build flags"
	@echo "  CXX                       C++ compiler execution command line"
	@echo "  CXXFLAGS                  C++ compiler build flags"
	@echo "  INCLUDE                   Additional paths for include files"
	@echo "  LD                        Linker execution command line"
	@echo "  LDFLAGS                   Linker flags for merging object files"
	@echo ""
