#!/bin/bash

# convert codelets
find codelets -name "*.pm" -exec codelets/codelet.pl codelets/vec.cl \{} \;

#embbed headers
files=`find codelets -name "*.h"`


echo "#ifndef VECTORPRIMITIVES_H_" > VectorPrimitives.h
echo "#define VECTORPRIMITIVES_H_" >> VectorPrimitives.h
echo "#ifdef _OPENMP" >> VectorPrimitives.h
echo " #include <omp.h>" >> VectorPrimitives.h
echo "#endif" >> VectorPrimitives.h

echo "#ifdef HAVE_SSE2" >> VectorPrimitives.h
echo " #include \"m128dc.h\"" >> VectorPrimitives.h
echo "#endif" >> VectorPrimitives.h

echo "#ifdef HAVE_AVX" >> VectorPrimitives.h
echo " #include \"m256dc.h\"" >> VectorPrimitives.h
echo "#endif" >> VectorPrimitives.h

for i in $files; do
   echo "#include \"$i\""  >> VectorPrimitives.h
done


echo "#endif" >> VectorPrimitives.h

