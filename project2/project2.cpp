#include "opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	// Read an image ��lena.png�� as a gray-scale image
	Mat image = imread("lena.png", 0);

	// Generate a 90-degree rotated image 
	Mat rotate_image(image.cols, image.rows, image.type());
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			rotate_image.at<uchar>(i, j) = image.at<uchar>(j, image.cols - 1 - i);
		}
	}

	//perform negative transformation if the pixel value is smaller than 127. Otherwise, perform gamma transformation with gamma as 10
	Mat result_img;
	int value = 0;
	MatIterator_<uchar> it, end;
	float gamma = 10;
	unsigned char pix[256];
	for (int i = 0; i < 256; i++) {
		pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
	}
	result_img = rotate_image.clone();
	for (int j = 0; j < rotate_image.rows; j++)
		for (int i = 0; i < rotate_image.cols; i++) {
			value = rotate_image.at<uchar>(j, i);
			if (value < 127) {
				result_img.at<uchar>(j, i) = 255 - rotate_image.at<uchar>(j, i);
			}
			else {
				result_img.at<uchar>(j, i) = pix[result_img.at<uchar>(j, i)];
			}
		}
	//Display the inputand the result on two windows
	imshow("gray image", image);
	imshow("result image", result_img);
	waitKey(0);
	return 0;
}