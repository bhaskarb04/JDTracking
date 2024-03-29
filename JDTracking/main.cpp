#include "Tracker.h"
#include "Viewer.h"
#include "RWVector.h"
#include "OSGView.h"
#define TIMER_INTERVAL 33
GLint window_width=1000;
GLint window_height=600;
#define DISPLAY 1

void make_video(vector<cv::Mat>,vector<cv::Mat>,string);
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
	 //osgView osgV;

	 //return 0;
	//Tracker constructor loads the images from the path automatically.For now it is assumed to be png
	//TODO: add functionality for other image formats(supported by opencv)
	string s1="C:\\Users\\Bhaskar\\Documents\\Fall\ 2012\\Research\\JDTracking\\Data\\";
	string s2="ManyCorn2000Hz_2-Depth";
	string mp4file=".mp4";
	string datfile=".dat";
	//2Cotton2000Hz_1-Depth
	//ManyCotton2000Hz_2-Depth
	//ManyCorn2000Hz_2-Depth
	//1Corn2000Hz_1
	//5Cotton2000Hz_1
	//Tracker track("C:\\Users\\Bhaskar\\Desktop\\JDTracking\\Take3-Images",false);
	
	string finalfile=s1+s2+mp4file;
	string datafile=s2+datfile;
	string datafile2=s2+"2"+datfile;
	string vidfile=s2+"_final2.avi";
	
	Tracker track(finalfile,true);
	
	track.clean_image(false);
	track.track_particles(true);
	save_vector(track.tracks,(char*)datafile.c_str(),track.imgrows,track.imgcols);
	//save_contour(track.get_contours(),(char*)datafile2.c_str());
	//return 0;
	
	int imgrows,imgcols;
	vector<vector<showcircle> >tracks2=read_vector((char*)datafile.c_str(),imgrows,imgcols);
	//vector<vector<vector<cv::Point> > > contours=read_contour((char*)datafile2.c_str());
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
	make_video(track.get_orig_images(),viewer->get_glVideo(),vidfile);
	 return 0;
 }

 void make_video(vector<cv::Mat>lhs,vector<cv::Mat>rhs,string vidname)
 {
	 if(lhs.size()==0 || rhs.size()==0 || (lhs.size() < rhs.size()))
		 return;
	 int bump=lhs.size() - rhs.size();
	 cv::Size imgsize=lhs[0].size();
	 cv::Mat vidimg(imgsize.height,2*imgsize.width,CV_8UC3);
	 cv::VideoWriter video(vidname, CV_FOURCC('D','I','V','X'), 30, vidimg.size(), true);
	 for(unsigned int i=0;i<rhs.size();i++)
	 {
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
	 }
	 video.release();
 }