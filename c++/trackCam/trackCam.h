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
		bool para1, para2;	//Whether each (AB and CD) and (BC and DA) are parallel
		float a, b;		//AB: y = ax + b
		float c, d;		//DA: y = cx + d
		float x_I1, y_I1, x_I2, y_I2; //Ideal point
		Rect bound;
		Scalar lBound, uBound; //HSV range for recognizing the red region

		//Private functions
		bool setScene();
		void setVertex();
		void drawEdge();
		void setBound();
		void setIdealPoint();
		void transformation(int, int, int&, int&);
};

#endif //TRACKCAM_H
