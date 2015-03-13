	LOCAL_PATH := $(call my-dir)
	
	include $(CLEAR_VARS)
	
	LOCAL_CFLAGS := 
	
	LOCAL_MODULE    := lib7z
	LOCAL_SRC_FILES :=\
	    ..\..\7zAlloc.c \
	    ..\..\7zArcIn.c \
	    ..\..\7zBuf.c \
	    ..\..\7zCrc.c \
	    ..\..\7zCrcOpt.c \
	    ..\..\7zDec.c \
	    ..\..\7zFile.c \
	    ..\..\7zStream.c \
	    ..\..\Bcj2.c \
	    ..\..\Bra.c \
	    ..\..\Bra86.c \
	    ..\..\CpuArch.c \
	    ..\..\Lzma2Dec.c \
	    ..\..\LzmaDec.c \
	    ..\..\Ppmd7.c \
		..\..\Ppmd7Dec.c
	
	
	LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/. \
	$(LOCAL_PATH)/../../
	
	include $(BUILD_STATIC_LIBRARY)
