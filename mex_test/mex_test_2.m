clear all;
handle1= init_wf_file('WF',100);
handle2= init_wf_file('WF-0_100');
s_wf1 = get_wf_handle(handle1);
s_wf2 = get_wf_handle(handle2);
handle3 = init_wf_struct(s_wf2);
handle4 = init_wf_struct(s_wf1);
s_wf3 = get_wf_handle(handle3);
s_wf4 = get_wf_handle(handle4);
s_wf1 = setfield(s_wf1,'Test','Hallo');
handle5 = init_wf_struct(s_wf1);
s_wf5 = get_wf_handle(handle5);

op_struct = struct('CLASS', 'OGridPosition');

op_handle = init_op(op_struct,handle2);