% Generate animated gif for MPI Heat2D
function generate_gif_heat2d

% Load dimensions
file=textread('param','%s','delimiter','\n');
% x_dim = width in common convention, into core of code = vertical axis
x_dim=str2double(file(2));
% y_dim = height in common convention, into core of code = horizontal axis
y_dim=str2double(file(4));
% Number of frames : must be equal to number of output files
numFrames=200;
% Time step between 2 frames
step=0.1;
animated(1,1,1,numFrames)=0;
% Main loop on number of frames
for i=1:numFrames
 % Create meshgrid
 [X,Y]=meshgrid(0:x_dim+1,0:y_dim+1);
 % Read all values
 Z=load(strcat('outputPar',num2str(i),'.dat'));
 % Surf plot
 figure(1);
 surf(X,Y,Z);
 % Parameters for surf plot
 colormap(jet);
 shading interp;
 view([0,0,1]);
 hc=colorbar;
 set(hc,'position',[0.932 0.3 0.02 0.6]);
 caxis([-10 10]);
 xlim([0 x_dim+1]);
 ylim([0 y_dim+1]);
 xlabel('x domain');
 ylabel('y domain');
 zlabel('temperature');
 % Pause
 pause(step);
 % Get current frame from figure
 frame=getframe(figure(1));
 if (i==1)
  [animated, cmap]=rgb2ind(frame.cdata,256,'nodither');
 else
  animated(:,:,1,i)=rgb2ind(frame.cdata,cmap,'nodither');
 end
end
% Write final animated gif
imwrite(animated,cmap,'Heat_2D.gif','DelayTime',step,'LoopCount',inf);
end
