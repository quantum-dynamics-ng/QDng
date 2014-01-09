dist_pkgdata_DATA = TGE-Arnoldi.qdi TGE-Cheby.qdi TGE-GSPO.qdi TGE-SIL.qdi TGE-SPO.qdi TGO-IR-Freq.qdi \
			  TGO-IR-Krotov-Projection.qdi TGO-IR-Krotov.qdi TGO-IR-Rabitz.qdi TGO-UV-Krotov.qdi TGP-Expec-HOFD.qdi \
			  TGP-Expec.qdi TGP-Expec-SP.qdi TGP-Gmat.qdi TGP-Kick.qdi TGPL-IR-Pulse-Cheby.qdi TGPL-IR-Pulse-GSPO.qdi \
			  TGPLMS-Cheby.qdi TGPLMS-SIL.qdi TGPMS-Cheby.qdi TGPMS-GSPO.qdi TGP-NIP-Cheby.qdi TLO-Cheby.qdi \
			  TLP-C-Cheby.qdi TLP-C-GSPO.qdi TLP-H-Cheby.qdi TLP-H-GSPO.qdi TLPL-Cheby.qdi TLPL-GSPO.qdi \
			  checktests.sh \
			  GridDip01.meta GridGauss.meta GridLaser_IR.meta GridLaser_w0.176.meta GridPot0_ef_2.wf GridPot0.op \
			  GridShapeIR.op LevelCmat.op Level_ef_1.wf LevelHmat.meta ShapeLevel.meta GridDip01.op GridGauss.wf \
			  GridLaser_IR.op GridLaser_w0.176.op GridPot0_ef_3.wf GridPot1.meta GridShapeUV.meta LevelDIPmat.meta \
			  Level_ef.meta LevelHmat.op ShapeLevel.op GridDip0.meta GridGM_00.meta GridLaser_w0.00713.meta GridPot0_ef_0.wf \
			  GridPot0_ef.meta GridPot1.op GridShapeUV.op LevelDIPmat.op LevelH2mat.meta LevelLaser_w0.15.meta \
			  GridDip0.op GridGM_00.op GridLaser_w0.00713.op GridPot0_ef_1.wf GridPot0.meta GridShapeIR.meta \
			  LevelCmat.meta Level_ef_0.wf LevelH2mat.op LevelLaser_w0.15.op
			   


check_SCRIPTS = TGE TGP TLP TLO TGO check.sh


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

TLO:
	for n in `ls TLO*.qdi`;\
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
	-rm -rf TLO-*/

