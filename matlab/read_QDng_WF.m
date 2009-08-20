%reads a WF file from QDng and creates a WF struct
%Standard arguments: WF filename (witout extention), number | WF filename (witout extention), state, number 
%implemented only for one dimmension yet!
%struct WF:
%
%WF.class => class of the WF
%WF.dims => #dims
%WF.grid => array (#dims x 3)
%            grid(?,1)=>#points in dim?
%            grid(?,2)=>min dim?
%            grid(?,3)=>max dim?
%WF.data => data

function WF=read_QDng_WF(WF_in_filename,varargin)

optargin = size(varargin,2);

if optargin == 1
   
   number=varargin{1};
   meta_in=[WF_in_filename '.meta'];
   meta_file=fopen(meta_in,'r');
   data_in=[WF_in_filename '_' int2str(number) '.wf'];
   wf_file=fopen(data_in,'rb');
   if wf_file == -1
       class=-1;
       WF=struct('class',class);
       return;
   end
   
elseif optargin == 2
   
   state=varargin{1};
   number=varargin{2};
   meta_in=[WF_in_filename '-' int2str(state) '.meta'];
   meta_file=fopen(meta_in,'r');
   data_in=[WF_in_filename '-' int2str(state) '_' int2str(number) '.wf'];
   wf_file=fopen(data_in,'rb');
   if wf_file == -1
       class=-1;
       WF=struct('class',class);
       return;
   end
end

while 1
    
    l_meta_data = fgetl(meta_file);
    if ~ischar(l_meta_data),   break,   end
    [name value]= strread(l_meta_data,'%s%s', 'delimiter', '=');
    if strmatch('#', name), continue , end
    if strmatch('dims', name), dims=value{1:end}; dims=sscanf(dims,'%d'); continue , end
    
end


frewind(meta_file)

if dims == 1
    
    grid=zeros(dims,3);
    
    while 1
    
        l_meta_data = fgetl(meta_file);
        if ~ischar(l_meta_data),   break,   end
        [name value]= strread(l_meta_data,'%s%s', 'delimiter', '=');
        if strmatch('#', name), continue , end
        if strmatch('CLASS', name), value=value{1:end};class=sscanf(value,'%s'); continue , end
        if strmatch('xmax0', name), value=value{1:end};grid(1,3)=sscanf(value,'%g');  continue , end
        if strmatch('xmin0', name), value=value{1:end};grid(1,2)=sscanf(value,'%g');  continue , end
        if strmatch('N0', name), value=value{1:end};grid(1,1)=sscanf(value,'%d');  continue , end
        
    end
    
    fclose(meta_file);
    if strncmp('WFGrid', class,6) || strncmp('WFMultistate', class,12) || strncmp('Grid', class,4)
        WF_all=fread(wf_file,'double');
        data=zeros(grid(1,1),1);
        for i=2:2:2*grid(1,1)
            data(i/2,1)=WF_all(i-1)+j*WF_all(i);
        end
    
        fclose(wf_file);
    end
    
    WF=struct('class',class,'dims',dims,'grid',grid,'data',data);
    
end
