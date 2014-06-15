function stream = qd_wf_to_stream(data, meta, class, encode_fcn)
%
% Encode a WF to stream format
%

% Create header
header = pblib_create_message(@pb_descriptor_QDLIB__FileSingleHeader);
header = pblib_set (header, 'meta_data', '');
header = pblib_set (header, 'compression', 0);

% Encode the wf data and meta data.
if iscell(data)
    if nargin > 3
        payload = qd_encode_WFMultistate(data, meta, class, encode_fcn);
    else
        payload = qd_encode_WFMultistate(data, meta, class);
    end 
    header = pblib_set (header, 'class', 'WFMultistate');
    header = pblib_set (header, 'more_files_follow', 1);
    header = pblib_set (header, 'payload_size', 4); % state count
else
    if nargin > 3
        payload = qd_encode_WF(data, meta, class, encode_fcn);
    else
        payload = qd_encode_WF(data, meta, class);
    end
    header = pblib_set (header, 'class', class);
    header = pblib_set (header, 'more_files_follow', 0);
    header = pblib_set (header, 'payload_size', numel(payload));
end


stream = qd_get_stream_intro();
header_buf = pblib_generic_serialize_to_string(header);
stream = [stream typecast(uint32(numel(header_buf)), 'uint8') header_buf payload];

end