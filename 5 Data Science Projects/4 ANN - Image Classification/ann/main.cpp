#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

#define LRNRATE .05
#define TRAIN 2000
#define VALID 100
#define TEST 500
#define MAXM_TRAIN 100

double train[5000][785];
double valid[2000][785];
double test[2000][785];
int listt[50];
double weights[1000][1000][50];              ///connecting previous k, next j, neurons at layer r
double biases[1000][50];

int train_line,valid_line,test_line,layer;

void read_input(){
    cout<<"<<<reading input>>>"<<endl;
    ifstream fin,fin1,fin2;
    fin.open("train_data.txt");
    fin1.open("valid_data.txt");
    fin2.open("test_data.txt");
    char comma,gender;
    if(fin.is_open() && fin1.is_open() && fin2.is_open()){
        for(int i=0;i<train_line;i++){
            for(int j=0;j<784;j++){
                fin>>train[i][j]>>comma;
            }
            fin>>train[i][784];
            ///cout<<train[i][784]<<endl;
        }
        ///fin.close();
        for(int i=0;i<valid_line;i++){
            for(int j=0;j<784;j++){
                fin1>>valid[i][j]>>comma;
            }
            fin1>>valid[i][784];
            ///cout<<valid[i][784]<<endl;
        }
        ///fin1.close();
        for(int i=0;i<test_line;i++){
            for(int j=0;j<784;j++){
                fin2>>test[i][j]>>comma;
            }
            fin2>>test[i][784];
        }
        ///fin2.close();
    }
    else cout<<"can't open the input file"<<endl;
}

void show_train_data(){
    for(int i=0;i<10;i++){
        for(int j=0;j<785;j++){
            cout<<train[i][j]<<" ";
        }
        cout<<endl;
    }
}

void ann(){
    srand(time(NULL));

    int maxm=784;
    for(int i=0;i<layer;i++){
        if(listt[i]>maxm) maxm=listt[i];
    }

    ///cout<<"maxm: "<<maxm<<endl;

    ///randomly initializing the weights and biases

    for(int i=0;i<layer;i++){
        for(int j=0;j<maxm;j++){
            for(int k=0;k<maxm;k++) weights[k][j][i]=pow((-1),(rand()%2))*(rand()%6)/100.0;    ///range -0.05 to 0.05
            ///for(int k=0;k<maxm;k++) weights[k][j][i]=-.05+fmod(rand(),(.05+.05));
        }
    }

    for(int i=0;i<layer;i++){
        for(int j=0;j<maxm;j++){
            biases[j][i]=pow((-1),(rand()%2))*(rand()%6)/100.0;
            ///biases[j][i]=-.05+(rand()%(5+5+1))/100.0;
            ///cout<<biases[j][i]<<" ";
        }
        ///cout<<endl;
    }


    int prev_acc=0;
    ///starting the training
    for(int loop=0;loop<MAXM_TRAIN;loop++){
        for(int i=0;i<train_line;i++){
            double y[maxm][layer+1]={0};
            for(int it=0;it<784;it++) y[it][0]=train[i][it];
            double v[maxm][layer]={0};

            double y_out[10]={0};
            y_out[(int)(train[i][784])]=1;
///for(int it=0;it<10;it++) cout<<y_out[it]<<" ";
///cout<<endl;

            ///going forward
            double sum_e=0;
            for(int r=0;r<layer;r++){
                for(int j=0;j<listt[r];j++){
                    double sum=0;
                    int prev_dim=0;
                    if(r==0) prev_dim=784;
                    else prev_dim=listt[r-1];

                    for(int k=0;k<prev_dim;k++){
                        sum+=weights[k][j][r]*y[k][r];
                    }
                    sum+=biases[j][r];

                    v[j][r]=sum;
                    if(r==layer-1) sum_e+=exp(sum);
                    y[j][r+1]=1.0/(1.0+exp(-1*sum));
                }
            }

///softmax for the final layer
            int r=layer-1;
            for(int j=0;j<listt[r];j++){
                y[j][r+1]=exp(v[j][r])/sum_e;
            }


            ///going backward
            double delta[maxm][layer]={0};
            for(int it=0;it<listt[layer-1];it++) delta[it][layer-1]=(y[it][layer]-y_out[it])*y[it][layer]*(1-y[it][layer]);

            for(int r=layer-2;r>=0;r--){
                for(int j=0;j<listt[r];j++){
                    double delta_sum=0;
                    for(int k=0;k<listt[r+1];k++) delta_sum+=delta[k][r+1]*weights[j][k][r+1];
                    delta_sum=delta_sum*y[j][r+1]*(1-y[j][r+1]);

                    delta[j][r]=delta_sum;
                }
            }

            ///updating weights and biases
            for(int r=0;r<layer;r++){
                for(int j=0;j<listt[r];j++){
                    int dim_w=0;
                    if(r==0) dim_w=784;
                    else dim_w=listt[r-1];

                    for(int k=0;k<dim_w;k++) weights[k][j][r]-=LRNRATE*delta[j][r]*y[k][r];
                    biases[j][r]-=LRNRATE*delta[j][r];
                }
            }
        }
        int accurate=0;
        for(int i=0;i<valid_line;i++){
            ///cout<<i<<endl;
            double y[maxm][layer+1]={0};
            for(int it=0;it<784;it++) y[it][0]=valid[i][it];
            double v[maxm][layer]={0};

            int y_out=valid[i][784];

            ///going forward
            double sum_ev=0;
            for(int r=0;r<layer;r++){
                for(int j=0;j<listt[r];j++){
                    double sum=0;
                    int prev_dim=0;
                    if(r==0) prev_dim=784;
                    else prev_dim=listt[r-1];

                    for(int k=0;k<prev_dim;k++){
                        sum+=weights[k][j][r]*y[k][r];
                    }
                    sum+=biases[j][r];

                    v[j][r]=sum;
                    if(r==layer-1) sum_ev+=exp(sum);
                    y[j][r+1]=1.0/(1.0+exp(-1*sum));
                    ///y[j][r+1]=1.0/(1.0+exp(-1*sum));
                }
            }

            int r=layer-1;
            for(int j=0;j<listt[r];j++){
                y[j][r+1]=exp(v[j][r])/sum_ev;
               /// cout<<y[j][r+1]<<endl;
            }

            ///cout<<endl;
            ///cout<<y_out<<endl;
            ///cout<<"---------------------"<<endl;

            int maxm=-1;double max_val=-1;
            for(int i=0;i<listt[layer-1];i++){
                if(y[i][layer]>max_val){
                    max_val=y[i][layer];
                    maxm=i;
                }
            }
            if(maxm==y_out) accurate++;
        }
        cout<<accurate<<endl;

        if(accurate>=prev_acc) prev_acc=accurate;
        ///else break;
    }
///test set output
    int accurate1=0;
    for(int i=0;i<test_line;i++){
        ///cout<<i<<endl;
        double y[maxm][layer+1]={0};
        for(int it=0;it<784;it++) y[it][0]=test[i][it];
        double v[maxm][layer]={0};

        int y_out=test[i][784];

        ///going forward
        double sum_et=0;
        for(int r=0;r<layer;r++){
            for(int j=0;j<listt[r];j++){
                double sum=0;
                int prev_dim=0;
                if(r==0) prev_dim=784;
                else prev_dim=listt[r-1];

                for(int k=0;k<prev_dim;k++){
                    sum+=weights[k][j][r]*y[k][r];
                }
                sum+=biases[j][r];

                v[j][r]=sum;
                if(r==layer-1) sum_et+=exp(sum);
                y[j][r+1]=1.0/(1.0+exp(-1*sum));

                ///y[j][r+1]=1.0/(1.0+exp(-1*sum));
            }
        }

        int r=layer-1;
        for(int j=0;j<listt[r];j++){
            y[j][r+1]=exp(v[j][r])/sum_et;
        }


        int maxm=-1;double max_val=-1;
        for(int i=0;i<listt[layer-1];i++){
            if(y[i][layer]>max_val){
                max_val=y[i][layer];
                maxm=i;
            }
        }
        if(maxm==y_out) accurate1++;
    }
    cout<<"in test data set accuracy is: "<<endl;
    cout<<((double)accurate1/test_line)*100.0<<" %"<<endl;
}


int main()
{
    train_line=TRAIN;
    valid_line=VALID;
    test_line=TEST;
    if(train_line+valid_line+test_line>70000) cout<<"no of data to be read is too larger than the input data set."<<endl;
    else{
        read_input();
        ///show_train_data();

        cout<<"no of layers :";
        cin>>layer;
        for(int i=0;i<layer;i++){
            cout<<"input the "<<i+1<<" th layer no of neurons."<<endl;
            cin>>listt[i];
        }
        cout<<"---------------------------------------------------------------"<<endl;
        ann();

    }
    return 0;
}
