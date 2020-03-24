# Package version
ARTIFACT_ID=lsp-test-fw
VERSION=1.0.0-devel

# List of dependencies
DEPENDENCIES=LSP_COMMON_LIB

# Variables that describe dependencies
LSP_COMMON_LIB_ID=lsp-common-lib
LSP_COMMON_LIB_VERSION=1.0.0
LSP_COMMON_LIB_URL=https://github.com/sadko4u/$(LSP_COMMON_LIB_ID).git
LSP_COMMON_LIB_PATH=$(MODULES)/$(LSP_COMMON_LIB_ID)
LSP_COMMON_LIB_INC=$(LSP_COMMON_LIB_PATH)/include
LSP_COMMON_LIB_SRC=$(LSP_COMMON_LIB_PATH)/src
#LSP_COMMON_LIB_TEST=

