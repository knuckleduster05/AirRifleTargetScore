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

vector<vector<Point>> contours3;
vector<vector<Point>> contours4;
vector<Moments> moments3;
vector<Point2f> masscenter3;
vector<Point2f> masscenter4;
vector<Vec4i> hierarchy3;
vector<int> circleRadi3;
vector<Point> circleCenters3;
Mat drawing4;
bool scanDebug = false;

bool checkCircular2(int cont);



void detectCircles(bool Debug) //this function has been phased out for now, as it was not useful for what I was doing so far, but may come in handy later in the project.
{
	scanDebug = Debug;
	Mat source,destination,canny_output3;
	source = imread("FullTarget.jpg");
	manipulateImage(source, destination);
	drawing4 = destination;
	blur(source, destination, Size(3, 3)); //blurs image for easier edge detection
	Canny(destination, canny_output3, 10, 30, 3); //performs canny edge detect on image

	findContours(canny_output3, contours3, hierarchy3, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); //performs the findContours function from openCV library, outputs contours and hierarchy
	vector<Moments> mul(contours3.size()); //local storage of moments
	vector<Point2f> mcl(contours3.size()); //local storage of mass centers
	masscenter3.resize(contours3.size());  //resizes the mcg global constant

	for (int i = 0; i < contours3.size(); i++)
	{
		mul[i] = moments(contours3[i], false); //performs moments function on all contours in image

	}

	for (int i = 0; i < contours3.size(); i++)
	{
		mcl[i] = Point2d(mul[i].m10 / mul[i].m00, mul[i].m01 / mul[i].m00); //calculates centers of contours in the image
	}

	for (int i = 0; i < mcl.size(); i++)
	{
		masscenter3[i] = mcl[i]; //saves masscenters globaly
	}
	cout << "size: " << contours3.size() << endl;
	
	int n = 0;
	Scalar color = Scalar(255, 255, 255); //saves a color for use in the drawContours function, I believe this one is white
	for (int i = 0; i < contours3.size(); i++)
	{
		if (checkCircular2(i)) //only passes if the contour is circular, with all 360* being  checked by pointPolygonTest
		{
			contours4.resize(n + 1);
			masscenter4.resize(n + 1);
			contours4[n].push_back(Point(0, 0));
			drawContours(drawing4, contours3, i, color, 1, 8, hierarchy, 0, Point()); //draws contour on Mat drawing, which is returned to source
			contours4[n] = contours3[i]; //saves the contour in a seperate set of contours for just circular objects
			masscenter4[n] = masscenter3[i]; //saves the masscenter to a seperate set of mass centers for just circular objects
			n++;
			cout << n << endl;
			//imshow("Detected Circles", drawing2); //writes the drawings to drawing2, and displays them in window "image5"
		}
	}
	imshow("test", drawing4);
}

Point findCenter2(int num) //finds the center of the contours and outputs it as a point
{
	return Point((int)round(masscenter3[num].x), (int) round(masscenter3[num].y));
}


bool checkCircular2(int cont) //function that finds all circles in the image
{
	l++;
	if (l % 1000 == 0)
	{
		cout << l << endl;
	}
	bool tests; //boolean used to detect circularity
	Point center = findCenter2(cont); //saving local center for each contour for duration of test
	int radius = (int) round(sqrt(contourArea(contours3[cont], false) / PI)); //finding the radius of the contour, assuming it is a circle
	drawContours(drawing, contours, cont, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
	for (int i = 0; i < 360; i++) //repeats test for entire circle
	{
		Point testInside = Point((int)round(.98*radius*cos(i*PI / 180) + center.x), (int) round(.98*radius*sin(i*PI / 180) + center.y)); //the test point used in pointpolygonTest
		Point testOutside = Point((int)round(1.01*radius*cos((i + .5)*PI / 180) + center.x), (int) round(1.01*radius*sin((i + .5)*PI / 180) + center.y)); //the test point used in pointpolygonTest
		//line(drawing, findCenter(cont), testInside, Scalar(255, 255, 255), 1, 8); //these are for debuging purposes, if you want to see the contours be tested, uncoment these three
		//line(drawing2, findCenter(cont), testOutside, Scalar(255, 255, 255), 1, 8);
		//imshow("Detected Circles2", drawing4); //writes the drawings to drawing2, and displays them in window "image5"
		//imshow("image4", drawing); 
		//if (i % 5 == 0 && !i == 0){ waitKey(1); }
		if ((contourArea(contours3[cont], false)<10000) || (-1 == pointPolygonTest(contours3[cont], testInside, false)) || 1 == pointPolygonTest(contours3[cont], testOutside, false)) //performs pointPolygonTest on the point in question, if true, boolean is saved as true, if false, jumps out of loop with boolean being false
		{
			tests = false;
			break;
		}
		else
		{

			line(drawing4, findCenter2(cont), testInside, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1, 8); //these are for debuging purposes, if you want to see the contours be tested, uncoment these three
			line(drawing4, findCenter2(cont), testOutside, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1, 8);
			tests = true;
		}
	}
	return tests;
}