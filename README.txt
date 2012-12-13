Written by Bhaskar Bhattacharya
Iowa State University

Data checked
2Cotton
ManyCotton
ManyCorn

As of 13th Oct 2012
*New Folder JDTracking2 for osg
*Changed to VS2008 because osg wasnt working well with VS2010 for some reason
*Added osgViewing now
*Will remove OpenGL component (kept as backup for Deere Day)

As of 9th Oct 2012

*Rearranged the way vecctor writing is done. Showcircle has coressponding contour(if visible)
*Added contour view of the 3Dpoint

As of 3rd Oct 2012
*Fixed bug which did not keep relative position of the contour wrt the images
*Added a video maker to create videos and make original with tracking videos side by side
*Made glut main loop escapable

As of 2nd Oct 2012
*Added feature to add video instead of list of images. OpenCV bug avi not working convert to mp4 and does
*Created 3 forms of visualization 2D and 3D 
	static and animation for 3D
	chose and highlight ones you want in 2D - Ctrl click
*Branched the visualization and tracking i.e. added a writer and reader for my tracking datastr


As of 10th Sept 2012 
*Upload list of images
*Detect the contours (no real detection in overlaps)
*Track each contour with a number