% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function stream = qd_wf_to_stream(data, meta, encode_fcn)
%
% Encode a WF to stream format
%

% Create header
header = pblib_create_message(@pb_descriptor_QDLIB__FileSingleHeader);


% Encode the wf data and meta data.
if iscell(data)
    if nargin > 3
        payload = qd_encode_WFMultistate(data, meta, encode_fcn);
    else
        payload = qd_encode_WFMultistate(data, meta);
    end 
    header = pblib_set (header, 'class', 'WFMultistate');
    header = pblib_set (header, 'more_files_follow', 1);
    header = pblib_set (header, 'payload_size', 4); % field length state count (int32)
    header = pblib_set (header, 'compression', 0);
else
    if nargin > 3
        payload = qd_encode_WF(data, meta, encode_fcn);
    else
        payload = qd_encode_WF(data, meta);
    end
    header = pblib_set (header, 'class', meta.class);
    header = pblib_set (header, 'more_files_follow', 0);
    header = pblib_set (header, 'payload_size', numel(payload));
    if isfield(meta,'compress') && meta.compress ~= 0
        header = pblib_set (header, 'compression', 1);
    else
        header = pblib_set (header, 'compression', 0);
    end
end

% Add stream intro, and add header in wireformat
stream = qd_get_stream_intro();
header_buf = pblib_generic_serialize_to_string(header);
stream = [stream typecast(uint32(numel(header_buf)), 'uint8') header_buf payload];

end