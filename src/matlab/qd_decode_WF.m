function [data, meta] = qd_decode_WF(fd, payload, class, decode_fcn)
%
% Decode the WF payloads. A customized deocde function is optional
%

switch(class)
    case 'WFGridCartesian'
        [data, meta] = qd_decode_WFGridCartesian(payload);
    case 'WFLevel'
        [data, meta] = qd_decode_WFLevel(payload);
    case 'WFMultistate'
        if nargin == 4
            [data, meta] = qd_decode_WFMultistate(fd, payload, decode_fcn);
        else
            [data, meta] = qd_decode_WFMultistate(fd, payload);
        end
    otherwise
        if (nargin == 4)
            [data, meta] = decode_fcn(payload);
        else
            error(['Unknown class: ' class '. Provide a custom decode fuction.']);
        end
end

end