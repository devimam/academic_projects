#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int ref1w,ref1h,ref2w,ref2h,testw,testh;
double ref1r[2000][2000];
double ref1g[2000][2000];
double ref1b[2000][2000];

double ref2r[2000][2000];
double ref2g[2000][2000];
double ref2b[2000][2000];

double testr[2000][2000];
double testg[2000][2000];
double testb[2000][2000];

double d[2000][2000];

void ReadBMP(char* filename,double r[2000][2000],double g[2000][2000],double b[2000][2000],int *width,int *height)
{
    int i;
    FILE* f = fopen(filename, "rb");

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    *width = *(int*)&info[18];
    *height = *(int*)&info[22];
/*
    int sz = 3 * (*width) * (*height);
    unsigned char* data = new unsigned char[sz]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), sz, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < sz; i += 3)
    {
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;

            int pixr=i/((*width)*3);
            int pixc=(i%((*width)*3))/3;
///cout<<pixr<<" "<<pixc<<endl;
            r[pixr][pixc]=(int)data[i]/255.0;
            g[pixr][pixc]=(int)data[i+1]/255.0;
            b[pixr][pixc]=(int)data[i+2]/255.0;

            cout<<(int)data[i]<<endl;
    }
*/


    int row_padded = (*width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    for(int i = 0; i < *height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < *width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;
            ///cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
            r[i][j/3]=(int)data[j];
            g[i][j/3]=(int)data[j+1];
            b[i][j/3]=(int)data[j+2];
        }
    }
    fclose(f);
}

void brute_force(){
/*
    int maxr=-1,maxc=-1;
    double maxval=-1;

    for(int m=0;m<testh-ref1h+1;m++){
        for(int n=0;n<testw-ref1w+1;n++){
            d[m][n]=0;
            for(int i=m;i<m+ref1h;i++){
                for(int j=n;j<n+ref1w;j++){
                    d[m][n]+=(testr[i][j]-ref1r[i-m][j-n])*(testr[i][j]-ref1r[i-m][j-n]);
                    d[m][n]+=(testg[i][j]-ref1g[i-m][j-n])*(testg[i][j]-ref1g[i-m][j-n]);
                    d[m][n]+=(testb[i][j]-ref1b[i-m][j-n])*(testb[i][j]-ref1b[i-m][j-n]);
                }
            }
            ///cout<<d[m][n]<<endl;
            if(maxval<d[m][n]){maxr=m;maxc=n;maxval=d[m][n];}
        ///cout<<m<<" "<<n<<" "<<d[m][n]<<endl;
        }
    }
    cout<<maxr<<" "<<maxc<<" "<<maxval<<endl;

*/

    int maxmr2=-1,maxmc2=-1;
    double maxval2=-1;
    for(int m=0;m<testh-ref2h+1;m++){
        for(int n=0;n<testw-ref2w+1;n++){
            d[m][n]=0;
            for(int i=m;i<m+ref2h;i++){
                for(int j=n;j<n+ref2w;j++){
                    d[m][n]+=(testr[i][j]-ref2r[i-m][j-n])*(testr[i][j]-ref2r[i-m][j-n]);
                    d[m][n]+=(testg[i][j]-ref2g[i-m][j-n])*(testg[i][j]-ref2g[i-m][j-n]);
                    d[m][n]+=(testb[i][j]-ref2b[i-m][j-n])*(testb[i][j]-ref2b[i-m][j-n]);
                }
            }
            if(maxval2==-1) {maxmr2=m;maxmc2=n;maxval2=d[m][n];}
            else if(maxval2>d[m][n]){maxmr2=m;maxmc2=n;maxval2=d[m][n];}

        }
    }
    cout<<maxmr2<<" "<<maxmc2<<" "<<maxval2<<endl;

}

int main()
{
    ///ReadBMP("ref1.bmp",ref1r,ref1g,ref1b,&ref1w,&ref1h);
    ReadBMP("ref2.bmp",ref2r,ref2g,ref2b,&ref2w,&ref2h);
    ReadBMP("test.bmp",testr,testg,testb,&testw,&testh);
    ///cout<<"sizes are: "<<ref1w<<" "<<ref1h<<" "<<ref2w<<" "<<ref2h<<" "<<testw<<" "<<testh<<endl;
    brute_force();
    return 0;
}
