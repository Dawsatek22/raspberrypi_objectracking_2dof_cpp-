/* this is a c++ code to stream on the raspberry pi camera(module 2 currently) to detect the color 
yellow and find the center of y direction
*/
#include <iostream> // for input and output stream
#include <opencv2/opencv.hpp> // to use opencv functions
#include <libcamera_app.hpp> // to use libcamera functions
#include <lccv.hpp> // to use lccv functions
using namespace cv;
int centery =48; // is the value of the center y value that was calculated
int y_medium; //  y range value  thats gets measured

int main() {

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
mask =mask1 | mask2 ;

// Clean noise before contour extraction.
Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(5,5)); // creates structuring element
erode(mask, mask, kernel); // erodes mask
dilate(mask, mask, kernel); // dillates mask

std::vector<std::vector<Point>> contours; /// creates coutours

findContours(mask, contours,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // finds coutours
// below checks if the frame video frame is availabe
if (!cam.getVideoFrame(frame,1000)) {
std::cout << "frame not openend"<<'\n';
cam.stopVideo();
break;
}
      for (std::size_t i = 0; i < contours.size(); ++i) {
        double const area = contourArea(contours[i]);
        if (area <= 500) {
            continue;
        }
           // creates object for detecting color
         Rect const box = boundingRect(contours[i]);
       
        y_medium = int(box.y + box.height/ 2   ); // is the x direction converted into a int
        //  puts a rectangle on countour
        rectangle(frame, box, cv::Scalar(0, 0, 255), 2);
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
int error = y_medium/6; // suppossed to be the offset
std::cout << "position of center" << centery <<'\n';
std::cout << "position of error" << error <<'\n';
std::cout << "position of y_medium" << y_medium <<'\n';

//imshow("hsv",hsv); // optional
// imshow("mask",mask); // optional
 imshow("show_coutours",frame); // now shows frame

if (waitKey(1) == ('q')) {

     destroyAllWindows();
     cam.stopVideo();
  break;
 return 0;


    }

}}