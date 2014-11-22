close all;clear all;clc;
%% declare vars
videoPath = 'video.mov';
areaPath = 'area_manu.xls';
dataPath = 'eye_data.xls';
startTimeStampEyeData = 0;   % Unit: ms , of the eyeData, not the video
videoStartTimestamp = 10000;
eyeDataGazeLXColumn = 5;
eyeDataGazeLYColumn = 6;
eyeDataWidth = 1024.0;
eyeDataHeight = 1008.0;
videoWidth = 624.0;
videoHeight = 256.0;
ProportionWidth = eyeDataWidth/videoWidth;
ProportionHeight = eyeDataHeight/videoHeight;
% Video Origin with respect to eye data.
OffsetX = 0;
OffsetY = (eyeDataHeight-videoHeight*ProportionWidth)/2;
%%
area_manu = xlsread(areaPath);
eyeOrgData = xlsread(dataPath);
%% find the eye data start index from startTimeStampEyeData.
% startIndexEyeData = find(eyeData(:,1)==startTimeStampEyeData)-1; % minus 1 for C++ from matlab
startIndexEyeData = 31000;% this is an example number
%% convert cordinate of eye_data.
eyeCvtData = eyeOrgData;
eyeCvtData(:,eyeDataGazeLXColumn) = (eyeOrgData(:,eyeDataGazeLXColumn)-OffsetX)/ProportionWidth;
eyeCvtData(:,eyeDataGazeLYColumn) = (eyeOrgData(:,eyeDataGazeLXColumn)-OffsetY)/ProportionWidth;
%% display
dispOnVideo(videoPath,area_manu,eyeCvtData,startIndexEyeData,videoStartTimestamp);








