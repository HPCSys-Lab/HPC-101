% Plot performances for MPI Heat2D
function plot_benchmark_heat2d

% Clear history
clear;
% Clear graphics
clf;
% Loading input file
x=load('benchmark_heat2d.txt');
% Get runtimes
for i=1:3
 runTime(1:9,i)=x((i-1)*9+1:i*9,3);
end
% Sequential time for each size
timeSeq(1:3)=runTime(1,1:3);
% Compute speedup values
for i=1:3
 speedUp(1:8,i)=timeSeq(i)./runTime(2:9,i);
end
% Standard deviation on runtimes : absolute error with factor*sigma
factorSigma=10;
for i=1:3
 absError(1:8,i)=factorSigma*speedUp(1:8,i).*((x((i-1)*9+1,4)./x((i-1)*9+1,3))+(x((i-1)*9+2:i*9,4)./x((i-1)*9+2:i*9,3)));
end
% Number of processes on abscissa
y=[2 4 8 16 32 64 128 256];
% Plot histogram
figure(1);
hbar=bar(log2(y),speedUp(1:8,1:3),'b');
hold on;
% Find x coordinates of each bar
for i=1:3
 xData(1:8,i)=hbar(i).XData+hbar(i).XOffset;
end
% Plot standard deviations
errorbar(xData,speedUp(1:8,1:3),absError(1:8,1:3),'LineStyle','none','CapSize',5,'LineWidth',2);
set(gca,'Xlim',[0 9]);
% Set Ylim to 1.1*max
set(gca,'Ylim',[0 (1.2*max(max(speedUp)))]);
set(gca,'xticklabel',{'2','4','8','16','32','64','128','256'});
xlabel('Number of Processes');
ylabel('SpeedUp');
title(['Benchmark : SpeedUp vs Number of Processes with ' num2str(factorSigma) ' \sigma']);
% Define colors of bars
set(hbar(1),'facecolor','b');
set(hbar(2),'facecolor','r');
set(hbar(3),'facecolor','g');
% Transparency
set(hbar,'facealpha',0.4);
% Define legend
hlegend=legend('N=512^2','N=1024^2','N=2048^2');
rect=[0.77,0.8,0.1,0.1];
set(hlegend,'Position',rect,'color','none');
end
