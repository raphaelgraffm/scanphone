function J=compute_gradient(I,k,dt,ep,lam,I0)


if ~exist('ep')
   ep=1;
end
if ~exist('dt')
   dt=ep/5;  
end
if ~exist('lam')
   lam=0.1;
end
if ~exist('I0')
	I0=I;
end

[ny,nx]=size(I); ep2=ep^2;

   % cacul des dérivées
   I_x = (I(:,[2:nx nx])-I(:,[1 1:nx-1]))/2;
   I_y = (I([2:ny ny],:)-I([1 1:ny-1],:))/2;
   I_xx = I(:,[2:nx nx])+I(:,[1 1:nx-1])-2*I;
   I_yy = I([2:ny ny],:)+I([1 1:ny-1],:)-2*I;
   Dp = I([2:ny ny],[2:nx nx])+I([1 1:ny-1],[1 1:nx-1]);
   Dm = I([1 1:ny-1],[2:nx nx])+I([2:ny ny],[1 1:nx-1]);
   I_xy = (Dp-Dm)/4;

% calcul du gradient du critère J =( || k*I -I0||_L2)^2 + lam*sum(sqrt(1+|grad(I)|^2))
   Num = I_xx.*(ep2+I_y.^2)-2*I_x.*I_y.*I_xy+I_yy.*(ep2+I_x.^2);
   Den = (ep2+I_x.^2+I_y.^2).^(3/2);
   I_t = -lam .* (Num./Den) + imfilter(( imfilter(I,k,'conv','symmetric')-I0),k,'conv','symmetric') ;
   I=I-dt*I_t;   %% evolve image by dt

J=I;
