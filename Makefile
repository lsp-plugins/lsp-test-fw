#!/usr/bin/make -f

# Setup variables
ARTIFACT_ID		    = lsp-test-fw
ARTIFACT_VARS       = LSP_TEST_FW_

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
.PHONY: config info help
.PHONY: gitmodules

all clean install uninstall depend:
	@test -f "$(CONFIG)" || (echo "$(CONFIG_MSG)" && exit 1)
	@$(MAKE) -c $(BASEDIR)/src $(@) CONFIG="$(CONFIG)" DESTDIR="$(DESTDIR)"

gitmodules:
	@test -f "$(CONFIG)" || (echo "$(CONFIG_MSG)" && exit 1)
	@$(MAKE) -c $(BASEDIR)/src $(@) CONFIG="$(CONFIG)"

config info:
	@echo "Configuring build..."
	@$(MAKE) -f "$(BASEDIR)/make/configure.mk" $(ARGS)

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
	@$(MAKE) -f "$(BASEDIR)/make/configure.mk" $(@)
