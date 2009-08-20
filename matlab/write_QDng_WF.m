%Writes a WF struct to outfile for QDng (*.wf & *.meta)
%Standard arguments: WF struct,Outfile (without extention)
%implemented only for one dimmension yet!
%struct WF:
%WF.class => class of the WF
%WF.dims => #dims
%WF.grid => array (#dims x 3)
%            grid(?,1)=>#points in dim?
%            grid(?,2)=>min dim?
%            grid(?,3)=>max dim?
%WF.data => data

function WF=write_QDng_WF(WF,outfilename)

if strncmp('WFGrid', WF.class,6) || strncmp('WFMultistate', WF.class,12) || strncmp('Grid', WF.class,4)
    if WF.dims == 1
        meta_out=[outfilename '.meta'];
        data_out=[outfilename '.wf'];
        meta_file=fopen(meta_out,'w');
        wf_file=fopen(data_out,'wb');
        fprintf(meta_file,'%s\n','# Generated file');
        fprintf(meta_file,'%s%s \n','CLASS = ',WF.class);
        fprintf(meta_file,'%s%d \n','dims = ',WF.dims);
        fprintf(meta_file,'%s%d \n','N0 = ',WF.grid(1,1));
        fprintf(meta_file,'%s%g \n','xmax0 = ',WF.grid(1,3));
        fprintf(meta_file,'%s%g \n','xmin0 = ',WF.grid(1,2));
        fclose(meta_file);
        for i=1:1:WF.grid(1,1)
            fwrite(wf_file,real(WF.data(i)),'double');
            fwrite(wf_file,imag(WF.data(i)),'double');
        end    
        fclose(wf_file);
    end
else
    disp('Error! Check inputarguments');
end
