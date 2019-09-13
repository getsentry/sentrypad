PREMAKE_DIR := premake
PREMAKE := premake5
CPUS ?= $(shell getconf _NPROCESSORS_ONLN)

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "TARGETS:"
	@echo "   fetch"
	@echo "   fetch-breakpad"
	@echo "   fetch-crashpad"
	@echo "   clean"
	@echo "   clean-breakpad"
	@echo "   clean-crashpad"
	@echo "   clean-build"
	@echo ""
	@echo "LINUX ONLY:"
	@echo "   configure"
	@echo "   test"
.PHONY: help

# Dependency Download

fetch: fetch-breakpad fetch-crashpad
.PHONY: fetch

fetch-breakpad:
	breakpad/fetch_breakpad.sh
.PHONY: fetch-breakpad

fetch-crashpad:
	crashpad/fetch_crashpad.sh
.PHONY: fetch-crashpad

# Cleanup

clean: clean-breakpad clean-crashpad clean-build
.PHONY: clean

clean-breakpad:
	rm -rf breakpad/build
.PHONY: clean-breakpad

clean-crashpad:
	rm -rf crashpad/build
.PHONY: clean-crashpad

clean-build:
	git clean -xdf -e $(PREMAKE_DIR)/$(PREMAKE) -- $(PREMAKE_DIR)
.PHONY: clean-build

# Development on Linux / macOS.
# Does not work on Windows

configure: $(PREMAKE_DIR)/Makefile
.PHONY: configure

test: configure
	$(MAKE) -C $(PREMAKE_DIR) -j$(CPUS) test_sentry
	$(PREMAKE_DIR)/bin/Release/test_sentry
.PHONY: test

$(PREMAKE_DIR)/Makefile: $(PREMAKE_DIR)/$(PREMAKE) $(wildcard $(PREMAKE_DIR)/*.lua)
	@cd $(PREMAKE_DIR) && ./$(PREMAKE) gmake2
	@touch $@

$(PREMAKE_DIR)/$(PREMAKE):
	@echo "Downloading premake"
	$(eval UNAME_S := $(shell uname -s))
	$(eval PREMAKE_DIST := $(if $(filter Darwin, $(UNAME_S)), macosx, linux))
	@curl -sL https://github.com/premake/premake-core/releases/download/v5.0.0-alpha14/premake-5.0.0-alpha14-$(PREMAKE_DIST).tar.gz | tar xz -C $(PREMAKE_DIR)

linux-shell:
	@docker build -t getsentry/sentry-native .
	@docker run --rm -v ${PWD}:/work -it getsentry/sentry-native bash
.PHONY: linux-shell
