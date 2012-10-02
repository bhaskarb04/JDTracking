#ifndef VIEWER_CPP

#define VIEWER_CPP


#include <cv.h>
#include <vector>
#include <algorithm>
using namespace std;
#include <glut.h>
#include "Tracker.h"
extern GLint window_width,window_height;
struct Color
{
	GLfloat r,g,b;
};
struct newpoint
{
	Color c;
	GLfloat x,y,z;
	int num;
};

class Viewer
{
	double cameradirection;
	double updownrotate;
	double rotX,rotY,rotZ;
	int Wx,Wy;
	int imgrows,imgcols;
	int frame;
	vector<vector<showcircle> > tracks;
	vector<vector<showcircle> > tracks2;
	vector<int>trackshow;
	vector<Color> colorlist;
	vector<newpoint> disppoints;

	void Camera(int pickmode,int x,int y);
	void DrawTracks();
	void DrawTracks(int end);
	void DrawAxes();
	bool check_if_set(int);
	void make_vector_proper();
	int find_num(GLdouble x,GLdouble y,int type);
public:
	Viewer();
	~Viewer();
	void Init();
	void Init2();
	void show_tracks(vector<vector<showcircle> >tracks,int imgrows,int imgcols);
	void display();
	void display2();
	void KeyboardNormal(unsigned char key,int x, int y);
	void SpecialKeyboard(int key,int x, int y);
	void reshape(GLint w,GLint h);
	void MouseClick(GLint button,GLint state,GLint x,GLint y);
	void timerfunc();
};

#endif