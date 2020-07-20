# Variables that describe dependencies
LSP_COMMON_LIB_VERSION     := 1.0.6
LSP_COMMON_LIB_NAME        := lsp-common-lib
LSP_COMMON_LIB_TYPE        := hdr
LSP_COMMON_LIB_URL         := https://github.com/sadko4u/$(LSP_COMMON_LIB_NAME).git

ifeq ($(PLATFORM),Windows)
  STDLIB_VERSION             := system
  STDLIB_TYPE                := opt
  STDLIB_LDFLAGS             := -lpthread -lshlwapi
else
  STDLIB_VERSION             := system
  STDLIB_TYPE                := opt
  STDLIB_LDFLAGS             := -lpthread
endif
