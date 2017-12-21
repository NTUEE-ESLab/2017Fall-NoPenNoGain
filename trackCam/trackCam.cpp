#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include "trackCam.h"

using namespace std;
using namespace cv;

TrackCam::~TrackCam(){
	//Release the camera
	if(camera->open())
		camera->release();
}

bool TrackCam::init(){
	//Return false if no camera connected successfully
	camera = new raspicam::RaspiCam_Cv;
	if(!camera->open()){
		cout << "Camera failed!" << endl;
		return false;
	}

	//Bound of HSV value for recognizing the red pixels
	lBound = Scalar(80, 80, 60);
	uBound = Scalar(100, 255, 255);

	//Return false if the surrounding would confused the process(i.e. some red objects)
	if(!setScene())
		return false;

	//Set the vertices of the screen
	setVertex();

	//Marked the edge and save the image
	drawEdge();

	//Set the bound for cropping the image to a smaller one
	setBound();

	//Set the ideal points for transforming the coordinates 
	setIdealPoint();

	return true;
}

void TrackCam::track(){
	Mat im, im_hsv_inv, im_mask;
	vector<Point> locations;
	int x_ave = 0;
	int y_ave = 0;
	int x_pre = -1;
	int y_pre = -1;
	int pix_num = 0;
	int x_trans, y_trans;
	float lamx, lamy;
	while(true){
		//Catch a frame
		camera->grab();
		camera->retrieve(im);

		//Crop it
		im = im(bound);

		//Get the red part
		cvtColor(~im, im_hsv_inv, COLOR_BGR2HSV);
		inRange(im_hsv_inv, lBound, uBound, im_mask);

		//Locate the center of the signal
		findNonZero(im_mask, locations);
		if(!locations.empty()){
			pix_num = locations.size();
			x_ave = y_ave = 0;
			for(int i=0; i<pix_num; ++i){
				x_ave += locations[i].x;
				y_ave += locations[i].y;
			}
			//Save the current position of the red spot
			x_ave /= pix_num;
			y_ave /= pix_num;

			//Transfer the coordinate
			transformation(x_ave, y_ave, x_trans, y_trans);
		}
		else x_ave = y_ave = -1;
		
		// Print the center of the signal in (origin | tranformed) coordinate
		if(x_ave == -1) cout << "\r(---, ---) | (---, ---)     " << flush;
		else cout << "\r(" << x_ave << ", " << y_ave << ") | (" << x_trans << ", " << y_trans << ")            " << flush;

		//Save current status as x_pre , y_pre for the next loop
		x_pre = x_ave;
		y_pre = y_ave;
	}
	return;
}

bool TrackCam::setScene(){
	Mat im_hsv_inv, im_mask;
	vector<Point> locations;

	//Grab the image of a empty sight
	camera->grab();
	camera->retrieve(sceneIm);

	//Convert from BGR to HSV and mask it
	cvtColor(~sceneIm, im_hsv_inv, COLOR_BGR2HSV);
	inRange(im_hsv_inv, lBound, uBound, im_mask);

	//Find if there exist some extra red object in the enpty scene
	findNonZero(im_mask, locations);

	//Return false if any red object is detected
	if(!locations.empty()){
		cout << "Dirty Scene!" << endl;
		return false;
	}

	//Save the images
	imwrite("./sceneIm.jpg", sceneIm);
	imwrite("./sceneIm_mask.jpg", im_mask);
	return true;
}

void TrackCam::setVertex(){
	//Catch the four calibration points
	for(int i=0; i<4; ++i){
		//Check until the calibration point is grabbed successfully
		bool end = false;
		while(!end){
			Mat im, im_hsv_inv, im_mask;
			vector<Point> locations;
			int pix_num = 0;

			//Count down on grabbibg action
			for(int s=5; s>0; --s){
				cout << "\rCapturing calibration point " << i << " in: " << s << flush;
				this_thread::sleep_for(chrono::seconds(1));
			}

			//Grab the image
			camera->grab();
			camera->retrieve(im);

			//Convert from BGR to HSV and mask it
			cvtColor(~im, im_hsv_inv, COLOR_BGR2HSV);
			inRange(im_hsv_inv, lBound, uBound, im_mask);

			//Find the red part
			findNonZero(im_mask, locations);

			//If red region is detected, compute its center. Otherwise, grab again.
			if(!locations.empty()){
				pix_num = locations.size();
				for(int j=0; j<pix_num; ++j){
					x[i] += locations[j].x;
					y[i] += locations[j].y;
				}
				x[i] /= pix_num;
				y[i] /= pix_num;
				cout << "\rVertex " << i << " at (" << x[i] << ", " << y[i] << ")                   " << endl;
				end = true;
			}
		}
	}

	return;
}

void TrackCam::drawEdge(){
	Mat edgeIm = sceneIm;

	//Draw the detected edge of screen on a scene image
	line(edgeIm, Point(x[0], y[0]), Point(x[1], y[1]), Scalar(0, 255, 0), 2);
	line(edgeIm, Point(x[1], y[1]), Point(x[2], y[2]), Scalar(0, 255, 0), 2);
	line(edgeIm, Point(x[2], y[2]), Point(x[3], y[3]), Scalar(0, 255, 0), 2);
	line(edgeIm, Point(x[3], y[3]), Point(x[0], y[0]), Scalar(0, 255, 0), 2);
	
	//Save the result
	imwrite("./edgeIm.jpg", edgeIm);

	return;
}

void TrackCam::setBound(){
	//Bound the screen with a smallest rectangle
	int x_start = min(x[0], x[3]);
	int y_start = min(y[0], y[1]);
	width = max(x[1] - x[0], x[2] - x[3]);
	height = max(y[3] - y[0], y[2] - y[1]);
	
	//Set the private variable, bound
	bound = Rect(x_start, y_start, width, height);
	cout << "Set bound as:" << endl;
	cout << "   - Origin: " << "(" << x_start << ", " << y_start << ")" << endl;
	cout << "   - width  : " << bound.width <<endl;
	cout << "   - height : " << bound.height <<endl;

	//Adjust the vertices' coordination due to the change of bound
	for(int i=0; i<4; ++i){
		x[i] -= x_start;
		y[i] -= y_start;
	}

	return;
}

void TrackCam::setIdealPoint(){
	//Check if the edges are parallel
	para1 = para2 = false;
	
	//The case if DA is parallel with BC
	cout << "The Vertical bounds: ";
	if((x[3] - x[0]) * (y[2] - y[1]) == (x[2] - x[1]) * (y[3] - y[0])){
		para1 = true;
		cout << "parallel";
	}
	else cout << "not parallel" << endl;

	//The case if AB is parallel with DC
	cout << "The horizontal bounds: ";
	if((x[1] - x[0]) * (y[2] - y[3]) == (x[2] - x[3]) * (y[1] - y[0])){
		para2 = true;
		cout << "parallel";
	}
	else cout << "not parallel" << endl;

	//Coefficients of the lines' equations
	float e, f, g, h;

	//Set ideal point 1 if AB and CD are not parallel
	if(para1 == false){
		//AB: y = ax + b
		//CD: y = ex + f
		a = (float( y[1] - y[0] )) / (x[1] - x[0]);
		b = - a * x[0] + y[0];
		e = (float( y[2] - y[3] )) / (x[2] - x[3]);
		f = - e * x[3] + y[3];

		//I1 is the intersection of AB and CD
		x_I1 = - (b - f) / (a - e);
		y_I1 = x_I1 * a + b;

		//Print the result
		cout << "Ideal point 1 at: (" << x_I1 << ", " << y_I1 << ")" << endl;
	}
	else cout << "No ideal point 1" << endl;

	//Set ideal point 2 if BC and DA are not parallel
	if(para2 == false){
		//DA: y = cx + d
		//BC: y = gx + h
		c = (float( y[3] - y[0] )) / (x[3] - x[0]);
		d = - c * x[3] + y[3];
		g = (float( y[2] - y[1] )) / (x[2] - x[1]);
		h = - g * x[2] + y[2];

		//I2 is the intersection of BC and DA
		x_I2 = - (d - h) / (c - g);
		y_I2 = x_I2 * c + d;

		//Print the result
		cout << "Ideal point 2 at: (" << x_I2 << ", " << y_I2 << ")" << endl;
	}
	else cout << "No ideal point 2" << endl;

	return;
}

void TrackCam::transformation(int x_T, int y_T, int &x_trans, int &y_trans){
	//
	float x_E, x_F;

	//Find the x coordinate of point E while BC and DA being, 1. parrallel, 2. not parrallel
	if(para1 == true){
		//The case that DA is parallel with BC
		//Determine the line being parallel to DA and passing through Vt 
		//that is, y = cx + r
		float r = - c * x_T + y_T;

		//this line intersects AB at point E
		//AB: y = ax + b
		x_E = - (b - r) / (a - c);
	}
	else{
		//The case that DA is not parallel with BC
		//AB: y = ax + b
		//I2Vt: y = gx + h
		float g = (float( y_I2 - y_T )) / ( x_I2 - x_T );
		float h = - g * x_T + y_T;

		//Point E is the intersection of AB and I2Vt
		x_E = - (b - h) / (a - g);
	}

	//Find the x coordinate of point F while BC and DA being, 1. parrallel, 2. not parrallel
	if(para2 == true){
		//The case that AB is parallel with CD
		//Determine the line being parallel to AD and passing through Vt 
		//that is, y = ax + r
		float r = - a * x_T + y_T;

		//this line intersects DA at point E
		//DA: y = cx + d
		x_F = - (d - r) / (c - a);
	}
	else{
		//DA: y = cx + d
		//I1Vt: y = ex + f
		float e = (float( y_I1 - y_T )) / ( x_I1 - x_T );
		float f = - e * x_T + y_T;
	
		//Point F is the intersection of DA and I1Vt
		x_F = - (d - f) / (c - e);
	}
	
	//Reciprocal of the cross ratio on line AI1
	float k = ((x_I1 - x[0]) * (x[1] - x_E)) / ((x[1] - x[0]) * (x_I1 - x_E));
	float l = ((x_I2 - x[0]) * (x[3] - x_F)) / ((x[3] - x[0]) * (x_I2 - x_F));

	//Compute x_trans and y_trans
	x_trans = int((1 - k) * width);
	y_trans = int((1 - l) * height);

	return;
}








