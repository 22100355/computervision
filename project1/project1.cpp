#include "opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	Mat frame;
	int fps;
	int delay;
	VideoCapture cap;
	int total_frames,current_frames=0;
	int time_in_mesc;

	// check if file exists. if none program ends
	if (cap.open("background.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}

	fps = cap.get(CAP_PROP_FPS);

	do {
		cap >> frame;
		total_frames = cap.get(CAP_PROP_FRAME_COUNT);
		time_in_mesc = cap.get(CAP_PROP_POS_MSEC);
		if (frame.empty()) {
			cout << "end of video" << endl;
			break;
		}
		imshow("video", frame);
		cout << "frames: " << current_frames++ << " / " << total_frames << endl;
		delay = 1000 / fps;
		waitKey(delay);
	} while (time_in_mesc <= 3000.0);

	waitKey(0);
	
}