function pl = qd_encode_WFMultistate(data, meta, encode_fcn)
%
% qd_encode_WFMultistate(data, meta, class)
% qd_encode_WFMultistate(data, meta, class, encode_fcn)
%
% Encode a Multistate wavefunction
% 
% data is a cell array containing the states
% meta descriptiv information
% class WF class
% encode_fcn 
%
% meta, class, encode_fcn can be cell arrays describing the states.
% If they are not, it's assumed that they are equal for all states.

pl = typecast(uint32(length(data)),'uint8');

for i=1:length(data);
    if iscell(meta)
        m = meta{i};
    else
        m = meta;
    end

    if nargin > 3
        if iscell(encode_fcn)
            e = encode_fcn{i};
        else
            e = encode_fcn;
        end
    end
    
    if nargin > 3
        payload = qd_encode_WF(data{i}, m, e);
    else
        payload = qd_encode_WF(data{i}, m);
    end
    
    header = pblib_create_message(@pb_descriptor_QDLIB__FileSingleHeader);
    header = pblib_set (header, 'class', m.class);
    header = pblib_set (header, 'meta_data', '');
    header = pblib_set (header, 'compression', 0);
    if i < length(data)
        header = pblib_set (header, 'more_files_follow', 1);
    else
        header = pblib_set (header, 'more_files_follow', 0);
    end
    
    header = pblib_set (header, 'payload_size', numel(payload));
    header_buf = pblib_generic_serialize_to_string(header);
    
    %        msd_size                                     msg        enc_data
    pl = [pl typecast(uint32(numel(header_buf)), 'uint8') header_buf payload];
end

end