BASEDIR            ?= ${CURDIR}
MODULES            ?= $(BASEDIR)/modules

include project.mk

# Tools
GIT                ?= git

DEVEL_TEST          = $(shell echo '$(VERSION)' | sed -E 's/.*devel.*/devel/')

# Determine proper branch/tag name
ifeq ($(DEVEL_TEST),devel)
  $(foreach dep, $(DEPENDENCIES), $(eval $(dep)_BRANCH=devel))
else
  $(foreach dep, $(DEPENDENCIES), $(eval $(dep)_BRANCH="$($(dep)_ID)-$($(dep)_VERSION)"))
endif

PATHS=$(foreach dep, $(DEPENDENCIES), $($(dep)_PATH))

# Branches
.PHONY: $(DEPENDENCIES)
.PHONY: tree

$(DEPENDENCIES):
	@echo "Cloning $($(@)_URL) -> $($(@)_PATH) [$($(@)_VERSION)]"
	@test -f "$($(@)_PATH)/.git/config" || $(GIT) clone "$($(@)_URL)" "$($(@)_PATH)"
	@$(GIT) -C "$($(@)_PATH)" fetch "$($(@)_URL)" 'refs/heads/*:refs/tags/*' --force
	@$(GIT) -c advice.detachedHead=false -C "$($(@)_PATH)" checkout origin/$($(@)_BRANCH) || \
	 $(GIT) -c advice.detachedHead=false -C "$($(@)_PATH)" checkout refs/tags/$($(@)_BRANCH)

tree: $(DEPENDENCIES)
	@echo "Fetching dependencies OK"

untree:
	@-rm -rf $(PATHS)
