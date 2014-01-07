function stream = qd_wf_to_stream(data, meta, class, encode_fcn)
%
% Encode a WF to stream format
%

% Encode the wf data and meta data.
switch (class)
    case 'WFLevel'
        payload = qd_encode_WFLevel(data);
    case 'WFGridCartesian'
        payload = qd_encode_WFGridCartesian(data, meta);
    otherwise
        if (nargin > 3)
            payload = encode_fcn(data, meta);
        else
            error('Unknown WF class. Please provide an enocde_fcn')
        end
end

% Create header
header = pb_read_QDLIB__FileSingleHeader([]);
header = pblib_set (header, 'class', class);
header = pblib_set (header, 'meta_data', '');
header = pblib_set (header, 'compression', 0);
header = pblib_set (header, 'more_files_follow', 0);
%header = pblib_set (header, 'name', name);
header = pblib_set (header, 'payload_size', numel(payload));

stream = qd_get_stream_intro();

header_buf = pblib_generic_serialize_to_string(header);
stream = [stream typecast(uint32(numel(header_buf)), 'uint8') header_buf payload];

end