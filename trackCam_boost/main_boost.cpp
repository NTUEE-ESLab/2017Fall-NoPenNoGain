#include <iostream>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include "trackCam_boost.cpp"

using namespace std;
using namespace cv;

TrackCam* trackCam = new TrackCam();

int main(int argc, char **argv){
	if(!trackCam->init())
		return -1;
	cout<<"fine."<<endl;
	
	//thread thread_frame = trackCam->frameThread();
	trackCam->track();
	
	//trackCam->stop();
	//thread_frame.join();
	return 0;
}

