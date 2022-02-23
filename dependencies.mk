#
# Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
#           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
#
# This file is part of lsp-test-fw
#
# lsp-test-fw is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# lsp-runime-lib is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with lsp-test-fw.  If not, see <https://www.gnu.org/licenses/>.
#

# Weak property
DEMO_TEST                  := 1

# List of dependencies
DEPENDENCIES = \
  LIBPTHREAD \
  LSP_COMMON_LIB
  
TEST_DEPENDENCIES = 

# Platform-dependent
ifeq ($(PLATFORM),Windows)
  TEST_DEPENDENCIES += \
    LIBSHLWAPI
endif

# Overall system dependencies
ALL_DEPENDENCIES = \
  $(DEPENDENCIES) \
  $(TEST_DEPENDENCIES) \
  LIBSHLWAPI
