function [fh, data] = qd_view_wf(path, opts)
%
% qd_view_wf(path)
% qd_view_wf(path, opts)
%
% plot a wave function 
%
% opts can contain the following fields;
% .basename   The wave function base name
% .index      index of the wf
% .decode_fcn a customized decode_fcn for qd_read_wf
% .fh         a figure handle. If not given a new figure is created.
% .dims       The dimensions which to be shown. Is relevant for ndims >= 2


if ~isa(opts, 'struct')
    error('opts must be a struct');
end

% create filename
if isfield(opts, 'basename')
    fname = [path '/' opts.basename '_' num2str(opts.index)];
else
    fname = path;
end

% read data
if isfield(opts, 'decode_fcn')
    [data, meta] = qd_read_wf(fname, opts.decode_fcn);
else
    [data, meta] = qd_read_wf(fname);
end

% pepare plot
if isfield(opts, 'fh')
    fh = figure(opts.fh);
else
    fh = figure();
end

% prepare data
if isfield(opts, 'dims')
    y=qd_int(data, opts.dims);
else
    y = data;
end

switch (ndims(y))
    case 1
        plot(abs(y));
    case 2
        surf(abs(y));
    otherwise
        error('Mulit-dim view not implemented yet. Choose dimensions with opts.dims');
end
        


end