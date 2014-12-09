#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgcodecs.hpp"
#include <math.h>
#include "DetectCircles.h"
#include "ScanSheet.h"
#define PI 3.14159265358979323846

using namespace std;
using namespace cv;

bool globalDebug = false;

void createWindows()
{
	namedWindow("Detected Contours", WINDOW_AUTOSIZE);
	namedWindow("Original Image /w Circles drawn in Blue", WINDOW_AUTOSIZE);
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
	
	res = detectContours(src, dst,globalDebug);
	dst = drawCircles(dst);
	resize(dst, dst, size);
	resize(res, res, size);
	//imshow("Detected Contours", res);
	imshow("Original Image /w Circles drawn in Blue", dst);
	scoreTarget();
	waitKey(0);
}

int main( int argc, char *argv[])
{
	if (argc == 1)
	{
		cout << "use: " << argv[0] << " [filename]\n To debug, type " <<argv[0] << " [filename] 1\n";
	}
	else if (argc == 2)
	{
		cout << "Name of file: " << argv[1];
		findMissing(argv[1]);
	}
	else if (argc == 3)
	{
		cout << "Name of file: " << argv[1];
		findMissing(argv[1]);
		globalDebug = true;
	}
	//detectCircles();
	//globalDebug = true;
	//findMissing("PaperTargets6.png"); //uncomment this and replace the target for manual entry

	waitKey(0);
	return(0);
}
