clear;clc;close all;

% Lecture de l'image niveau de gris
I=double((imread('cb.png'))); % load image
%I=imread('cb_NokiaN80_modemacro_yaourt_petit.png');

% Lecture d'une image en couleur
%I=double(rgb2gray(imread('06032009001_494.jpg')))
  % r le rayon � choisir "� la main" ... pour le moment ;-)
r = 5.8
% Le noyau de flou, on suppose que le flou est un flou de d�focalisation
k=fspecial('disk',r);
% si le flou est gaussien : k=fspecial('gaussian',[nx ny],sigma);% [nx ny] d�finisant le support du noyau, sigma l'�cart-type de la gaussienne
%pour plus d'infos sur la fonction fspecial taper sur l'invite matlab : help fspecial


%I0 est l'image floue
% si l'on veut g�n�rer des images floues bruit�es synth�tiques : d�commenter la suite
%I = =double(imread('4BARSKEW2.png'));
%std_n=0.5;
%In = randn(size(I))*(std_n); % Bruit gaussien
%I0 = imfilter(I,k,'conv','symmetric') + In;  % Image floue bruit�e synth�tique

I0=I;

% visualisation de l'image floue
figure(1); imshow(uint8(I0)); title('Image floue')


%Initialisation de l'algorithme
% J l'image restaur�e
J=I0; 
% param�tres
ep_J = 0.00000000000001; % erreur relative minimale en J
% param�tre de r�gularisation (contr�lant le bruit)
lam=0.1; J_old=0;

% dt pas de la descente du gradient
dt=0.9; eps=1.0; 

i=1;

while (   (mean(mean(abs(J - J_old))) /(mean(mean(J_old))+1)> ep_J )),  % iterate until
                                            % convergence
   J_old = J;
   J=compute_gradient_tych(J,k,dt,eps,lam,I0);     % scalar lam
   J=min(max(J,0),255);
   i = i +1
   error = mean(mean(abs(J - J_old)))/(mean(mean(J_old))+1)
   figure(2); imagesc((J)),axis image,colormap(gray);title('Image restauree'),drawnow;
end % for i
% nombre d'it�rations final
nbiter = i


