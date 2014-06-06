#include "Tracker.h"
#include "RWVector.h"
#include "OSGView.h"
#define DO_ANALYSIS true
#define TIMER_INTERVAL 33
GLint window_width=1000;
GLint window_height=600;
#define DISPLAY 1

void make_video(vector<cv::Mat>,vector<cv::Mat>,string,string);
void make_video(vector<cv::Mat>,vector<cv::Mat>,string,string,int loops);
 int main(int argc, char** argv) 
 {
	bool analysis_code_do=DO_ANALYSIS;
	osgView osgview;
	//Tracker constructor loads the images from the path automatically.For now it is assumed to be png
	//TODO: add functionality for other image formats(supported by opencv)
	string s1=FILE_PATH;
	string s2="5Cotton";
	string mp4file=".mp4";
	string datfile=".dat";
	string finalfile=s1+s2+mp4file;
	string datafile=s2+datfile;
	string datafile2=s2+"2"+datfile;
	string vidfile=s2+"_final3.avi";
	string depthpath="C:\\Users\\Bhaskar\\Dropbox\\manyCotton";
	string filewritepath=s1+"FinalImages\\";
	
	Tracker track(finalfile,true);
	if(analysis_code_do)
	{
		track.clean_image(false);
		track.track_particles(false);
		save_vector(track.tracks,(char*)datafile.c_str(),track.imgrows,track.imgcols,track.backgrounddepth());
	}
	cv::destroyAllWindows();
	int imgrows=0,imgcols=0;
	double bgdepth=0;
	vector<vector<showcircle> >tracks2=read_vector((char*)datafile.c_str(),imgrows,imgcols,bgdepth);
	bgdepth=0;
	osgview.setmax(imgcols,imgrows,255,bgdepth);
	osgview.set_tracks(tracks2);
	osgview.drawAnimation();
	//osgview.convert2vtk(s2);
	//osgV.draw();
//	make_video(track.get_orig_images(),osgV.osgVideo,vidfile,filewritepath,NO_LOOPS);
	return 0;
 }

 void make_video(vector<cv::Mat>lhs,vector<cv::Mat>rhs,string vidname,string path)
 {
	 if(lhs.size()==0 || rhs.size()==0 || (lhs.size() < rhs.size()))
		 return;
	 int bump=lhs.size() - rhs.size();
	 cv::Size imgsize=lhs[0].size();
	 cv::Mat vidimg(imgsize.height,2*imgsize.width,CV_8UC3);
	 cv::VideoWriter video(vidname, CV_FOURCC('D','I','V','X'), 30, vidimg.size(), true);
	 for(unsigned int i=0;i<rhs.size();i++)
	 {
		 string filename;
		 char num[5];
		 sprintf(num,"%d",i);
		 filename=path+string(num)+".png";
		 cv::Mat l=lhs[i+bump];
		 cv::Mat r=rhs[i];
		 cv::Mat lroi=vidimg(cv::Rect(0,0,imgsize.width,imgsize.height));
		 cv::Mat rroi=vidimg(cv::Rect(imgsize.width,0,imgsize.width,imgsize.height));
		 cv::resize(rhs[i],r,imgsize);
		 l.copyTo(lroi);
		 r.copyTo(rroi);;
		 cv::imshow("finalvideo",vidimg);
		 cv::waitKey(10);
		 video<<vidimg;
		 video<<vidimg;
		 video<<vidimg;
		 cv::imwrite(filename,vidimg);
	 }
	 video.release();
 }
 void make_video(vector<cv::Mat>lhs,vector<cv::Mat>rhs,string vidname,string path,int loops)
 {
	if(LOOP)
		return;
	 if(lhs.size()==0 || rhs.size()==0)
	 return;
	 cv::Size imgsize=lhs[0].size();
	 cv::Mat vidimg(imgsize.height,2*imgsize.width,CV_8UC3);
	 cv::VideoWriter video(vidname, CV_FOURCC('D','I','V','X'), 30, vidimg.size(), true);
	 for(unsigned int i=0;i<rhs.size();i++)
	 {
		 string filename;
		 char num[5];
		 sprintf(num,"%d",i);
		 filename=path+string(num)+".png";
		 cv::Mat l=lhs[i%lhs.size()];
		 cv::Mat r=rhs[i];
		 cv::Mat lroi=vidimg(cv::Rect(0,0,imgsize.width,imgsize.height));
		 cv::Mat rroi=vidimg(cv::Rect(imgsize.width,0,imgsize.width,imgsize.height));
		 cv::resize(rhs[i],r,imgsize);
		 l.copyTo(lroi);
		 r.copyTo(rroi);;
		 //cv::imshow("finalvideo",vidimg);
		 //cv::waitKey(10);
		 video<<vidimg;
		 video<<vidimg;
		 video<<vidimg;
		 cv::imwrite(filename,vidimg);
	 }
	 video.release();
 }