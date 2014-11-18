#include"mex.h"
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

typedef struct Area{
    double x;
    double y;
    double r;
}Area, *pArea;

/* data Index */
#define dataGazeLXColumn    4
#define dataGazeLYColumn    5
#define dataIndexX(frameSeq,startIndex,rows)   ( dataGazeLXColumn*(rows)+(startIndex)+(frameSeq) )
#define dataIndexY(frameSeq,startIndex,rows)   ( dataGazeLYColumn*(rows)+(startIndex)+(frameSeq) )
/* area index */
#define AreaMember (sizeof(Area)/sizeof(double))
#define areaIndexX(frameSeq,areaSeq,rows) ( (AreaMember*(areaSeq))*(rows)+(frameSeq) )
#define areaIndexY(frameSeq,areaSeq,rows) ( (AreaMember*(areaSeq)+1)*(rows)+(frameSeq) )
#define areaIndexR(frameSeq,areaSeq,rows) ( (AreaMember*(areaSeq)+2)*(rows)+(frameSeq) )

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    //indicate the color of feature circle
    CvScalar FaceCirclecolors[] = 
                                {
                                    {{0, 0, 255}},
                                    {{0, 128, 255}},
                                    {{0, 255, 255}},
                                    {{0, 255, 0}},
                                    {{255, 128, 0}},
                                    {{255, 255, 0}},
                                    {{255, 0, 0}},
                                    {{255, 0, 255}}
                                };
    //get area data
    int areaRows = mxGetM(prhs[1]);
    int areaColumns = mxGetN(prhs[1]);
    double *areaData = (double *)mxGetData(prhs[1]);
    //get eye data
    int eyeRows = mxGetM(prhs[2]);
    int eyeColumns = mxGetN(prhs[2]);
    double *eyeData = (double *)mxGetData(prhs[2]);
    //get start index of eye data
    int startIndexEyeData = (int)(*(double *)mxGetData(prhs[3]));
    // get videoStartTimetamp to play video
    double videoStartTimetamp = *(double *)mxGetData(prhs[4]);
    mexPrintf("startIndexEyeData is %f, areaData size is %d, %d, eyeRows is %d, %d, videoStartTimetamp is %f\n",startIndexEyeData, areaRows, areaColumns, eyeRows, eyeColumns, videoStartTimetamp);
    //load video
    char *videoPath = mxArrayToString(prhs[0]);
    CvCapture * capture = cvCreateFileCapture(videoPath);  //open video
    if(capture==NULL) 
    {
        mexPrintf("open video Error");
        return;
    }
    int fps=(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS );   //read fps
    double vfps = 1000 / fps;  //get time of every frame
    int frames=(int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);//get total number of frames
    mexPrintf("fps is %d,frames number is %d\n", fps, frames);
    const char *pstrWindowsTitle1 = "video";
    cvNamedWindow(pstrWindowsTitle1,CV_WINDOW_AUTOSIZE);
    IplImage * frame = NULL;
    double ratio;
    CvSeq *pcvSeqFaces = NULL;
    CvRect* r = NULL;
    CvPoint center;
    int radius;
    cvSetCaptureProperty(capture, CV_CAP_PROP_POS_MSEC, videoStartTimetamp);//go to the start frame according to the time in ms
    while(1)
    {   
        if( !(frame = cvQueryFrame(capture)) )//get one frame
        {
            mexPrintf("read frame Error or Finished\n");
            break;
        }
        int frameSeq=(int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);   //get frame sequence
        //mexPrintf("frame number = %d\n",frameSeq);
        int m;
        for(m=0;m<(int)(areaColumns/AreaMember);m++) //draw area
        {
            center.x = (int)areaData[areaIndexX(frameSeq,m,areaRows)];
            center.y = (int)areaData[areaIndexY(frameSeq,m,areaRows)];
            radius = (int)areaData[areaIndexR(frameSeq,m,areaRows)];
            cvCircle(frame, center, radius, FaceCirclecolors[m%7], 2);
        }
        //draw data
        center.x = eyeData[dataIndexX(frameSeq,startIndexEyeData,eyeRows)];
        center.y = eyeData[dataIndexY(frameSeq,startIndexEyeData,eyeRows)];
        radius = 5;
        cvCircle(frame, center, radius, FaceCirclecolors[0], 2);
        //display
        cvShowImage(pstrWindowsTitle1,frame);      
        if(27 == cvWaitKey(1))  break;// you can adjust the speed of video play with vfps, unit: ms
    }
    cvDestroyWindow(pstrWindowsTitle1);
    cvReleaseCapture(&capture);
}









