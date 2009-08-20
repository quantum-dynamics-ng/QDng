%Writes a Operator struct to outfile for QDng (*.op & *.meta)
%Standard arguments: OP struct,Outfile (without extention)
%implemented only for one dimmension yet!
%struct OP:
%
%OP.dims => #dims
%OP.class => class
%OP.grid => array (#dims x 3)
%            grid(?,1)=>#points in dim?
%            grid(?,2)=>min dim?
%            grid(?,3)=>max dim?
%OP.data => data

function OP=write_QDng_OP(OP,outfilename)

if strncmp('Grid', OP.class,4)
    if OP.dims == 1
        meta_out=[outfilename '.meta'];
        data_out=[outfilename '.op'];
        meta_file=fopen(meta_out,'w');
        op_file=fopen(data_out,'wb');
        fprintf(meta_file,'%s\n','# Generated file');
        fprintf(meta_file,'%s%s \n','CLASS = ',OP.class);
        fprintf(meta_file,'%s%d \n','dims = ',OP.dims);
        fprintf(meta_file,'%s%d \n','N0 = ',OP.grid(1,1));
        fprintf(meta_file,'%s%g \n','xmax0 = ',OP.grid(1,3));
        fprintf(meta_file,'%s%g \n','xmin0 = ',OP.grid(1,2));
        fclose(meta_file);
        for i=1:1:OP.grid(1,1)
            fwrite(op_file,OP.data(i),'double');
        end    
        fclose(op_file);
    end
else
    disp('Error! Check inputarguments');
end