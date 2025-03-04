THIS_ROOT:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

include $(YAUL_INSTALL_ROOT)/share/build.pre.mk

# Required for library usage
include $(YAUL_INSTALL_ROOT)/share/build.mic3d.mk

# Each asset follows the format: <path>;<symbol>. Duplicates are removed
BUILTIN_ASSETS+= \
	assets/test.tex;test_tex \
	assets/test.pal;test_pal \

SH_PROGRAM:= vdp1-mic3d
SH_SRCS:= \
	source/saturn/vdp1-mic3d.c \
\
	source/saturn/meshes/mesh_torus.c \
	source/saturn/meshes/mesh_cube.c \
	source/saturn/meshes/mesh_m.c \
	source/saturn/meshes/mesh_i.c \
	source/saturn/meshes/mesh_c.c \
\
	source/saturn/graphics/graphics.c \
	source/saturn/graphics/graphics_mika.c \
	source/saturn/graphics/graphics_tails.c \
	source/saturn/graphics/graphics_baku.c \
\
    source/sattex.c

SH_CFLAGS+= -O2 -Isource -DDEBUG -g $(MIC3D_CFLAGS)
SH_LDFLAGS+= $(MIC3D_LDFLAGS)

IP_VERSION:= V1.000
IP_RELEASE_DATE:= 20160101
IP_AREAS:= JTUBKAEL
IP_PERIPHERALS:= JAMKST
IP_TITLE:= MIC 3D
IP_MASTER_STACK_ADDR:= 0x06004000
IP_SLAVE_STACK_ADDR:= 0x06001E00
IP_1ST_READ_ADDR:= 0x06004000
IP_1ST_READ_SIZE:= 0

include $(YAUL_INSTALL_ROOT)/share/build.post.iso-cue.mk
