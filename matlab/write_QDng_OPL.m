%Writes a Laser Operator struct to outfile for QDng (*.op & *.meta)
%Standard arguments: OPL struct,Outfile (without extention)
%struct OPL:
%
%OPL.class => class (Laser)
%OPL.Nt => Timesteps
%OPL.dt => Timestep
%OPL.data => Pulse

function OPL=write_QDng_OPL(OPL,outfilename)

if strncmp('Laser', OPL.class,5)
    meta_out=[outfilename '.meta'];
    data_out=[outfilename '.op'];
    meta_file=fopen(meta_out,'w');
    opl_file=fopen(data_out,'wb');
    fprintf(meta_file,'%s\n','# Generated file');
    fprintf(meta_file,'%s%s \n','CLASS = ',OPL.class);
    fprintf(meta_file,'%s%d \n','Nt = ',OPL.Nt);
    fprintf(meta_file,'%s%g \n','dt = ',OPL.dt);
    fclose(meta_file);
    for i=1:1:OPL.Nt
        fwrite(opl_file,OPL.data(i),'double');
    end    
    fclose(opl_file);
end
