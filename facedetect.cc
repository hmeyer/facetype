#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <raspicam/raspicam_cv.h>

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
                                 "{width|640|}"
                                 "{height|480|}");
        parser.about( "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face) in a video stream.\n"
                      "You can use Haar or LBP features.\n\n" );

        face_cascade_name = parser.get<String>("face_cascade");

        //-- 1. Load the cascades
        if( !face_cascade.load( face_cascade_name ) ) { parser.printMessage(); cerr << "--(!)Error loading face cascade" << endl; return -1; };

        //-- 2. set camera params
	raspicam::RaspiCam_Cv camera;
	camera.set( CAP_PROP_FORMAT, CV_8UC1 );
        camera.set ( CAP_PROP_FRAME_WIDTH, parser.get<int>("width") );
        camera.set ( CAP_PROP_FRAME_HEIGHT, parser.get<int>("height") );
	//Open camera
	cout << "Opening Camera..." << endl;
	if (!camera.open()) {
		cerr << "Error opening the camera" << endl;
		return -1;
	}

        Typewriter typi;

        while(!typi.should_stop()) {
//                typi.wait_for_space();
                typi.print_char('\r');

                Mat frame;
                Ascii ascii;
                Mat croppedFaceImage;

                bool detected_face = false;

                while(!detected_face && !typi.should_stop()) {

                        if (!camera.grab()) {
                                cerr << "capture error" << endl;
                                continue;
                        }
                        camera.retrieve(frame);
                        if (frame.empty()) {
                                cerr << "captured empty frame" << endl;
                                continue;
                        }
			assert(frame.size.dims() == 2);
                        cout << "captured: " << frame.size[1] << " x " << frame.size[0] << endl;
			// cv::flip(frame, frame, -1);
                        cout << "detecting" << endl;

                        if (!detectAndCrop( frame, face_cascade, &croppedFaceImage, ascii.aspect() )) {
                                cerr << "didn't detect face" << endl;
                                continue;
                        }

                        detected_face = true;
                        cout << "successfully detected. displaying." << endl;
                        auto typi_print = [&typi](char c, bool bold) {
                                                  tty_print(c, bold);
//                                                  typi.print_char(c, bold ? kBold : kNormal);
                                          };
                        ascii.displayImage(&croppedFaceImage, typi_print);
/*
                        typi.print_string("\n"
                                          "          "
                                          "          "
                                          "          "
                                          "          "
                                          "          "
                                          "        "
                                          "facetype 2019", kBold);
*/
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
        equalizeHist( img, img );

        cascade.detectMultiScale( img, faces,
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
                        r.width = std::min(nw, img.size().width - r.x);
                } else {
                        int nh = r.width / crop_aspect;
                        r.y = std::max(0, r.y - (nh - r.height) / 2);
                        r.height = std::min(nh, img.size().height - r.y);
                }

                *cropped = img(r).clone();
                equalizeHist( *cropped, *cropped);
                return true;
        }
        return false;
}

