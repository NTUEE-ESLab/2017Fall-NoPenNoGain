#include <iostream>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include "trackCam.cpp"

using namespace std;

TrackCam* trackCam = new TrackCam();

int main(int argc, char **argv){
	if(!trackCam->init())
		return -1;
	cout<<"Camera initialization successed."<<endl;
	trackCam->track();

	return 0;
}

