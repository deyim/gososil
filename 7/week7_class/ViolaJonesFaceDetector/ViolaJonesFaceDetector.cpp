// ViolaJonesFaceDetector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <Windows.h>

using namespace std;
using namespace cv;

bool detectAndDraw( Mat& img, Mat& imgMarked, Mat& imgCropped,
                   CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
                   double scale);
int GetDirFiles(const char *, vector<string> *);
void MakeIntegral(Mat& img, double ** imgIntegral);
double Integral_Compute(int x1, int y1, int x2, int y2, double ** imgIntegral);
double Original_Compute(int x1, int y1, int x2, int y2, Mat& img);

// OpenCV is installed in C:/opencv249
String cascadeName = "C:/OpenCV249/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
String nestedCascadeName = "C:/OpenCV249/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
							

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	bool detected;
	double scale = 1.3, t = 0;
	//double Integral_value = 0, Original_value = 0;
	string inputFileName, outFileName;
	Mat image, imageMarked, imageCropped, imageCropResized, imageMean;
    CascadeClassifier cascade, nestedCascade;
    
	string inputDir = "../DBlab7";  // input directory
	string outputDir = "../output";
	string outMarkDir = outputDir + "/marked/";  // output directory for marked face and eyes
	string outCropDir = outputDir + "/cropped/"; // output directory for cropped/resized images
	
	vector<string> fileList, fileListCropped;
	GetDirFiles(inputDir.c_str(), &fileList);

	//for (i=0; i<150; i++)
	//	cout << i << " " << fileList[i] << endl;

	if( !nestedCascade.load( nestedCascadeName ) ) {
        cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
		return -1;
	}

	
    if( !cascade.load( cascadeName ) ) {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }
	
	cvNamedWindow( "result", 1 );
	//
	int x1 = 15, y1 = 23, x2 = 33, y2 = 51;
	inputFileName = inputDir;
	inputFileName = inputFileName + "/" + fileList[0];
	image = imread( inputFileName, 1 );	

	//		실습문제 1번을 위한 변수 선언 및 함수 호출
	double **imgIntegral = NULL; 
	imgIntegral = (double **)malloc(sizeof(double*) * image.rows);
	for (int i = 0 ; i< image.rows ; i++){
		imgIntegral[i] = (double *)malloc(sizeof(double) * image.cols);
	}
	MakeIntegral(image,imgIntegral);	// Integral Image를 생성해주는 함수

	// Integral Image를 이용한 사각형 넓이의 합 구하는 시간 측정
	t = (double)cvGetTickCount();	
	double Integral_value = Integral_Compute(x1,y1,x2,y2,imgIntegral);
	t = (double)cvGetTickCount() - t;
	printf( "Integral detection time = %g ms, Value : %f\n", t/((double)cvGetTickFrequency()*1000.), Integral_value);

	// 일반적인 방법을 이용하여 사각형 넓이의 합 구하는 시간 측정
	t = (double)cvGetTickCount();
	double Original_value = Original_Compute(x1,y1,x2,y2,image);
	t = (double)cvGetTickCount() - t;
    printf( "Original detection time = %g ms, Value : %f\n", t/((double)cvGetTickFrequency()*1000.), Original_value);
	
	
	
	for (i=0; i<fileList.size(); i++) {
		inputFileName = inputDir;
		inputFileName = inputFileName + "/" + fileList[i];

		image = imread( inputFileName, 1 );
		if(image.empty()) {
			cout << "Couldn't read " << inputFileName << endl;
			continue;
		}
		else {
			detected = 0;
			detected = detectAndDraw( image, imageMarked, imageCropped, cascade, nestedCascade, scale );
		}
		
		// We better rotate the face image using detected eye coordinates, then crop and resize, 
		// but we will simply resize the detected face region for simplicity
		if (detected) {
			
			resize(imageCropped, imageCropResized, Size(200,200));
			outFileName = outCropDir + '/' + fileList[i];
			imwrite(outFileName, imageCropResized);
			//
			// ADD YOUR CODE HERE
			// 
			// 1. Resize imageCropped to become an image of size 200x200
			//	  Use the function cv::resize()
			//    Use the variable imageCropResized for resized image
			// 
			// 2. Save the imageCropResized to outCropDir
			//    Make the file name same as the original input 
			//

		}
		
		//
		// ADD YOUR CODE HERE
		// 
		// 2. Save the imageMarked to outMarkDir
		//    Make the file name same as the original input 
		//
		outFileName = outMarkDir + '/' + fileList[i];
		imwrite(outFileName, imageMarked);

		imshow( "result", imageMarked );
		waitKey(50); // wait for numbered milliseconds
	}
	
	int nCrop = GetDirFiles(outCropDir.c_str(), &fileListCropped);
	imageMean = Mat(200, 200, CV_32F, 0);  // 32bit float	// 계산된 평균 픽셀값을 저장해나가는 변수

	Mat temp;	// 기존 이미지의 datatype을 Convert해준 것을 임시 저장하는 변수
	double beta = 1.0/nCrop; //	평균값 계산때 사용

	for (i=0; i<nCrop; i++) {
		inputFileName = outCropDir + "/" + fileListCropped[i];
		image = imread(inputFileName, 0); // read as grayscale
		
		image.convertTo(temp, CV_32F);
		imageMean = imageMean + (temp*beta);
		//
		// ADD YOUR CODE HERE
		// 
		// 1. Convert the unsigned int type image to 32 bit floating point
		//    Use the function convertTo()
		//    
		// 2. Add the 32 bit floating point iamge matrix to imageMean
		//    The pixel value needs to be scaled before added
		//    If all the image are added first then divided by nCrop, 
		//     an overflow may happen, iespecially when nCrop is very large
		//

	}

	outFileName = outputDir + "/imageMean.jpg";
	imwrite( outFileName, imageMean );

    cvDestroyWindow("result");

    return 0;
}

void MakeIntegral(Mat& img, double ** imgIntegral){		
			
	// ADD YOUR CODE HERE
	// 
	// 1. Make the Integral_compute_fuction
	double result = 0, init = 0;
	int i,j,x,y;
	for(j = 0 ; j < img.cols ; j++){
		double b = img.at<uchar>(j,0);
		if(j==0) imgIntegral[0][j] = img.at<uchar>(j,0);
		else imgIntegral[0][j] = (img.at<uchar>(j,0) + imgIntegral[0][j-1]);
	}
	for(i = 0 ; i < img.rows ; i++){
		double a = img.at<uchar>(0,i);
		if(i==0) imgIntegral[i][0] = img.at<uchar>(0,i);
		else imgIntegral[i][0] = img.at<uchar>(0,i) + imgIntegral[i-1][0];
	} 

	for(i = 1 ; i < img.rows ; i++){
		for(j = 1 ; j < img.cols ; j++){
			imgIntegral[i][j] = imgIntegral[i-1][j] + imgIntegral[i][j-1] - imgIntegral[i-1][j-1] + img.at<uchar>(j,i);
		}
	}
			
}

double Integral_Compute(int x1, int y1, int x2, int y2, double ** imgIntegral){
	// ADD YOUR CODE HERE
	double result=0;
	double inte1, inte2, inte3, inte4;
	inte4 = imgIntegral[x2][y2];
	inte3 = imgIntegral[x2][y1-1];
	inte2 = imgIntegral[x1-1][y2];
	inte1 = imgIntegral[x1-1][y1-1];

	result = inte4 - inte3 - inte2 + inte1;
	return result;
	
}

double Original_Compute(int x1, int y1, int x2, int y2, Mat& img){

	double result = 0;
	for(int i = x1; i <= x2; i++){
		for(int j = y1; j <= y2; j++){
			result += img.at<uchar>(j,i);
		}
	}	
	return result;
}


//분석
bool detectAndDraw( Mat& img, Mat& imgMarked, Mat& imgCropped,
                   CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
                   double scale)
{
    int i = 0;
    double t = 0;
    vector<Rect> faces;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );
    t = (double)cvGetTickCount() - t;
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
	
	imgMarked = img.clone();

    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;
        center.x = cvRound((r->x + r->width*0.5)*scale);
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        //circle( img, center, radius, color, 3, 8, 0 );
		rectangle( imgMarked,
           Point( center.x-radius, center.y-radius ),
           Point( center.x+radius, center.y+radius),
           Scalar( 0, 255, 0 ),  // B, G, R
           3,
           8 );

		// we assume there is only one face in an image; if multiple faces are detected,
		// only the last one will be saved
		imgCropped = img(Rect(center.x-radius,center.y-radius,radius*2,radius*2));

        if( nestedCascade.empty() )
            continue;
        smallImgROI = smallImg(*r);
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CV_HAAR_FIND_BIGGEST_OBJECT
            //|CV_HAAR_DO_ROUGH_SEARCH
            //|CV_HAAR_DO_CANNY_PRUNING
            |CV_HAAR_SCALE_IMAGE
            ,
            Size(30, 30) );
        for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
        {
            center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
            center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
            radius = cvRound((nr->width + nr->height)*0.25*scale);
            //circle( img, center, radius, color, 3, 8, 0 );
			circle( imgMarked, center, radius, Scalar( 0, 255, 0 ), 3, 8, 0 );
        }
    }

	if (faces.size() > 0)
		return 1;
	else
		return 0;

}

int GetDirFiles(const char *path_name, vector<string> *fileList)
{
	WIN32_FIND_DATA FN;
	HANDLE hFind;
	char search_arg[MAX_PATH], new_file_path[MAX_PATH];
	string cs;
	sprintf(search_arg, "%s\\*.*", path_name);
	int i;

	hFind = FindFirstFile( (LPCTSTR) search_arg, &FN);
	//cout << (hFind != INVALID_HANDLE_VALUE) << endl << FN.cFileName << endl;
	if (hFind != INVALID_HANDLE_VALUE) {
		i = 0;
		do {
			cs = FN.cFileName;
			if (cs != "." && cs != "..") {
				fileList->push_back(cs);
				//cout << i << "  " << cs << "  " << fileList->at(i) << endl;
				i++;
			}
		} while (FindNextFile(hFind, &FN) != 0);
		if (GetLastError() == ERROR_NO_MORE_FILES)
			FindClose(hFind);
	}

	return i;
}
