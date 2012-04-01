# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include device/lenovo/common/BoardConfigCommon.mk
TARGET_BOOTLOADER_BOARD_NAME := 3GC101
TARGET_SPECIFIC_HEADER_PATH := device/lenovo/3GC101/include
TARGET_IS_LEPHONE_002E := true
BOARD_KERNEL_CMDLINE := console=ttyMSM2,115200n8 androidboot.hardware=3GC101