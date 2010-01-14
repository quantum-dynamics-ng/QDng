%mexFunction to load a Wavefunction from struct
%output (one argument) is the ObjectHandle
%input: 1) two arguments -> structmxArray, WF-module e.g. GridCartesian
%		the struct needs a field named data with the data of the Wavefunction
%		the other fieldnames must be thoses needed to init the specific WF
%		e.g. CLASS, dims, N0, xmax0, xmin0 etc for GridCartesian
% function [handle] = init_wf_struct(struct_mxArray, WF-module)