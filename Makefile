include make.local

DIRS 		= src test
V           = @

.PHONY: subdirs ${DIRS}

all: subdirs

subdirs: $(DIRS)

$(DIRS):
	$(V)make -C $@ 

SUBDIRS_CLEAN = $(patsubst %,%.clean,$(DIRS))
.PHONY: $(SUBDIRS_CLEAN)
$(SUBDIRS_CLEAN):
	@make -C $(@:.clean=) clean

.PHONY: clean
clean: $(SUBDIRS_CLEAN)
	rm -f config.status config.log
