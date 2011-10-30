#!/bin/bash


CheckTGE-im-Energy()
{
   a=`grep '0[[:space:]]*0.0037100' ${1}-im/energy.dat`
   test -n "$a" || return 1
   a=`grep '1[[:space:]]*0.0110668' ${1}-im/energy.dat`
   test -n "$a" || return 1
   a=`grep '2[[:space:]]*0.0181411' ${1}-im/energy.dat`
   test -n "$a" || return 1
   a=`grep '3[[:space:]]*0.0249017' ${1}-im/energy.dat`
   test -n "$a" || return 1
   a=`grep '4[[:space:]]*0.0313281' ${1}-im/energy.dat`
   test -n "$a" || return 1
   a=`grep '5[[:space:]]*0.0374320' ${1}-im/energy.dat`
   test -n "$a" || return 1
   a=`grep '6[[:space:]]*0.0431786' ${1}-im/energy.dat`
   test -n "$a" || return 1
   a=`grep '7[[:space:]]*0.0485' ${1}-im/energy.dat`
   test -n "$a" || return 1
   a=`grep '8[[:space:]]*0.0535' ${1}-im/energy.dat`
   test -n "$a" || return 1   
   a=`grep '9[[:space:]]*0.058' ${1}-im/energy.dat`
   test -n "$a" || return 1

  return 0

}

CheckTGE-ac-Energy()
{
   a=`grep '0[[:space:]]*0.003714' ${1}-ac/energy.dat`
   test -n "$a" || return 1
   a=`grep '1[[:space:]]*0.011074' ${1}-ac/energy.dat`
   test -n "$a" || return 1
   a=`grep '2[[:space:]]*0.018152' ${1}-ac/energy.dat`
   test -n "$a" || return 1
   a=`grep '3[[:space:]]*0.024892' ${1}-ac/energy.dat`
   test -n "$a" || return 1
   a=`grep '4[[:space:]]*0.031319' ${1}-ac/energy.dat`
   test -n "$a" || return 1
   a=`grep '5[[:space:]]*0.037427' ${1}-ac/energy.dat`
   test -n "$a" || return 1
}


CheckTGE()
{
   echo -n "Checking TGE-$1"
   if [ ! -f "TGE-$1.log" ]; then
      echo -e "\r\t\t\t\t\t\t\t\t\t[skiped]"
      return 0
   fi

   a=`grep -r 'Initial Norm & energy: 1.000000e+00.*1.205115e-02' TGE-$1.log`
   if [ -z "$a" ]; then 
      echo -e "\r\t\t\t\t\t\t\t\t\t[FAILED]"
      return 0
   fi

   success=0;
   
   echo -ne "\r\t\t\t\t\t\t\t(Imag)"
   CheckTGE-im-Energy TGE-$1
   if [ $? == 0 ]; then
        echo -e "\r\t\t\t\t\t\t\t\t\t[OK]"
   else
        echo -e "\r\t\t\t\t\t\t\t\t\t[FAILED]"
   fi
   
   echo -ne "\r\t\t\t\t\t\t\t(AC)"
   CheckTGE-ac-Energy TGE-$1
   if [ $? == 0 ]; then
        echo -e "\r\t\t\t\t\t\t\t\t\t[OK]"
   else
        echo -e "\r\t\t\t\t\t\t\t\t\t[FAILED]"
   fi
}


CheckT()
{
   echo -n "Checking $1"
   if [ ! -f "$1.log" ]; then
      echo -e "\r\t\t\t\t\t\t\t\t\t[skiped]"
      return 0
   fi
   
   exp=`echo "$2" | sed "s/[[:space:]]/[[:space:]]*/g"`
   a=`grep -r "$exp" $1.log`
   if [ -z "$a" ]; then 
      echo -e "\r\t\t\t\t\t\t\t\t\t[FAILED]"
      return 0
   fi
   
   echo -e "\r\t\t\t\t\t\t\t\t\t[OK]"
}

CheckT-Expec()
{
   echo -n "Checking $1 expectation values"
   if [ ! -f "$1/expec_post.dat" ]; then
      echo -e "\r\t\t\t\t\t\t\t\t\t[skiped]"
      return 0
   fi
   
   exp=`echo "$2" | sed "s/[[:space:]]/[[:space:]]*/g"`
   a=`grep -r "$exp" $1/expec_post.dat`
   if [ -z "$a" ]; then 
      echo -e "\r\t\t\t\t\t\t\t\t\t[FAILED]"
      return 0
   fi
   
   echo -e "\r\t\t\t\t\t\t\t\t\t[OK]"
}





# Test list
CheckTGE Cheby
CheckTGE GSPO
CheckTGE SPO
CheckTGE SIL


CheckT TGP-Expec           "999    19980.0         1.000000        0.00371015      0.29455816,-0.95562855i"
CheckT TGP-Expec-SP        "999    19980.0         1.000000        0.01452795      0.16644956,-0.00396797i"
CheckT TGP-Gmat            "999	   19980.0	   1.000000	   0.01452795	   0.16644956,-0.00396797i"
CheckT TGP-Kick            "999    19980.0         1.000000        0.00399372      0.29628786,-0.88631099i"
CheckT TGPL-IR-Pulse-Cheby "6749   67490.0         1.000000        0.00604211      0.61287273,-0.55531094i"
CheckT TGPL-IR-Pulse-GSPO  "6749   67490.0         1.000000        0.00610920      0.64184923,-0.51274602i"
CheckT TGPLMS-Cheby        "399    399.0           1.000000        0.525309        0.474691        0.08634621      -0.02885164,0.70029396i"
CheckT TGPLMS-SIL          "399    399.0           1.000000        0.525309        0.474691        0.08634621      -0.02885164,0.70029396i"
CheckT TGPMS-Cheby         "999    9990.0          1.000000        0.500000        0.500000        0.08955451      0.43841526,-0.36254206i"
CheckT TGPMS-GSPO          "2999   5998.0          1.000000        0.500000        0.500000        0.08955443      -0.52591724,-0.11017722i"
CheckT TGP-NIP-Cheby       "999    9990.0          0.095927        0.00003431      -0.19531028,-0.12381382i"

CheckT-Expec TGP-Expec          "20000  -0.003629369969 3.195115857     0.9999999494    0.001866654974  0.001843493743"
CheckT-Expec TGP-Expec-SP               "20000  -0.1930887407   2.903701977     0.9999243652    0.005676941624  0.008851007011"
CheckT-Expec TGP-Gmat               "20000	-0.1930887406	2.903701976	0.9999243648	0.005676941622	0.008851007007"	
CheckT-Expec TGP-Kick           "20000  -0.8196827822   3.238666035     0.9999981134    0.001955128877  0.002038595158"
CheckT-Expec TGPL-IR-Pulse-Cheby                "67500  -0.7771795203   3.392412378     0.6839625456    0.316024514     1.168923795e-05 0.002541139459  0.003500975278"
CheckT-Expec TGPL-IR-Pulse-GSPO         "67500	-0.9119311777	3.39065341	0.6748559675	0.3251328889	9.140715502e-06	0.002579467065	0.003529714442"
CheckT-Expec TGPLMS-Cheby               "400    -0.3059539159   3.364408943     1.661276151     1.739725823     0.001196532131  0.007276466639  0.001079240961  0.07679400487"
CheckT-Expec TGPLMS-SIL         "400    -0.3059539159   3.364408943     1.661276151     1.739725823     0.001196532131  0.007276466639  0.001079240961  0.07679400487"
CheckT-Expec TGPMS-Cheby                "10000   0.002276941983  -0.8301279178   1.597681576     2.105015076     0.0009318813189 0.003846538403  0.000923193038  0.08385289728"
CheckT-Expec TGPMS-GSPO         "6000   -0.001522647817 -0.3453753214   1.597981865     2.267997167     0.000935796418  0.003081294696  0.0009192775766 0.08461806305"
CheckT-Expec TGP-NIP-Cheby              "10000   0.04022609252   0.3328758816    3.393281906e-05 2.855740893e-07"


CheckT TLP-H-GSPO "255	1020.0		1.000000	0.10000000	0.10158570,0.99482679i"
CheckT TLP-H-Cheby "255	1020.0		1.000000	0.10000000	0.10158570,0.99482679i"

CheckT TLP-C-GSPO "4095	2047.5		1.000000	0.2500[0-9]*	0.74667743,-0.48847656i"
CheckT TLP-C-Cheby "511	1022.0		1.000000	0.25000000	0.71945570,-0.67057075i"

CheckT TLPL-GSPO "628	314.0		1.000000	0.17477[0-9]*	0.70775373,-0.02443914i"
CheckT-Expec TLPL-GSPO "314.5	0.5015125974	0.4984801635"

CheckT TLPL-Cheby "628	314.0		1.000000	0.174828[0-9]*	0.70749434,-0.02443081i"
CheckT-Expec TLPL-Cheby "314.5	0.5011450916	0.49884[0-9]*"

CheckT TGO-IR-Freq "8               1.00000000      1.00000000      0.99999166      0.00000000              0.99999166              0.03590463"
CheckT-Expec TGO-IR-Freq "67500	-0.07038699496	3.273401771	0.0004253823329	0.9995664784	1.5555799[0-9]*e-09	0.0054054203	0.005658837725"

CheckT TGO-IR-Krotov "7               1.00000000      1.00000000      0.99999246      0.00000000              0.99999245              0.03603239"
CheckT-Expec TGO-IR-Krotov "67500   -0.06947785326  3.273269064     0.0004316602399 0.9995603049    4.42313541[0-9]*e-09 0.005404982106  0.005659550024"
			    
CheckT TGO-IR-Rabitz "8               1.00000000      1.00000000      0.98651573      0.17732921              0.80918652              0.03017470"
CheckT-Expec TGO-IR-Rabitz "67500   -0.5790327461   3.273414639     0.01357943699   0.9864109718    5.62146863[0-9]*e-08 0.005358664254  0.005608851261"
			        
CheckT TLO-Cheby "4		1.00000000	1.00000000	1.00000000	0.00000000		1.00000000		0.27266223"
CheckT-Expec TLO-Cheby "314.5	5.8783488[0-9]*e-11	0.9999999991"

CheckT TGO-IR-Krotov-Projection "8               1.00000000      0.99999197      0.00000000              0.99999196              0.03547645"
CheckT-Expec TGO-IR-Krotov-Projection "67500   -0.07383542147  3.273386152     0.0004402455912 0.9995512863    1.177022072e-08 0.005405440377  0.005659010713"

CheckT TGO-UV-Krotov "20              1.00000000      0.95790483      0.00004724              0.95785759              0.86229164"
CheckT-Expec TGO-UV-Krotov "400     0.02248363247   4.972625083     0.02065551066   3.488165409     0.9579048291    7.300543295e-05 0.008375108941  4.21576805e-05  0.1626162722"
