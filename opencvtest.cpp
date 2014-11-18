#include"mex.h"

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    char name[256];
    int buflens =mxGetNumberOfElements(prhs[0]);
    mxGetString(prhs[0], name, buflens+1);
     if(!mxIsChar(prhs[0]))
     {
        mexErrMsgTxt("First parameter must be string/n");
        return ;
     }
    mexPrintf(name);
    IplImage * img = cvLoadImage(name);
     if(img->imageData == NULL)
     {
        mexErrMsgTxt("Error in image/n");
        return;
     }

    cvNamedWindow("test");
    cvShowImage("test",img);

    cvWaitKey(0);

    return;

}