 Develop a program that 1) displays canny edges regions and 2) draws
lines corresponding to lanes of a video
1) Display canny edges
 Read ‘Road.mp4’
 Set left_roi and right_roi
 LT and RB of left_roi: (200,400) , (600,600)
 LT and RB of right_roi: (600,400), (1000,600)
 For each ROI, display canny edges
 Use cvtColor to make a gray-scale image
 Use CV_BGR2GRAY
 Use blur function with mask size as 5X5
 For Thresholds for canny, use 10 and 60
 Display two windows
 ‘Left canny’, ‘Right canny’
 Use namedWindow and moveWindow for locating the windows
 Example
 namedWindow("Left canny");
 moveWindow("Left canny", 200, 0);
 imshow("Left canny", canny_left);
  Locate the window whose name is ‘Left canny’ to (200,0)
 Locate ‘Left canny’ to (200,0) and ‘Right canny’ to (600,0)

3) Draw lines corresponding to lanes
 Use left_roi and right_roi defined in assignment 5
 Perform line filtering and line merging
 Line filtering: Use angle information of lines
 Left_roi: Use lines whose angle is between 30 and 60 degress
 Right_roi: Use lines whose angle is between 120 and 150 degress
 Line merging
 Take average of rho and theta of filtered lines
 After line merging there should be one left-line and one right-line
 The color of lines should be red
 Each line should be reached to the boundary of a frame

 Display one window
 ‘Frame’
 Display frames that are between 0~20 seconds
 Overall, your program should display 3 windows.
 2 canny edges windows
 1 line filtering/merging window
