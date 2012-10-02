#ifndef RW_VEC
#define RW_VEC


#include <fstream>
#include <vector>
using namespace std;

#include "Tracker.h"
void save_vector(vector<vector<showcircle> > myvec, char *fname,int rows,int cols);
vector<vector<showcircle> > read_vector(char *fname,int &rows,int&cols);

#endif