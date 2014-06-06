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
#include <osgViewer/Viewer>



#include "Tracker.h"

#define TIMER_ 0.100
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define LOOP true
#define FILE_PATH "C:\\Users\\Bhaskar\\Documents\\Fall\ 2012\\Research\\JDTracking\\Data\\"
#define SCALE 26
#define WITH_MODEL true
#define WITH_MODEL_PLATE true
#define BG_ERROR 0
#define ZSCALE 1
#define ZTRANSLATE 0
#define WITH_HEAD true
#define NO_LOOPS 2
#define TRANS_STEP 500


class OSGViewer;
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
	osg::ref_ptr<osg::Image> image;
	vector<vector<vector<osg::Vec3d> > >contourlist;
	bool pause_flag;
	osg::ref_ptr<osg::Group> main_node;
	
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
	void _setmax(int,int,int,double);
	void _settracks(vector<vector<showcircle> > t);
	//void set_contours(vector<vector<float
	void update(float time);
	void update2(float time);
	void update3();
	void draw();
	void drawAnimation();
	void drawAnimation2();

	vector<cv::Mat> osgVideo;
	friend class myKeyboardEventHandler;
	friend class OSGViewer;
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
	int num;
public:
	ParticleCallback(){num=0;}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
	  osg::ref_ptr<osg::Group> ParticleData = dynamic_cast<osg::Group*> (node);
	  if(ParticleData)
	  {
		 ParticleData->getChild(5)->asGroup()->getChild(num)->asSwitch()->setAllChildrenOn();
		 if(num!=0)
			ParticleData->getChild(5)->asGroup()->getChild(num-1)->asSwitch()->setAllChildrenOff();
		 else
			ParticleData->getChild(5)->asGroup()->getChild(ParticleData->getChild(5)->asGroup()->getNumChildren()-1)->asSwitch()->setAllChildrenOff();

		 num++;
		 num=num%ParticleData->getChild(5)->asGroup()->getNumChildren();
	  }
	  traverse(node, nv); 
	}
};

class OSGViewer
{
	osgView *osgv;
	osg::Image *image;
	//ParticleCallback *pcb;
public:
	OSGViewer(){osgv=new osgView;}
	~OSGViewer(){delete osgv;}
	void setmax(int a,int b,int c,double d){osgv->_setmax(a,b,c,d);}
	void settracks(vector<vector<showcircle> > t){osgv->_settracks(t);}
	void run();
	vector<cv::Mat> getVideoVector(){return osgv->osgVideo;}
};

#endif