LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)
LOCAL_MODULE := 7z
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/lib7z.a 
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := curl
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/libcurl.a 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ssl
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/libssl.a 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/libcrypto.a 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jsoncpp
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/libjsoncpp.a 
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := png
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/libpng.a 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := freetype
LOCAL_SRC_FILES := ../obj/local/$(TARGET_ARCH_ABI)/libfreetype.a 
include $(PREBUILT_STATIC_LIBRARY)




include $(CLEAR_VARS)
 
LOCAL_LDLIBS := -llog -lGLESv1_CM -lz
 
LOCAL_MODULE    := AppMain

LOCAL_C_INCLUDES :=\
$(LOCAL_PATH)/../../middleware\
$(LOCAL_PATH)/../../middleware/scene\
$(LOCAL_PATH)/../../middleware/tools\
$(LOCAL_PATH)/../../middleware/inc\
$(LOCAL_PATH)/../../middleware/inc/freetype\
$(LOCAL_PATH)/../../classes\




JMW_CPP_FILE_LIST := $(wildcard $(LOCAL_PATH)/../../middleware/*.cpp)
JMW_CPP_FILE_LIST += $(wildcard $(LOCAL_PATH)/../../middleware/**/*.cpp)
JMW_CPP_FILE_LIST += $(wildcard $(LOCAL_PATH)/../../classes/*.cpp)

LOCAL_SRC_FILES :=main.cpp \
JniFunc.cpp \
$(JMW_CPP_FILE_LIST:$(LOCAL_PATH)/%=%) 



LOCAL_CFLAGS += -D__ANDROID__ -std=c++11 -fexceptions -frtti -pthread 

LOCAL_STATIC_LIBRARIES := 7z curl ssl crypto jsoncpp png freetype
 
include $(BUILD_SHARED_LIBRARY)
