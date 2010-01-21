clear all;

files_a = ['WF-0_100' ; 'WF-0_200' ; 'WF-0_300' ];
files_c = {'WF-0_100' ; 'WF-0_10' ; 'WF-0_1'};

disp('Testing init_wf_file routines');

handle1= init_wf_file('WF-0_100');
handle2= init_wf_file('WF',100);
handle3= init_wf_file(files_a);
handle4= init_wf_file(files_c);

disp('Done');

disp('Testing del routines');

delete_wf_handle(handle1);
delete_wf_handle();

handle1= init_wf_file('WF-0_100');
handle2= init_wf_file('WF',100);
handle3= init_wf_file(files_a);
handle4= init_wf_file(files_c);

disp('Done');

disp('Testing get_wf_handle routine');

s_wf1 = get_wf_handle(handle1);
s_wf2 = get_wf_handle(handle2);
s_wf3 = get_wf_handle(handle3);
s_wf4 = get_wf_handle(handle4);

disp('Done');

disp('Testing init_wf_struct routines');

s_wf5 = setfield(s_wf1,'Test','Hallo1');
s_wf6 = setfield(s_wf2,'Test','Hallo1');

handle5 = init_wf_struct(s_wf1);
handle6 = init_wf_struct(s_wf2);
handle7 = init_wf_struct(s_wf3);
handle8 = init_wf_struct(s_wf4);
handle9 = init_wf_struct(s_wf5);
handle10 = init_wf_struct(s_wf6);

s_wf7 = get_wf_handle(handle5);
s_wf8 = get_wf_handle(handle6);
s_wf9 = get_wf_handle(handle7);
s_wf10 = get_wf_handle(handle8);
s_wf11 = get_wf_handle(handle9);
s_wf12 = get_wf_handle(handle10);

disp('Done');

disp('Testing Writing routines');

succ = write_wf_file(handle1, 'TestWFout')
succ = write_wf_file(handle1, 'TestWFout',100)

succ = write_wf_file(handle2, 'TestWFMSout')
succ = write_wf_file(handle2, 'TestWFMSout',100)

disp('Done');

disp('Testing mod routines');

disp('add');

handle11 = wf_mod('+',handle1,handle1);
handle12 = wf_mod('+',handle1,handle1,handle1,handle1);
handle13 = wf_mod('+',handle2,handle2);

disp('sub');

handle14 = wf_mod('-',handle1,handle1);
handle15 = wf_mod('-',handle1,handle1,handle1,handle1);
handle16 = wf_mod('-',handle2,handle2);

disp('mult')

handle17 = wf_mod('*',handle1,2);
handle18 = wf_mod('*',handle2,2);

norm  = wf_mod('*',handle1,handle1)
norm  = wf_mod('*',handle2,handle2)

handle19 = wf_mod('p*',handle1,handle1);
handle20 = wf_mod('p*',handle2,handle2);

handle21 = wf_mod('c*',handle1,handle1);
handle22 = wf_mod('c*',handle2,handle2);

disp('Done');

disp('Testing get_all_handle routines');

all_handles = get_all_wf_handle();

disp('Done');

delete_wf_handle();
handle1= init_wf_file('WF-0_100');
handle2= init_wf_file('WF',100);

disp('Testing init_op routines');

op_struct = struct('CLASS', 'OGridPosition');

op_handle1 = init_op(op_struct,handle1);
op_handle3 = init_op(op_struct,handle1);
op_handle4 = init_op(op_struct,handle1);

disp('     Testing init_op routines (Multistate)');

op_structMS = struct('CLASS', 'OPMultistate','operators','5','Operator',[]);

for i=1:1:5
    op_structMS.Operator(i).CLASS = 'OGridPosition';
    op_structMS.Operator(i).row = int2str(i-1);
    op_structMS.Operator(i).col = int2str(i-1);
end
disp(op_structMS);

op_handle2 = init_op(op_structMS,handle2);

disp('     Done');

disp('Done');

disp('Testing apply_op routines');

handle3=wf_mod('*',handle1,1);
handle4=wf_mod('*',handle2,1);

handle5 = apply_op(op_handle1,handle3);
handle6 = apply_op(op_handle2,handle4);

disp('Done');

disp('Testing expec_op routines');

val1 = expec_op(op_handle1,handle1)
val2 = expec_op(op_handle2,handle2)

disp('Done');

disp('Testing delete_op_handle routines');

delete_op_handle(op_handle1);
delete_op_handle();

disp('Done');

disp('Test succsessfully terminated');
