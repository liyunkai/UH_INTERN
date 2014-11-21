#include"mex.h"
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{                     
    //load video
    char *videoPath = mxArrayToString(prhs[0]);
    CvCapture * capture = cvCreateFileCapture(videoPath);  //open video
    if(capture==NULL) 
    {
        mexPrintf("open video Error\n");
        return;
    }
    int fps=(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS );   //read fps
    double vfps = 1000 / fps;  //get time of every frame
    int frames=(int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);//get total number of frames
    const char *pstrWindowsTitle1 = "video";
    cvNamedWindow(pstrWindowsTitle1,CV_WINDOW_AUTOSIZE);
    IplImage * frame = NULL;
    double frameSeq;
    CvSeq *pcvSeqFaces = NULL;
    CvRect* r = NULL;
    CvPoint center;
    int radius;
    long row = 0;
    char inputChar;
    for(row=0;;row++)
    {
        if( row >= frames ) break;
        
        if( !(frame = cvQueryFrame(capture)) )//get one frame
        {
            mexPrintf("read frame Error or Finished\n");
            break;
        }
		frameSeq = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES );
        //the font variable    
        CvFont font;    
        double hScale=0.5;   
        double vScale=0.5;    
        int lineWidth=0.2;//  
        char showMsg[30];
        sprintf(showMsg,"Frame sequence is %d",(int)frameSeq);
        // 
        cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//   
        // cvPoint   
        cvPutText(frame,showMsg,cvPoint(10,50),&font,CV_RGB(255,0,0));//put text on video
        cvShowImage(pstrWindowsTitle1,frame);                          //display
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




