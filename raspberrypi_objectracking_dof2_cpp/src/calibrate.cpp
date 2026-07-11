// this is a code to calibrate the x and y direction servos
#include <unistd.h> // if you need the sleep function
#include <iostream> //for input and output
#include <PiPCA9685/PCA9685.h> // pca9685 header

#define SERVOMIN 300// This is the minimum pulse length count (out of 4096)
#define SERVOMAX 575// This is the maximum? pulse length count (out of 4096)
// the map function is created below to map the SERVOMIN and SERVOMAX values
long mapservo(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
int xpin = 0;
int ypin = 1;
int pulsval; // pulse value
int servoval;

int main() {

  PiPCA9685::PCA9685 pca{"/dev/i2c-1",0x40};
 pca.set_pwm_freq(60.0); // current frequency used
int servoval = mapservo(pulsval,0,180,SERVOMIN,SERVOMAX);



  

    pca.set_pwm(xpin,0,servoval);
    pca.set_pwm(ypin,180,servoval);
   sleep(1);

  
std::cout << "servo is calibrated to prefered position" << '\n';
}