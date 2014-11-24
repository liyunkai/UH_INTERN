function info = func(data,startTimeStamp,areaCvt)
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
keyFrameEnd = length(areaCvt);
Step = 4;
AreaMember = 4;
GazeLXColumn = 5;   % indicate the column of GazeLeft
GazeLYColumn = 6;
TimeColumn = 1;
Num_FixOnFace = 0;
Dur_FixOnface = 0;
startDataIndex = find(data(:,1),startTimeStamp);
%% an example, decide if the data fixations are in the face area, we can also calculate the duration meantime.
for m = keyFrameStart : keyFrameEnd     %the start frame of video
    for n = startDataIndex : startDataIndex+Step    %the start index of eye data
        for k = 1:AreaMember:length(areaCvt(1,:))-AreaMember
            rX = areaCvt(m,k);
            rY = areaCvt(m,k+1);
            rWidth = areaCvt(m,k+2);
			rHeight = areaCvt(m,k+3);
            if (data(n,GazeLXColumn)>rX) && (data(n,GazeLXColumn)<rX+rWidth) && (data(n,GazeLYColumn)>rY) && (data(n,GazeLYColumn)<rY+rHeight)
                Num_FixOnFace = Num_FixOnFace + 1;
                Dur_FixOnface = Dur_FixOnface + (data(n,TimeColumn)-data(n-1,TimeColumn));
                % do something else
				
                break;
            end
        end
    end
    startDataIndex = startDataIndex + Step;
end
%% convert information out
info.NumberFixationOnFace = Num_FixOnFace;
info.DurationFixationOnFace = Dur_FixOnface;
%do something else

end



