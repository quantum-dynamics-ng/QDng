% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function qd_view_efs(path, opts)
%
% Inspect a set of eigenfunctions
%
% function qd_view_efs(path, opts)
%
% opts can contain the following fields;
% .basename   The wave function base name
% .decode_fcn a customized decode_fcn for qd_read_wf
% .fh         a figure handle. If not given a new figure is created.
% .dims       The dimensions to be shown. Is relevant for ndims >= 2

if ~exist('opts', 'var')
    opts.basename = 'ef';
    opts.fh = figure();
elseif ~isstruct('opts')
    error('opts must be given as struct');
end

% open figure
if ~isfield(opts, 'fh')
    opts.fh = figure();
end

energies=dlmread([path '/energy.dat']);
Nef = size(energies,1);

fprintf('n\tE-E0 [au]\n');
for i=1:Nef
    opts.index = i-1;
    qd_view_wf(path, opts);
    fprintf('%d\t%.6f', energies(i,1), energies(i,3));
    input('');
end

end
