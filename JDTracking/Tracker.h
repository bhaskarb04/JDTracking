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

struct showcircle
{
	cv::Point p;
	float z;
	int radius;
	bool visible;
	int age;
	vector<cv::Point> contour;
};

class trackpoint
{
	vector<showcircle> centres;
	vector<cv::Point2f> motion;
	vector<int> age;
	vector<showcircle> centres_p;
	showcircle contourcentre(vector<cv::Point> c);
	void nearest(vector<showcircle> &inputcentres, vector<vector<cv::Point> >&contours);
public:
	

	trackpoint();
	~trackpoint();
	cv::Mat update(vector<vector<cv::Point> > contour,cv::Mat img);
	vector<showcircle> centre_return(){return centres_p;}
};

class Tracker
{
	//variables
	string path_original; //original path name kept in case modification is required
	vector<cv::Mat> list_images_org;
	vector<cv::Mat> list_images;
	vector<string> list_image_names;
	vector<vector<vector<cv::Point> > >contours;
	vector<vector<vector<cv::Point> > > all_contours;
	cv::Mat ref;
	//functions
	void load_images(string p,bool show);
	void load_images_video(string p,bool show);
	void make_tracks(bool show,int wait);
public:
	vector<vector<showcircle> > tracks;
	
	Tracker();
	Tracker(string p,bool choice);

	~Tracker();
	void clean_image(bool show);
	void track_particles(bool show);
	void optical_flow(bool show);
	vector<cv::Mat> get_orig_images(){return list_images_org;}
	vector<vector<vector<cv::Point> > > get_contours(){return contours;}
	int imgrows,imgcols;
	
	friend class trackpoint;
};

#endif