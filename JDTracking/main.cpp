#include "Tracker.h"
 // A Simple Camera Capture Framework 
 int main() 
 {
	 //Tracker constructor loads the images from the path automatically.For now it is assumed to be png
	 //TODO: add functionality for other image formats(supported by opencv)
	 Tracker track("C:\\Users\\Bhaskar\\Desktop\\JDTracking\\Take3-Images");
	 track.clean_image(false);
	 track.track_particles(false);//right now only finds the contours.
	 //track.optical_flow(true);
	 return 0;
 }