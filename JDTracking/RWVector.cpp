#include "RWVector.h"

void write_single_vector(const char* vec,int size,ofstream &os)
{
	os.write((const char*)&size,sizeof(size));
	os.write(vec,size*sizeof(showcircle));
}


void save_vector(vector<vector<showcircle> > myvec, char *fname,int rows,int cols)
{
	ofstream os (fname,ios::binary);
	os.write((const char*)&rows,sizeof(rows));
	os.write((const char*)&cols,sizeof(cols));
	int size1=myvec.size();
	os.write((const char*)&size1,sizeof(size1));
	for(unsigned int i=0;i<size1;i++)
		write_single_vector((const char*)&myvec[i][0],myvec[i].size(),os);

	os.close();
}

vector<vector<showcircle> > read_vector(char *fname,int &rows,int&cols)
{
	ifstream is (fname,ios::binary);
	int size;
	is.read((char*)&rows,sizeof(rows));
	is.read((char*)&cols,sizeof(cols));
	is.read((char*)&size,sizeof(size));
	vector<vector<showcircle> > readvec(size);
	for(unsigned int i=0;i<size;i++)
	{
		int size2;
		is.read((char*)&size2,sizeof(size2));
		readvec[i].resize(size2);
		is.read((char*)&readvec[i][0], size2 * sizeof(showcircle));
	}
	return readvec;
}