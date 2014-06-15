function payload = qd_encode_WF(data, meta, class, encode_fcn)
%
% qd_encode_WF(data, meta, class)
% qd_encode_WF(data, meta, class, encode_fcn)
%
% Encode a WF to stream format.

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

end

