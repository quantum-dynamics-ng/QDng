function [data, meta] = qd_decode_WF(fd, payload, header, decode_fcn)
%
% [data, meta] = qd_decode_WF(fd, payload, header)
% [data, meta] = qd_decode_WF(fd, payload, header, decode_fcn)
%
% Decode the WF payloads. A customized deocde function is optional
%

% Use custom decode
if (nargin == 4)
    [data, meta] = decode_fcn(payload);
    return;
end

% Well know formats 
switch(header.class)
    case 'WFGridCartesian'
        [data, meta] = qd_decode_WFGridCartesian(payload, header.compression);
    case 'WFLevel'
        [data, meta] = qd_decode_WFLevel(payload, header.compression);
    case 'WFMultistate'
        if nargin == 4
            [data, meta] = qd_decode_WFMultistate(fd, payload, decode_fcn);
        else
            [data, meta] = qd_decode_WFMultistate(fd, payload);
        end
    otherwise
        error(['Unknown class: ' header.class '. Provide a custom decode fuction.']);
end

end