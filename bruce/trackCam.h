#ifndef TRACKCAM_H
#define TRACKCAM

#include <iostream>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>

using namespace cv;

class TrackCam{
	public:
		~TrackCam();

		bool init();
		void track();
	private:
		//Private Variables
		raspicam::RaspiCam_Cv* camera;
		Mat sceneIm;
		int x[4], y[4];	//Observed positions of the target screen's vertices
		int width, height;	//Target screen size
		//int bx0, bx1, bx2, by0, by1, by2; //For solving the coordination trasfering problem
		float a, b;	//AB: y = ax + b
		float c, d;	//DA: y = ax + b
		float x_I1, y_I1, x_I2, y_I2; //Ideal point
		Rect bound;
		Scalar lBound, uBound; //HSV range for recognizing the red region

		//Private functions
		bool setScene();
		void setVertex();
		void drawEdge();
		void setBound();
		void setIdealPoint();
		void transformation(int, int, float&, float&);
};

#endif //TRACKCAM_H
