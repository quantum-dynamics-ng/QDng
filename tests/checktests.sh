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
CheckTGE SIL
CheckTGE Arnoldi


CheckT TGP-Expec           "999    19980.0         1.000000        0.00371015      0.29455816,-0.95562855i"
CheckT TGP-Expec-SP        "999    19980.0         1.000000        0.01452795      0.16644956,-0.00396797i"
CheckT TGP-Expec-HOFD      "999    19980.0         1.000000        0.01452795      0.16645027,-0.00396856i"
CheckT TGP-Gmat            "999	   19980.0	   1.000000	   0.01452795	   0.16644956,-0.00396797i"
CheckT TGP-Kick            "999    19980.0         1.000000        0.00399372      0.29628786,-0.88631099i"
CheckT TGPL-IR-Pulse-Cheby "6749   67490.0         1.000000        0.00604211      0.61287273,-0.55531094i"
CheckT TGPL-IR-Pulse-GSPO  "6749   67490.0         1.000000        0.00610920      0.64184923,-0.51274602i"
CheckT TGPLMS-Cheby        "399    399.0           1.000000        0.525309        0.474691        0.08634621      -0.02885164,0.70029396i"
CheckT TGPLMS-SIL          "399    399.0           1.000000        0.525309        0.474691        0.08634621      -0.02885164,0.70029396i"
CheckT TGPMS-Cheby         "999    9990.0          1.000000        0.500000        0.500000        0.08955451      0.43841526,-0.36254206i"
CheckT TGPMS-GSPO          "2999   5998.0          1.000000        0.500000        0.500000        0.08955443      -0.52591724,-0.11017722i"
CheckT TGP-NIP-Cheby       "999    9990.0          0.095927        0.00003431      -0.19531028,-0.12381382i"

CheckT-Expec TGP-Expec          "20000   -0.003629369971 3.195115856     0.9999999491    0.001866654973  0.001843493742"
CheckT-Expec TGP-Expec-SP               "20000   -0.1930887415   2.903701974     0.9999243639    0.005676941612  0.008851006999"
CheckT-Expec TGP-Expec-HOFD             "20000 -0.193103135   2.903702031 0.9999244383   0.005676945764 0.008851002793"
CheckT-Expec TGP-Gmat               "20000   -0.1930887527   2.903701979     0.9999243659    0.005676941625  0.008851007017"	
CheckT-Expec TGP-Kick           "20000   -0.8196827819   3.238666034     0.9999981131    0.001955128876  0.002038595158"
CheckT-Expec TGPL-IR-Pulse-Cheby                "67500   -0.7771795235   3.392412384     0.6839625471    0.3160245142    1.168923795e-05 0.002541139462  0.003500975282"
CheckT-Expec TGPL-IR-Pulse-GSPO         "67500	-0.9119311777	3.39065341	0.6748559675	0.3251328889	9.140715502e-06	0.002579467065	0.003529714442"
CheckT-Expec TGPLMS-Cheby               "400     -0.3059539159   3.364408943     1.661276152     1.739725823     0.001196532131  0.00727646664   0.001079240961  0.07679400487"
CheckT-Expec TGPLMS-SIL         "400    -0.3059539159   3.364408943     1.661276151     1.739725823     0.001196532131  0.007276466639  0.001079240961  0.07679400487"
CheckT-Expec TGPMS-Cheby                "10000   0.002276941983  -0.830127918    1.597681576     2.105015076     0.0009318813186 0.003846538403  0.0009231930376 0.0838528973"
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
CheckT-Expec TGO-IR-Freq "67500   -0.07038699611  3.273401771     0.0004253823444 0.9995664784    1.5555773[0-9]*e-09 0.0054054203    0.005658837725"

CheckT TGO-IR-Krotov "7               1.00000000      1.00000000      0.99999246      0.00000000              0.99999246              0.03603239"
CheckT-Expec TGO-IR-Krotov "67500   -0.0694778544[0-9]  3.273269064     0.000431660252[0-9] 0.9995603049    4.423130[0-9]*e-09 0.005404982106  0.005659550024"
			    
CheckT TGO-IR-Rabitz "8               1.00000000      1.00000000      0.98651573      0.17732921              0.80918652              0.03017470"
CheckT-Expec TGO-IR-Rabitz "67500   -0.5790327472   3.273414639     0.01357943706   0.9864109718    5.621477[0-9]*e-08 0.005358664256  0.005608851259"
			        
CheckT TLO-Cheby "4		1.00000000	1.00000000	1.00000000	0.00000000		1.00000000		0.27266223"
CheckT-Expec TLO-Cheby "314.5   5.878[0-9]*e-11 0.9999999991"

CheckT TGO-IR-Krotov-Projection "8               1.00000000      0.99999197      0.00000000              0.99999196              0.03547645"
CheckT-Expec TGO-IR-Krotov-Projection "67500   -0.0738354228   3.273386152     0.000440245604[0-9]* 0.9995512863    1.1770227[0-9]*e-08 0.005405440377  0.005659010713"

CheckT TGO-UV-Krotov "20              1.00000000      0.95790483      0.00004724              0.95785759              0.86229164"
CheckT-Expec TGO-UV-Krotov "400     0.02248363247   4.972625083     0.02065551066   3.48816541      0.9579048291    7.300543295e-05 0.008375108942  4.215768051e-05       0.1626162722"
