%mexFunction to load a Wavefunction from struct
%output (one argument) is the ObjectHandle
%input: 1) one argument -> structmxArray
%		the struct needs a field named data with the data of the Wavefunction
%		and one named CLASS with the right WF-module e.g. GridCartesian
%		the other fieldnames must be thoses needed to init the specific WF
%		e.g. dims, N0, xmax0, xmin0 etc for GridCartesian
% function [handle] = init_wf_struct(struct_mxArray, WF-module)