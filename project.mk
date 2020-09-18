# Package version
ARTIFACT_NAME               = lsp-test-fw
ARTIFACT_DEPS               = 
ARTIFACT_VARS               = LSP_TEST_FW
ARTIFACT_HEADERS            = lsp-plug.in
ARTIFACT_VERSION            = 1.0.6
ARTIFACT_EXPORT_ALL         = 1

# Weak property
DEMO_TEST                  := 1

# List of dependencies
DEPENDENCIES = \
  STDLIB \
  LSP_COMMON_LIB

TEST_DEPENDENCIES =

ALL_DEPENDENCIES = \
  $(DEPENDENCIES) \
  $(TEST_DEPENDENCIES)
