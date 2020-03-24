#!/usr/bin/make -f

# Basic initialization
include project.mk
export ARTIFACT_ID
export VERSION 

# Setup variables
ARTIFACT_VARS       = LSP_TEST_FW

# Setup paths
BASEDIR             = ${CURDIR}
CONFIG             ?= ${CURDIR}/.config.mk
CONFIG_MSG          = System not properly configured. Please launch 'make config' first

# Checks
ifeq ("$(wildcard $(CONFIG))", "")
  CONFIGURED          = 0
else
  CONFIGURED          = 1
endif

export ARTIFACT_ID
export ARTIFACT_VARS

.DEFAULT_GOAL      := all
.PHONY: all clean install uninstall depend
.PHONY: config unconfig info help
.PHONY: gitmodules

all install uninstall depend:
	@test -f "$(CONFIG)" || (echo "$(CONFIG_MSG)" && exit 1)
	@$(MAKE) -s -c $(BASEDIR)/src $(@) CONFIG="$(CONFIG)" DESTDIR="$(DESTDIR)"
	
clean:
	@echo "Cleaning build directory $($(ARTIFACT_VARS)_BUILD)/$(ARTIFACT_ID)"
	@test -f "$(CONFIG)" || (echo "$(CONFIG_MSG)" && exit 1)
	@-rm -rf $($(ARTIFACT_VARS)_BUILD)/$(ARTIFACT_ID)
	@echo "Clean OK"

gitmodules:
	@test -f "$(CONFIG)" || (echo "$(CONFIG_MSG)" && exit 1)
	@$(MAKE) -s -c $(BASEDIR)/src $(@) CONFIG="$(CONFIG)"

config info:
	@echo "Configuring build..."
	@$(MAKE) -s -f "$(BASEDIR)/make/configure.mk" $(@) CONFIG="$(CONFIG)" $(MAKEFLAGS)
	@echo "Configure OK"

unconfig:
	@echo "Cleaning config..."
	@$(MAKE) -s -f "$(BASEDIR)/make/configure.mk" $(@) CONFIG="$(CONFIG)"
	@echo "Configuration clean OK"

help:
	@echo "Available targets:"
	@echo "  all            Build all binaries"
	@echo "  clean          Clean all binaries"
	@echo "  config         Configure build"
	@echo "  depend         Update build dependencies"
	@echo "  help           Print this help message"
	@echo "  info           Output build configuration"
	@echo "  install        Install all binaries into the system"
	@echo "  uninstall      Uninstall binaries"
	@echo ""
	@echo "List of available variables:"
	@$(MAKE) -s -f "$(BASEDIR)/make/configure.mk" $(@)
	@echo ""
