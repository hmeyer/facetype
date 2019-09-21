#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <raspicam/raspicam_cv.h>

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <memory>

#include "ascii.h"
#include "detector.h"
#include "cv_detector.h"
#include "typewriter.h"
#include <wiringPi.h>


using namespace std;
using namespace cv;


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

string now_string(const string& format = "%F %T") {
  time_t now_c = chrono::system_clock::to_time_t(chrono::system_clock::now());
  tm now_tm = *std::localtime(&now_c);
  char buff[70];
  if (strftime(buff, sizeof buff, format.c_str(), &now_tm)) {
    return buff;
  }
  return string();
}


/** @function main */
int main( int argc, const char** argv )
{
        CommandLineParser parser(argc, argv,
                                 "{help h||}"
                                 "{face_cascade|/opt/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml|}"
                                 "{width|640|}"
                                 "{height|480|}"
                                 "{detection_width|320|}"
                                 "{detection_height|240|}");
        parser.about( "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face) in a video stream.\n"
                      "You can use Haar or LBP features.\n\n" );

	unique_ptr<Detector> detector;
	string cascade_name = parser.get<String>("face_cascade");

	detector =  make_unique<CvDetector>(cascade_name);
        detector->set_detection_size(parser.get<int>("detection_width") ,parser.get<int>("detection_height") );

        //-- set camera params
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
                typi.wait_for_space();
                typi.print_char('\r');

                Mat frame;
                Ascii ascii(Typewriter::kWidth, Typewriter::kHeight);
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

                        if (!detector->detectAndCrop( frame, ascii.aspect(), &croppedFaceImage )) {
                                cerr << "didn't detect face" << endl;
                                continue;
                        }

                        detected_face = true;
  			cout << now_string();
                        cout << " successfully detected. displaying." << endl;
                        auto typi_print = [&typi](char c, bool bold) {
                                                  tty_print(c, bold);
                                                  typi.print_char(c, bold ? kBold : kNormal);
                                          };
                        ascii.displayImage(&croppedFaceImage, typi_print);
                        typi.print_char('\n');
                        typi.print_align_right("facetype " + now_string("%Y"), kBold);
                        typi.print_char('\n');
                }

        }
        cout << endl << "clean exit" << endl;
        return 0;
}

