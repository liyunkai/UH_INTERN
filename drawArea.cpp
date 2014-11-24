#include"mex.h"

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

typedef struct Area{
    double x;
    double y;
    double width;
    double height;
}Area, *pArea;

#define AreaMember (sizeof(Area)/sizeof(double))
#define Columns (AreaMember*7)
#define ArrSize(frames) ((frames)*Columns)
#define IndexX(frameSeq,areaSeq,frames) ( (AreaMember*(areaSeq))*(frames)+(frameSeq) )
#define IndexY(frameSeq,areaSeq,frames) ( (AreaMember*(areaSeq)+1)*(frames)+(frameSeq) )
#define IndexWidth(frameSeq,areaSeq,frames) ( (AreaMember*(areaSeq)+2)*(frames)+(frameSeq) )
#define IndexHeight(frameSeq,areaSeq,frames) ( (AreaMember*(areaSeq)+3)*(frames)+(frameSeq) )

// for here, you need to modify the paths to your local haarxxx.xml paths
const char *pstrEyeCascadePath = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml";
const char *pstrProfileFaceCascadePath = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_profileface.xml";
const char *pstrMouthCascadePath = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_mouth.xml";
const char *pstrUpperBodyCascadePath = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_upperbody.xml";
const char *pstrFrontFaceCascadePath = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt_tree.xml";

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // load Haar feature classifiers
    CvHaarClassifierCascade *pHaarEyeCascade = NULL, *pHaarProfileFaceCascade = NULL, *pHaarMouthCascade = NULL, *pHaarUpperBodyCascade = NULL, *pHaarFrontFaceCascade = NULL;
	if ( !(pHaarEyeCascade = (CvHaarClassifierCascade*)cvLoad(pstrEyeCascadePath)) )
    {
        mexPrintf("load Haar Eye Cascade Error\n"); 
        return;
    }
    if ( !(pHaarProfileFaceCascade = (CvHaarClassifierCascade*)cvLoad(pstrProfileFaceCascadePath)) )
    {
        mexPrintf("load Haar Eye-Glasses Cascade Error\n"); 
        return;
    }
    if ( !(pHaarMouthCascade = (CvHaarClassifierCascade*)cvLoad(pstrMouthCascadePath)) )
    {
        mexPrintf("load Haar Mouth Cascade Error\n"); 
        return;
    }
    if ( !(pHaarUpperBodyCascade = (CvHaarClassifierCascade*)cvLoad(pstrUpperBodyCascadePath)) )
    {
        mexPrintf("load Haar Mouth Cascade Error\n"); 
        return;
    }
    if ( !(pHaarFrontFaceCascade = (CvHaarClassifierCascade*)cvLoad(pstrFrontFaceCascadePath)) )
    {
        mexPrintf("load Haar Mouth Cascade Error\n"); 
        return;
    }
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
        mexPrintf("open video Error");
        return;
    }
    
    int fps=(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS );   //read fps
    double vfps = 1000 / fps;  //get time of every frame
    int frames=(int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);//get total number of frames
    mexPrintf("fps is %d,frames number is %d\n", fps, frames);
    
    double  *dynamicData = (double *)mxMalloc(ArrSize(frames) * sizeof(double));
    const char *pstrWindowsTitle1 = "video";
    const char *pstrWindowsTitle2 = "result";
    cvNamedWindow(pstrWindowsTitle1,CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsTitle2, CV_WINDOW_AUTOSIZE);
    IplImage * frame = NULL;
    IplImage *pGrayImage = NULL;
    CvMemStorage *pcvMStorage = cvCreateMemStorage(0);
    double ratio;
    CvSeq *pcvSeqFaces = NULL;
    CvRect* r = NULL;
    long row = 0;
    double startTimestamp = *(double *)mxGetData(prhs[1]);
    //cvSetCaptureProperty(capture, CV_CAP_PROP_POS_MSEC , startTimestamp);//go to the start frame according to the time in ms
    for(row=0;;row++)
    {
        if( row >= frames ) break;
        
        if( !(frame = cvQueryFrame(capture)) )//get one frame
        {
            mexPrintf("read frame Error or Finished\n");
            break;
        }
        cvShowImage(pstrWindowsTitle1,frame);                          //display
        pGrayImage = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
        cvCvtColor(frame, pGrayImage, CV_BGR2GRAY);
        // recognize and mark
        cvClearMemStorage(pcvMStorage);
        pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarProfileFaceCascade, pcvMStorage);
        // mark
        int frameSeq=(int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);   //get frame sequence
        int column = 0;
        for(; column <pcvSeqFaces->total; column++)
        {
            if( column >= Columns/AreaMember ) break;
            r = (CvRect*)cvGetSeqElem(pcvSeqFaces, column);
            cvRectangle(frame,cvPoint(r->x,r->y-20),cvPoint(r->x + r->width,r->y + r->height),FaceCirclecolors[column % 8], 2);
            *(dynamicData+IndexX(frameSeq,column,frames)) = r->x;
            *(dynamicData+IndexY(frameSeq,column,frames)) = r->y-20;
            *(dynamicData+IndexWidth(frameSeq,column,frames)) = r->width;
            *(dynamicData+IndexHeight(frameSeq,column,frames)) = r->height+20;
        }
        int hit = column;
        // recognize and mark
        cvClearMemStorage(pcvMStorage);
        pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarFrontFaceCascade, pcvMStorage);
        for(; column-hit <pcvSeqFaces->total; column++)
        {
            if( column >= Columns/AreaMember ) break;
            r = (CvRect*)cvGetSeqElem(pcvSeqFaces, column-hit);
           cvRectangle(frame,cvPoint(r->x,r->y-20),cvPoint(r->x + r->width,r->y + r->height),FaceCirclecolors[column % 8], 2);
            *(dynamicData+IndexX(frameSeq,column,frames)) = r->x;
            *(dynamicData+IndexY(frameSeq,column,frames)) = r->y-20;
            *(dynamicData+IndexWidth(frameSeq,column,frames)) = r->width;
            *(dynamicData+IndexHeight(frameSeq,column,frames)) = r->height+20;
        }

        cvShowImage(pstrWindowsTitle2, frame);
        cvReleaseImage(&pGrayImage);

        if(27 == cvWaitKey(1))  break;
        
        if(row == 0)
        {
            ratio = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_AVI_RATIO);     //get location of frame in the video
        }
    }
    
    cvReleaseMemStorage(&pcvMStorage);
    cvDestroyWindow(pstrWindowsTitle1);
    cvDestroyWindow(pstrWindowsTitle2);
    cvReleaseCapture(&capture);
    
    plhs[0] = mxCreateNumericMatrix(0, 0, mxDOUBLE_CLASS, mxREAL);
    mxSetPr(plhs[0], dynamicData);
    mxSetM(plhs[0], frames);
    mxSetN(plhs[0], Columns);

}









