# Definitions
PREFIX                     := /usr/local
BASEDIR                    := $(CURDIR)
BUILDDIR                   := ${BASEDIR}/.build
CONFIG                     := ${BASEDIR}/.config.mk
MODULES                    := ${BASEDIR}/modules

include $(BASEDIR)/make/system.mk
include $(BASEDIR)/project.mk
include $(BASEDIR)/dependencies.mk

ifeq ($(findstring devel, $(VERSION)),devel)
  $(foreach dep, $(DEPENDENCIES), \
    $(eval \
      $(dep)_BRANCH=devel \
    ) \
  )
else
  $(foreach dep, $(DEPENDENCIES), \
    $(eval \
      $(dep)_BRANCH="$($(dep)_NAME)-$($(dep)_VERSION)" \
    ) \
  )
endif

define pkgconfig =
  name := $(1)
  $(if $($(name)_CFLAGS),,  $(eval $(name)_CFLAGS  := $(shell pkg-config --cflags "$($(name)_NAME)")))
  $(if $($(name)_LDLAGS),,  $(eval $(name)_LDFLAGS := $(shell pkg-config --libs "$($(name)_NAME)")))
  $(if $($(name)_OBJ),,     $(eval $(name)_OBJ     :=)))
endef

define bldconfig =
  name := $(1)

  $(if $($(name)_CFLAGS),,  $(eval $(name)_CFLAGS  := "-I$($(name)_INC)"))
  $(if $($(name)_LDLAGS),,  $(eval $(name)_LDFLAGS :=))
  $(if $($(name)_OBJ),,     $(eval $(name)_OBJ     := "$($(name)_BIN)/$($(name)_NAME).o"))
endef

define vardef =
  name := $(1)
  # Override variables if they are not defined
  $(if $($(name)_PATH),,    $(eval $(name)_PATH    := $(MODULES)/$($(name)_NAME)))
  $(if $($(name)_INC),,     $(eval $(name)_INC     := $($(name)_PATH)/include))
  $(if $($(name)_SRC),,     $(eval $(name)_SRC     := $($(name)_PATH)/src))
  $(if $($(name)_TEST),,    $(eval $(name)_TEST    := $($(name)_PATH)/test))
  $(if $($(name)_BIN),,     $(eval $(name)_BIN     := $(BUILDDIR)/$($(name)_NAME)))
  
  $(if $(findstring system, $($(name)_VERSION)), \
    $(eval $(call pkgconfig, $(name))), \
    $(eval $(call bldconfig, $(name))) \
  )
endef

# Define predefined variables
$(ARTIFACT_VARS)_NAME      := $(ARTIFACT_NAME)
$(ARTIFACT_VARS)_VERSION   := $(VERSION)
$(ARTIFACT_VARS)_PATH      := $(BASEDIR)

$(info $(ARTIFACT_VARS)_PATH=$($(ARTIFACT_VARS)_PATH))

$(foreach name, $(DEPENDENCIES) $(ARTIFACT_VARS), $(eval $(call vardef, $(name))))

$(info $(ARTIFACT_VARS)_PATH=$($(ARTIFACT_VARS)_PATH))

CONFIG_VARS = \
  $(COMMON_VARS) \
  $(foreach name, $(DEPENDENCIES) $(ARTIFACT_VARS), \
    $(name)_VERSION \
    $(name)_BRANCH \
    $(name)_PATH \
    $(name)_INC \
    $(name)_SRC \
    $(name)_TEST \
    $(name)_BIN \
    $(name)_CFLAGS \
    $(name)_LDFLAGS \
    $(name)_OBJ \
  )

$(info CONFIG_VARS: $(CONFIG_VARS))

.DEFAULT_GOAL      := config
.PHONY: config
.PHONY: $(CONFIG_VARS)

prepare:
	@echo "Configuring build..."
	@test -f "$(CONFIG)" || echo "# Configuration file" >"$(CONFIG)"

$(CONFIG_VARS): prepare
	@echo "$(@)=$($(@))" >> "$(CONFIG).tmp"

config: $(CONFIG_VARS)
	@mv -f "$(CONFIG).tmp" "$(CONFIG)"
	@echo "Configured OK"
