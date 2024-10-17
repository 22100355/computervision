#include "opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

double dr_li(Mat canny, Mat& dr_line, double fi_degree, double se_degree, int check) {
    Mat result = dr_line;
    double x, m;
    float rho, theta, a, b, x0, y0, rsum = 0, tsum = 0, xadd, yadd;
    Point p1, p2;
    vector<Vec2f> lines;
    HoughLines(canny, lines, 1, CV_PI / 180, 80, 0, 0, fi_degree * CV_PI / 180, se_degree * CV_PI / 180);

    if (check == 1) {
        xadd = 200;
        yadd = 400;
    }
    else {
        xadd = 600;
        yadd = 400;
    }
    for (int i = 0; i < lines.size(); i++) {
        rsum += lines[i][0];
        tsum += lines[i][1];
    }

    rho = rsum / lines.size();
    theta = tsum / lines.size();
    a = cos(theta);
    b = sin(theta);
    x0 = a * rho + xadd;
    y0 = b * rho + yadd;

    m = (double)((y0 + 300 * a) - y0) / ((x0 + 300 * -b) - x0);
    x = (result.rows - 200 - y0+m * x0) / m;

    p1 = Point(cvRound(x0 + 2000 * (-b)), cvRound(y0 + 2000 * a));
    p2 = Point(cvRound(x0 - 2000 * (-b)), cvRound(y0 - 2000 * a));
    return x;
}
bool Lane_departure(Mat& dest, double x)
{
    Mat result = dest;
    double center, y;
    y = result.rows - 200;
    center = result.cols / 2;

    if (abs(center - x) < 60)
    {
        return true;
    }
    else
        return false;
}
int Text(Mat& temp, int count)
{
    if (count == 0)
        return count;
    else
    {
        count--;
        putText(temp, "Lane departure", Point(temp.cols / 2 - 100, 50), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
        return count;
    }
}

int mv_Text(Mat& temp, int count)
{
    if (count == 0)
        return count;
    else
    {
        count--;
        putText(temp, "Start moving!", Point(temp.cols / 2 - 100, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
        return count;
    }
}

int main() {
    bool front_car=false, my_car=false;
    Mat frame, gray_scale_image, lt_canny, ri_canny;
    double fps, Left_x, Right_x;
    VideoCapture cap;
    Rect left_roi(200, 400, 400, 200);
    Rect right_roi(600, 400, 400, 200);
    
    Ptr<BackgroundSubtractorMOG2> bg_model = createBackgroundSubtractorMOG2();
    Rect sky(0, 0, 1280, 250);
    Rect carFrontROI(490, 500, 250, 90);

    Mat fgMask, bgImage;
    vector<vector<Point>> contours;

    int count = 0, count1 = 0;
    if (cap.open("Project2_video.mp4") == 0) {
        cout << "no such file!" << endl;
        return -1;
    }
    fps = cap.get(CAP_PROP_FPS);
    int delay = 1000 / fps;
    do {

        cap >> frame;
        if (frame.empty()) {
            cout << "end of video" << endl;
            break;
        }
        resize(frame, frame, Size(1280, 720));
        cvtColor(frame, gray_scale_image, CV_BGR2GRAY);
        lt_canny = gray_scale_image(left_roi);
        ri_canny = gray_scale_image(right_roi);
        blur(lt_canny, lt_canny, Size(5, 5));
        Canny(lt_canny, lt_canny, 10, 60, 3);
        blur(ri_canny, ri_canny, Size(5, 5));
        Canny(ri_canny, ri_canny, 10, 60, 3);


        Left_x = dr_li(lt_canny, frame, 30, 60, 1);
        Right_x = dr_li(ri_canny, frame, 120, 170, 0);
        if (Lane_departure(frame, Left_x)|| Lane_departure(frame, Right_x))
            count = 50;

        count = Text(frame, count);

        
        bg_model->apply(frame, fgMask);

        
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
        morphologyEx(fgMask, fgMask, MORPH_OPEN, kernel);

        Mat skyMask = fgMask(sky);
        findContours(skyMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if (!contours.empty()) {
            my_car = true;
        }
        else my_car = false;

        Mat carFrontMask = fgMask(carFrontROI);
        findContours(carFrontMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if (!contours.empty()) {
            front_car = true;
        }
        else front_car = false;
        if (!my_car && front_car) count1 = 50;
        if (my_car) count1 = 0;
        count1 = mv_Text(frame, count1);

        imshow("Frame", frame);
        waitKey(delay);

    } while (1);

    waitKey(0);

}
