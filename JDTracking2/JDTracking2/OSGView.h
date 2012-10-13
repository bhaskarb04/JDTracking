#ifndef OSG_VIEW_JD
#define OSG_VIEW_JD


#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Camera>
#include <osgUtil/SmoothingVisitor>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgGA/CameraManipulator>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>

#include "Tracker.h"

#define TIMER_ 0.100

class osgView
{
	int xmax,ymax,zmax,totalelements,maxsize,animationcount,animationend;
	osg::ref_ptr<osg::Vec4Array> colorlist;
	vector<osg::ref_ptr<osg::Vec3Array> > tracks;
	osg::ref_ptr<osg::PositionAttitudeTransform> transform;
	bool set_track,drawA;
	//osg::ref_ptr<osg::ref_ptr<osg::Vec3Array> >tracks;

	osg::ref_ptr<osg::Geode> drawAxes();
	osg::ref_ptr<osg::Geode> drawFlow();
	osg::ref_ptr<osg::Geode> drawTractor();
	void drawFlowAnimation();
	void makeColorlist(vector<vector<showcircle> > tracks);
	void update(float time);
public:
	osgView();
	~osgView();
	void setmax(int,int,int);
	void set_tracks(vector<vector<showcircle> > t);
	void draw();
	void drawAnimation();
};

#endif