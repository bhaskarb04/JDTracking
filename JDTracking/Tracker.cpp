#include "Tracker.h"
//constructors
Tracker::Tracker()
{
	cout<<"Please enter the filename"<<endl;
	cin>>path_original;
	load_images(path_original,false);
}

Tracker::Tracker(string pname)
{
	path_original=pname;
	load_images(path_original,false);
}

//destructor
Tracker::~Tracker()
{
	list_images.clear();
	list_image_names.clear();
}
//function to load the images
void Tracker::load_images(string pname,bool show)
{
	DIR *dir;
	struct dirent *ent;
    dir = opendir (pname.c_str());
	if (dir != NULL) 
	{
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) 
		{
			string s(ent->d_name);
			int x=s.find(".png");
			if(s.find(".png")!=string::npos)
				list_image_names.push_back(s);
		}
		closedir (dir);
	} 
	else 
	{
		/* could not open directory */
		perror ("Cant opend Directory");
		return;
	}
	for(unsigned int i=0;i<list_image_names.size();i++)
	{
		list_images.push_back(cv::imread(pname+"\\"+list_image_names[i]));
		list_images_org.push_back(list_images[i]);
		if(show)
		{
			cv::imshow("showme",list_images[i]);
			cv::waitKey(50);
		}
	}
}

void Tracker::clean_image(bool show)
{
	cv::Mat strelem;
	strelem=cv::getStructuringElement(cv::MORPH_ELLIPSE,cvSize(4,4));
	for(unsigned int i=0; i < list_images.size();i++)
	{
		cv::Mat erode1,erode2,dilate1,dilate2;
		cv::erode(list_images[i],erode1,strelem);
		cv::dilate(erode1,dilate2,strelem,cvPoint(-1,-1),2);
		cv::erode(dilate2,erode2,strelem);
		list_images[i]=erode2;
		if(show)
		{
			cv::imshow("showme",erode2);
			cv::waitKey(50);
		}
	}
}

void Tracker::track_particles(bool show)
{
	vector<vector<cv::Point> > contour;
	for(unsigned int i=0;i<list_images.size();i++)
	{
		cv::Mat img;
		if(list_images[i].channels()>1)
			cv::cvtColor(list_images[i],img,CV_RGB2GRAY);
		else
			img=list_images[i];
		cv::findContours(img,contour,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
		for(unsigned j=0;contour.size()>0 && j<contour.size();j++)
		{
			double area=cv::contourArea(contour[j]);
			if( area< img.rows*img.cols*0.001)
			{
				contour.erase(contour.begin()+j);
				j--;
			}
		}
		contours.push_back(contour);
		if(show)
		{
			cv::drawContours(list_images[i],contour,-1,CV_RGB(255,0,0),2);
			cv::imshow("showme",list_images[i]);
			cv::waitKey(50);
		}	
	}
	make_tracks();
}

void Tracker::optical_flow(bool show)
{
	cv::Mat previmg=list_images[0];
	vector<cv::Point2f>prevpts(100),nextpts(100);
	vector<unsigned char>status(100);
	vector<float>err(100);
	//cv::cvtColor(list_images[0],previmg,CV_RGB2GRAY);
	for(unsigned int i=1;i<list_images.size();i++)
	{
		cv::Mat grayimg=list_images[i];
		//cv::cvtColor(list_images[i],grayimg,CV_RGB2GRAY);
		cv::calcOpticalFlowPyrLK(previmg,grayimg,prevpts,nextpts,status,err);
		previmg=grayimg;
		if(show)
		{
			cv::Mat imgdraw=list_images[i-1];
			for(unsigned int j=0;j<prevpts.size();j++)
			{
				cv::line(imgdraw,prevpts[j],nextpts[j],CV_RGB(255,0,0));	
			}
			
			cv::imshow("showme",imgdraw);
			cv::waitKey(50);
		}
	}
}

void Tracker::make_tracks()
{
	cv::VideoWriter record("Particles.avi",CV_FOURCC('D','I','V','X'),30, list_images[0].size(), true);
	trackpoint tt;
	for(unsigned int i=0;i<contours.size();i++)
	{
		if(contours[i].size()==0) continue;
		cv::Mat showimage=tt.update(contours[i],list_images_org[i]);
		//cv::imshow("showme",showimage);
		record<<showimage;
		record<<showimage;
		record<<showimage;
		//cv::waitKey(200);
	}
}
trackpoint::trackpoint()
{

}
trackpoint::~trackpoint()
{

}
cv::Point trackpoint::contourcentre(vector<cv::Point> c)
{
	cv::Point2f midpoint=cvPoint(0,0);
	for(unsigned int i=0;i<c.size();i++)
	{
		midpoint.x+=c[i].x;
		midpoint.y+=c[i].y;
	}
	midpoint.x/=c.size();
	midpoint.y/=c.size();
	return(midpoint);
}
cv::Mat trackpoint::update(vector<vector<cv::Point> >contour,cv::Mat img)
{
	//find nearest point from centre + motion
	vector<cv::Point> tempcentres;
	for(unsigned int i=0;i<contour.size();i++)
	{
		tempcentres.push_back(contourcentre(contour[i]));
	}
	nearest(tempcentres);
	for(unsigned int i=0;i<tempcentres.size();i++)
	{
		centres.push_back(tempcentres[i]);
		motion.push_back(cvPoint(0,0));
		age.push_back(AGE_THRESH);
	}
	for(unsigned int i=0;i<centres.size();i++)
	{
		if(age[i]>0)
		{
			char num[5];
			cv::circle(img,centres[i],3,CV_RGB(255,0,0),1);
			_itoa(i,num,10);
			cv::putText(img,string(num),centres[i],CV_FONT_HERSHEY_SCRIPT_SIMPLEX,2,CV_RGB(0,0,255));
		}
	}
	return(img);
}

double dist(cv::Point p1,cv::Point p2)
{
	return ((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

void trackpoint::nearest(vector<cv::Point> &inputcentres)
{
	for(unsigned int i=0;i<centres.size();i++)
	{
		int centre=-1;
		double maxdistance=DISTANCE_THRESH*DISTANCE_THRESH;
		
		for(unsigned int j=0;j<inputcentres.size();j++)
		{
			cv::Point nextpoint;
			nextpoint.x=centres[i].x+motion[i].x;
			nextpoint.y=centres[i].y+motion[i].y;
			double tdist=dist(nextpoint,inputcentres[j]);
			if( tdist < DISTANCE_THRESH*DISTANCE_THRESH && tdist < maxdistance && age[i]>=0)
			{
				maxdistance=tdist;
				centre=j;
			}
		}
		
		if(centre>=0)
		{
			motion[i]=inputcentres[centre]-centres[i];
			centres[i]=inputcentres[centre];
			inputcentres.erase(inputcentres.begin()+centre);
			age[i]=AGE_THRESH;
		}
		age[i]--;
	}
}