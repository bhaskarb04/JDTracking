#include "RWVector.h"

void write_single_vector(const char* vec,int size,ofstream &os,int type)
{
	os.write((const char*)&size,sizeof(size));
	if(type==1)
		os.write(vec,size*sizeof(showcircle));
	else if(type==2)
		os.write(vec,size*sizeof(cv::Point));
}

void write_single_vector(vector<showcircle> myvec2,int size,ofstream &os)
{
	os.write((const char*)&size,sizeof(size));
	for(int i=0;i<size;i++)
	{
		showcircle sc=myvec2[i];
		os.write((const char*)&(sc.age),sizeof(sc.age));
		os.write((const char*)&(sc.p),sizeof(sc.p));
		os.write((const char*)&(sc.radius),sizeof(sc.radius));
		os.write((const char*)&(sc.visible),sizeof(sc.visible));
		os.write((const char*)&(sc.z),sizeof(sc.z));
		int myvecsize = myvec2[i].contour.size();
		if(myvecsize)
			write_single_vector((const char*)&(sc.contour[0]),myvecsize,os,2);
		else
			os.write((const char*)&myvecsize,sizeof(myvecsize));
	}
}


void save_vector(vector<vector<showcircle> > myvec, char *fname,int rows,int cols)
{
	ofstream os (fname,ios::binary);
	os.write((const char*)&rows,sizeof(rows));
	os.write((const char*)&cols,sizeof(cols));
	int size1=myvec.size();
	os.write((const char*)&size1,sizeof(size1));
	for(unsigned int i=0;i<size1;i++)
	{
		int myvecsize=myvec[i].size();
		if(myvecsize)
			write_single_vector(myvec[i],myvec[i].size(),os);
		else
			os.write((const char*)&myvecsize,sizeof(myvecsize));
	}
	os.close();
}

/*void save_contour(vector<vector<vector<cv::Point> > >myvec, char *fname)
{
	ofstream os (fname,ios::binary);
	
	int size1=myvec.size();
	os.write((const char*)&size1,sizeof(size1));
	for(unsigned int i=0;i<size1;i++)
	{
		int size2=myvec[i].size();
		os.write((const char*)&size2,sizeof(size2));
		for(unsigned int j=0;j<size2;j++)
		{
			int myvecsize = myvec[i][j].size();
			if(myvecsize)
				write_single_vector((const char*)&myvec[i][j][0],myvecsize,os,2);
			else
				os.write((const char*)&myvecsize,sizeof(myvecsize));
			
		}
	}
	os.close();
}
*/
vector<vector<showcircle> > read_vector(char *fname,int &rows,int&cols)
{
	ifstream is (fname,ios::binary);
	int size;
	is.read((char*)&rows,sizeof(rows));
	is.read((char*)&cols,sizeof(cols));
	is.read((char*)&size,sizeof(size));
	vector<vector<showcircle> > readvec(size);
	vector<showcircle> nothing;
	for(unsigned int i=0;i<size;i++)
	{
		int size2;
		is.read((char*)&size2,sizeof(size2));
		if(size2)
		{
			readvec[i].resize(size2);
			for(int j=0;j<size2;j++)
			{
				showcircle sc;
				is.read((char*)&(sc.age),sizeof(sc.age));
				is.read((char*)&sc.p,sizeof(sc.p));
				is.read((char*)&sc.radius,sizeof(sc.radius));
				is.read((char*)&sc.visible,sizeof(sc.visible));
				is.read((char*)&sc.z,sizeof(sc.z));
				int myvecsize;
				is.read((char*)&myvecsize,sizeof(myvecsize));
				if(myvecsize)
				{
					sc.contour.resize(myvecsize);
					is.read((char*)&sc.contour[0],sizeof(cv::Point)*myvecsize);
				}
				readvec[i][j]=sc;
			}
		}
	}
	return readvec;
}
/*
vector<vector<vector<cv::Point> > > read_contour(char *fname)
{
	ifstream is (fname,ios::binary);
	int size;
	is.read((char*)&size,sizeof(size));
	vector<vector<vector<cv::Point> > > readvec(size);
	vector<showcircle> nothing;
	for(unsigned int i=0;i<size;i++)
	{
		int size2;
		is.read((char*)&size2,sizeof(size2));
		if(size2)
		{
			readvec[i].resize(size2);
			for(unsigned int j=0;j<size2;j++)
			{
				int size3;
				is.read((char*)&size3,sizeof(size3));
				readvec[i][j].resize(size3);
				is.read((char*)&readvec[i][j][0], size3 * sizeof(cv::Point));
			}
			
		}
	}
	return readvec;
}
*/