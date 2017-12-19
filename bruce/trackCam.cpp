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
	if(camera->open())
		camera->release();
}

bool TrackCam::init(){
	camera = new raspicam::RaspiCam_Cv;
	if(!camera->open()){
		cout<<"Camera failed!"<<endl;
		return false;
	}
	lBound = Scalar(80, 80, 60);
	uBound = Scalar(100, 255, 255);
	if(!setScene())
		return false;
	setVertex();
	drawEdge();
	setBound();
	setParam();
	return true;
}

void TrackCam::track(){
	Mat im, im_hsv_inv, im_mask;
	//Rect bound = getBound();
	Rect bound = Rect(600, 300, 400, 500);
	vector<Point> locations;
	int x_ave = 0;
	int y_ave = 0;
	int x_pre = -1;
	int y_pre = -1;
	int pix_num = 0;
	while(true){
		//Catch a frame
		camera->grab();
		camera->retrieve(im);

		//Crop it
		//im = im(bound);

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
			x_ave /= pix_num;
			y_ave /= pix_num;
		}
		else x_ave = y_ave = -1;
		
		// Print the center of the signal
		if(x_ave == -1) cout<<"\r(---, ---)"<<flush;
		else cout<<"\r("<<x_ave<<", "<<y_ave<<")"<<flush;

		//if(x_ave != -1 && x_pre != -1){
		//	line(im_track, Point(x_pre, y_pre), Point(x_ave, y_ave), Scalar(0, 255, 0), 5);

		//Set x_pre , y_pre
		x_pre = x_ave;
		y_pre = y_ave;
	}
	return;
}

bool TrackCam::setScene(){
	Mat im_hsv_inv, im_mask;
	vector<Point> locations;
	camera->grab();
	camera->retrieve(sceneIm);
	imwrite("./sceneIm.jpg", sceneIm);

	cvtColor(~sceneIm, im_hsv_inv, COLOR_BGR2HSV);
	inRange(im_hsv_inv, lBound, uBound, im_mask);
	imwrite("./sceneIm_mask.jpg", im_mask);

	findNonZero(im_mask, locations);
	if(!locations.empty()){
		cout<<"Dirty Scene!"<<endl;
		return false;
	}
	return true;
}

void TrackCam::setVertex(){
	for(int i=0; i<4; ++i){
		bool end = false;
		while(!end){
			Mat im, im_hsv_inv, im_mask;
			vector<Point> locations;
			int pix_num = 0;
			for(int s=5; s>0; --s){
				cout<<"\rCapturing calibration point "<<i<<" in: "<<s<<flush;
				this_thread::sleep_for(chrono::seconds(1));
			}
			camera->grab();
			camera->retrieve(im);

			cvtColor(~im, im_hsv_inv, COLOR_BGR2HSV);
			inRange(im_hsv_inv, lBound, uBound, im_mask);

			findNonZero(im_mask, locations);
			if(!locations.empty()){
				pix_num = locations.size();
				for(int j=0; j<pix_num; ++j){
					x[i] += locations[j].x;
					y[i] += locations[j].y;
				}
				x[i] /= pix_num;
				y[i] /= pix_num;
				cout<<"\rVertex "<<i<<" at ("<<x[i]<<", "<<y[i]<<")                   "<<endl;
				end = true;
			}
		}
	}

	return;
}

void TrackCam::drawEdge(){
	Mat edgeIm = sceneIm;

	line(edgeIm, Point(x[0], y[0]), Point(x[1], y[1]), Scalar(0, 255, 0), 5);
	line(edgeIm, Point(x[1], y[1]), Point(x[2], y[2]), Scalar(0, 255, 0), 5);
	line(edgeIm, Point(x[2], y[2]), Point(x[3], y[3]), Scalar(0, 255, 0), 5);
	line(edgeIm, Point(x[3], y[3]), Point(x[0], y[0]), Scalar(0, 255, 0), 5);
	
	imwrite("./edgeIm.jpg", edgeIm);

	return;
}

void TrackCam::setBound(){
	int x_start = min(x[0], x[3]);
	int y_start = min(y[0], y[1]);
	int x_len = max(x[1] - x[0], x[2] - x[3]);
	int y_len = max(y[3] - y[0], y[2] - y[1]);
	
	bound = Rect(x_start, y_start, x_len, y_len);

	return;
}

void TrackCam::setParam(){
	bx0 = x[0] - x[1] + x[2] - x[3];
	bx1 = x[0] - x[1];
	bx2 = x[0] - x[3];
	by0 = y[0] - y[1] + y[2] - y[3];
	by1 = y[0] - y[1];
	by2 = y[0] - y[3];

	return;
}

void TrackCam::calibration(int x_t, int y_t, float &lamx, float &lamy){
	float a = float(bx1 * by0 - bx0 * by1);
	float b = float(- by0 * (x[0] - x_t) + bx0 * (y[0] - y_t) + bx2 * by1 - bx1 * by2);
	float c = float(by2 * (x[0] - x_t) - bx2 * (y[0] - y_t));

	float sq = sqrt(b * b - 4 * a * c);

	lamx = (- b + sq) / (2 * a);
	if(lamx > 1 || lamx < 0)
		lamx = (- b - sq) / (2 * a);
	lamy = (bx1 * lamx - (x[0] - x_t)) / (bx0 * lamx - bx2);

	return;
}

		
