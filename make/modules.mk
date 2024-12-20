#
# Copyright (C) 2024 Linux Studio Plugins Project <https://lsp-plug.in/>
#           (C) 2024 Vladimir Sadovnikov <sadko4u@gmail.com>
#
# This file is part of lsp-test-fw
#
# lsp-test-fw is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# lsp-test-fw is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with lsp-test-fw.  If not, see <https://www.gnu.org/licenses/>.
#
ifneq ($(VERBOSE),1)
.SILENT:
endif

BASEDIR                := $(CURDIR)
CONFIG                 := $(BASEDIR)/.config.mk

include $(BASEDIR)/project.mk
include $(BASEDIR)/make/functions.mk
ifeq ($(TREE),1)
  include $(BASEDIR)/make/system.mk
  include $(BASEDIR)/make/tools.mk
  include $(BASEDIR)/modules.mk
else
  -include $(CONFIG)
endif
include $(BASEDIR)/dependencies.mk

MERGED_DEPENDENCIES        := \
  $(DEPENDENCIES) \
  $(TEST_DEPENDENCIES)
UNIQ_MERGED_DEPENDENCIES   := $(filter-out $(ARTIFACT_ID),$(call uniq, $(MERGED_DEPENDENCIES)))
UNIQ_ALL_DEPENDENCIES      := $(filter-out $(ARTIFACT_ID),$(call uniq, $(ALL_DEPENDENCIES)))

# Find the proper branch of the GIT repository
MODULES                    ?= $(BASEDIR)/modules
GIT                        ?= git

ifeq ($(DEVEL),1)
  X_URL_SUFFIX                = _RW
else
  X_URL_SUFFIX                = _RO
endif

ifeq ($(TREE),1)
  $(foreach dep,$(UNIQ_ALL_DEPENDENCIES), \
    $(eval $(dep)_URL=$($(dep)_URL$(X_URL_SUFFIX))) \
  )
  
  ifeq ($(findstring -devel,$(ARTIFACT_VERSION)),-devel)
    $(foreach dep, $(UNIQ_ALL_DEPENDENCIES), \
      $(eval $(dep)_BRANCH=devel) \
      $(eval $(dep)_PATH=$(MODULES)/$($(dep)_NAME)) \
    )
  else
    $(foreach dep, $(UNIQ_ALL_DEPENDENCIES), \
      $(eval $(dep)_BRANCH="$($(dep)_VERSION)") \
      $(eval $(dep)_PATH=$(MODULES)/$($(dep)_NAME)) \
    )
  endif
endif

# Form list of modules, exclude all modules that have 'system' version
SRC_MODULES         = $(foreach dep, $(UNIQ_MERGED_DEPENDENCIES), $(if $(findstring src,$($(dep)_TYPE)),$(dep)))
HDR_MODULES         = $(foreach dep, $(UNIQ_MERGED_DEPENDENCIES), $(if $(findstring hdr,$($(dep)_TYPE)),$(dep)))
ALL_SRC_MODULES     = $(foreach dep, $(UNIQ_ALL_DEPENDENCIES), $(if $(findstring src,$($(dep)_TYPE)),$(dep)))
ALL_HDR_MODULES     = $(foreach dep, $(UNIQ_ALL_DEPENDENCIES), $(if $(findstring hdr,$($(dep)_TYPE)),$(dep)))

# Branches
.PHONY: $(ALL_SRC_MODULES) $(ALL_HDR_MODULES)
.PHONY: fetch prune clean

$(ALL_SRC_MODULES) $(ALL_HDR_MODULES):
	echo "Cloning $($(@)_URL) -> $($(@)_PATH) [$($(@)_BRANCH)]"
	test -f "$($(@)_PATH)/.git/config" || $(GIT) clone "$($(@)_URL)" "$($(@)_PATH)"
	mkdir -p $(dir $($(@)_PATH))
	$(GIT) -C "$($(@)_PATH)" reset --hard
	$(GIT) -C "$($(@)_PATH)" fetch origin --force --prune --prune-tags
	$(GIT) -C "$($(@)_PATH)" fetch origin 'refs/tags/*:refs/tags/*' --force
	if $(GIT) -C "$($(@)_PATH)" rev-parse -q --verify "origin/$($(@)_BRANCH)" >/dev/null; then \
	  $(GIT) -c advice.detachedHead=false -C "$($(@)_PATH)" checkout -B "$($(@)_BRANCH)" "origin/$($(@)_BRANCH)" >/dev/null; \
	elif $(GIT) -C "$($(@)_PATH)" rev-parse -q --verify "refs/tags/$($(@)_BRANCH)" >/dev/null; then \
	  $(GIT) -c advice.detachedHead=false -C "$($(@)_PATH)" checkout "refs/tags/$($(@)_BRANCH)"; \
	elif $(GIT) -C "$($(@)_PATH)" rev-parse -q --verify "origin/$($(@)_NAME)-$($(@)_BRANCH)" >/dev/null; then \
	  $(GIT) -c advice.detachedHead=false -C "$($(@)_PATH)" checkout -B "$($(@)_NAME)-$($(@)_BRANCH)" "origin/$($(@)_NAME)-$($(@)_BRANCH)"; \
	else \
	  $(GIT) -c advice.detachedHead=false -C "$($(@)_PATH)" checkout "refs/tags/$($(@)_NAME)-$($(@)_BRANCH)"; \
	fi

fetch: $(SRC_MODULES) $(HDR_MODULES)

tree: $(ALL_SRC_MODULES) $(ALL_HDR_MODULES)

clean:
	echo rm -rf "$($(ARTIFACT_VARS)_BIN)/$(ARTIFACT_NAME)"
	-rm -rf "$($(ARTIFACT_VARS)_BIN)/$(ARTIFACT_NAME)"

prune:
	echo "Removing $(notdir $(MODULES))"
	-rm -rf $(MODULES)
	

