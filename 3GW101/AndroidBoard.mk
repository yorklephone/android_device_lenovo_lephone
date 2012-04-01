
#LOCAL_PATH := device/lenovo/3GW101/

#
# Boot files
#
#ifeq ($(TARGET_PREBUILT_KERNEL),)
#TARGET_PREBUILT_KERNEL := $(LOCAL_PATH)/kernel
#endif

#file := $(INSTALLED_KERNEL_TARGET)
#ALL_PREBUILT += $(file)
#$(file): $(TARGET_PREBUILT_KERNEL) | $(ACP)
#	$(transform-prebuilt-to-target)

ifeq ($(KERNEL_DEFCONFIG),)
ifeq ($(TARGET_BUILD_VARIANT),eng)
    KERNEL_DEFCONFIG := lephone_002w_user_defconfig
else
    KERNEL_DEFCONFIG := lephone_002w_user_defconfig
endif
endif

include kernel/AndroidKernel.mk

file := $(INSTALLED_KERNEL_TARGET)
ALL_PREBUILT += $(file)
$(file) : $(TARGET_PREBUILT_KERNEL) | $(ACP)
	$(transform-prebuilt-to-target)



