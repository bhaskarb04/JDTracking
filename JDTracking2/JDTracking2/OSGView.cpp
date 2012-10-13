#include "OSGView.h"

osgView::osgView()
{
	set_track=false;
	drawA=false;
}

osg::ref_ptr<osg::Geode> osgView::drawAxes()
{
	osg::ref_ptr<osg::Geode> return_axes=new osg::Geode;
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(6);
	(*vertices)[0].set( 0.0f, 0.0f, 0.0f); //origin
	(*vertices)[1].set( 100.0f, 0.0f, 0.0f); //X
	(*vertices)[2].set( 0.0f, 0.0f, 0.0f); //origin
	(*vertices)[3].set( 0.0f, 100.0f, 0.0f); //Y
	(*vertices)[4].set( 0.0f, 0.0f, 0.0f); //origin
	(*vertices)[5].set( 0.0f, 0.0f, 100.0f); //Z

	osg::ref_ptr<osg::DrawArrays>indices =new osg::DrawArrays(osg::PrimitiveSet::LINES,0,6);
	
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(6);
	(*colors)[0].set(1.0,0.0,0.0,0.0);
	(*colors)[1].set(1.0,0.0,0.0,0.0);
	(*colors)[2].set(0.0,1.0,0.0,0.0);
	(*colors)[3].set(0.0,1.0,0.0,0.0);
	(*colors)[4].set(0.0,0.0,1.0,0.0);
	(*colors)[5].set(0.0,0.0,1.0,0.0);
	
	geom->setVertexArray( vertices.get() );

	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	
	geom->addPrimitiveSet( indices.get() );
	osg::StateSet* stateset = new osg::StateSet;
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	geom->setStateSet(stateset);

	
	return_axes->addDrawable(geom);
	return(return_axes.release());
}

osg::ref_ptr<osg::Geode> osgView::drawTractor()
{
	osg::ref_ptr<osg::Box> tractor= new osg::Box(osg::Vec3d(xmax/2,ymax/2,-10),xmax,ymax,10);
	osg::ref_ptr<osg::ShapeDrawable> tractorDrawable = new osg::ShapeDrawable(tractor);
	tractorDrawable->setColor(osg::Vec4(0,1,0,0));
	osg::ref_ptr<osg::Geode> tractorGeode = new osg::Geode();
	tractorGeode->addDrawable(tractorDrawable);
	
	return(tractorGeode.release());
}
void osgView::draw()
{
	//drawing declarations
	osgViewer::Viewer viewer;
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Geode> axes,flow;	

	//call drawing methods
	axes=drawAxes();
	if(set_track)
		flow=drawFlow();

	//link to root
	root->addChild(axes);
	if(set_track)
		root->addChild(flow);
	
	//set to viewer and draw
	viewer.home();
	viewer.setSceneData( root );
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.run();
}

void osgView::update(float time)
{
	osg::ref_ptr<osg::Geode> new_flow=new osg::Geode;
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(totalelements);
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(totalelements);
	osg::StateSet* stateset = new osg::StateSet;
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	int count=0;
	if(transform->getNumChildren() > 1)
		transform->removeChild(1,transform->getNumChildren()-1);
	for(unsigned int i=0;i<animationcount;i++)
	{
		if(tracks[i]->getNumElements())
		{
			for(unsigned int j=0;j<tracks[i]->getNumElements();j++)
			{
				(*vertices)[count]=(*tracks[i])[j];
				(*colors)[count]=(*colorlist)[j];
				count++;
				if(i == animationcount - 1)
				{	
					osg::ref_ptr<osg::Sphere> cogs = new osg::Sphere((*tracks[i])[j],5.0);
					osg::ref_ptr<osg::ShapeDrawable> cogDrawable = new osg::ShapeDrawable(cogs);
					osg::ref_ptr<osg::Geode> cogGeode = new osg::Geode();
					cogGeode->addDrawable(cogDrawable);
					transform->addChild(cogGeode);
				}
			}

		}
	}
	osg::ref_ptr<osg::DrawArrays>indices =new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,totalelements);
	geom->setVertexArray( vertices.get() );
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	geom->addPrimitiveSet( indices.get() );
	
	//stateset->setAttribute(new osg::point( 3.0f ),osg::StateAttribute::ON);
	geom->setStateSet(stateset);
	new_flow->addDrawable(geom);
	transform->setChild(0,new_flow);
	animationcount++;
	if(animationcount == animationend)
		animationcount=1;
}
void osgView::drawAnimation()
{
	osgViewer::Viewer viewer;
	osg::Camera *cam= new osg::Camera();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Geode> axes,flow,tractor;
	

	//call drawing methods
	axes=drawAxes();
	tractor=drawTractor();
	if(set_track)
		drawFlowAnimation();

	//link to root
	root->addChild(axes);
	root->addChild(tractor);
	if(set_track)
	{
		root->addChild(transform->asPositionAttitudeTransform());
	}
	
	//set to viewer and draw
	//viewer.home();
	//viewer.getCameraManipulator()->setHomePosition(osg::Vec3d(100,100,100),osg::Vec3d(0,0,0),osg::Vec3d(0,0,1));
	
	//viewer.getCamera()->setProjectionMatrixAsPerspective(45.0, 1.0, 0.5, 1000); 
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(200,200,200),osg::Vec3d(0,0,0),osg::Vec3d(0,1,0));
	//viewer.setCamera(cam);
	viewer.setSceneData( root );
	
	while(!viewer.done())
	{
		viewer.frame();
		update(TIMER_);
	}

}
osgView::~osgView()
{

}

void osgView::setmax(int x,int y,int z)
{
	xmax=x;
	ymax=y;
	zmax=z;
	
}
void osgView::makeColorlist(vector<vector<showcircle> > tracks)
{
	colorlist = new osg::Vec4Array;
	maxsize=0;totalelements=0;
	for(unsigned int i=0;i<tracks.size();i++)
	{
		totalelements+=tracks[i].size();
		maxsize=(maxsize<tracks[i].size())?tracks[i].size():maxsize;
	}
	
	int rr=0,gg=0,bb=0;
	osg::Vec4 c;
	for(unsigned int i=0;i<maxsize;i++)
	{
		rr=rr%256;
		gg=gg%256;
		bb=bb%256;
		if(rr%256==0 && gg%256==0 && bb%256==0)
		{
			rr=rand()%256;gg=rand()%256;bb=rand()%256;
		}
		
		c.set(float(rr)/255,float(gg)/255,float(bb)/255,0.0);
		colorlist->push_back(c);
		rr+=10;
		gg+=20;
		bb+=30;
	}
}
void osgView::set_tracks(vector<vector<showcircle> > t)
{
	makeColorlist(t);
	set_track=true;
	for (unsigned int i=0;i<t.size();i++)
	{
		osg::ref_ptr<osg::Vec3Array> track = new osg::Vec3Array;
		osg::Vec3 temp;
		for(unsigned int j=0;j<t[i].size();j++)
		{
			if(!t[i][j].visible)
				continue;
			double x,y,z;
			x=(double)t[i][j].p.x;
			y=(double)t[i][j].p.y;
			z=255 - t[i][j].z;
			temp.set(x,y,z);
			track->push_back(temp);
		}
		tracks.push_back(track);
	}
	animationcount=1;
	animationend=t.size();
}

osg::ref_ptr<osg::Geode> osgView::drawFlow()
{
	osg::ref_ptr<osg::Geode> return_flow=new osg::Geode;
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(totalelements);
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(totalelements);
	int count=0;

	for(unsigned int i=0;i<tracks.size();i++)
	{
		if(tracks[i]->getNumElements())
		{
			for(unsigned int j=0;j<tracks[i]->getNumElements();j++)
			{
				(*vertices)[count]=(*tracks[i])[j];
				(*colors)[count]=(*colorlist)[j];
				count++;
			}

		}
	}
	osg::ref_ptr<osg::DrawArrays>indices =new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,totalelements);

	geom->setVertexArray( vertices.get() );
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	geom->addPrimitiveSet( indices.get() );
	osg::StateSet* stateset = new osg::StateSet;
	//stateset->setAttribute(new osg::point( 3.0f ),osg::StateAttribute::ON);
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	geom->setStateSet(stateset);
	return_flow->addDrawable(geom);
	return (return_flow.release());
}
void osgView::drawFlowAnimation()
{
	transform = new osg::PositionAttitudeTransform();
	osg::ref_ptr<osg::Geode> temp = new osg::Geode();
	transform->addChild(temp);
	transform->setPosition(osg::Vec3(0,0,0));
	transform->addChild(temp);
	transform->setPosition(osg::Vec3(0,0,0));
	//transform[1].addChild(temp);
	//transform[1].setPosition(osg::Vec3(0,0,0));
	return;
}