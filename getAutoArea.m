close all;clear all;clc;
videoPath = 'video.mov';
areaPath = 'area_auto.xls';
startTimeStamp = 100000; % set the start time of drawing feature areas of video
areaAuto = drawArea(videoPath,startTimeStamp);
xlswrite(areaPath, areaAuto);


