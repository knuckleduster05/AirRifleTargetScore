#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgcodecs.hpp"
#include <math.h>
#define PI 3.14159265358979323846

using namespace std;
using namespace cv;

Mat src;
Mat dst;
Mat canny_output;
Mat drawing2;


void createWindows()
{
	namedWindow("Image", WINDOW_AUTOSIZE);
	namedWindow("Image2", WINDOW_AUTOSIZE);
	//namedWindow("Image3", WINDOW_AUTOSIZE);
	//cout << "created window";
}

void readImages()
{
	//src = imread("Circle.png", 1); // a circle
	src = imread("PaperTargets.png", 1);                //Highest Resolution image, no problem detecting circles with current settings
	//src = imread("AirRifleTarget.png", IMREAD_COLOR);			   //Medium Res image, need to adjust thickness of contour lines if I want to read that small
	//src = imread("AirRifleTarget2.png", IMREAD_COLOR);		   //Lowest Res image, does not detect contours because the area of the circles is much too small for the current allowance.
	//dst = imread("AirRifleTarget.png", IMREAD_COLOR);
	//dst = imread("PaperTargets.png", IMREAD_GRAYSCALE);
	if (!src.data)
	{
		cout << "Could not find or read image";
	}
}

Mat detectContours()
{
	int p = 0;
	blur(src, dst, Size(3, 3));										//Performs a box filter blur operation on the Mat "src" and outputs it to the Mat "dst" 
	//GaussianBlur(src, dst, Size(9, 9), 8, 8);
	vector<vector<Point> > contours;  //creates a vector of vectors that stores points, and this stores all the contours, which are just series of streight lines connecting to eachother.
	vector<Vec4i> hierarchy;
	Canny(dst, canny_output, 10, 30, 3);
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	double circs[10]{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	Point centers[10];
	int avgcenterX=0,avgcenterY=0;
	Point avgcenterpoint;
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i], false);
		double r = sqrt(area / PI);
		if (area > 10000)
		{
			p++;
			Scalar color = Scalar(255, 255, 255);
			//drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
			for (int l = 0; l < 10; l++)
			{
				double err = 100*(abs(r - circs[l]) / abs((r + circs[l]) / 2));
				if (err > 9)
				{
					if (circs[l] == -1)
					{
						circs[l] = r;
						centers[l] = mc[i];
						l = 11;
					}
				} else if (err <= 9)
				{
					circs[l] = ((circs[l] + r) / 2);
					centers[l] = mc[i];
					l = 11;
				}
			}
			for (int i = 0; i < 10; i++)
			{
				avgcenterX += centers[i].x;
				avgcenterY += centers[i].y;
				if (i == 9)
				{
					
					avgcenterX = avgcenterX / 11;
					avgcenterY = avgcenterY / 11;
					cout << avgcenterX << ":" << avgcenterY << endl;
					avgcenterpoint = Point(avgcenterX, avgcenterY);
					
				}
			}
		}

		}
	cout << "number of contours: " << p<<endl;
	for (int i = 0; i < 10; i++)
	{
		cout <<"Circle #"<<i<<": "<< circs[i]<<endl;
		cout << "Center of Circle #" << i << ": " << centers[i] << endl;
		circle(drawing, avgcenterpoint, circs[i], Scalar(255, 255, 255));
	}
	return drawing;
}

void detectCircles()
{
	cvtColor(drawing2, drawing2, COLOR_BGR2GRAY);
	//cout << "converted color";
	//GaussianBlur(dst, dst, Size(9, 9), 8, 8);
	//cout << "passed blur";
	vector<Vec3f> circles;
	//cout << "made circle vecotrs";
	HoughCircles(drawing2, circles, HOUGH_GRADIENT, 1, dst.rows, 200, 100, 0, 0);
	//cout << "made circles";
	cout << "Number of circles: " << circles.size() << endl;
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

void displayImages()
{
	imshow("Image", dst);
	imshow("Image2", detectContours());
	//imshow("Image3", drawing2_g);
	//cout << "showing image";
}

int main()
{
	//cout << "got to main";
	createWindows();
	readImages();
	detectContours();
	//detectCircles();
	displayImages();
	waitKey(0);
	return(0);
}
