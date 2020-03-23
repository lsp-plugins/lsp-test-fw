BASEDIR             = ${CURDIR}
CONFIG             ?= ${CURDIR}/.config.mk

# Define predefined variables
$(ARTIFACT_VARS)_PATH      ?= $(BASEDIR)
$(ARTIFACT_VARS)_BUILD     ?= $($(ARTIFACT_VARS)_PATH)/.build
$(ARTIFACT_VARS)_INC       ?= $($(ARTIFACT_VARS)_PATH)/include
$(ARTIFACT_VARS)_LIB       ?= $(ARTIFACT_ID)
$(ARTIFACT_VARS)_OBJ       ?= $(ARTIFACT_ID)
$(ARTIFACT_VARS)_SRC       ?= $($(ARTIFACT_VARS)_PATH)/src
$(ARTIFACT_VARS)_TEST      ?= $($(ARTIFACT_VARS)_PATH)/test

# Configure system properties
-include $(CONFIG)
include ${CURDIR}/make/system.mk

AFFECTED_VARS = \
	$(COMMON_VARS) \
	$(ARTIFACT_VARS)_BUILD \
	$(ARTIFACT_VARS)_INC \
	$(ARTIFACT_VARS)_LIB \
	$(ARTIFACT_VARS)_OBJ \
	$(ARTIFACT_VARS)_PATH \
	$(ARTIFACT_VARS)_SRC \
	$(ARTIFACT_VARS)_TEST

GREP_VARS = $(shell echo "$(AFFECTED_VARS)" | sed "s/ \+/\|/g")

.DEFAULT_GOAL      := help
.PHONY: config unconfig info help
.PHONY: prepare
.PHONY: $(AFFECTED_VARS)

help:
	@echo "  $(ARTIFACT_VARS)_BUILD   location of the project build files"
	@echo "  $(ARTIFACT_VARS)_INC     path to include files"
	@echo "  $(ARTIFACT_VARS)_LIB     path to output shared object/library file"
	@echo "  $(ARTIFACT_VARS)_OBJ     path to output object file"
	@echo "  $(ARTIFACT_VARS)_PATH    location of the project"
	@echo "  $(ARTIFACT_VARS)_SRC     path to source code files"
	@echo "  $(ARTIFACT_VARS)_TEST    path to source test files"

prepare:
	@test -f "$(CONFIG)" || echo "# Configuration file" >"$(CONFIG)"
	@grep -E -v "$(GREP_VARS)" "$(CONFIG)" >"$(CONFIG).tmp"

$(AFFECTED_VARS): prepare
	@echo "$(@)=$($(@))" >> "$(CONFIG).tmp"

config: $(AFFECTED_VARS)
	@mv -f "$(CONFIG).tmp" "$(CONFIG)"

cleanconfig:
	@-rm -f "$(CONFIG)"

