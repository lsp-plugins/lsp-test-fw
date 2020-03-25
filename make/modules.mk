BASEDIR            := $(CURDIR)
CONFIG             := $(CURDIR)/.config.mk

include $(CONFIG)

# Tools
GIT                := git

# Form list of modules, exclude all modules that have 'system' version
MODULES             = $(foreach dep, $(DEPENDENCIES), $(if $(findstring system,$($(dep)_VERSION)),,$(dep)))

# Branches
.PHONY: $(MODULES)
.PHONY: fetch prune

$(MODULES):
	@echo "Cloning $($(@)_URL) -> $($(@)_PATH) [$($(@)_BRANCH)]"
	@test -f "$($(@)_PATH)/.git/config" || $(GIT) clone "$($(@)_URL)" "$($(@)_PATH)"
	@$(GIT) -C "$($(@)_PATH)" fetch "$($(@)_URL)" 'refs/heads/*:refs/tags/*' --force
	@$(GIT) -c advice.detachedHead=false -C "$($(@)_PATH)" checkout origin/$($(@)_BRANCH) || \
	 $(GIT) -c advice.detachedHead=false -C "$($(@)_PATH)" checkout refs/tags/$($(@)_BRANCH)

fetch: $(MODULES)

prune:
	@-find 'modules' -mindepth 1 -maxdepth 1 -type d -exec rm -rf '{}' \;

