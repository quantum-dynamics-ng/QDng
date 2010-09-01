noinst_DATA = TGE-Cheby.qdi  TGE-GSPO.qdi  TGE-SIL.qdi  TGE-SPO.qdi \
	      TGP-Expec.qdi  TGP-Expec-SP.qdi  TGPMS-Cheby.qdi  TGPMS-GSPO.qdi

check_SCRIPTS = eigen.sh propa.sh check.sh


eigen.sh:
	for n in `ls TGE*.qdi`;\
	do\
	 name=`echo $$n | sed s/\.qdi//`;\
	 echo "Running test $$name"; \
	 ../src/qdng $${name}.qdi > $${name}.log;\
	done 


propa.sh:
	for n in `ls TGP*.qdi`;\
	do\
	 name=`echo $$n | sed s/\.qdi//`;\
	 echo "Running test $$name"; \
	 ../src/qdng $${name}.qdi > $${name}.log;\
	done 

check.sh:
	$(srcdir)/checktests.sh

CLEANFILES=  *.log
