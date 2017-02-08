

pkgdocdir = $(datadir)/doc/qdng
dist_pkgdoc_DATA = Handbook.tex qdng.bib \ 
	tutorial/example_1_qd_setup_input.m \
	tutorial/example_2_qd_check_efs.m \
	tutorial/example_3_qd_setup_propa.m \
	tutorial/example_4_qd_check_propa.m \
	tutorial/efs.qds \
        tutorial/propa.qds 


if HAVE_PDFLATEX
 MANNAME = Handbook
 dist_pkgdoc_DATA += $(MANNAME).pdf
# docfiles += $(MANNAME).pdf
 MANTEXSRC = $(MANNAME).tex
 MANPDF = $(MANNAME).pdf

 CLEANFILES = $(MANNAME).log $(MANNAME).out $(MANNAME).aux \
             $(MANNAME).toc $(MANNAME).bbl $(MANNAME).blg

 $(MANPDF): $(srcdir)/$(MANTEXSRC)
	$(PDFLATEX) $(srcdir)/$(MANNAME).tex
	BIBINPUTS=$(srcdir) bibtex $(MANNAME) 
	$(PDFLATEX) $(srcdir)/$(MANNAME).tex
	$(PDFLATEX) $(srcdir)/$(MANNAME).tex
endif

