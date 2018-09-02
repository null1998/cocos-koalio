LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := main.cpp \
                   AppDelegate.cpp \
                   HelloWorldScene.cpp\
                   FirstScene.cpp\
                   GameLayer.cpp\
                   Player.cpp\
                   TileRect.cpp\
                   Tools.cpp\
LOCAL_C_INCLUDES := $(LOCAL_PATH)

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH))
$(call import-add-path, $(LOCAL_PATH)/../cocos2d)
$(call import-module, android/cpufeaures)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
