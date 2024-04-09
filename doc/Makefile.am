

pkgdocdir = $(datadir)/doc/qdng

dist_pkgdoc_DATA = Handbook.tex qdng.bib \
	tutorial/example_1_qd_setup_input.m \
	tutorial/example_2_qd_check_efs.m \
	tutorial/example_3_qd_setup_propa.m \
	tutorial/example_4_qd_check_propa.m \
	tutorial/efs.qds \
  tutorial/propa.qds

if ENABLE_HANDBOOK
DOC_BASENAME = Handbook
TEX_SOURCE = $(DOC_BASENAME).tex
PDF_OUTPUT = $(DOC_BASENAME).pdf


# if HAVE_PDFLATEX
pdf_DATA = $(PDF_OUTPUT)
pkgdoc_DATA = $(PDF_OUTPUT)

$(PDF_OUTPUT): $(TEX_SOURCE)
	pdflatex $(TEX_SOURCE)
	bibtex $(DOC_BASENAME)
	pdflatex $(TEX_SOURCE)
	pdflatex $(TEX_SOURCE)

install-pdf: $(PDF_OUTPUT)
	$(MKDIR_P) $(DESTDIR)$(pkgdocdir)
	$(INSTALL_DATA) $(PDF_OUTPUT) $(DESTDIR)$(pkgdocdir)

all-local: $(PDF_OUTPUT)
install-data-hook: install-pdf

#  MANNAME = Handbook
#  # dist_pkgdoc_DATA += $(MANNAME).pdf
# # docfiles += $(MANNAME).pdf
#  MANTEXSRC = $(MANNAME).tex
#  MANPDF = $(MANNAME).pdf
#
CLEANFILES = $(MANNAME).log $(MANNAME).out $(MANNAME).aux \
					 $(MANNAME).toc $(MANNAME).bbl $(MANNAME).blg
# endif

endif
