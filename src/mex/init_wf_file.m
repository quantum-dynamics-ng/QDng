%mexFunction to load a Wavefunction from file
%output (one argument) is the ObjectHandle
%input: 1) two arguments -> Filename, WF-module e.g. GridCartesian
%       2) three arguments -> array of filenames, Multistate, WF-module e.g. GridCartesian
%       3) five arguments -> filename, Multistate, WF-module e.g. GridCartesian, #States, #WF
%	function [handle]=init_wf_file(File_name,varargin);