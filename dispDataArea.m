close all;clear all;clc;
%% declare vars
videoPath = 'video.mov';
areaPath = 'area_manu.xls';
dataPath = 'eye_data.xls';
startTimeStampEyeData = 415015;   % Unit: ms , of the eyeData, not the video
videoStartTimestamp = 0;
eyeDataGazeLXColumn = 5;
eyeDataGazeLYColumn = 6;
eyeDataWidth = 1280.0;
eyeDataHeight = 1024.0;
videoWidth = 624.0;     %the resolution of video to get face area
videoHeight = 256.0;
blackWidth = 1920;      %the actual video resolution, there are black parts in the video
blackHeight = 1080;
ProportionWidth = eyeDataWidth/videoWidth;
ProportionHeight = eyeDataHeight/videoHeight;
ProportionBlackWidth = blackWidth/eyeDataWidth;
% Video Origin with respect to eye data.
OffsetX = 0;
OffsetY = (eyeDataHeight-videoHeight*ProportionWidth)/2;
%%
area_manu = xlsread(areaPath);
eyeOrgData = xlsread(dataPath);
%% find the eye data start index from startTimeStampEyeData.  q1:find result is a empty [].q2:should align the time stamp of video and data.
% startIndexEyeData = find(eyeOrgData(:,1)==startTimeStampEyeData)-1; % minus 1 for C++ from matlab
startIndexEyeData = 25126;% this is an example number
%% convert cord inate of eye_data.
eyeCvtData = eyeOrgData;
eyeCvtData(:,eyeDataGazeLXColumn) = (eyeOrgData(:,eyeDataGazeLXColumn)-OffsetX)/ProportionWidth;
eyeCvtData(:,eyeDataGazeLYColumn) = (eyeOrgData(:,eyeDataGazeLYColumn)-OffsetY)/ProportionWidth;
%% display
dispOnVideo(videoPath,area_manu,eyeCvtData,startIndexEyeData,videoStartTimestamp);








