clear;

function w=cross_prod(u,v)
    w=zeros(3,1)
    D=[u(:),v(:)]
    w(1)=det([[1;0;0],D])
    w(2)=det([[0;1;0],D])
    w(3)=det([[0;0;1],D])
endfunction

data=read('contour.dat',-1,3)
data2=read('intersections.dat',-1,3)

dat = data2;
for i=1:2:length(dat)

    //plot(dat(i:i+1,2)+rand()/50,dat(i:i+1,3),"<")
    param3d(dat(i:i+1,1),dat(i:i+1,2),dat(i:i+1,3))
end
