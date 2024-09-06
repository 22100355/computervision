#include "opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	Mat moon_image = imread("moon.jpeg", 0);

	Mat moon_com(moon_image.rows, moon_image.cols, moon_image.type());

	Mat moon_sharpening_image_ri = moon_image(Rect(moon_image.cols / 2, 0, moon_image.cols / 2, moon_image.rows));
	Mat moon_sharpening_image_le = moon_image(Rect(0, 0, moon_image.cols / 2, moon_image.rows));

	Mat blur, sharpening;
	GaussianBlur(moon_sharpening_image_ri, blur, Size(3, 3), 0, 0, BORDER_DEFAULT);
	sharpening = moon_sharpening_image_ri + (moon_sharpening_image_ri - blur);

	moon_sharpening_image_le.copyTo(moon_com(Rect(0, 0, moon_sharpening_image_le.cols, moon_sharpening_image_le.rows)));
	sharpening.copyTo(moon_com(Rect(moon_sharpening_image_le.cols, 0, sharpening.cols, sharpening.rows)));

	Mat saltnpepper_image = imread("saltnpepper.png", 0);
	Mat saltnpepper_com(saltnpepper_image.rows, saltnpepper_image.cols, saltnpepper_image.type());

	Mat saltnpepper_median_image_ri = saltnpepper_image(Rect(saltnpepper_image.cols / 2, 0, saltnpepper_image.cols / 2, saltnpepper_image.rows));
	Mat saltnpepper_median_image_le = saltnpepper_image(Rect(0, 0, saltnpepper_image.cols / 2, saltnpepper_image.rows));

	Mat mf;
	medianBlur(saltnpepper_median_image_le, mf, 9);

	mf.copyTo(saltnpepper_com(Rect(0, 0, mf.cols, mf.rows)));
	saltnpepper_median_image_ri.copyTo(saltnpepper_com(Rect(mf.cols, 0, saltnpepper_median_image_ri.cols, saltnpepper_median_image_ri.rows)));


	imshow("moon", moon_image);
	imshow("moon_filtered", moon_com);
	imshow("saltnpepper", saltnpepper_image);
	imshow("saltnpepper_filtered", saltnpepper_com);
	waitKey(0);
	return 0;
}
