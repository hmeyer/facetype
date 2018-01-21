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
void detectAndDisplay( Mat frame );
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
                                 "{face_cascade|../../data/haarcascades/haarcascade_frontalface_alt.xml|}");
        parser.about( "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face) in a video stream.\n"
                      "You can use Haar or LBP features.\n\n" );

        face_cascade_name = parser.get<String>("face_cascade");
        VideoCapture capture;

        //-- 1. Load the cascades
        if( !face_cascade.load( face_cascade_name ) ) { parser.printMessage(); cerr << "--(!)Error loading face cascade" << endl; return -1; };

        //-- 2. Read the video stream
        capture.open( 0 );
        if ( !capture.isOpened() ) { cerr << "--(!)Error opening video capture" << endl; return -1; }
        capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 400);


        Typewriter typi;

        while(!typi.should_stop()) {
                typi.wait_for_space();
                typi.print_char('\r');

                Mat frame;
                Ascii ascii;
                Mat croppedFaceImage;


                while(!capture.read(frame)) ;
                if( frame.empty() )
                {
                        cerr << " --(!) No captured frame -- Break!" << endl;
                        continue;
                }

                cout << "detecting" << endl;
                if (detectAndCrop( frame, face_cascade, &croppedFaceImage, ascii.aspect() )) {
                        cout << "display" << endl;
                        auto typi_print = [&typi](char c, bool bold) {
                                                  tty_print(c, bold);
                                                  typi.print_char(c, bold ? kBold : kNormal);
                                          };
                        ascii.displayImage(&croppedFaceImage, typi_print);
                }


                //                //-- 3. Apply the classifier to the frame
                //                detectAndDisplay( frame );
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
