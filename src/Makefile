rwildcard				= $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
CXX_SRC                 = $(call rwildcard, , *.cpp)
CXX_OBJ                 = $(patsubst %.cpp, $($(ARTIFACT_VARS)_BIN)/%.o, $(CXX_SRC))
CXX_FILE                = $(patsubst $($(ARTIFACT_VARS)_BIN)/%.o, %.cpp, $(@))

CONFIG                 := "$(CURDIR)/.config.mk"

include $(CONFIG)

INCLUDE_DEPS           := $(foreach dep, $(DEPENDENCIES) $(ARTIFACT_VARS), $(if $($(dep)_INC), -I$($(dep)_INC)))

$(info $(LSP_TEST_FW_OBJ))
$(info CXX_SRC = $(CXX_SRC))
$(info CXX_OBJ = $(CXX_OBJ))
$(info INCLUDE_DEPS = $(INCLUDE_DEPS))

.PHONY: compile depend all

compile: $(LSP_TEST_FW_OBJ)

all: $(LSP_TEST_FW_OBJ)
	@echo "build all"

$(LSP_TEST_FW_OBJ): $(CXX_OBJ)
	@echo ""

$(CXX_OBJ):
	@echo "  $(CXX) $(CXX_FILE)"
	@mkdir -p $(dir $@)
	@echo $(CXX) -o $(@) -c $(CXX_FILE) -fPIC $(CXXFLAGS) $(INCLUDE) $(INCLUDE_DEPS)
	@$(CXX) -o $(@) -c $(CXX_FILE) -fPIC $(CXXFLAGS) $(INCLUDE) $(INCLUDE_DEPS)

  

