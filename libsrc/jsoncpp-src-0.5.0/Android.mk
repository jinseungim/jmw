LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp LOCAL_MODULE := libJsoncpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_SRC_FILES := ../jsoncpp/src/lib_json/json_reader.cpp \ 
../jsoncpp/src/lib_json/json_value.cpp \ 
../jsoncpp/src/lib_json/json_writer.cpp

include $(BUILD_SHARED_LIBRARY)