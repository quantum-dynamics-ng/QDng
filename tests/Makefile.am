noinst_DATA = TGE-Cheby.qdi  TGE-GSPO.qdi  TGE-SIL.qdi  TGE-SPO.qdi \
	      TGP-Expec.qdi  TGP-Expec-SP.qdi  TGPMS-Cheby.qdi  TGPMS-GSPO.qdi

check_SCRIPTS = TGE TGP TLP TGO check.sh


TGE:
	for n in `ls TGE*.qdi`;\
	do\
	 name=`echo $$n | sed s/\.qdi//`;\
	 echo "Running test $$name"; \
	 ../src/qdng $${name}.qdi > $${name}.log;\
	done; \
	a=`grep "Normal termination" $${name}.log | wc -l`; \
	test "$$a" -eq 1 ||  echo "!!!$${name} Unclean termination" 

TGP:
	for n in `ls TGP*.qdi`;\
	do\
	 name=`echo $$n | sed s/\.qdi//`;\
	 echo "Running test $$name"; \
	 ../src/qdng $${name}.qdi > $${name}.log;\
	done; \
	a=`grep "Normal termination" $${name}.log | wc -l`; \
	test "$$a" -eq 1 ||  echo "!!!$${name} Unclean termination" 

TLP:
	for n in `ls TLP*.qdi`;\
	do\
	 name=`echo $$n | sed s/\.qdi//`;\
	 echo "Running test $$name"; \
	 ../src/qdng $${name}.qdi > $${name}.log;\
	done; \
	a=`grep "Normal termination" $${name}.log | wc -l`; \
	test "$$a" -eq 1 ||  echo "!!!$${name} Unclean termination" 

TGO:
	for n in `ls TGO*.qdi`;\
	do\
	 name=`echo $$n | sed s/\.qdi//`;\
	 echo "Running test $$name"; \
	 ../src/qdng $${name}.qdi > $${name}.log;\
	done; \
	a=`grep "Normal termination" $${name}.log | wc -l`; \
	test "$$a" -eq 1 ||  echo "!!!$${name} Unclean termination" 


check.sh:
	$(srcdir)/checktests.sh

CLEANFILES=  *.log wisdom gmon.out

clean-local:
	-rm -rf TGE-*/
	-rm -rf TGP-*/
	-rm -rf TGPMS-*/
	-rm -rf TGPLMS-*/
	-rm -rf TLP-*/
	-rm -rf TLPL-*/
	-rm -rf TGPL-*/
	-rm -rf TGO-*/

