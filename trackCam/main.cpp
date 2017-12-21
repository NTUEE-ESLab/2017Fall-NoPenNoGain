#include <iostream>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include "trackCam.cpp"

using namespace std;
using namespace cv;

TrackCam* trackCam = new TrackCam();

int main(int argc, char **argv){
	if(!trackCam->init())
		return -1;
	cout<<"fine."<<endl;
	trackCam->track();

	return 0;
}

