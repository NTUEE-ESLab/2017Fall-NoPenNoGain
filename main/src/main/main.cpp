#include<iostream>
#include<string>

#include<thread>
#include<mutex>
#include<condition_variable>

#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include "trackCam.cpp"



extern "C"{
	#include <stdio.h>
	#include <stdlib.h>
	#include "gattlib.h"
	#include "util.h"
}

using namespace std::cout;
using namespace std::string;
using namespace std::condition_variable;
using namespace std::mutex;
using namespace std::unique_lock;
using namespace std::thread
using namespace cv;

// ble variables
gatt_connection_t* connection_pen;
gatt_connection_t* connection_com;
char* addr_pen;
static uuid_t g_uuid_pen;
char* addr_com;
static uuid_t g_uuid_com;
long int value_data_read;
long int value_data_write;

// global variables
bool button = false;
float x, y;

// multi-thread manipulators
condition_variable cv_m;
mutex mu;
bool do_mouse = false;
bool do_pen = false;
bool do_opncv = false;

// opencv class
TrackCam* trackCam = new TrackCam();

/*
void mouse_thread(){
	while(1){
		//do the compute here
		
		unique_lock<mutex> lk(mu);
		cv_m.wait(lk, []{return do_mouse;});
		do_mouse = false;
		do_opencv = true;
		
		//do something here
		
		lk.unlock();
		cv_m.notify_one();
	}
}
*/
void pen_thread(){
	
	size_t len;
	uint8_t buffer[100];
	
	while(1){
		//
		ret = gattlib_read_char_by_uuid(connection_pen, &g_uuid_pen, buffer, &len);
		if(ret != 0){
			fprintf(stderr, "Fail to read value from the pen.\n");
			return 1;
		}
		
		unique_lock<mutex> lk(mu);
		cv_m.wait(lk, []{return do_pen;});
		do_pen = false;
		do_opencv = true;
		
		// turn buffer into bool button
		
		lk.unlock();
		cv_m.notify_one();
	}
}

void opencv_thread(){
	while(1){
		//do the computation here
		trackCam->track();
	
		unique_lock<mutex> lk(mu);
		cv_m.wait(lk, []{return do_opencv;});
		do_opencv = false;
		do_mouse = true;
		
		//modify global variable here
		
		lk.unlock();
		cv_m.notify_one();
	}
}


int main(int argc, char* argv[]){
	int ret;
	
	addr_pen = argv[1];
	addr_com = argv[2];
	g_uuid_pen = argv[3];
	g_uuid_com = argv[4];
	
	// connect ble to pen and computer
	connection_pen = gattlib_connect(NULL, addr_pen, BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0);
	connection_com = gattlib_connect(NULL, addr_com, BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0);
	if (connection_pen == NULL) {
		fprintf(stderr, "Fail to connect to the pen.\n");
		return 1;
	}
	if (connection_com == NULL) {
		fprintf(stderr, "Fail to connect to the computer.\n");
		return 1;
	}
		
	// initialize camera here
	if(!trackCam->init()){
		cout << "Fail to initialize camera.\n"
		return 1;
	}
	
	thread pen(pen_thread);
	thread opencv(opencv_thread);
	
	while(1){
		//the main loop
		//every loop means transferring a mouse commend from rpi to computer via ble
		//so implement the mouse routine here
		
		
		
		ret = gattlib_write_char_by_uuid(connection_com, &g_uuid, value_data_com, sizeof(value_data_com));
		if(ret != 0){
			fprintf(stderr, "Fail to send signals to the computer.\n");
			return 1;
		}
	}
}
