# Package version
ARTIFACT_NAME               = lsp-test-fw
ARTIFACT_DESC               = Test framework for executing automated and manual tests
ARTIFACT_ID                 = LSP_TEST_FW
ARTIFACT_HEADERS            = lsp-plug.in
ARTIFACT_VERSION            = 1.0.7-devel
ARTIFACT_EXPORT_ALL         = 1

# Weak property
DEMO_TEST                  := 1

# List of dependencies
TEST_DEPENDENCIES = \
  LIBPTHREAD \
  LSP_COMMON_LIB
  
DEPENDENCIES = 

# Platform-dependent
ifeq ($(PLATFORM),Windows)
  TEST_DEPENDENCIES += \
    LIBSHLWAPI
endif

# Overall system dependencies
ALL_DEPENDENCIES = \
  $(DEPENDENCIES) \
  $(TEST_DEPENDENCIES) \
  LIBSHLWAPI
