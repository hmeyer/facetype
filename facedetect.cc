#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <stdio.h>
#include <iostream>

#include "ascii.h"
#include "typewriter.h"
#include <wiringPi.h>


using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat& frame, CascadeClassifier& cascade, double crop_aspect );
bool detectAndCrop( Mat& img, CascadeClassifier& cascade,
                    Mat* cropped, double crop_aspect);

/** Global variables */
String face_cascade_name;
CascadeClassifier face_cascade;
String window_name = "Capture - Face detection";


void tty_print(char c, bool bold) {
        if (c == '\r') {
                std::cout << std::endl;
                return;
        }
        if (bold) {
                std::cout << "\e[1m";
        } else {
                std::cout << "\e[0m";
        }
        std::cout << c << std::flush;
}

/** @function main */
int main( int argc, const char** argv )
{
        CommandLineParser parser(argc, argv,
                                 "{help h||}"
                                 "{face_cascade|/opt/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml|}"
                                 "{video_device|/dev/video0|}");
        parser.about( "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face) in a video stream.\n"
                      "You can use Haar or LBP features.\n\n" );

        face_cascade_name = parser.get<String>("face_cascade");
        VideoCapture capture;

        //-- 1. Load the cascades
        if( !face_cascade.load( face_cascade_name ) ) { parser.printMessage(); cerr << "--(!)Error loading face cascade" << endl; return -1; };

	String video_device = parser.get<String>("video_device"); 

        //-- 2. Read the video stream
        capture.open( video_device );
        if ( !capture.isOpened() ) { cerr << "--(!)Error opening video capture" << endl; return -1; }
        capture.set(CAP_PROP_FRAME_WIDTH, 320);
        capture.set(CAP_PROP_FRAME_HEIGHT, 200);

/*
        Mat frame;
        Ascii ascii;
        cout << "capture" << endl;
        while (  capture.read(frame) )
        {
                if( frame.empty() )
                {
                        printf(" --(!) No captured frame -- Break!");
                        break;
                }
		// cv::flip(frame, frame, -1);
                cout << "detecting" << endl;
                //-- 3. Apply the classifier to the frame
                detectAndDisplay( frame,face_cascade, ascii.aspect());
       
                int c = waitKey(10);
                if( (char)c == 27 ) { break; } // escape
                cout << "capture" << endl;
        }
*/
        Typewriter typi;

        while(!typi.should_stop()) {
//                typi.wait_for_space();
                typi.print_char('\r');

                Mat frame;
                Ascii ascii;
                Mat croppedFaceImage;

                bool detected_face = false;

                while(!detected_face && !typi.should_stop()) {

                        if (!capture.read(frame)) {
                                cerr << "capture error" << endl;
                                continue;
                        }
                        if (frame.empty()) {
                                cerr << "captured empty frame" << endl;
                                continue;
                        }
			assert(frame.size.dims() == 2);
                        cout << "captured: " << frame.size[1] << " x " << frame.size[0] << endl;
			// cv::flip(frame, frame, -1);
                        cout << "detecting" << endl;
/*
                        if (!detectAndCrop( frame, face_cascade, &croppedFaceImage, ascii.aspect() )) {
                                cerr << "didn't detect face" << endl;
                                continue;
                        }
*/
                        detected_face = true;
                        cout << "successfully detected. displaying." << endl;
                        auto typi_print = [&typi](char c, bool bold) {
                                                  tty_print(c, bold);
                                                  typi.print_char(c, bold ? kBold : kNormal);
                                          };
/*
                        ascii.displayImage(&croppedFaceImage, typi_print);
                        typi.print_string("\n"
                                          "          "
                                          "          "
                                          "          "
                                          "          "
                                          "          "
                                          "        "
                                          "facetype 2019", kBold);
*/
//                        ascii.displayImage(&croppedFaceImage, tty_print);
			Mat gray( cvRound (frame.rows), cvRound(frame.cols), CV_8UC1 );
			cvtColor( frame, gray, COLOR_RGB2GRAY );
                        ascii.displayImage(&gray, tty_print);
                }

                //-- 3. Apply the classifier to the frame
                // detectAndDisplay(frame, face_cascade);
        }
        cout << endl << "clean exit" << endl;
        return 0;
}

bool detectAndCrop( Mat& img, CascadeClassifier& cascade,
                    Mat* cropped, double crop_aspect) {
        vector<Rect> faces;
        Mat gray( cvRound (img.rows), cvRound(img.cols), CV_8UC1 );

        cvtColor( img, gray, COLOR_RGB2GRAY );
        equalizeHist( gray, gray );

        cascade.detectMultiScale( gray, faces,
                                  1.1, 2, 0
                                  //|CASCADE_FIND_BIGGEST_OBJECT
                                  //|CASCADE_DO_ROUGH_SEARCH
                                  |CASCADE_SCALE_IMAGE
                                  ,
                                  Size(20, 20) );
        if (faces.size() > 0) {
                Rect r = faces[0];

                double aspect = r.width * 1.0 / r.height;

                if (crop_aspect > aspect) {
                        int nw = crop_aspect * r.height;
                        r.x = std::max(0, r.x - (nw - r.width) / 2);
                        r.width = std::min(nw, gray.size().width - r.x);
                } else {
                        int nh = r.width / crop_aspect;
                        r.y = std::max(0, r.y - (nh - r.height) / 2);
                        r.height = std::min(nh, gray.size().height - r.y);
                }

                *cropped = gray(r).clone();
                equalizeHist( *cropped, *cropped);
                return true;
        }
        return false;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat& frame, CascadeClassifier& cascade, double crop_aspect )
{
        std::vector<Rect> faces;
        Mat frame_gray;

        cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
        equalizeHist( frame_gray, frame_gray );

        //-- Detect faces
        cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(20, 20) );

        for( size_t i = 0; i < faces.size(); i++ )
        {
                Rect r = faces[i];

                double aspect = r.width * 1.0 / r.height;

                if (crop_aspect > aspect) {
                        int nw = crop_aspect * r.height;
                        r.x = std::max(0, r.x - (nw - r.width) / 2);
                        r.width = std::min(nw, frame_gray.size().width - r.x);
                } else {
                        int nh = r.width / crop_aspect;
                        r.y = std::max(0, r.y - (nh - r.height) / 2);
                        r.height = std::min(nh, frame_gray.size().height - r.y);
                }

                Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
                ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
                Point p1( r.x, r.y );
                Point p2( r.x+r.width, r.y+r.height );
                rectangle( frame, p1, p2, Scalar( 255, 0, 255 ), 4);

                Mat faceROI = frame_gray( faces[i] );
                std::vector<Rect> eyes;

                // //-- In each face, detect eyes
                // eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
                //
                // for( size_t j = 0; j < eyes.size(); j++ )
                // {
                //         Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
                //         int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
                //         circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
                // }
        }
        //-- Show what you got
        imshow( window_name, frame );
}
