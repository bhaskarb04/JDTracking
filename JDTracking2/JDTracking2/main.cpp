#include "Tracker.h"
#include "Viewer.h"
#include "RWVector.h"
#include "OSGView2.h"
#define TIMER_INTERVAL 33
GLint window_width=1000;
GLint window_height=600;
#define DISPLAY 1

void make_video(vector<cv::Mat>,vector<cv::Mat>,string,string);
void make_video(vector<cv::Mat>,vector<cv::Mat>,string,string,int loops);
Viewer *viewer;
void Display(void)
{
	if(DISPLAY==1)
		viewer->display();
	else
		viewer->display2();
}

void HandleKeyboard(unsigned char key,int x,int y)
{
	viewer->KeyboardNormal(key,x,y);
}

void HandleSpecialKeyboard(int key,int x,int y)
{
	viewer->SpecialKeyboard(key,x,y);
}

void HandleIdle(void)
{
   glutPostRedisplay();
}
void MouseButton(GLint button, GLint state, GLint x, GLint y)
{
	viewer->MouseClick(button,state,x,y);
}

void TimerFunction(int value)
{
	viewer->timerfunc();
	glutPostRedisplay();
	glutTimerFunc(TIMER_INTERVAL, TimerFunction,1);
}

 int main(int argc, char** argv) 
 {
	OSGViewer osgviewer;

	//return 0;
	//Tracker constructor loads the images from the path automatically.For now it is assumed to be png
	//TODO: add functionality for other image formats(supported by opencv)
	string s1=FILE_PATH;
	string s2="BoeDepth6";
	string mp4file=".mp4";
	string datfile=".dat";
	//2Cotton2000Hz_1-Depth
	//ManyCotton2000Hz_2-Depth


	//ManyCorn2000Hz_2-Depth
	//1Corn2000Hz_1

	//5Cotton2000Hz_1
	//manyCotton

	//2Corn2000Hz_1
	//Tracker track("C:\\Users\\Bhaskar\\Desktop\\JDTracking\\Take3-Images",false);
	
	string finalfile=s1+s2+mp4file;
	string datafile=s2+datfile;
	string datafile2=s2+"2"+datfile;
	string vidfile=s2+"_final3.avi";
	string depthpath="C:\\Users\\Bhaskar\\Dropbox\\manyCotton";
	string filewritepath=s1+"FinalImages\\";
	
	Tracker track(finalfile,true);
	
	//track.clean_image(false);
	//track.track_particles(false);
	//save_vector(track.tracks,(char*)datafile.c_str(),track.imgrows,track.imgcols,track.backgrounddepth());
	//save_contour(track.get_contours(),(char*)datafile2.c_str());
	//return 0;
	
	int imgrows=0,imgcols=0;
	double bgdepth=0;
	vector<vector<showcircle> >tracks2=read_vector((char*)datafile.c_str(),imgrows,imgcols,bgdepth);
	//vector<vector<vector<cv::Point3f> > > contours=find_contours(depthpath,tracks2,imgrows,imgcols);
	bgdepth=0;
	osgviewer.setmax(imgcols,imgrows,255,bgdepth);
	osgviewer.settracks(tracks2);
	osgviewer.run();
	//osgV.draw();
//	make_video(track.get_orig_images(),osgV.osgVideo,vidfile,filewritepath,NO_LOOPS);
	return 0;
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width,window_height);
	glutCreateWindow("Viewer");
	viewer= new Viewer();

	glutDisplayFunc(Display);
	glutKeyboardFunc(HandleKeyboard);
	glutSpecialFunc(HandleSpecialKeyboard);
	//glutIdleFunc(HandleIdle);
	glutMouseFunc(MouseButton);
	glutTimerFunc(TIMER_INTERVAL, TimerFunction,1);

	if(DISPLAY==1)
		viewer->Init();
	else
		viewer->Init2();
	viewer->show_tracks(tracks2,imgrows,imgcols);
	//viewer->show_contours(contours);
	try
	{
		glutMainLoop();
	}
	catch(char *e)
	{
		cout<<e<<endl;
	}
	//make_video(track.get_orig_images(),viewer->get_glVideo(),vidfile);
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