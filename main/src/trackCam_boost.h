#ifndef TRACKCAM_H
#define TRACKCAM
#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>

using namespace std;
using namespace cv;

class TrackCam{
	public:
		inline ~TrackCam();

		inline bool init();
		inline void track();
		inline void frame();
		inline thread frameThread();
		inline void stopFrame();
	private:
		//Private Variables
		raspicam::RaspiCam_Cv* camera;
		Mat sceneIm;
		int x[4], y[4];	//Observed positions of the target screen's vertices
		int width, height;	//Target screen size
		bool para1, para2;
		//int bx0, bx1, bx2, by0, by1, by2; //For solving the coordination trasfering problem
		float a, b;	//AB: y = ax + b
		float c, d;	//DA: y = ax + b
		float x_I1, y_I1, x_I2, y_I2; //Ideal point
		Rect bound;
		Scalar lBound, uBound; //HSV range for recognizing the red region
		bool stop = 0; //flag for stopping the frame thread

		//Private functions
		inline bool setScene();
		//void setVertex();
		//void drawEdge();
		//void setBound();
		//void setIdealPoint();
		//void transformation(int, int, float&, float&);
		//void setParam();
};

#endif //TRACKCAM_H
