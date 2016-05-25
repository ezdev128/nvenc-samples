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
#  findgllib.mk is used to find the necessary GL Libraries for specific distributions
#               this is supported on Mac OSX and Linux Platforms
#
################################################################################

DISTRO  = $(shell lsb_release -i -s)

# OS Name (Linux or Darwin)
OSUPPER = $(shell uname -s 2>/dev/null | tr "[:lower:]" "[:upper:]")
OSLOWER = $(shell uname -s 2>/dev/null | tr "[:upper:]" "[:lower:]")

ifeq ("$(OSUPPER)","LINUX")
    # $(info) >> findgllib.mk -> LINUX path <<<)
    # Each set of Linux Distros have different paths for where to find their OpenGL libraries reside
    ifeq ("$(DISTRO)","Ubuntu")
      ifeq ($(OS_SIZE),64)
        GLPATH    := /usr/lib64 /usr/lib/nvidia-current /usr/lib32/nvidia-current
        GLLINK    := -L/usr/lib64 -L/usr/lib/nvidia-current -L/usr/lib32/nvidia-current
      else
        GLPATH    := /usr/lib/nvidia-current
        GLLINK    := -L/usr/lib/nvidia-current
      endif
    endif
    ifeq ("$(DISTRO)","Debian")
      ifeq ($(OS_SIZE),64)
        GLPATH    := /usr/lib64 /usr/lib/nvidia-current /usr/lib32/nvidia-current
        GLLINK    := -L/usr/lib64 -L/usr/lib/nvidia-current -L/usr/lib32/nvidia-current
      else
        GLPATH    := /usr/lib/nvidia-current
        GLLINK    := -L/usr/lib/nvidia-current
      endif
    endif
    ifeq ("$(DISTRO)","SUSE")
      ifeq ($(OS_SIZE),64)
        GLPATH    := /usr/lib64 /usr/X11R6/lib64  /usr/X11R6/lib
        GLLINK    := -L/usr/lib64 -L/usr/X11R6/lib64 -L/usr/X11R6/lib
      else
        GLPATH    := /usr/X11R6/lib
        GLLINK    := -L/usr/X11R6/lib
      endif
    endif
    ifeq ("$(DISTRO)","Fedora")
      ifeq ($(OS_SIZE),64)
        GLPATH    := /usr/lib64 /usr/lib64/nvidia
        GLLINK    := -L/usr/lib64 -L/usr/lib64/nvidia
      else
        GLPATH    := /usr/lib/nvidia
        GLLINK    := -L/usr/lib/nvidia
      endif
    endif
    ifeq ("$(DISTRO)","Redhat")
      ifeq ($(OS_SIZE),64)
        GLPATH    := /usr/lib64 /usr/lib64/nvidia
        GLLINK    := -L/usr/lib64 -L/usr/lib64/nvidia
      else
        GLPATH    := /usr/lib/nvidia
        GLLINK    := -L/usr/lib/nvidia
      endif
    endif
    ifeq ("$(DISTRO)","CentOS")
      ifeq ($(OS_SIZE),64)
        GLPATH    := /usr/lib64 /usr/lib64/nvidia
        GLLINK    := -L/usr/lib64 -L/usr/lib64/nvidia
      else
        GLPATH    := /usr/lib/nvidia
        GLLINK    := -L/usr/lib/nvidia
      endif
    endif
  
    ifeq ($(ARMv7),1)
      ifeq ($(OS_SIZE),64)
        GLLINK    := -L/usr/lib/x86_64-linux-gnu
      else
        GLLINK    := -L/usr/lib/i386-linux-gnu
      endif
    endif

  # find libGL, libGLU, libXi, 
  GLLIB           := $(shell find $(GLPATH) -name libGL.so  -print 2>/dev/null)
  GLULIB          := $(shell find $(GLPATH) -name libGLU.so -print 2>/dev/null)
  X11LIB          := $(shell find $(GLPATH) -name libX11.so -print 2>/dev/null)
  XILIB           := $(shell find $(GLPATH) -name libXi.so  -print 2>/dev/null)
  XMULIB          := $(shell find $(GLPATH) -name libXmu.so -print 2>/dev/null)

  ifeq ($(GLLIB),'')
      $(info >>> WARNING - libGL.so not found, refer to CUDA Samples release notes for how to find and install them. <<<)
      EXEC=@echo "[@]"
  endif
  ifeq ($(GLULIB),'')
      $(info >>> WARNING - libGLU.so not found, refer to CUDA Samples release notes for how to find and install them. <<<)
      EXEC=@echo "[@]"
  endif
  ifeq ($(X11LIB),'')
      $(info >>> WARNING - libX11.so not found, refer to CUDA Samples release notes for how to find and install them. <<<)
      EXEC=@echo "[@]"
  endif
  ifeq ($(XILIB),'')
      $(info >>> WARNING - libXi.so not found, refer to CUDA Samples release notes for how to find and install them. <<<)
      EXEC=@echo "[@]"
  endif
  ifeq ($(XMULIB),'')
      $(info >>> WARNING - libXmu.so not found, refer to CUDA Samples release notes for how to find and install them. <<<)
      EXEC=@echo "[@]"
  endif
else
    # $(info) >> findgllib.mk -> DARWIN path <<<)
endif

