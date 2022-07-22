TARGETS = paper.pdf

# Name the dependencies.
deps    := $(patsubst %.pdf,.deps/%.d,$(TARGETS))

all: $(deps) $(TARGETS)

%.pdf:
	pdflatex $(*F)
	bibtex $(*F)
	pdflatex $(*F)
	pdflatex $(*F)

clean:
	rm -rf *~ .deps *.aux *.bbl *.blg *.dvi *.log *.toc $(TARGETS)

# Build dependencies.
.deps/%.d : %.tex Makefile mktexdep
	-@mkdir -p $(dir $@)
	@echo Creating dependencies for $<
	@./mktexdep $< > $@

# Include auto-generated dependencies only if the goal is not 'clean'
ifneq ($(MAKECMDGOALS),clean)
include $(foreach dep,$(deps),$(wildcard $(dep)))
endif
