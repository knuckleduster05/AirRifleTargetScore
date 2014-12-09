AirRifleTargetScore
===================

Author: Jack Manning
To use:
1. Open the command prompt and navigate to the directory containing the binary, CompSciIndependantProjectJackManning\x64\Release
2. Type the name of the exe, CompSciIndependantProjectJackManning.exe with no paramaters to find the usage for the file.
3. Type the name of the exe with a pointer to a picture as its argument to score the picture as a target

To compile:
Build the open-cv library for visual studio, this is a very intensive process, that will take trial and error, but you will learn a lot about how code works for a program and what goes into an external library like this.

This tutorial should help:
http://opencv-srf.blogspot.com/2013/05/installing-configuring-opencv-with-vs.html

Once this is done, open the project file in visual studio
to run a test, you can uncoment the line in the main function of the source.cpp file that says findMissing("PaperTargets6_4.png");
NOTE: this will throw an error and quit on you if you do not have a file named PaperTargets6_4.png in the same directory as your source.cpp, if it gives you trouble and you are sure that the file is in the right place, try importing it in the resource files section of visual studio, on the right side bar.


How it works:
This code uses the OpenCV library to read a picture of an air rifle target, find circles in the image, and determine what score the target should get, baised on what ring is broken in the target. To find the circles, it performs a contour detect test on the image, which takes a black and white image and finds all of the borders between the two. Once this is done, it goes through and test every detected contour to see if it matches the description of a circle, testing for points that should be inside the "circle" and outside the "circle". This removes all contours that do not match the profile of a circle. Once this is done, and the circles are saved in a seperate file, it starts to average the circles whose radii are within 9% of eachother. This saves all circles in the image, in the form of centerpoints and radii, in a vector, which is then run through to see if there is a missing circle in the vector. If there is an abnormaly large gap between two of the circles in the image, the code points it out, and assumes that there was a complete circle there, but it was broken by the air rifle. After we find and print the score of the target, correlating to the missing ring on the target, we print all of the other detected circles on the source image, and display it to the user, so they can know if the code missed a circle that should have been hit or any other anomaly. If the code is run in debug mode, it will print out additional information durring the testing, including showing all of the detected contours, and a visualization of the pointInContour tests in action.
