#ifndef TRACKER_CPP
#define TRACKER_CPP

#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;
#include "dirent.h"

#define DISTANCE_THRESH 50
#define AGE_THRESH 4
class trackpoint
{
	vector<cv::Point> centres;
	vector<cv::Point2f> motion;
	vector<int> age;

	cv::Point contourcentre(vector<cv::Point> c);
	void nearest(vector<cv::Point> &inputcentres);
public:
	trackpoint();
	~trackpoint();
	cv::Mat update(vector<vector<cv::Point> > contour,cv::Mat img);
};

class Tracker
{
	//variables
	string path_original; //original path name kept in case modification is required
	vector<cv::Mat> list_images_org;
	vector<cv::Mat> list_images;
	vector<string> list_image_names;
	vector<vector<vector<cv::Point> > >contours;
	//functions
	void load_images(string p,bool show);
	void make_tracks();
public:
	vector<vector<cv::Point> > tracks;
	
	Tracker();
	Tracker(string p);
	~Tracker();
	void clean_image(bool show);
	void track_particles(bool show);
	void optical_flow(bool show);
	
	friend class trackpoint;
};

#endif