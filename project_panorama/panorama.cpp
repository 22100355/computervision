#include "opencv.hpp"
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

Mat pano(Mat image1, Mat image2) {
    Mat descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector< vector< DMatch> > matches;
    vector< DMatch > goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    int i, k;
    float nndr;

    if (image1.empty() || image2.empty()) {
        cout << "fail" << endl;
        return image1;
    }

    orbF->detectAndCompute(image1, noArray(), keypoints1, descriptors1);
    orbF->detectAndCompute(image2, noArray(), keypoints2, descriptors2);

    k = 2;
    matcher.knnMatch(descriptors1, descriptors2, matches, k);
    nndr = 0.9f;
    for (i = 0; i < matches.size(); i++) {
        if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
            goodMatches.push_back(matches[i][0]);
        }
    }
    sort(goodMatches.begin(), goodMatches.end(), [](const DMatch& a, const DMatch& b) {
        return a.distance < b.distance;
        });

    vector<Point2f> points1, points2;
    for (size_t i = 0; i < goodMatches.size(); i++) {
        points1.push_back(keypoints1[goodMatches[i].queryIdx].pt);
        points2.push_back(keypoints2[goodMatches[i].trainIdx].pt);
    }

    Mat H = findHomography(points2, points1, RANSAC);

    Mat result;
    warpPerspective(image2, result, H, Size(image1.cols * 1.4, image1.rows * 1.2));

    for (int y = 0; y < image1.rows; y++) {
        for (int x = 0; x < image1.cols; x++) {
            if (result.at<Vec3b>(y, x) == Vec3b(0, 0, 0)) {
                result.at<Vec3b>(y, x) = image1.at<Vec3b>(y, x);
            }
        }
    }

    return result;
}

int main() {
    Mat image1 = imread("panorama_image/pano1.JPG");
    Mat image2 = imread("panorama_image/pano2.JPG");
    Mat image3 = imread("panorama_image/pano3.JPG");
    Mat image4 = imread("panorama_image/pano4.JPG");
    Mat result = pano(image1, image2);
    Mat result1 = pano(result, image3);
    Mat panorama = pano(result1, image4);

    resize(panorama, panorama, Size(1440, 720));
    
    imshow("Panorama", panorama);
    waitKey(0);

}

