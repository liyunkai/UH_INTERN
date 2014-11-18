#include"mex.h"
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

typedef struct Area{
    double x;
    double y;
    double r;
}Area, *pArea;
#define AreaMember (sizeof(Area)/sizeof(double))
#define Columns (AreaMember*7)
#define IndexX(x,y,frames) ((AreaMember*y)*(frames)+(x))
#define IndexY(x,y,frames) ((AreaMember*y+1)*(frames)+(x))
#define IndexR(x,y,frames) ((AreaMember*y+2)*(frames)+(x))
#define ArrSize(frames) ((frames)*Columns)

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
        if(27 == cvWaitKey(vfps/2))  break;
		if(112 == cvWaitKey(vfps/2))
		{
			while(1)
			{
                if( 112 == cvWaitKey(0) )   break;
			}
		}
    }
    cvDestroyWindow(pstrWindowsTitle1);
    cvReleaseCapture(&capture);
}









