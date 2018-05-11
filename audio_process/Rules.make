# Note! At the end of the file ../Rules.make will be included and possibly
# override most of these variables. This allows the DVSDK to share one
# Rules.make while still allowing DMAI to be used standalone. Also, currently
# the DM6446 and DM6467 does not share the same components, so special DM6446
# components are defined here. These should eventually be aligned.

#cross complile evn: rfs_include
COMPILE_INCLUDE := /opt/nvidia/Linux_for_Tegra_64_tx1/rootfs/usr/include/

#cross complile evn: rfs
ROOTGS_PATH := /opt/nvidia/Linux_for_Tegra_64_tx1/rootfs

#cross complile tools
COMPILE_PREFIX := aarch64-linux-gnu-
CC			:= $(COMPILE_PREFIX)gcc
CXX     := $(COMPILE_PREFIX)g++
AR			:= $(COMPILE_PREFIX)ar
LD			:= $(COMPILE_PREFIX)gcc
RANLIB 	:= $(COMPILE_PREFIX)ranlib
#ARFLAGS := $(COMPILE_PREFIX)rcs

TARGET_CPU = arm









