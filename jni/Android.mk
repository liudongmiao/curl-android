LOCAL_PATH:= $(call my-dir)

ANDROID_ROOT := /mnt/data/android
CFLAGS := -Wpointer-arith -Wwrite-strings -Wunused -Winline -Wnested-externs -Wmissing-declarations -Wmissing-prototypes -Wno-long-long -Wfloat-equal -Wno-multichar -Wsign-compare -Wno-format-nonliteral -Wendif-labels -Wstrict-prototypes -Wdeclaration-after-statement -Wno-system-headers -DHAVE_CONFIG_H

include $(CLEAR_VARS)
include $(LOCAL_PATH)/curl/lib/Makefile.inc
LOCAL_MODULE := static-curl
LOCAL_SRC_FILES := $(addprefix curl/lib/,$(CSOURCES))
LOCAL_C_INCLUDES += $(LOCAL_PATH)/curl/include $(LOCAL_PATH)/curl/lib $(LOCAL_PATH)/ares $(ANDROID_ROOT)/external/openssl/include
LOCAL_CFLAGS += $(CFLAGS) -DHAVE_CONFIG_H
LOCAL_STATIC_LIBRARIES := static-ares
LOCAL_LDFLAGS := -lssl -lcrypto -lz -L$(ANDROID_ROOT)/out/target/product/generic/system/lib
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
include $(LOCAL_PATH)/ares/Makefile.inc
LOCAL_MODULE := static-ares
LOCAL_SRC_FILES := $(addprefix ares/,$(CSOURCES))
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ares
LOCAL_CFLAGS += $(CFLAGS) -DHAVE_CONFIG_H
include $(BUILD_STATIC_LIBRARY)

# include $(BUILD_SHARED_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := curl
LOCAL_SRC_FILES := curl.c
LOCAL_STATIC_LIBRARIES := static-curl
LOCAL_C_INCLUDES += $(LOCAL_PATH)/curl/include $(LOCAL_PATH)/curl/lib
LOCAL_CFLAGS += -Wall
LOCAL_LDFLAGS += -llog -lssl -lcrypto -lz -L$(ANDROID_ROOT)/out/target/product/generic/system/lib
include $(BUILD_SHARED_LIBRARY)
