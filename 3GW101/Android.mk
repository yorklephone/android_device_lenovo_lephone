LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_DEVICE),3GW101)
    include $(call all-makefiles-under,$(LOCAL_PATH))
endif
