close all;clear all;clc;
%% vars define
areaPath = 'area_manu.xls';
dataPath = 'data';
ASD_Oxy = 'ASD_Oxy';
ASD_Pla = 'ASD_Pla';
Ctrl_Oxy = 'Ctrl_Oxy';
Ctrl_Pla = 'Ctrl_Pla';
startTimeStamp = 40000;
%% load area
areaManu = xlsread(areaPath);
%% convert cordinate here
areaCvt = areaManu;
%% load eye data of 4 groups.
path = [dataPath '\' ASD_Oxy];
dataList = dir(path);
for m=3:length(dataList)
    data = xlsread([path '\' dataList(m).name]);
    info = func(data,startTimeStamp,areaManu); % you can rename the func name and the output, this is an example
end
path = [dataPath '\' ASD_Pla];
dataList = dir(path);
for m=3:length(dataList)
    data = xlsread([path '\' dataList(m).name]);
    info = func(data,startTimeStamp,areaManu);
end
path = [dataPath '\' Ctrl_Oxy];
dataList = dir(path);
for m=3:length(dataList)
    data = xlsread([path '\' dataList(m).name]);
    info = func(data,startTimeStamp,areaManu);
end
path = [dataPath '\' Ctrl_Pla];
dataList = dir(path);
for m=3:length(dataList)
    data = xlsread([path '\' dataList(m).name]);
    info = func(data,startTimeStamp,areaManu);
end
%% analyse











