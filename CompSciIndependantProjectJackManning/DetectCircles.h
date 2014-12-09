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

bool circlesDebug = false;
int l = 0;
RNG rng(0xFFFFFFFF);
Mat canny_output; //global stored canny output
Mat srce; //globaly stored version of src
Mat dest; //globaly stored version of dst
Mat drawing; //globaly stored version of drawings
Mat drawing2;
vector<vector<Point>> contours; //storage of all contours in image
vector<vector<Point>> contours2; //storage of all  circles in image
vector<Moments> mug(contours.size()); //globaly stored moments
vector<Point2d> mcg(contours.size()); //globaly stored masscenters
vector<Point2d> mc2(contours.size()); //masscenters of circles
vector<Vec4i> hierarchy; //hierarchy of contours in image
vector<double> circleRadi; //stores the radi of the averaged circles
vector<Point> circleCenters; //stores the centers of the averaged circles

Mat detectContours(Mat src, Mat dst); //function that calls all  other functions in this  header
void manipulateImage(Mat src, Mat dst); //function that performs all image manipulation
void findContoursandMoments(); //function that finds and stores all information useful to the detection
void collectCircles(); //function that finds all circles in the image
void averageCircles(); //averages out the circles into ones of the same size
void drawCircles();//this draws the circles that I averaged on the original source picture, to determine accuracy

Mat detectContours(Mat src, Mat dst,bool Debug2)  //function that calls all  other functions in this  header
{
	circlesDebug = Debug2;
	srce = src;  //saves src globaly
	dest = dst; //saves dst globaly
	manipulateImage(srce, dest); //performs manipulateImage on srce and dest
	drawing = Mat::zeros(canny_output.size(), CV_8UC3); //holds drawings for the debuging section
	drawing2 = Mat::zeros(canny_output.size(), CV_8UC3); //holds drawings for the debuging section
	//cout << "Going to contours/moments" << endl;
	findContoursandMoments(); //runs findContoursandMoments
	//cout << "Going to collectCircles" << endl;
	collectCircles(); //runs collectCircles()
	averageCircles();
	return drawing; //returns drawing to the main source file, drawing is edited in collectCircles currently
}

void manipulateImage(Mat src, Mat dst) //function that performs all image manipulation
{
	//resize(src, src, Size(1200, 1200));
	blur(src, dst, Size(3, 3)); //blurs image for easier edge detection
	Canny(dst, canny_output, 10, 30, 3); //performs canny edge detect on image
	//imshow("test2", src);
}

void findContoursandMoments() //function that finds and stores all information useful to the detection
{
	
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); //performs the findContours function from openCV library, outputs contours and hierarchy
	vector<Moments> mul(contours.size()); //local storage of moments
	vector<Point2f> mcl(contours.size()); //local storage of mass centers
	mcg.resize(contours.size());  //resizes the mcg global constant

	for (int i = 0; i < contours.size(); i++) 
	{
		mul[i] = moments(contours[i], false); //performs moments function on all contours in image
		
	}

	for (int i = 0; i < contours.size(); i++)
	{
		mcl[i] = Point2d(mul[i].m10 / mul[i].m00, mul[i].m01 / mul[i].m00); //calculates centers of contours in the image
	}
	
	for (int i = 0; i < mcl.size(); i++)
	{
		mcg[i] = mcl[i]; //saves masscenters globaly
	}
	//cout << "size: " << contours.size() << endl;
}

Point findCenter(int num) //finds the center of the contours and outputs it as a point
{
	return Point((int) round(mcg[num].x),(int) round(mcg[num].y));
}

bool checkCircular(int cont) //function that finds all circles in the image
{
	bool tests; //boolean used to detect circularity
	Point center = findCenter(cont); //saving local center for each contour for duration of test
	double radius = sqrt(contourArea(contours[cont], false) / PI); //finding the radius of the contour, assuming it is a circle
	drawContours(drawing, contours, cont, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
	for (int i = 0; i < 360; i++) //repeats test for entire circle
	{	
		Point testInside = Point((int) round(.98*radius*cos(i*PI / 180) + center.x), (int) round( .98*radius*sin(i*PI / 180) + center.y)); //the test point used in pointpolygonTest
		Point testOutside = Point((int) round(1.01*radius*cos((i + .5)*PI / 180) + center.x), (int) round(1.01*radius*sin((i + .5)*PI / 180) + center.y)); //the test point used in pointpolygonTest
		
		if (circlesDebug)
		{
			Size size4(800, 800);
			Mat tempDraw = drawing;
			Mat tempDraw2 = drawing2;
			resize(tempDraw, tempDraw, size4);
			resize(tempDraw2, tempDraw2, size4);
			imshow("Detected Circles", tempDraw); //writes the drawings to drawing2, and displays them in window "image5"
			imshow("image4", tempDraw2);
			if (i % 5 == 0 && !i == 0){ waitKey(1); }
		}




		if ((contourArea(contours[cont], false)<10000) || (-1 == pointPolygonTest(contours[cont], testInside, false)) || 1 == pointPolygonTest(contours[cont], testOutside, false)) //performs pointPolygonTest on the point in question, if true, boolean is saved as true, if false, jumps out of loop with boolean being false
		{
			tests = false;
			break;
		}
		else
		{
			line(drawing2, findCenter(cont), testInside, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1, 8); //these are for debuging purposes, if you want to see the contours be tested, uncoment these three
			line(drawing2, findCenter(cont), testOutside, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 1, 8);
			//line(drawing2, findCenter(cont), testOutside, Scalar(0,0,255), 1, 8);
			//line(drawing2, findCenter(cont), testInside, Scalar(255,0,0), 1, 8);
			tests = true;
		}
	}
	return tests;
}

void collectCircles() //function that finds all circles in the image
{
	
	int n = 0;
	Scalar color = Scalar(255, 255, 255); //saves a color for use in the drawContours function, I believe this one is white
	for (int i = 0; i < contours.size(); i++)
	{
		if (checkCircular(i)) //only passes if the contour is circular, with all 360* being  checked by pointPolygonTest
		{
			contours2.resize(n+1);
			mc2.resize(n + 1);
			contours2[n].push_back(Point(0,0));
			drawContours(drawing2, contours, i, color, 1, 8, hierarchy, 0, Point()); //draws contour on Mat drawing, which is returned to source
			contours2[n] = contours[i]; //saves the contour in a seperate set of contours for just circular objects
			mc2[n] = mcg[i]; //saves the masscenter to a seperate set of mass centers for just circular objects
			n++; 
			//cout << n << endl;
			//imshow("Detected Circles", drawing2); //writes the drawings to drawing2, and displays them in window "image5"
		}
	}
}

void averageCircles() //averages out the circles into ones of the same size
{
	circleRadi.push_back(1); //expands the vector holding the averaged circle radi
	circleCenters.push_back(Point(-1, -1)); //expands the vector holding the averaged circle centers
	circleRadi[0] = -1; //assigns a value to the array holder created above
	circleCenters[0] = Point(-1, -1); //assigns a value to the holder created above

	for (int i = 0; i < contours2.size(); i++) //goes through all of the circular contours
	{
		for (int l = 0; l < circleRadi.size(); l++) //checks against all the circles in the circleRadi vector
		{
			int r = (int) round(sqrt(contourArea(contours2[i], false) / PI)); //finds the radius of the current contour
			double err = 100 * (abs(r - circleRadi[l]) / abs((r + circleRadi[l]) / 2)); //finds the % diff between the current contours radius and the one in the vector we are checking agianst

			if (err > 9) //asks if the error is greater than 9%
			{
				if (circleRadi[l] == -1) //checks to see if the value we place in there if there is a placeholder exists
				{
					circleRadi[l] = r; //if the placeholder is there, replace the value of circleRadi with this value, and exit
					circleCenters[l] = mc2[i];//if the placeholder is there, replace the value of CircleCenters with this value, and exit
					circleRadi.push_back(1); //expand the vector for circleRadi by 1
					circleCenters.push_back(Point(-1,-1)); //expand the vector for circleCenters by 1
					circleRadi[l + 1] = -1; //assign placeholder value for circleRadi
					circleCenters[l + 1] = Point(-1, -1); //assign placeholder value for circleCenters
					l = (int) circleRadi.size(); //jump out of the for loop for the average circles
				}
			}
			else if (err <= 9) //this averages the circles in the value if they are within 9% difference to eachother
			{
				circleRadi[l] = ((circleRadi[l] + r) / 2);
				l = (int) circleRadi.size();
			}
		}
	}
	circleRadi.pop_back(); //deletes the last value in the vector, cleaning it up
	circleCenters.pop_back(); //deletes the last value in the vector, cleaning it up
}

Mat drawCircles(Mat dst) //this draws the circles that I averaged on the original source picture, to determine accuracy
{
	for (int i = 0; i < circleCenters.size(); i++) //for all the averaged circles
	{
		circle(dst, circleCenters[i], (int) round(circleRadi[i]), Scalar(255, 0, 0)); //draw the circles, with their own centerpoints too
	}
	return dst; //return the file that I started working with
}

void scoreTarget() //this function uses the logic I developed earlier to determine the score of the target
{
	int count = 1, acount = 0; //variales used in finding the missing ring
	vector<int> diffRadi; //vector used to find differences in the other vectors
	diffRadi.push_back(1); //expands the vector
	diffRadi[0] = -1; //assigns known value to the vector
	for (int i = 0; i < circleRadi.size(); i++)
	{
		if (i == circleRadi.size() - 1) //checks if we have hit the end of the vector
		{
			break;
		}
		else //finds the difference between the two circleRadi
		{
			diffRadi[i] = (int) round(circleRadi[i + 1] - circleRadi[i]); //assigns value of difference
			diffRadi.push_back(1); //expands vector
			diffRadi[i + 1] = -1; //assigns known quantity
		}
	}
	diffRadi.pop_back(); //shrinks vector down to proper size at end
	for (int i = 0; i < diffRadi.size(); i++) //goes through and checks to see if the diffsizes are any different from eachother
	{
		if (i == diffRadi.size() - 1) //stops if it hits the end
		{
			break;
		}
		else //finds %err between the two values
		{
			double err = 100 * (abs(diffRadi[i] - diffRadi[i + 1]))/(abs(diffRadi[i]+diffRadi[i+1])/2);
			if (err > 50) //if they are way different, then there is something wrong with that circle
			{
				if (i != diffRadi.size() - 1)
				{
					if (err > 90)
					{
						acount = count + 1;
					}
					else
					{
						acount = count;
					}
				}
				count++;
			}
			else
			{
				count++;
			}
		}
	}
	if (acount == 0)
	{
		if (abs(circleRadi[diffRadi.size() - 1]) < abs(1.1*diffRadi[diffRadi.size() - 1])) //if the circles that are not endpoint circles are okay, then we need to test the endpoints, to do this we take the average distance between the circles, and see if it can fit within the diameter of the smallest circle. If it does not, then that is the smallest possible circle we can assume is there, and the outer circle must be the one that is missing
		{
			acount = 1; //triggers if smallest circle is there
			cout << "Score: " << acount;
		}
		else
		{
			acount = 10;
			cout << "Score: " << acount;
		}
	}
	else
	{
		cout << "Score: " << acount + 1 << endl;
	}
	Size size(800, 800);
	resize(drawing2, drawing2, size);
	//imshow("Detected Circles", drawing2);
}