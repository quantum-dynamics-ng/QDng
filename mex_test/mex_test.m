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


disp('Testing init_op routines');

op_struct = struct('CLASS', 'OGridPosition');

op_handle1 = init_op(op_struct,handle1);
op_handle2 = init_op(op_struct,handle1);
op_handle3 = init_op(op_struct,handle1);

op_structPot = struct('CLASS', 'GridPotential','file','Pot1');
op_handlePot = init_op(op_structPot,handle1);

op_structKin = struct('CLASS', 'GridNablaSq','dims','1','mass0','35636');
op_handleKin = init_op(op_structKin,handle1);

disp('     Testing init_op routines (Multistate)');

op_structMS = struct('CLASS', 'OPMultistate','operators','5','Operator',[]);

for i=1:1:5
    op_structMS.Operator(i).CLASS = 'OGridPosition';
    op_structMS.Operator(i).row = int2str(i-1);
    op_structMS.Operator(i).col = int2str(i-1);
end
disp(op_structMS);

op_structMS_Pot = struct('CLASS', 'OPMultistate','operators','5','Operator',[]);

for i=1:1:5
    op_structMS_Pot.Operator(i).CLASS = 'GridPotential';
    op_structMS_Pot.Operator(i).file = 'Pot1';
    op_structMS_Pot.Operator(i).row = int2str(i-1);
    op_structMS_Pot.Operator(i).col = int2str(i-1);
end
disp(op_structMS_Pot);

op_handleMS_Pot = init_op(op_structMS_Pot,handle2);

op_handle4 = init_op(op_structMS,handle2);

op_structPot_Sum = struct('CLASS', 'OPSum','operators','2','Operator',[]);
op_structPot_Sum.Operator(1).CLASS = 'GridPotential';
op_structPot_Sum.Operator(2).CLASS = 'GridPotential';
op_structPot_Sum.Operator(1).file = 'Pot1';
op_structPot_Sum.Operator(2).file = 'Pot1';

op_handlePot_Sum = init_op(op_structPot_Sum,handle1);

op_structMS_Summ = struct('CLASS', 'OPMultistate','operators','5','Operator',[]);

for i=1:1:5
    op_structMS_Summ.Operator(i).CLASS = 'Sum';
    op_structMS_Summ.Operator(i).operators='2';
    op_structMS_Summ.Operator(i).Operator=[];
    for k=1:1:2
        op_structMS_Summ.Operator(i).Operator(k).CLASS = 'GridPotential';
        op_structMS_Summ.Operator(i).Operator(k).file = 'Pot1';
    end
    op_structMS_Summ.Operator(i).row = int2str(i-1);
    op_structMS_Summ.Operator(i).col = int2str(i-1);
end
disp(op_structMS_Summ);

op_handleMS_Sum = init_op(op_structMS_Summ,handle2);

disp('     Done');

disp('Done');

disp('Testing apply_op routines');

handle1_copy = wf_mod('*',handle1,1);

handle23 = apply_op(op_handle1,handle1);
handle24 = apply_op(op_handle4,handle2);
handle25 = apply_op(op_handleKin,handle1);
handle26 = apply_op(op_handleMS_Sum,handle2);

apply_op(op_handle1,handle1_copy);

disp('Done');

disp('Testing expec_op routines');

val1 = expec_op(op_handle1,handle1)
disp('Expected: 7.4002');
val2 = expec_op(op_handle4,handle2)
disp('Expected: 7.4002');
val3 = expec_op(op_handle1,handle1_copy)
disp('Expected: 406.0050');

val4 = expec_op(op_handlePot,handle1)
disp('Expected: 1.0236e-04');

val5 = expec_op(op_handlePot_Sum,handle1)
disp('Expected: 2.0472e-04');

val6 = expec_op(op_handleKin,handle1)

val7 = expec_op(op_handleMS_Pot,handle2)

val8 = expec_op(op_handleMS_Sum,handle2)

disp('Done');

disp('Testing get_all_op_handle routines');

all_op_handles = get_all_op_handle();
disp(all_op_handles);

disp('Done');

disp('Testing delete_op_handle routine');

delete_op_handle(op_handle1);
delete_op_handle();

disp('Done');

disp('Test succsessfully terminated');
