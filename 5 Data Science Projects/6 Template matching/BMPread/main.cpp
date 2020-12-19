#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int ref1w,ref1h,ref2w,ref2h,testw,testh;

double ref1g[2000][2000];

double ref2g[2000][2000];

double testg[2000][2000];

double d[2000][2000];

void brute_force(){
    int maxr=-1,maxc=-1;
    double maxval=-1;

    for(int m=0;m<testh-ref1h+1;m++){
        for(int n=0;n<testw-ref1w+1;n++){

            d[m][n]=0;
            for(int i=m;i<m+ref1h;i++){
                for(int j=n;j<n+ref1w;j++){
                    d[m][n]+=testg[i][j]*ref1g[i-m][j-n];
                }
            }
            if(d[m][n]>maxval){
                maxr=m;
                maxc=n;
                maxval=d[m][n];
            }

        }
    }
    cout<<maxr<<" "<<maxc<<" "<<maxval<<endl;
}

int main()
{
    FILE* f = fopen("ref2.bmp", "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);
    ref1w=*(int*)&info[18];
    ref1h=*(int*)&info[22];
    int row_padded = (ref1w*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    for(int i = 0; i < ref1h; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < ref1w*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;
            ///cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
            ref1g[i][j/3]=((int)data[j+2]+(int)data[j]+(int)data[j+1])/3.0;
        }
    }
    fclose(f);

    FILE* f1 = fopen("test.bmp", "rb");
    unsigned char info1[54];
    fread(info1, sizeof(unsigned char), 54, f1);
    testw=*(int*)&info1[18];
    testh=*(int*)&info1[22];
    int row_padded1 = (testw*3 + 3) & (~3);
    unsigned char* data1 = new unsigned char[row_padded1];
    unsigned char tmp1;

    for(int i = 0; i < testh; i++)
    {
        fread(data1, sizeof(unsigned char), row_padded1, f1);
        for(int j = 0; j < testw*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp1 = data1[j];
            data1[j] = data1[j+2];
            data1[j+2] = tmp1;
            ///cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
            testg[i][j/3]=((int)data1[j+2]+(int)data1[j]+(int)data1[j+1])/3.0;
            ///cout<<testg[i][j/3]<<" ";
        }
    }
    fclose(f1);


    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++) cout<<ref1g[i][j]<<" ";
    }
    cout<<endl;

    for(int i=700;i<710;i++){
        for(int j=700;j<710;j++) cout<<testg[i][j]<<" ";
    }
    cout<<endl;

    ///brute_force();
    return 0;
}
