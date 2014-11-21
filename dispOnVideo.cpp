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
    //grid
    int widthVideo = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
    int heightVideo = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
    int gridStep = 40;
    const char *pstrWindowsTitle1 = "video";
    cvNamedWindow(pstrWindowsTitle1,CV_WINDOW_AUTOSIZE);
    IplImage * frame = NULL;
    double ratio;
    CvSeq *pcvSeqFaces = NULL;
    CvRect* r = NULL;
    CvPoint center;
    int radius;
    //the font variable    
    CvFont font;    
    double hScale=0.5;   
    double vScale=0.5;    
    double lineWidth=0.2;//  
    char showMsg[30];
    //control key
    char inputChar;
    ////go to the start frame according to the time in ms
    cvSetCaptureProperty(capture, CV_CAP_PROP_POS_MSEC, videoStartTimetamp);
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
            sprintf(showMsg,"(%d,%d)",center.x,center.y);
            cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);// 
            cvPutText(frame,showMsg,center,&font,FaceCirclecolors[m%7]);
        }
        //draw data
        center.x = eyeData[dataIndexX(frameSeq,startIndexEyeData,eyeRows)];
        center.y = eyeData[dataIndexY(frameSeq,startIndexEyeData,eyeRows)];
        radius = 5;
        cvCircle(frame, center, radius, FaceCirclecolors[0], 2);
        sprintf(showMsg,"(%d,%d)",center.x,center.y);
        cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);// 
        cvPutText(frame,showMsg,center,&font,CV_RGB(0,255,0));
        //draw Grids
        frameSeq = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES );
        sprintf(showMsg,"Frame sequence is %d",(int)frameSeq);
        // 
        cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//   
        // cvPoint   
        cvPutText(frame,showMsg,cvPoint(10,50),&font,CV_RGB(255,0,0));//put text on video
        //draw grids on video
        CvPoint startPoint,endPoint;
        for(int p=0;p<=widthVideo/gridStep;p++)
        {
            startPoint.x = p*gridStep;
            startPoint.y = 0;
            endPoint.x = p*gridStep;
            endPoint.y = heightVideo;
            cvLine(frame,startPoint,endPoint,CV_RGB(0,0,255));
            sprintf(showMsg,"%d",p*gridStep);
            cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);// 
            startPoint.y = startPoint.y +20;
            cvPutText(frame,showMsg,startPoint,&font,CV_RGB(255,0,0));
        }
        for( int q=0;q<=heightVideo/gridStep;q++)
        {
            startPoint.x = 0;
            startPoint.y = q*gridStep;
            endPoint.x = widthVideo;
            endPoint.y = q*gridStep;
            cvLine(frame,startPoint,endPoint,CV_RGB(0,0,255));
            sprintf(showMsg,"%d",q*gridStep);
            cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);// 
            cvPutText(frame,showMsg,startPoint,&font,CV_RGB(255,0,0));
        }      
        //display
        cvShowImage(pstrWindowsTitle1,frame);      
        if( inputChar == 110 )   goto NEXT_LAST;    
        if( inputChar == 108 )   goto NEXT_LAST;   
        inputChar = cvWaitKey(vfps/2);
        if( inputChar == 112 )  //hit P
        {
            NEXT_LAST:
            while(1)    //Pause
            {
                inputChar = cvWaitKey(0);
                if( inputChar == 112 )   break; //hit P, continue;
                if( inputChar == 27 )   break;  //hit ESC to exit.
                if( inputChar == 110 )   break; //hit N, the next frame
                if( inputChar == 108 )   //hit L, the last frame. But can't work
                {
                    cvSetCaptureProperty(capture, CV_CAP_PROP_POS_MSEC , cvGetCaptureProperty(capture, CV_CAP_PROP_POS_MSEC)-vfps);
                    break; 
                }
            }
        }
        if( inputChar == 27 )   break; 
    }
    cvDestroyWindow(pstrWindowsTitle1);
    cvReleaseCapture(&capture);
}









