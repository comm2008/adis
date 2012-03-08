include Makefile.inc

SUBDIRS = src
BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

# Not really necessary now since there is only one
# source directory, but may be necessary later if more than one
# directory with source files exists

.PHONY: all $(BUILDDIRS)
all: $(BUILDDIRS)

$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)

.PHONY: clean $(CLEANDIRS)
clean: $(CLEANDIRS)

$(CLEANDIRS):
	$(MAKE) -C $(@:clean-%=%) clean
