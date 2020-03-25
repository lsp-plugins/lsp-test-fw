#!/usr/bin/make -f

# Location
BASEDIR                     ?= ${CURDIR}
MODULES                     ?= $(BASEDIR)/modules

# Basic initialization
# Checks
ifeq ("$(wildcard $(CONFIG))", "")
  CONFIGURED          = 0
else
  CONFIGURED          = 1
endif

# Setup paths
BASEDIR            := $(CURDIR)
CONFIG             := $(BASEDIR)/.config.mk
CHK_CONFIG          = test -f "$(CONFIG)" || (echo "System not properly configured. Please launch 'make config' first" && exit 1)

.DEFAULT_GOAL      := all
.PHONY: all clean prune install uninstall depend

all install uninstall depend:
	@$(CHK_CONFIG)
	@$(MAKE) -s -c $(BASEDIR)/src $(@) CONFIG="$(CONFIG)" DESTDIR="$(DESTDIR)"
	
clean:
	@echo "Cleaning build directory $($(ARTIFACT_VARS)_BUILD)/$(ARTIFACT_ID)"
	@$(CHK_CONFIG)
	@-rm -rf $($(ARTIFACT_VARS)_BUILD)/$(ARTIFACT_ID)
	@echo "Clean OK"

prune:
	@echo "Pruning the whole project tree"
	@$(MAKE) -s -f "make/modules.mk" $(@) BASEDIR="$(BASEDIR)" CONFIG="$(CONFIG)"
	@echo "Prune OK"

# Git-related tasks
.PHONY: fetch
fetch:
	@echo "Fetching source code dependencies"
	@$(MAKE) -s -f "make/modules.mk" $(@) BASEDIR="$(BASEDIR)" CONFIG="$(CONFIG)"
	@echo "Fetch OK"

# Configuration-related targets
.PHONY: config help

config:
	@$(MAKE) -s -f "$(BASEDIR)/make/configure.mk" $(@) CONFIG="$(CONFIG)" $(MAKEFLAGS)

help:
	@echo "Available targets:"
	@echo "  all                       Build all binaries"
	@echo "  clean                     Clean all binaries"
	@echo "  config                    Configure build"
	@echo "  depend                    Update build dependencies"
	@echo "  fetch                     Fetch all source code dependencies from git"
	@echo "  help                      Print this help message"
	@echo "  info                      Output build configuration"
	@echo "  install                   Install all binaries into the system"
	@echo "  uninstall                 Uninstall binaries"
	@echo ""
	@$(MAKE) -s -f "$(BASEDIR)/make/configure.mk" $(@)
	@echo ""
