#!/bin/bash


CheckTGE-im-Energy()
{
   a=`grep '0	0.003739' ${1}-im/energy.dat`
   test -n "$a" || return -1
   a=`grep '1	0.011152' ${1}-im/energy.dat`
   test -n "$a" || return -1
   a=`grep '2	0.018277' ${1}-im/energy.dat`
   test -n "$a" || return -1
   a=`grep '3	0.025083' ${1}-im/energy.dat`
   test -n "$a" || return -1
   a=`grep '4	0.031550' ${1}-im/energy.dat`
   test -n "$a" || return -1
   a=`grep '5	0.037688' ${1}-im/energy.dat`
   test -n "$a" || return -1
   a=`grep '6	0.043463' ${1}-im/energy.dat`
   test -n "$a" || return -1
   a=`grep '7	0.048867' ${1}-im/energy.dat`
   test -n "$a" || return -1
   a=`grep '8	0.05387' ${1}-im/energy.dat`
   test -n "$a" || return -1   
   a=`grep '9	0.058' ${1}-im/energy.dat`
   test -n "$a" || return -1

}

CheckTGE-ac-Energy()
{
   a=`grep '0	0.00373' ${1}-ac/energy.dat`
   test -n "$a" || return -1
   a=`grep '1	0.0111' ${1}-ac/energy.dat`
   test -n "$a" || return -1
   a=`grep '2	0.0182' ${1}-ac/energy.dat`
   test -n "$a" || return -1
   a=`grep '3	0.0247' ${1}-ac/energy.dat`
   test -n "$a" || return -1
   a=`grep '4	0.0318' ${1}-ac/energy.dat`
   test -n "$a" || return -1
   a=`grep '5	0.0377' ${1}-ac/energy.dat`
   test -n "$a" || return -1
}


CheckTGE()
{
   echo -n "Checking TGE-$1-im"
   if [ ! -f "TGE-$1.log" ]; then
      echo -e "  [skiped]"
      return 0
   fi

   a=`grep -r 'Initial Norm & energy: 1.000000e+00.*1.216324e-02' TGE-$1.log`
   if [ -z "$a" ]; then 
      echo -e "  [FAILED]"
      return 0
   fi

   a=`grep -r 'Initial Norm & energy: 1.000000e+00.*2.124845e-02' TGE-$1.log`
   if [ -z "$a" ]; then 
      echo -e "  [FAILED]"
      return 0
   fi

   CheckTGE-im-Energy TGE-$1 || echo "!!!TGE-$1 Im Energy test failed"
   CheckTGE-ac-Energy TGE-$1 || echo "!!!TGE-$1 AC Energy test failed"

   echo -e "  [OK]"
}


CheckT()
{
   echo -n "Checking $1"
   if [ ! -f "$1.log" ]; then
      echo -e "  [skiped]"
      return 0
   fi
   
   exp=`echo "$2" | sed "s/[[:space:]]/[[:space:]]/g"`
   a=`grep -r "$exp" $1.log`
   if [ -z "$a" ]; then 
      echo -e "  [FAILED]"
      return 0
   fi
   
   echo -e "  [OK]"
}

CheckT-Expec()
{
   echo -n "Checking $1 expectation values"
   if [ ! -f "$1/expec_post.dat" ]; then
      echo -e "  [skiped]"
      return 0
   fi
   
   exp=`echo "$2" | sed "s/[[:space:]]/[[:space:]]/g"`
   a=`grep -r "$exp" $1/expec_post.dat`
   if [ -z "$a" ]; then 
      echo -e "  [FAILED]"
      return 0
   fi
   
   echo -e "  [OK]"
}





# Test list
CheckTGE GSPO
CheckTGE SPO
CheckTGE Cheby
CheckTGE SIL


CheckT TGP-Expec "999	19980.0		1.000000	0.00373956	0.77637501,-0.63027125i"
CheckT TGP-Expec-SP "999	19980.0		1.000000	0.01456307	0.15255984,-0.17686907i"
CheckT TGP-Kick "999	19980.0		1.000000	0.00402761	0.71221627,-0.59726333i"

CheckT-Expec TGP-Expec "19980	5.37[0-9]*e-10	3.195884335	0.9999999999	0.001889422012	0.001850133601"
CheckT-Expec TGP-Expec-SP "19980	4.654250747	3.110790591	0.9999999999	0.009802138725	0.004760929388"
CheckT-Expec TGP-Kick "19980	-0.8690168821	3.158794699	0.9999973668	0.002232932204	0.001794682062"

CheckT TGP-NIP-Cheby "999	9990.0		0.093011	0.00003355	-0.18644212,-0.13025716i"
CheckT-Expec TGP-NIP-Cheby "9990	0.03862894299	0.3229754163	3.318259189e-05	2.824626741e-07"

CheckT TGPMS-Cheby "999	9990.0		1.000000	0.500000	0.500000	0.08958392	0.63547332,-0.27681845i"
CheckT-Expec TGPMS-Cheby "9990	5.268[0-9]*e-10	-0.893048894	1.59794216	2.065919242	0.0009447110023	0.003724027954	0.000925066804	0.0839901112"

CheckT TGPMS-GSPO "2999	5998.0		1.000000	0.500000	0.500000	0.08958381	-0.51881914,-0.20949660i"
CheckT-Expec TGPMS-GSPO "5998	6.859536[0-9]*e-06	-0.7190485243	1.597942183	2.254396369	0.0009446907332	0.003212836957	0.0009250870758	0.08450119799"

CheckT TGPL-IR-Pulse-Cheby "6749	67490.0		1.000000	0.00745241	0.35666953,0.60985414i"
CheckT-Expec TGPL-IR-Pulse-Cheby "67490	-1.604217534	3.08638951	0.4991352256	0.5008383665	2.640812[0-9]*e-05	0.004058411281	0.003393997148"

CheckT TGPL-IR-Pulse-GSPO "6749	67490.0		1.000000	0.00744574	0.32243224,0.62934450i"
CheckT-Expec TGPL-IR-Pulse-GSPO "67490	1.470996404	3.396652104	0.5000325293	0.499939165	2.822771342e-05	0.00323072644	0.004215045972"

CheckT TGPLMS-Cheby "399	399.0		1.000000	0.526464	0.473536	0.08618058	-0.03652410,0.70095082i"
CheckT-Expec TGPLMS-Cheby "399	-0.3039781607	3.366694881	1.665264056	1.738778381	0.001215563382	0.007299005702	0.001082545761	0.07658349977"

CheckT TGPLMS-SIL "399	399.0		1.000000	0.526464	0.473536	0.08618058	-0.03652410,0.70095082i"
CheckT-Expec TGPLMS-SIL "399	-0.3039781607	3.366694881	1.665264056	1.738778381	0.001215563382	0.007299005702	0.001082545761	0.07658349977"

CheckT TLP-H-GSPO "255	1020.0		1.000000	0.10000000	0.10158570,0.99482679i"
CheckT TLP-H-Cheby "255	1020.0		1.000000	0.10000000	0.10158570,0.99482679i"

CheckT TLP-C-GSPO "4095	2047.5		1.000000	0.2500[0-9]*	0.74667743,-0.48847656i"
CheckT TLP-C-Cheby "511	1022.0		1.000000	0.25000000	0.71945570,-0.67057075i"

CheckT TLPL-GSPO "628	314.0		1.000000	0.17477[0-9]*	0.70775373,-0.02443914i"
CheckT-Expec TLPL-GSPO "314	0.5015125974	0.4984815286"

CheckT TLPL-Cheby "628	314.0		1.000000	0.17482853	0.70749434,-0.02443081i"
CheckT-Expec TLPL-Cheby "314	0.5011450916	0.49884903"

CheckT TGO-IR-Freq "7		1.00000000	1.00000000	0.99999964	0.99999964		0.03250311"
CheckT-Expec TGO-IR-Freq "67490 -0.1064581017   3.284369552     0.0005751770158 0.9994217321    2.907750491e-06 0.005432099349  0.005715727257"
