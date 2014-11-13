#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgcodecs.hpp"
#include <math.h>
#include "DetectCircles.h"
#define PI 3.14159265358979323846

using namespace std;
using namespace cv;

void createWindows()
{
	namedWindow("Image", WINDOW_AUTOSIZE);
	namedWindow("Image2", WINDOW_AUTOSIZE);
	//namedWindow("Image3", WINDOW_AUTOSIZE);
	//cout << "created window";
	
}

Mat readImages(const string filename)
{
	Mat src;
	src = imread(filename, 1);                //Highest Resolution image, no problem detecting circles with current settings
	if (!src.data)
	{
		cout << "Could not find or read image";
	}
	return src;
}



void detectCircles() //this function has been phased out for now, as it was not useful for what I was doing so far, but may come in handy later in the project.
{
	Mat drawing2, dst, src;
	
	cvtColor(drawing2, drawing2, COLOR_BGR2GRAY);
	//cout << "converted color";
	//GaussianBlur(dst, dst, Size(9, 9), 8, 8);
	//cout << "passed blur";
	vector<Vec3f> circles;
	//cout << "made circle vecotrs";
	HoughCircles(drawing2, circles, HOUGH_GRADIENT, 1, dst.rows, 200, 100, 0, 0);
	//cout << "made circles";
	//cout << "Number of circles: " << circles.size() << endl;
	for (size_t i = 0; i < circles.size(); i++)
	{

		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		circle(dst, center, 2, Scalar(255, 0, 0), -1, 8, 0);

		circle(dst, center, radius, Scalar(0, 255, 255), 10, 8, 0);
		circle(drawing2, center, 2, Scalar(255, 0, 0), -1, 8, 0);

		circle(drawing2, center, radius, Scalar(0, 255, 255), 10, 8, 0);

	}
	//cout << "passed for loop";
}

void displayImages(Mat input)
{
	imshow("Image2", input);
}

void findMissing(const string filename)
{
	Mat src = readImages(filename);
	Mat dst= src;
	
	Mat res;
	Size size(800, 800);
	
	res = detectContours(src, dst);
	dst = drawCircles(dst);
	resize(dst, dst, size);
	resize(res, res, size);
	imshow("Image", res);
	imshow("Image2", dst);
	scoreTarget();
	waitKey(0);
}

int main()
{
	findMissing("PaperTargets6_4.png");
	waitKey(0);
	return(0);
}
