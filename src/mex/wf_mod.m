%mexFunction to load a Wavefunction from struct
%output (one argument) is the ObjectHandle
%input: 1)three or more input-arguments -> modifier (+,-,*,c*,p*), Wavefunction 1, Wavefunction 2, etc 
%					   c* -> psi1^* x psi2 (pointwise)
%					   p* -> psi1 x psi2 (pointwise)
%	function [handle]=wf_mod('modifier',WF1,WF2 || double,etc);