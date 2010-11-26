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
CheckT TGP-Kick            "999    19980.0         1.000000        0.00399372      0.29628786,-0.88631099i"
CheckT TGPL-IR-Pulse-Cheby "6749   67490.0         1.000000        0.00604211      0.61287273,-0.55531094i"
CheckT TGPL-IR-Pulse-GSPO  "6749   67490.0         1.000000        0.00609535      0.64134370,-0.51333831i"
CheckT TGPLMS-Cheby        "399    399.0           1.000000        0.525309        0.474691        0.08634621      -0.02885164,0.70029396i"
CheckT TGPLMS-SIL          "399    399.0           1.000000        0.525309        0.474691        0.08634621      -0.02885164,0.70029396i"
CheckT TGPMS-Cheby         "999    9990.0          1.000000        0.500000        0.500000        0.08955451      0.43841526,-0.36254206i"
CheckT TGPMS-GSPO          "2999   5998.0          1.000000        0.500000        0.500000        0.08955443      -0.52591724,-0.11017722i"
CheckT TGP-NIP-Cheby       "999    9990.0          0.095927        0.00003431      -0.19531028,-0.12381382i"

CheckT-Expec TGP-Expec          "19980  -0.003629369969 3.195115857     0.9999999494    0.001866654974  0.001843493743"
CheckT-Expec TGP-Expec-SP               "19980  -0.1930887407   2.903701977     0.9999243652    0.005676941624  0.008851007011"
CheckT-Expec TGP-Kick           "19980  -0.8196827822   3.238666035     0.9999981134    0.001955128877  0.002038595158"
CheckT-Expec TGPL-IR-Pulse-Cheby                "67490  -0.7771795203   3.392412378     0.6839625456    0.316024514     1.168923795e-05 0.002541139459  0.003500975278"
CheckT-Expec TGPL-IR-Pulse-GSPO         "67490  0.9110827157    3.054504003     0.6748633501    0.3251223502    2.664048725e-06 0.003455272754  0.002640099626"
CheckT-Expec TGPLMS-Cheby               "399    -0.3059539159   3.364408943     1.661276151     1.739725823     0.001196532131  0.007276466639  0.001079240961  0.07679400487"
CheckT-Expec TGPLMS-SIL         "399    -0.3059539159   3.364408943     1.661276151     1.739725823     0.001196532131  0.007276466639  0.001079240961  0.07679400487"
CheckT-Expec TGPMS-Cheby                "9990   0.002276941983  -0.8301279178   1.597681576     2.105015076     0.0009318813189 0.003846538403  0.000923193038  0.08385289728"
CheckT-Expec TGPMS-GSPO         "5998   -0.001522647817 -0.3453753214   1.597981865     2.267997167     0.000935796418  0.003081294696  0.0009192775766 0.08461806305"
CheckT-Expec TGP-NIP-Cheby              "9990   0.04022609252   0.3328758816    3.393281906e-05 2.855740893e-07"


CheckT TLP-H-GSPO "255	1020.0		1.000000	0.10000000	0.10158570,0.99482679i"
CheckT TLP-H-Cheby "255	1020.0		1.000000	0.10000000	0.10158570,0.99482679i"

CheckT TLP-C-GSPO "4095	2047.5		1.000000	0.2500[0-9]*	0.74667743,-0.48847656i"
CheckT TLP-C-Cheby "511	1022.0		1.000000	0.25000000	0.71945570,-0.67057075i"

CheckT TLPL-GSPO "628	314.0		1.000000	0.17477[0-9]*	0.70775373,-0.02443914i"
CheckT-Expec TLPL-GSPO "314	0.5015125974	0.4984815286"

CheckT TLPL-Cheby "628	314.0		1.000000	0.17482853	0.70749434,-0.02443081i"
CheckT-Expec TLPL-Cheby "314	0.5011450916	0.49884903"

CheckT TGO-IR-Freq "8               1.00000000      1.00000000      0.99999166      0.00000000              0.99999166              0.03590463"
CheckT-Expec TGO-IR-Freq "67490   0.07733109246   3.282288243     0.0004824557783 0.9994672915    3.255204247e-05 0.005401848414  0.005662411742"

CheckT TGO-IR-Krotov "7               1.00000000      1.00000000      0.99999246      0.00000000              0.99999245              0.03603239"
CheckT-Expec TGO-IR-Krotov "67490   -0.08823169619  3.265339893     0.001020043086  0.9988490085    0.000108960111  0.005451948943  0.005608049249"

CheckT TGO-IR-Rabitz "8               1.00000000      1.00000000      0.98651573      0.17732921              0.80918652              0.03017470"
CheckT-Expec TGO-IR-Rabitz "67490   0.6038675681    3.282040522     0.01422469307   0.9857293803    1.959763203e-05 0.005357049568  0.005606125338
"

