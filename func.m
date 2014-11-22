function info = func(data,startTimeStamp,areaManu)
%% the number of key frames where face appears, not that exact
ChildAppear = 76;
MaidAppear = 250;
MaidAppearFace = 360;
MaidTurnsBack = 745;
ManAppear = 938;
LadyFace = 1044;
ManAppear2 = 1110;
LadtFace2 = 1215;
ManAppear3 = 1265;
%% vars define
keyFrameStart = 1;
keyFrameEnd = length(areaManu);
Step = 4;
GazeLXColumn = 5;   % indicate the column of GazeLeft
GazeLYColumn = 6;
TimeColumn = 1;
Num_FixOnFace = 0;
Dur_FixOnface = 0;
startDataIndex = find(data(:,1),startTimeStamp);
%% an example, decide if the data fixations are in the face area, we can also calculate the duration meantime.
for m = keyFrameStart : keyFrameEnd
    for n = startDataIndex : startDataIndex+Step
        for k = 1:3:length(areaManu(1,:))-3
            centerX = areaManu(m,k);
            centerY = areaManu(m,k+1);
            radius = areaManu(m,k+2);
            if (data(n,GazeLXColumn)>centerX-radius) && (data(n,GazeLXColumn)<centerX+radius) && (data(n,GazeLYColumn)>centerY-radius) && (data(n,GazeLYColumn)<centerY+radius)
                Num_FixOnFace = Num_FixOnFace + 1;
                Dur_FixOnface = Dur_FixOnface + (data(n,TimeColumn)-data(n-1,TimeColumn));
                
                break;
            end
        end
    end
    startDataIndex = startDataIndex + Step;
end
%% convert information out
info.NumberFixationOnFace = Num_FixOnFace;
info.DurationFixationOnFace = Dur_FixOnface;


end



