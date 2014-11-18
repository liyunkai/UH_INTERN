close all;clear all;clc;
videoPath = 'video.mov';
areaPath = 'area_auto.xls';
startTimeStamp = 100000;

areaAuto = drawArea(videoPath,startTimeStamp);
xlswrite(areaPath, areaAuto);


