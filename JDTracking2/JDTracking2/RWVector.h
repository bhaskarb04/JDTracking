#ifndef RW_VEC
#define RW_VEC


#include <fstream>
#include <vector>
using namespace std;

#include "Tracker.h"
void save_vector(vector<vector<showcircle> > myvec, char *fname,int rows,int cols);
vector<vector<showcircle> > read_vector(char *fname,int &rows,int&cols);
void save_contour(vector<vector<vector<cv::Point> > >myvec, char *fname);
vector<vector<vector<cv::Point> > > read_contour(char *fname);

#endif