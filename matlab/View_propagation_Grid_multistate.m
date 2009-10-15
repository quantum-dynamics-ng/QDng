% Script to view the propagtion of a grid multistate propagation
QDng_prop_meta=input('QDng Propagation meta file?  ','s');
meta_file=fopen(QDng_prop_meta,'r');
dir=str(1:max(findstr(QDng_prop_meta, '/')))
while 1    
    l_meta_data = fgetl(meta_file);
    disp(l_meta_data);
    if ~ischar(l_meta_data),   break,   end
    [name value]= strread(l_meta_data,'%s%s', 'delimiter', '=');
    if strmatch('#', name), continue , end
    if strmatch('CLASS', name), value=value{1:end};class=sscanf(value,'%s'); continue , end
    if strmatch('WFCLASS', name), value=value{1:end};wfclass=sscanf(value,'%s'); continue , end
    if strmatch('States', name), value=value{1:end};states=sscanf(value,'%d');  continue , end
    if strmatch('Nt', name), value=value{1:end};Ts=sscanf(value,'%d');  continue , end
    if strmatch('Wcycle', name), value=value{1:end};wcycle=sscanf(value,'%d');  continue , end
    if strmatch('dt', name), value=value{1:end};dt=sscanf(value,'%g');  continue , end
    if strmatch('WFBaseName', name), value=value{1:end};base_name=sscanf(value,'%s');  continue , end     
end
fclose(meta_file);

view_ts=input('View Propagation for each Timestep Y/N [Y]?  ','s');
if isempty(view_ts)
     view_ts= 'Y';
end

if view_ts=='Y'
   each=input('Show every?  ');
   for t=0:each:Ts/wcycle
      for s=0:1:states-1
         disp([dir base_name]);
         WF=read_QDng_WF([dir base_name],s,t);
         dx=(WF.grid(1,3)-WF.grid(1,2))/(WF.grid(1,1)-1);
         x=WF.grid(1,2):dx:WF.grid(1,3);
         subplot(states,1,states-s);
         plot(x,abs(WF.data),'-b',x,real(WF.data),'-g',x,imag(WF.data),'-r');
         title(['WF-' int2str(s) '\_' int2str(t) ' Time = ' num2str(t/41.34) ' fs' ])
      end
      pause(0.1);
   end
end