close all;clear all;clc;
%%
videoPath = 'video.mov';
areaPath = 'area_manu.xls';
dataPath = 'eyeData.xls';
startTimeStampEyeData = 0;   % Unit: ms , of the eyeData, not the video
videoStartTimestamp = 10000;
%%
area_manu = xlsread(areaPath);
eyeData = xlsread(dataPath);
%%
% startIndexEyeData = find(eyeData(:,1)==startTimeStampEyeData)-1; % minus 1 for C++ from matlab
startIndexEyeData = 41000;% this is an example number
%%
dispOnVideo(videoPath,area_manu,eyeData,startIndexEyeData,videoStartTimestamp);








