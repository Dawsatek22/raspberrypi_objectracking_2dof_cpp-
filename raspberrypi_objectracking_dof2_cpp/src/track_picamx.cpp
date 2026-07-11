/* this is a c++ code to stream on the raspberry pi 
camera(module 2 currently) to track the yellow color in 
the x direction with the servo
*/
#include <iostream> // for input and output stream
#include <opencv2/opencv.hpp> // to use opencv functions
#include <libcamera_app.hpp> // to use libcamera functions
#include <lccv.hpp> // to use lccv functions
#include <PiPCA9685/PCA9685.h> // pca9685 header

#define SERVOMIN 300// This is the minimum pulse length count (out of 4096)
#define SERVOMAX 575// This is tqhe maximum? pulse length count (out of 4096)
// the map function is created below to map the SERVOMIN and SERVOMAX values
long mapservo(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int pulsval; // pulse value
int servoval; // map value for thr servos
int position; //  value for the servo
int centerx= 91 ; // is the value of the center x value that was calculated
int x_medium; //  x range value  thats gets measured
int pinx = 0; // the pin for the x direction
using namespace cv;

int main() {

PiPCA9685::PCA9685 track{"/dev/i2c-1",0x40}; // creates servo object.
// if PCA9685 default adress = 0x40 you can also do: PiPCA9685::PCA9685 track{}; instead. 
track.set_pwm_freq(60.0);
servoval = mapservo(pulsval,0,90,SERVOMIN,SERVOMAX);
track.set_pwm(pinx,180,servoval); //  servos is calibrated
std::cout << "servo is set to 180 degrees angle"<< '\n';
sleep(2);
 

uint32_t num_cams = LibcameraApp::GetNumberCameras();
std::cout << "Found " << num_cams << " cameras." << std::endl;
uint32_t width = 980; // the width of the camera
uint32_t height = 580; // height of the camera

Mat frame = Mat(height, width, CV_8UC3); // Mat object is made


// below are lccv camera configuration
lccv::PiCamera cam(0);
cam.options->brightness=0.5; // warning brightness can affect performance
cam.options->video_width=width;
cam.options->video_height=height;
cam.options->framerate=30;
cam.options->verbose=true;
cam.startVideo();





while (true) {
 

// yellow wraps around hue=0, so use two ranges.
Scalar lower_color1(22, 38, 160);
Scalar upper_color1(33, 244, 255);
Scalar lower_color2(23, 39, 170);
Scalar upper_color2(34, 244, 255);
Mat mask1 ,mask2, mask, hsv; // creates mat objects for mask and hsv
cvtColor(frame , hsv,COLOR_BGR2HSV);
inRange(hsv,lower_color1,upper_color1,mask1);
inRange(hsv,lower_color1,upper_color2,mask2);
mask = mask1 | mask2 ;

if (!cam.getVideoFrame(frame,1000)) {
std::cout << "frame not openend"<<'\n';
cam.stopVideo();
break;
}

// Clean noise before contour extraction.
Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(5,5)); // creates structuring element
erode(mask, mask, kernel); // erodes mask
dilate(mask, mask, kernel); // dillates mask

std::vector<std::vector<Point>> contours; /// creates coutours

findContours(mask, contours,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // finds coutours
// below checks if the frame video frame is availabe



 for (std::size_t i = 0; i < contours.size(); ++i) {
        double const area = contourArea(contours[i]);
        if (area <= 500) {
            continue;
        }
           // creates object for detecting color
         Rect const box = boundingRect(contours[i]);
        x_medium = int(box.x + box.width/ 2   ); // is the x direction converted into a int
        //  puts a rectangle on countour
        rectangle(frame, box,Scalar(60, 255, 255), 2);
        std::cout << "color is " <<  area << '\n';
       // put the color name on the countour
       putText(
            frame,
            "yellow",
            box.tl(),
            cv::FONT_HERSHEY_SIMPLEX,
            1.0,
            cv::Scalar(255, 230, 70),
            2
        );
    }

 int error = x_medium/6; // supossed to be the offset
//position = error;

std::cout << "position of center" << centerx <<'\n';
std::cout << "position of error" << error <<'\n';
std::cout << "position of x_medium" << x_medium <<'\n';

if (error > centerx) {


    position += 4;
}



if (error < centerx) {


    position -= 4;
}

// position limits are set below
if (position < 1) {

position = 0;
std::cout << "position of servos is reached 0" << '\n';

}



if (position > 180 ) {

position = 180;
std::cout << "position of servos is reached 180" << '\n';

}


 track.set_pwm(pinx,position,servoval); // moves servos acording to the position value
std::cout << "angle of x direction ="<< position <<'\n';

// imshow("hsv",hsv); // optional
// imshow("mask",mask); // optional
 imshow("show_coutours",frame); // now shows frame

if (waitKey(1) == ('q')) { // breaks loop when pressed q
    cam.stopVideo();
    destroyAllWindows();
    break;

}}}
