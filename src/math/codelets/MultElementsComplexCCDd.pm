NAME MultElementsComplex
VAR cVec* C
VAR cVec* A
VAR dVec* B
VAR double d
EXP C = C * A * I * B * d
SSE_EXP C = C * A.MultImag(B) * d
