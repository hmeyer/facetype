// #include "opencv2/objdetect.hpp"
// #include "opencv2/highgui.hpp"
// #include "opencv2/imgproc.hpp"
//
// #include <stdio.h>
//
// using namespace std;
// using namespace cv;
//
// /** Function Headers */
// void detectAndDisplay( Mat frame );
//
// /** Global variables */
// String face_cascade_name;
// CascadeClassifier face_cascade;
// String window_name = "Capture - Face detection";
//
// /** @function main */
// int main( int argc, const char** argv )
// {
//         CommandLineParser parser(argc, argv,
//                                  "{help h||}"
//                                  "{face_cascade|../../data/haarcascades/haarcascade_frontalface_alt.xml|}");
//
//         parser.about( "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face) in a video stream.\n"
//                       "You can use Haar or LBP features.\n\n" );
//         parser.printMessage();
//
//         face_cascade_name = parser.get<String>("face_cascade");
//         VideoCapture capture;
//         Mat frame;
//
//         //-- 1. Load the cascades
//         if( !face_cascade.load( face_cascade_name ) ) { printf("--(!)Error loading face cascade\n"); return -1; };
//
//         //-- 2. Read the video stream
//         capture.open( 0 );
//         if ( !capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }
//
//         while ( capture.read(frame) )
//         {
//                 if( frame.empty() )
//                 {
//                         printf(" --(!) No captured frame -- Break!");
//                         break;
//                 }
//
//                 //-- 3. Apply the classifier to the frame
//                 detectAndDisplay( frame );
//
//                 if( waitKey(10) == 27 ) { break; } // escape
//         }
//         return 0;
// }
//
// /** @function detectAndDisplay */
// void detectAndDisplay( Mat frame )
// {
//         std::vector<Rect> faces;
//         Mat frame_gray;
//
//         cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
//         equalizeHist( frame_gray, frame_gray );
//
//         //-- Detect faces
//         face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(60, 60) );
//
//         for ( size_t i = 0; i < faces.size(); i++ )
//         {
//                 Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
//                 ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
//                 Mat faceROI = frame_gray( faces[i] );
//         }
//         //-- Show what you got
//         imshow( window_name, frame );
// }


// #include "opencv2/objdetect/objdetect.hpp"
// #include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"
// #include "opencv2/core/core.hpp"
//
// #include "opencv2/highgui/highgui_c.h"
//
#include "ascii.h"
#include "typewriter.h"
#include <wiringPi.h>
#include <stdexcept>
#include <chrono>

#include <cctype>
#include <iostream>
#include <string>
#include <iterator>
#include <stdio.h>
//
//
// using namespace std;
// using namespace cv;
//
//
// static void help()
// {
//         cout << "\nThis program demonstrates the cascade recognizer. Now you can use Haar or LBP features.\n"
//                 "This classifier can recognize many kinds of rigid objects, once the appropriate classifier is trained.\n"
//                 "It's most known use is for faces.\n"
//                 "Usage:\n"
//                 "./facedetect [--cascade=<cascade_path> this is the primary trained classifier such as frontal face]\n"
//                 "   [--scale=<image scale greater or equal to 1, try 1.3 for example>]\n"
//                 "   [filename|camera_index]\n\n"
//                 "see facedetect.cmd for one call:\n"
//                 "./facedetect --cascade=\"opencv/data/haarcascades/haarcascade_frontalface_alt.xml\" --scale=1.3\n\n"
//                 "During execution:\n\tHit any key to quit.\n"
//                 "\tUsing OpenCV version " << CV_VERSION << "\n" << endl;
// }
//
// bool detectAndCrop( Mat& img, CascadeClassifier& cascade,
//                     double scale, Mat* cropped, double aspect);
//
// string cascadeName = "opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
//
int main( int argc, const char** argv ) {
        Typewriter typi;

        for (char c = 0; c < 255; c++) {
                std::cout << "printing: " << +c << "(" << c << ")" << std::endl;
                if (c && ((c&15)==0)) {
                        typi.print_char('\r');
                }
                typi.print_char(c);
        }
};
//
//         CvCapture* capture = 0;
//         Mat frame, frameCopy, image;
//         const string scaleOpt = "--scale=";
//         size_t scaleOptLen = scaleOpt.length();
//         const string cascadeOpt = "--cascade=";
//         size_t cascadeOptLen = cascadeOpt.length();
//         string inputName;
//
//         help();
//
//         CascadeClassifier cascade;
//         double scale = 1;
//
//         for( int i = 1; i < argc; i++ )
//         {
//                 cout << "Processing " << i << " " <<  argv[i] << endl;
//                 if( cascadeOpt.compare( 0, cascadeOptLen, argv[i], cascadeOptLen ) == 0 )
//                 {
//                         cascadeName.assign( argv[i] + cascadeOptLen );
//                         cout << "  from which we have cascadeName= " << cascadeName << endl;
//                 }
//                 else if( scaleOpt.compare( 0, scaleOptLen, argv[i], scaleOptLen ) == 0 )
//                 {
//                         if( !sscanf( argv[i] + scaleOpt.length(), "%lf", &scale ) || scale < 1 )
//                                 scale = 1;
//                         cout << " from which we read scale = " << scale << endl;
//                 }
//                 else if( argv[i][0] == '-' )
//                 {
//                         cerr << "WARNING: Unknown option %s" << argv[i] << endl;
//                 }
//                 else
//                         inputName.assign( argv[i] );
//         }
//
//         if( !cascade.load( cascadeName ) )
//         {
//                 cerr << "ERROR: Could not load classifier cascade" << endl;
//                 help();
//                 return -1;
//         }
//
//         if( inputName.empty() || (isdigit(inputName.c_str()[0]) && inputName.c_str()[1] == '\0') )
//         {
//                 capture = cvCaptureFromCAM( inputName.empty() ? 0 : inputName.c_str()[0] - '0' );
//                 int c = inputName.empty() ? 0 : inputName.c_str()[0] - '0';
//                 if(!capture) {
//                         cerr << "Capture from CAM " <<  c << " didn't work" << endl;
//                         return -1;
//                 }
//         }
//
//         Ascii ascii;
//         cv::Mat croppedFaceImage;
//
//         cout << "In capture ..." << endl;
//         cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 320);
//         cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 200);
//         cout << "resolution: " << cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH) << " x  " << cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT) << endl;
//         while(!ascii.eventHappened()) {
//                 cout << "query frame" << endl;
//                 IplImage* iplImg = cvQueryFrame( capture );
//                 cout << "arrToMat" << endl;
//                 frame = cv::cvarrToMat(iplImg);
//                 if( frame.empty() )
//                         break;
//                 cout << "copy frame" << endl;
//                 if( iplImg->origin == IPL_ORIGIN_TL )
//                         frame.copyTo( frameCopy );
//                 else
//                         flip( frame, frameCopy, 0 );
//
//                 cout << "detecting" << endl;
//                 if (detectAndCrop( frameCopy, cascade, scale, &croppedFaceImage, ascii.aspect() )) {
//                         cout << "display" << endl;
//                         ascii.displayImage(&croppedFaceImage);
//                 }
//         }
//         cvReleaseCapture( &capture );
//         return 0;
// }
//
// bool detectAndCrop( Mat& img, CascadeClassifier& cascade,
//                     double scale, Mat* cropped, double crop_aspect) {
//         double t = 0;
//         vector<Rect> faces, faces2;
//         Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
//
//         cvtColor( img, gray, COLOR_BGR2GRAY );
//         resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
//         equalizeHist( smallImg, smallImg );
//
//         t = (double)cvGetTickCount();
//         cascade.detectMultiScale( smallImg, faces,
//                                   1.1, 2, 0
//                                   //|CASCADE_FIND_BIGGEST_OBJECT
//                                   //|CASCADE_DO_ROUGH_SEARCH
//                                   |CASCADE_SCALE_IMAGE
//                                   ,
//                                   Size(30, 30) );
//         t = (double)cvGetTickCount() - t;
//         if (faces.size() == 1) {
//                 Rect r = faces[0];
//                 r.x *= scale;
//                 r.y *= scale;
//                 r.width *= scale;
//                 r.height *= scale;
//
//                 double aspect = r.width * 1.0 / r.height;
//
//                 if (crop_aspect > aspect) {
//                         int nw = crop_aspect * r.height;
//                         r.x = std::max(0, r.x - (nw - r.width) / 2);
//                         r.width = std::min(nw, gray.size().width - r.x);
//                 } else {
//                         int nh = r.width / crop_aspect;
//                         r.y = std::max(0, r.y - (nh - r.height) / 2);
//                         r.height = std::min(nh, gray.size().height - r.y);
//                 }
//
//                 *cropped = gray(r).clone();
//                 equalizeHist( *cropped, *cropped);
//                 return true;
//         }
//         return false;
// }
