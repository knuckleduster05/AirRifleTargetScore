AirRifleTargetScore
===================

Author: Jack Manning

To compile:
Build the open-cv library for visual studio, this is a very intensive process, that will take trial and error, but you will learn
a lot about how code works for a program and what goes into an external library like this.

This tutorial should help:
http://opencv-srf.blogspot.com/2013/05/installing-configuring-opencv-with-vs.html

Once this is done, open the project file in visual studio
to run a test, you can uncoment the line in the main function of the source.cpp file that says findMissing("PaperTargets6_4.png");
NOTE: this will throw an error and quit on you if you do not have a file named PaperTargets6_4.png in the same directory as your 
source.cpp, if it gives you trouble and you are sure that the file is in the right place, try importing it in the resource files 
section of visual studio, on the right side bar.

If you do not want to compile the file, but just want to see it in action, the x64/Release folder has a copy of the most recent build 
for your use, just call it in the command line, using the name of the file as the only argument to the file. To quit the program, 
just hit any key when it is done scoring the target.
