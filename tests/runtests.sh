#!/bin/bash

if [ -z "$1" ]; then
   echo "usage $0 <path to qdng>"
   exit 1
fi


CheckTermination()
{
   a=`grep "Normal termination" $1.log | wc -l`
   test "$a" -eq 1 ||  echo "!!!$1 Unclean termination" 
}


qdng=$1

test_names="TGE-GSPO TGE-SPO TGE-SIL TGE-Cheby TGP-Expec TGP-Expec-SP TGP-NIP-Cheby TGPMS-Cheby TGPMS-GSPO"

for name in $test_names; do
   echo "Running test: $name"
   $qdng ${name}.qdi > ${name}.log
   CheckTermination ${name}
done


