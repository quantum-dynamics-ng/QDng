NAME MultElementsComplexEq
VAR cVec* C
VAR cVec* A
VAR dVec* B
VAR double d
EXP C = A * (B * I) * d
SSE_EXP C = A.MultImag(B) * d
