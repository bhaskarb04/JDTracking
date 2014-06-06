#ifndef OSG_VIEW_JD
#define OSG_VIEW_JD


#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Camera>
#include <osg/Point>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Image>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/TrackballManipulator>
#include <osgGA/CameraManipulator>
#include <osgUtil/SmoothingVisitor>
#include <osgUtil\DelaunayTriangulator>
#include <osgViewer/Viewer>

#include "Tracker.h"

#define TIMER_ 0.100
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define LOOP true
#define FILE_PATH "../../Data/"
#define SCALE 26
#define WITH_MODEL false
#define WITH_MODEL_PLATE true
#define BG_ERROR 0
#define ZSCALE 1
#define ZTRANSLATE 0
#define WITH_HEAD true
#define NO_LOOPS 1
#define TRANS_STEP 500

class osgView : public osg::Referenced
{
	int xmax,ymax,zmax,totalelements,maxsize,animationcount,animationend;
	double zscale,ztranslate;
	double xtrans_particle,ytrans_particle,ztrans_particle;
	double xtrans_object,ytrans_object,ztrans_object;
	double bgwall;
	int loopme;
	osg::ref_ptr<osg::Vec4Array> colorlist;
	vector<osg::ref_ptr<osg::Vec3Array> > tracks;
	osg::ref_ptr<osg::PositionAttitudeTransform> transform;
	osg::ref_ptr<osg::Geode> axes,flow;
	osg::ref_ptr<osg::Transform> tractor,tractorplate;
	bool set_track,drawA;
	vector<vector<bool> >visible;
	osgViewer::Viewer viewer;
	osg::ref_ptr<osg::Image> image;
	vector<vector<vector<osg::Vec3d> > >contourlist;
	bool pause_flag;
	vector<cv::Mat> depthmaps;
	vector<vector<osg::Geometry*> > listofgeoms;
	//osg::ref_ptr<osg::ref_ptr<osg::Vec3Array> >tracks;

	osg::ref_ptr<osg::Geode> drawAxes();
	osg::ref_ptr<osg::Geode> drawFlow();
	osg::ref_ptr<osg::Geode> drawTractorplate();
	osg::ref_ptr<osg::PositionAttitudeTransform> drawTractor();
	void drawFlowAnimation();
	void makeColorlist(vector<vector<showcircle> > tracks);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&);
	void getvideo();
	void correct_for_age();
public:
	osgView();
	~osgView();
	void setmax(int,int,int,double);
	void set_tracks(vector<vector<showcircle> > t);
	void set_depthmap(string filename, int start, int end);
	//void set_contours(vector<vector<float
	void update(float time);
	void update2(float time);
	void draw();
	void drawAnimation();
	void drawAnimation2();
	//void convert2vtk(string);

	vector<cv::Mat> osgVideo;
	friend class myKeyboardEventHandler;
};

class myKeyboardEventHandler : public osgGA::GUIEventHandler
{
	osgView *ptr;
public:
	myKeyboardEventHandler (osgView *);
   virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&);
   virtual void accept(osgGA::GUIEventHandlerVisitor& v)   { v.visit(*this); };
};

class ParticleCallback : public osg::NodeCallback 
{
public:
   virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
   {
      osg::ref_ptr<osgView> ParticleData = 
         dynamic_cast<osgView*> (node->getUserData() );
      if(ParticleData)
      {
         ParticleData->update(TIMER_);
      }
      traverse(node, nv); 
   }
};

#endif