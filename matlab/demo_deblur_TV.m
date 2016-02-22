clear;clc;close all;

% Lecture de l'image niveau de gris
%I=double((imread(' images_test/cb.png'))); % load image
I=double((imread('images_test/cbsiemens.png'))); % load image
% Lecture de l'image couleur
%I=double(rgb2gray(imread(' images_test/cb_livre_petit.png')));
%I=double(rgb2gray(imread('images_test/cbyaourt2.jpg')));

% r le rayon � choisir "a la main" ... pour le moment ;-)
%r = 7.06 cb_livre_petit.png à verifier que c'est décoder sur xzing
%r = 5.8 cb.png
%r=7.0 cbyaourt2.jpg
% Le noyau de flou, on suppose que le flou est un flou de d�focalisation
r=5.7
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
ep_J = 0.0001; % erreur relative minimale en J
% param�tre de r�gularisation (contr�lant le bruit)
lam=0.1; J_old=0;

% dt pas de la descente du gradient
dt=0.9; eps=1.0; 

i=1;

while (   (mean(mean(abs(J - J_old))) /(mean(mean(J_old))+1)> ep_J )),  % iterate until
                                            % convergence
   J_old = J;
   J=compute_gradient(J,k,dt,eps,lam,I0);     % scalar lam
   J=min(max(J,0),255);
   i = i +1
   error = mean(mean(abs(J - J_old)))/(mean(mean(J_old))+1)
   figure(2); imagesc((J)),axis image,colormap(gray);title('Image restauree'),drawnow;
end % for i
% nombre d'it�rations final
nbiter = i


