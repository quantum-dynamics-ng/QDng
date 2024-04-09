% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function intro = qd_get_stream_intro()

intro = [uint8('QDngPrtoBuf') 0 typecast(uint32(1), 'uint8')];

end