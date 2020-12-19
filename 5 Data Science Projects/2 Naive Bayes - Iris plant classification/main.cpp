#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

#define ROW 150
#define FEATURE 4               ///column no = feature + 1
#define CLASS 3
#define MAX_FTRE_VALUE 3

double train_input[ROW][FEATURE+1];
double test_input[ROW][FEATURE+1];
double input[ROW][FEATURE+1];

double mean[ROW][FEATURE];
double std_dev[ROW][FEATURE];
double prob[ROW][FEATURE];
double single_test[FEATURE+1];

char COMMA;

void take_input(){
    ifstream file;
    file.open("input.txt");
    int i=0;
    if(file.is_open()){
            for(int i=0;i<150;i++){

                    for(int k=0;k<FEATURE;k++) file>>input[i][k]>>COMMA;
                    string tmp;
                    file>>tmp;
                    if(tmp=="Iris-setosa"){
                        input[i][FEATURE]=1;
                    }
                    else if(tmp=="Iris-versicolor"){
                        input[i][FEATURE]=2;
                    }
                    else if(tmp=="Iris-virginica"){
                        input[i][FEATURE]=3;
                    }
            }
    }
    else cout<<"Can't open the file"<<endl;
}

void show_input_arr(){
    for(int i=0;i<150;i++){
        for(int k=0;k<FEATURE;k++) cout<<input[i][k]<<" , ";
        cout<<input[i][FEATURE]<<endl;
    }
}

void split_input(){

        for(int i=0;i<CLASS;i++){
            for(int j=0;j<50;j++){
                if(j<40){
                    for(int k=0;k<FEATURE+1;k++) train_input[i*40+j][k]=input[i*50+j][k];
                }
                else{
                    for(int k=0;k<FEATURE+1;k++) test_input[i*10+(j-40)][k]=input[i*50+j][k];
                }
            }
        }
}

void show_input_data(){
    cout<<"train-------------"<<endl;
    for(int i=0;i<120;i++){
        for(int j=0;j<FEATURE;j++) cout<<train_input[i][j]<<" , ";
        cout<<train_input[i][FEATURE]<<endl;
    }
    cout<<"test----------------"<<endl;
    for(int i=0;i<30;i++){
        for(int j=0;j<FEATURE;j++) cout<<test_input[i][j]<<" , ";
        cout<<test_input[i][FEATURE]<<endl;
    }
}

void calculate_mean(){
    for(int i=0;i<CLASS;i++){
        for(int k=0;k<FEATURE;k++){
            double sum=0;
            for(int j=0;j<40;j++){
                sum+=train_input[i*40+j][k];
            }
            double tmp_mean=sum/40;
            mean[i][k]=tmp_mean;
            //cout<<tmp_mean<<endl;
        }
    }
}

void calculate_stdev(){
    cout<<"calculating std dev"<<endl;
    for(int i=0;i<CLASS;i++){
        for(int k=0;k<FEATURE;k++){
            double sum=0;
            for(int j=0;j<40;j++){
                sum+=(train_input[i*40+j][k]-mean[i][k])*(train_input[i*40+j][k]-mean[i][k]);
            }
            double tmp_stdev=sum/40;
            std_dev[i][k]=tmp_stdev;
            //cout<<tmp_stdev<<endl;
        }
    }
}

double calculate_prob(int xi,int i,int k){
   // cout<<"calculating prob"<<endl;

    return (1/sqrt(2*3.1416*std_dev[i][k]))*exp(-1*((xi-mean[i][k])*(xi-mean[i][k]))/(2*std_dev[i][k]));
}

void test(){
    int cnt=0;
    for(int it=0;it<30;it++){
        for(int jt=0;jt<FEATURE;jt++) single_test[jt]=test_input[it][jt];

        double maxm=0;
        int class_id=-1;
        int k;
        for( k=0;k<CLASS;k++){
            double result=1;
            for(int j=0;j<FEATURE;j++){
                result*=calculate_prob(single_test[j],k,j);
            }
            if(result>maxm) {
                maxm=result;
                class_id=k;
            }

        }
        cout<<class_id+1<<endl;
        if(class_id+1==test_input[it][FEATURE]) cnt++;

    }
    cout<<cnt*100/30.0<<" %"<<endl;

}

int main()
{
    take_input();
    //show_input_arr();
    split_input();
    //show_input_data();

    calculate_mean();

    calculate_stdev();

    test();

    return 0;
}
