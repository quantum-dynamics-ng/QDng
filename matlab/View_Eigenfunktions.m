% Script to view the calculated eigenfunktion of a potential
QDng_ef_dir=input('QDng Eigenfunktion Directory?  ','s');
Nef=0;
WF=read_QDng_WF([QDng_ef_dir '/ef'],Nef);
while  WF.class ~=-1   
       dx=(WF.grid(1,3)-WF.grid(1,2))/(WF.grid(1,1)-1);
       x=WF.grid(1,2):dx:WF.grid(1,3);
       plot(x,abs(WF.data),'-b',x,real(WF.data),'-g',x,imag(WF.data),'-r');
       pause();
       Nef=Nef+1;
       WF=read_QDng_WF([QDng_ef_dir '/ef'],Nef);
end

