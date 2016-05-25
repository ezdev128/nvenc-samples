################################################################################
#
# Copyright 1993-2014 NVIDIA Corporation.  All rights reserved.
#
# NOTICE TO USER:   
#
# This source code is subject to NVIDIA ownership rights under U.S. and 
# international Copyright laws.  
#
# NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE 
# CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR 
# IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH 
# REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF 
# MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.   
# IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL, 
# OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
# OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
# OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE 
# OR PERFORMANCE OF THIS SOURCE CODE.  
#
# U.S. Government End Users.  This source code is a "commercial item" as 
# that term is defined at 48 C.F.R. 2.101 (OCT 1995), consisting  of 
# "commercial computer software" and "commercial computer software 
# documentation" as such terms are used in 48 C.F.R. 12.212 (SEPT 1995) 
# and is provided to the U.S. Government only as a commercial end item.  
# Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through 
# 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the 
# source code with only those rights set forth herein.
#
################################################################################
#
# Build script for project
#
################################################################################

include ./findcudalib.mk

# Flags to detect either a Linux system (linux) or Mac OSX (darwin)
DARWIN = $(strip $(findstring DARWIN, $(OSUPPER)))

# Makefile include to help find GL Libraries
ifneq ($(DARWIN),)
  # This sample for NVCUVID is not supported on Mac OSX
  $(info >>> WARNING - cudaDecodeGL is not supported on Mac OSX waiving build <<<)
  EXEC          := @echo "@" 
else
  EXEC          ?=
  include ./findgllib.mk
  include ./findcudalib.mk
endif

# Location of the CUDA Toolkit binaries and libraries
CUDA_PATH       ?= /usr/local/cuda
CUDA_INC_PATH   ?= $(CUDA_PATH)/include
CUDA_BIN_PATH   ?= $(CUDA_PATH)/bin
ifneq ($(DARWIN),)
  CUDA_LIB_PATH  ?= $(CUDA_PATH)/lib
else
  ifeq ($(OS_SIZE),32)
    CUDA_LIB_PATH  ?= $(CUDA_PATH)/lib
  else
    CUDA_LIB_PATH  ?= $(CUDA_PATH)/lib64
  endif
endif

# Common binaries
GCC             ?= g++
NVCC            ?= $(CUDA_BIN_PATH)/nvcc -ccbin $(GCC)

# Extra user flags
EXTRA_NVCCFLAGS ?=
EXTRA_LDFLAGS   ?=
EXTRA_CC_FLAGS  ?=

# CUDA code generation flags
GENCODE_FLAGS   := -arch sm_11

# OS-specific build flags
ifneq ($(DARWIN),) 
      LDFLAGS   := -Xlinker -rpath $(CUDA_LIB_PATH) -L$(CUDA_LIB_PATH) -lcudart -lnvcuvid -lcuda
      CCFLAGS   := -arch $(OS_ARCH) 
else
      LDFLAGS   := -L$(CUDA_LIB_PATH) $(CUDALINK) $(GLLINK) -lcudart -lnvcuvid -lcuda 
  ifeq ($(OS_ARCH),armv7l)
    ifeq ($(abi),gnueabihf)
      LDFLAGS   += -Xlinker --dynamic-linker=/lib/ld-linux-armhf.so.3
      CCFLAGS   += -mfloat-abi=hard
    else
      # default to gnueabi
      override abi := gnueabi
      CCFLAGS   += -mfloat-abi=softfp
    endif
  else
    ifeq ($(OS_SIZE),32)
      CCFLAGS   := -m32
    else
      CCFLAGS   := -m64
    endif
  endif
endif

# OS-architecture specific flags
ifeq ($(OS_SIZE),32)
      NVCCFLAGS := -m32
  ifeq ($(ARMv7),1)
      NVCCFLAGS += -target-cpu-arch ARM
  endif
else
      NVCCFLAGS := -m64
endif
NVCCFLAGS += -maxrregcount=0 -ptx

# OpenGL specific libraries
ifneq ($(DARWIN),)
    # Mac OSX specific libraries and paths to include
      LIBPATH_OPENGL  := -L../../common/lib/$(OSLOWER) -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -lGL -lGLU ../../common/lib/$(OSLOWER)/libGLEW.a
else
    # Linux specific libraries and paths to include
    ifeq ($(OS_SIZE),32)
      LIBPATH_OPENGL  := -L/usr/X11R6/lib
    else
      LIBPATH_OPENGL  := -L/usr/X11R6/lib64
    endif
    LIBPATH_OPENGL  := -L../../common/lib/$(OSLOWER)/$(OS_ARCH) -lGL -lGLU -lX11 -lXmu -lglut ../../common/lib/$(OSLOWER)/$(OS_ARCH)/libGLEW.a
endif

# Debug build flags
ifeq ($(dbg),1)
      CCFLAGS   += -g
      NVCCFLAGS += -g -G
      TARGET := debug
else
      TARGET := release
endif


# Common includes and paths for CUDA
INCLUDES      := -I$(CUDA_INC_PATH) -I. -I.. -I../../common/inc -I$(CUDA_PATH)/extras/CUPTI/include
LDFLAGS       += $(LIBPATH_OPENGL)

ifeq ($(OS_SIZE),32)
      PTX_FILE := NV12ToARGB_drvapi_win32.ptx
else
      PTX_FILE := NV12ToARGB_drvapi_x64.ptx
endif
# Target rules
all: build

build: NV12ToARGB_drvapi.ptx cudaDecodeGL

NV12ToARGB_drvapi.ptx: NV12ToARGB_drvapi.cu
	$(EXEC) $(NVCC) $(NVCCFLAGS) $(EXTRA_NVCCFLAGS) $(GENCODE_FLAGS) $(INCLUDES) -o $@ -c $<
	$(EXEC) mkdir -p ./data
	$(EXEC) cp -f $@ ./data/$(PTX_FILE)
	$(EXEC) mkdir -p ../../bin/$(OS_ARCH)/$(OSLOWER)/$(TARGET)$(if $(abi),/$(abi))
	$(EXEC) cp -f $@ ../../bin/$(OS_ARCH)/$(OSLOWER)/$(TARGET)$(if $(abi),/$(abi))/$(PTX_FILE)

VideoSource.o: VideoSource.cpp
	$(EXEC) $(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<
	
VideoParser.o: VideoParser.cpp
	$(EXEC) $(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<
	
VideoDecoder.o: VideoDecoder.cpp
	$(EXEC) $(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<
	
videoDecodeGL.o: videoDecodeGL.cpp
	$(EXEC) $(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<
	
ImageGL.o: ImageGL.cpp
	$(EXEC) $(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<
	
FrameQueue.o: FrameQueue.cpp
	$(EXEC) $(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<
	
cudaProcessFrame.o: cudaProcessFrame.cpp
	$(EXEC) $(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<
	
cudaModuleMgr.o: cudaModuleMgr.cpp
	$(EXEC) $(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<

cudaDecodeGL: VideoSource.o VideoParser.o VideoDecoder.o videoDecodeGL.o ImageGL.o FrameQueue.o cudaProcessFrame.o cudaModuleMgr.o
	$(EXEC) $(GCC) $(CCFLAGS) -o $@ $+ $(LDFLAGS) $(EXTRA_LDFLAGS)
	$(EXEC) mkdir -p ../../bin/$(OS_ARCH)/$(OSLOWER)/$(TARGET)$(if $(abi),/$(abi))
	$(EXEC) cp $@ ../../bin/$(OS_ARCH)/$(OSLOWER)/$(TARGET)$(if $(abi),/$(abi))

run: build
	$(EXEC) ./cudaDecodeGL

clean:
	$(EXEC) rm -f cudaDecodeGL NV12ToARGB_drvapi.ptx VideoSource.o VideoParser.o VideoDecoder.o videoDecodeGL.o ImageGL.o FrameQueue.o cudaProcessFrame.o cudaModuleMgr.o data/$(PTX_FILE)
	$(EXEC) rm -rf ../../bin/$(OS_ARCH)/$(OSLOWER)/$(TARGET)$(if $(abi),/$(abi))/$(PTX_FILE)
	$(EXEC) rm -rf ../../bin/$(OS_ARCH)/$(OSLOWER)/$(TARGET)$(if $(abi),/$(abi))/cudaDecodeGL
