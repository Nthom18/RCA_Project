I = imread('cam_snapshots/snapshot18.png');
imshow(I)
[x,y] = ginput(2);

r = abs((x(2) - x(1))/2)