n = 4;

M = zeros(9*n);
M(2*n,2*n) = 1; M(4*n,4*n) = 1; M(6*n,6*n) = 1; M(8*n,8*n) = 1;

[H,T,R] = hough(M, 'RhoResolution',0.5,'Theta',-90:0.5:89);

subplot(2,1,1);
imshow(M);
title('points in image');
xlabel('x'), ylabel('y');
subplot(2,1,2);
imshow(imadjust(rescale(H)),'XData',T,'YData',R,...
      'InitialMagnification','fit');
title('Hough transform on points');
xlabel('\theta'), ylabel('\rho');
axis on, axis normal, hold on;
colormap(gca,hot);
