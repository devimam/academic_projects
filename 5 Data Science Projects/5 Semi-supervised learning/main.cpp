#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <map>
#include <ctime>
#include <cstdlib>

using namespace std;

#define ROW 669
#define COL 9
#define ATT_MAX_VALUE 10         ///each attributes all possible values
#define YES -1                  ///for leveling
#define NO -2                   ///for leveling
#define UNDEFINED -3
#define LEAF -4

#define SUBSET 350

///attributes----------------------------------------------------------
#define clump_thickness 0
#define uniformity_cell_size 1
#define uniformity_cell_shape 2
#define marginal_adhesion 3
#define single_epithelial_cell_size 4
#define bare_Nuclei 5
#define bland_chromatin 6
#define normal_nucleoli 7
#define class_att 8
///--------------------------------------------------------------------

struct node{
    int id;

    int att_no;                         ///this nodes attribute
    int level;                          ///decision yes/no/undefined
};

map<int,node> tree;                     ///mapping each id to the corresponding node
map<int,node> tree_arr[1000];

char COMMA;

int input_data[ROW][COL];

void read_input_data(){
    ifstream file;
    file.open("input.csv");
    string temp;

    if(file.is_open()){
        getline(file,temp);   ///ignore the 1st line as it defines the attribute names only

        for(int r=0;r<ROW;r++){
            for(int c=0;c<COL-1;c++) file>>input_data[r][c]>>COMMA;             ///(col-1) as the last one does not contain the comma
            file>>input_data[r][COL-1];
        }

        file.close();
    }
    else cout<<"can't open the file"<<endl;
}

void show_input_data(){
    for(int r=0;r<ROW;r++){
        for(int c=0;c<COL-1;c++) cout<<input_data[r][c]<<",";
        cout<<input_data[r][COL-1]<<endl;
    }
}

double calculate_entropy(int yes,int no){
    if(yes==0 || no==0) return 0;               ///log2(0)=0 guess
    if(yes==no) return 1;

    double p_plus=1.0*yes/(yes+no);
    double p_minus=1.0*no/(yes+no);

    return -p_plus*log2(p_plus)-p_minus*log2(p_minus);
}
///-----------------------------------------------------------------------------------------------------------------------
int find_best_att(int examples[ROW][COL],int r,int c,int att_list[COL],int att_list_size){      ///att list size=c-1

    ///building attribute entropy table
    double att_ent_table[COL*ATT_MAX_VALUE][3];                 ///0 col=yes, 1 col=no, 2 col=entropy

    int yes,no;
    for(int att=0;att<att_list_size;att++){
        for(int att_val=1;att_val<=ATT_MAX_VALUE;att_val++){
            yes=0;
            no=0;

            for(int i=0;i<r;i++){
                if(examples[i][att]==att_val && examples[i][c-1]==1) yes++;
                else if(examples[i][att]==att_val && examples[i][c-1]==0) no++;
            }
            att_ent_table[att*ATT_MAX_VALUE+(att_val-1)][0]=yes;
            att_ent_table[att*ATT_MAX_VALUE+(att_val-1)][1]=no;
            att_ent_table[att*ATT_MAX_VALUE+(att_val-1)][2]=calculate_entropy(yes,no);
        }
    }
    ///calculating entropy for the whole example data set
    yes=0;
    no=0;
    for(int i=0;i<r;i++){
        if(examples[i][c-1]==1) yes++;
        else no++;
    }
    att_ent_table[(c-1)*ATT_MAX_VALUE][0]=yes;
    att_ent_table[(c-1)*ATT_MAX_VALUE][1]=no;
    att_ent_table[(c-1)*ATT_MAX_VALUE][2]=calculate_entropy(yes,no);

/*
    ///show att_ent_table
    for(int i=0;i<(c-1)*ATT_MAX_VALUE+1;i++){
        cout<<i<<" "<<att_ent_table[i][0]<<" "<<att_ent_table[i][1]<<" "<<att_ent_table[i][2]<<endl;
    }
*/
    ///calculating maximum gain
    int att_no=UNDEFINED;
    double max_gain=UNDEFINED,gain;
    for(int i=0;i<c-1;i++){
        gain=att_ent_table[(c-1)*ATT_MAX_VALUE][2];                 ///calculating gain for attribute no i
        double s=r;

        for(int j=0;j<ATT_MAX_VALUE;j++){
            double sv=att_ent_table[i*ATT_MAX_VALUE+j][0]+att_ent_table[i*10+j][1];

            gain-=(sv/s)*att_ent_table[i*ATT_MAX_VALUE+j][2];
        }
///cout<<i<<" gain is "<<gain<<endl;
        if(gain>max_gain) {max_gain=gain;att_no=i;}
    }

    return att_no;    ///it returns the corresponding column value of the examples matrix
}

node Decision_Tree(int examples[ROW][COL],int r,int c,int att_list[COL-1],int att_list_size,int id){

    node n;
    n.id=id;
    n.att_no=UNDEFINED;
    n.level=UNDEFINED;

    ///calculating total no of yes and no
    int yes=0,no=0;
    for(int i=0;i<r;i++){
        if(examples[i][c-1]==1) yes++;                  ///as the last column contains the target attribute
        else no++;
    }

///cout<<"yes: "<<yes<<" no: "<<no<<endl;

    if(no==0){
        n.att_no=LEAF;
        n.level=YES;                    ///if all examples are positive
    }
    else if(yes==0){
        n.att_no=LEAF;
        n.level=NO;                     ///if all examples are negative
    }
    else if(att_list_size==0) {
        n.att_no=LEAF;
        if(yes>=no) n.level=YES;
        else n.level=NO;
    }
    else{

        int best_col=find_best_att(examples,r,c,att_list,att_list_size);         ///find_best_att returns the column no corresponding to maximum information gain
        n.att_no=att_list[best_col];                                                ///the real attribute no corresponding to best_col
///cout<<"best attribute number and column value:"<<n.att_no<<" "<<best_col<<endl;

        for(int att_val=1;att_val<=ATT_MAX_VALUE;att_val++){
            int child_id=id*ATT_MAX_VALUE+att_val;                  ///example for root(0) child are 1 to 10
///cout<<"child id "<<child_id<<" for id "<<id<<endl;


            ///filtering subset of examples based on best_col att_val
            int new_examples[ROW][COL];
            int new_r=0;
            int new_c=c-1;


            for(int i=0;i<r;i++){
///cout<<"i= "<<i<<" examples[i][best_col]= "<<examples[i][best_col]<<" attribute_value= "<<att_val<<endl;
                if(examples[i][best_col]==att_val){
                    new_c=0;

                    for(int j=0;j<c;j++){
                        if(j!=best_col){
///cout<<i<<" "<<j<<endl;
                            new_examples[new_r][new_c]=examples[i][j];
                            new_c++;
                        }
                    }

                    new_r++;
                }
            }

            new_c=c-1;
/*
            ///showing the new examples
            for(int i=0;i<new_r;i++){
                for(int j=0;j<new_c;j++) cout<<new_examples[i][j]<<" ";
                cout<<endl;
            }
*/

            int new_att_list[COL];
            int new_att_list_size=0;                ///initialization with 1

            for(int i=0;i<att_list_size;i++){
                if(att_list[i]!=n.att_no) {new_att_list[new_att_list_size]=att_list[i];new_att_list_size++;}
            }
/*
            ///show the filtered attribute list
            for(int i=0;i<new_att_list_size;i++) cout<<new_att_list[i]<<" ";
            cout<<endl;
*/
            if(new_r==0){

                node n1;
                n1.id=child_id;
                n1.att_no=LEAF;
                if(yes>=no) n1.level=YES;
                else n1.level=NO;

                tree[child_id]=n1;
            }
            else{
                tree[child_id]=Decision_Tree(new_examples,new_r,c-1,new_att_list,c-2,child_id);
///cout<<child_id<<" "<<tree[child_id].att_no<<" "<<tree[child_id].level<<endl;
            }
        }

    }

    return n;
}

node Decision_stump(int examples[ROW][COL],int r,int c,int att_list[COL-1],int att_list_size,int id){

    node n;
    n.id=id;
    n.att_no=UNDEFINED;
    n.level=UNDEFINED;

    ///calculating total no of yes and no
    int yes=0,no=0;
    for(int i=0;i<r;i++){
        if(examples[i][c-1]==1) yes++;                  ///as the last column contains the target attribute
        else no++;
    }

///cout<<"yes: "<<yes<<" no: "<<no<<endl;

    if(no==0){
        n.att_no=LEAF;
        n.level=YES;                    ///if all examples are positive
    }
    else if(yes==0){
        n.att_no=LEAF;
        n.level=NO;                     ///if all examples are negative
    }
    else if(att_list_size==0) {
        n.att_no=LEAF;
        if(yes>=no) n.level=YES;
        else n.level=NO;
    }
    else{

        int best_col=find_best_att(examples,r,c,att_list,att_list_size);         ///find_best_att returns the column no corresponding to maximum information gain
        n.att_no=att_list[best_col];                                                ///the real attribute no corresponding to best_col
///cout<<"best attribute number and column value:"<<n.att_no<<" "<<best_col<<endl;

        for(int att_val=1;att_val<=ATT_MAX_VALUE;att_val++){
            int child_id=id*ATT_MAX_VALUE+att_val;                  ///example for root(0) child are 1 to 10
///cout<<"child id "<<child_id<<" for id "<<id<<endl;


            ///filtering subset of examples based on best_col att_val
            int new_examples[ROW][COL];
            int new_r=0;
            int new_c=c-1;


            for(int i=0;i<r;i++){
///cout<<"i= "<<i<<" examples[i][best_col]= "<<examples[i][best_col]<<" attribute_value= "<<att_val<<endl;
                if(examples[i][best_col]==att_val){
                    new_c=0;

                    for(int j=0;j<c;j++){
                        if(j!=best_col){
///cout<<i<<" "<<j<<endl;
                            new_examples[new_r][new_c]=examples[i][j];
                            new_c++;
                        }
                    }

                    new_r++;
                }
            }

            new_c=c-1;
/*
            ///showing the new examples
            for(int i=0;i<new_r;i++){
                for(int j=0;j<new_c;j++) cout<<new_examples[i][j]<<" ";
                cout<<endl;
            }
*/

            int new_att_list[COL];
            int new_att_list_size=0;                ///initialization with 1

            for(int i=0;i<att_list_size;i++){
                if(att_list[i]!=n.att_no) {new_att_list[new_att_list_size]=att_list[i];new_att_list_size++;}
            }
/*
            ///show the filtered attribute list
            for(int i=0;i<new_att_list_size;i++) cout<<new_att_list[i]<<" ";
            cout<<endl;
*/
            if(new_r==0){

                node n1;
                n1.id=child_id;
                n1.att_no=LEAF;
                if(yes>=no) n1.level=YES;
                else n1.level=NO;

                tree[child_id]=n1;
            }
            else{
                tree[child_id]=Decision_Tree(new_examples,new_r,c-1,new_att_list,0,child_id);
///cout<<child_id<<" "<<tree[child_id].att_no<<" "<<tree[child_id].level<<endl;
            }
        }

    }

    return n;
}

void supervised_learning(){
    read_input_data();
    ///show_input_data();

    srand(time(NULL));

    int train_row=ROW*80/100;
    int test_row=ROW-train_row;

    double total_recall=0;
    double total_precision=0;
    double total_accuracy=0;
    double total_f1score=0;

    ///dividing training and testing samples
    for(int i=0;i<100;i++){
        int training_data[ROW][COL];
        int testing_data[ROW][COL];
///cout<<"258"<<endl;
        ///---------------------preparing initial attribute list-----------------------------------------------------
        int att_list[COL-1];                            ///it does not contain the target attribute i.e class
        for(int j=0;j<COL-1;j++) att_list[j]=j;         /// 0 indicates attribute no 0 etc.

        int true_pos=0;
        int false_neg=0;
        int false_pos=0;
        int true_neg=0;

        double recall=0;
        double precision=0;
        double accuracy=0;
        double f1score=0;
///cout<<"271"<<endl;
        int rand_list[ROW]={0};
        for(int j=0;j<train_row;){
            int ran_no=rand()%ROW;
            ///cout<<ran_no<<" ";
            if(rand_list[ran_no]!=1){
                rand_list[ran_no]=1;
                j++;
            }
        }
///cout<<"280"<<endl;
        int trn_no=0;
        int tst_no=0;
        for(int j=0;j<ROW;j++){
            if(rand_list[j]==1){
                for(int k=0;k<COL;k++) training_data[trn_no][k]=input_data[j][k];
                trn_no++;
            }
            else{
                for(int k=0;k<COL;k++) testing_data[tst_no][k]=input_data[j][k];
                tst_no++;
            }
        }
///cout<<"before calling id3\n";
        tree[0]=Decision_Tree(training_data,train_row,COL,att_list,COL-1,0);
        ///tree[0]=Decision_stump(training_data,train_row,COL,att_list,COL-1,0);
        cout<<i<<"-th Training Completed"<<endl;

        for(int j=0;j<test_row;j++){
            int input[COL];
            for(int k=0;k<COL-1;k++) input[k]=testing_data[j][k];

            node n=tree[0];
            while(n.att_no!=LEAF){
                int child_address=n.id*ATT_MAX_VALUE+input[n.att_no];
                n=tree[child_address];
            }

            if(n.level==YES){
                //cout<<"YES"<<endl;
                if(testing_data[j][COL-1]==1) true_pos++;
                else false_pos++;
            }
            else if(n.level==NO){
             //       cout<<"NO"<<endl;
                if(testing_data[j][COL-1]==1) false_neg++;
                else true_neg++;
            }
        }

        recall=1.0*true_pos/(true_pos+false_neg);
        precision=1.0*true_pos/(true_pos+false_pos);
        accuracy=1.0*(true_pos+true_neg)/(true_pos+true_neg+false_neg+false_pos);
        f1score=2*precision*recall/(precision+recall);

        cout<<recall<<" "<<precision<<" "<<accuracy<<" "<<f1score<<endl;

        tree.clear();

        total_recall+=recall;
        total_precision+=precision;
        total_accuracy+=accuracy;
        total_f1score+=f1score;
    }
    cout<<"avg_recall = "<<total_recall/100<<" avg_precision = "<<total_precision/100<<" avg_accuracy = "<<total_accuracy/100<<" avg_f1score= "<<total_f1score/100<<endl;

}

void semi_supervised_learning(){
    read_input_data();

    srand(time(NULL));

    int train_row=ROW*80/100;               ///80% data for training
    int test_row=ROW-train_row;

    double total_recall=0;
    double total_precision=0;
    double total_accuracy=0;
    double total_f1score=0;

    for(int i=0;i<100;i++){
        int training_data[ROW][COL];
        int testing_data[ROW][COL];
        int leveled_data[ROW][COL];
        int unleveled_data[ROW][COL];
///cout<<"258"<<endl;
        ///---------------------preparing initial attribute list-----------------------------------------------------
        int att_list[COL-1];                            ///it does not contain the target attribute i.e class
        for(int j=0;j<COL-1;j++) att_list[j]=j;         /// 0 indicates attribute no 0 etc.

        int true_pos=0;
        int false_neg=0;
        int false_pos=0;
        int true_neg=0;

        double recall=0;
        double precision=0;
        double accuracy=0;
        double f1score=0;
///cout<<"271"<<endl;
        int rand_list[ROW]={0};
        for(int j=0;j<train_row;){
            int ran_no=rand()%ROW;
            ///cout<<ran_no<<" ";
            if(rand_list[ran_no]!=1){
                rand_list[ran_no]=1;
                j++;
            }
        }
///cout<<"280"<<endl;
        int trn_no=0;
        int tst_no=0;
        for(int j=0;j<ROW;j++){
            if(rand_list[j]==1){
                for(int k=0;k<COL;k++) training_data[trn_no][k]=input_data[j][k];
                trn_no++;
            }
            else{
                for(int k=0;k<COL;k++) testing_data[tst_no][k]=input_data[j][k];
                tst_no++;
            }
        }

        int leveled_row=train_row*60/100;           ///60% of training data for leveled row
        int unleveled_row=train_row-leveled_row;
        int percentage=unleveled_row*10/100;        ///taking 10% for semi supervised learning

        for(int j=0;j<train_row;j++){
            if(j<leveled_row){
                for(int k=0;k<COL;k++) leveled_data[j][k]=training_data[j][k];
            }
            else{
                for(int k=0;k<COL-1;k++) unleveled_data[j-leveled_row][k]=training_data[j][k];
            }
        }

        int pos=0;
        while(leveled_row!=train_row){
            tree.clear();
            tree[0]=Decision_Tree(leveled_data,leveled_row,COL,att_list,COL-1,0);
            for(int j=0;j<min(unleveled_row,percentage);j++){
                int input[COL];
                for(int k=0;k<COL-1;k++) input[k]=unleveled_data[pos+j][k];

                node n=tree[0];
                while(n.att_no!=LEAF){
                    int child_address=n.id*ATT_MAX_VALUE+input[n.att_no];
                    n=tree[child_address];
                }

                if(n.level==YES){
                    //cout<<"YES"<<endl;
                    for(int k=0;k<COL-1;k++) leveled_data[leveled_row][k]=unleveled_data[pos+j][k];
                    leveled_data[leveled_row][COL-1]=1;
                    leveled_row++;
                }
                else if(n.level==NO){
                 //       cout<<"NO"<<endl;
                    for(int k=0;k<COL-1;k++) leveled_data[leveled_row][k]=unleveled_data[pos+j][k];
                    leveled_data[leveled_row][COL-1]=0;
                    leveled_row++;
                }
            }
///cout<<"434"<<endl;
            pos+=min(percentage,unleveled_row);
            unleveled_row-=min(percentage,unleveled_row);
        }
///cout<<"438"<<endl;

        tree.clear();
        tree[0]=Decision_Tree(leveled_data,leveled_row,COL,att_list,COL-1,0);
        cout<<i<<"-th Training Completed"<<endl;

        for(int j=0;j<test_row;j++){
            int input[COL];
            for(int k=0;k<COL-1;k++) input[k]=testing_data[j][k];

            node n=tree[0];
            while(n.att_no!=LEAF){
                int child_address=n.id*ATT_MAX_VALUE+input[n.att_no];
                n=tree[child_address];
            }

            if(n.level==YES){
                //cout<<"YES"<<endl;
                if(testing_data[j][COL-1]==1) true_pos++;
                else false_pos++;
            }
            else if(n.level==NO){
             //       cout<<"NO"<<endl;
                if(testing_data[j][COL-1]==1) false_neg++;
                else true_neg++;
            }
        }

        recall=1.0*true_pos/(true_pos+false_neg);
        precision=1.0*true_pos/(true_pos+false_pos);
        accuracy=1.0*(true_pos+true_neg)/(true_pos+true_neg+false_neg+false_pos);
        f1score=2*precision*recall/(precision+recall);

        cout<<recall<<" "<<precision<<" "<<accuracy<<" "<<f1score<<endl;

        tree.clear();

        total_recall+=recall;
        total_precision+=precision;
        total_accuracy+=accuracy;
        total_f1score+=f1score;
    }
    cout<<"avg_recall = "<<total_recall/100<<" avg_precision = "<<total_precision/100<<" avg_accuracy = "<<total_accuracy/100<<" avg_f1score= "<<total_f1score/100<<endl;
}

void k_fold_cross_validation(int subsets_no){
    read_input_data();

    srand(time(NULL));

    double total_recall=0;
    double total_precision=0;
    double total_accuracy=0;
    double total_f1score=0;

    int block_size=ROW/subsets_no;                  ///each block total no of data

    for(int i=0;i<subsets_no;i++){
        int training_data[ROW][COL];
        int testing_data[ROW][COL];
///cout<<"258"<<endl;
        ///---------------------preparing initial attribute list-----------------------------------------------------
        int att_list[COL-1];                            ///it does not contain the target attribute i.e class
        for(int j=0;j<COL-1;j++) att_list[j]=j;         /// 0 indicates attribute no 0 etc.

        int true_pos=0;
        int false_neg=0;
        int false_pos=0;
        int true_neg=0;

        double recall=0;
        double precision=0;
        double accuracy=0;
        double f1score=0;

        int trn_no=0;
        int tst_no=0;
        for(int j=0;j<subsets_no*block_size;j++){
            if(j>=i*block_size && j<(i+1)*block_size){
                for(int k=0;k<COL;k++) testing_data[tst_no][k]=input_data[j][k];
                tst_no++;
            }
            else{
                for(int k=0;k<COL;k++) training_data[trn_no][k]=input_data[j][k];
                trn_no++;
            }
        }

        tree[0]=Decision_Tree(training_data,trn_no,COL,att_list,COL-1,0);
        cout<<i<<"-th block output: "<<endl;

        for(int j=0;j<tst_no;j++){
            int input[COL];
            for(int k=0;k<COL-1;k++) input[k]=testing_data[j][k];

            node n=tree[0];
            while(n.att_no!=LEAF){
                int child_address=n.id*ATT_MAX_VALUE+input[n.att_no];
                n=tree[child_address];
            }

            if(n.level==YES){
                //cout<<"YES"<<endl;
                if(testing_data[j][COL-1]==1) true_pos++;
                else false_pos++;
            }
            else if(n.level==NO){
             //       cout<<"NO"<<endl;
                if(testing_data[j][COL-1]==1) false_neg++;
                else true_neg++;
            }
        }

        recall=1.0*true_pos/(true_pos+false_neg);
        precision=1.0*true_pos/(true_pos+false_pos);
        accuracy=1.0*(true_pos+true_neg)/(true_pos+true_neg+false_neg+false_pos);
        f1score=2*precision*recall/(precision+recall);

        cout<<recall<<" "<<precision<<" "<<accuracy<<" "<<f1score<<endl;

        tree.clear();

        total_recall+=recall;
        total_precision+=precision;
        total_accuracy+=accuracy;
        total_f1score+=f1score;
    }
    cout<<"avg_recall = "<<total_recall/subsets_no<<" avg_precision = "<<total_precision/subsets_no<<" avg_accuracy = "<<total_accuracy/subsets_no<<" avg_f1score= "<<total_f1score/subsets_no<<endl;
}

void leave_one_out_cross_validation(){
    read_input_data();

    srand(time(NULL));

    double total_recall=0;
    double total_precision=0;
    double total_accuracy=0;
    double total_f1score=0;

    for(int i=0;i<ROW;i++){
        int training_data[ROW][COL];
        int testing_data[ROW][COL];
///cout<<"258"<<endl;
        ///---------------------preparing initial attribute list-----------------------------------------------------
        int att_list[COL-1];                            ///it does not contain the target attribute i.e class
        for(int j=0;j<COL-1;j++) att_list[j]=j;         /// 0 indicates attribute no 0 etc.

        int true_pos=0;
        int false_neg=0;
        int false_pos=0;
        int true_neg=0;

        double recall=0;
        double precision=0;
        double accuracy=0;
        double f1score=0;

        int trn_no=0;
        int tst_no=0;
        for(int j=0;j<ROW;j++){
            if(j==i){
                for(int k=0;k<COL;k++) testing_data[tst_no][k]=input_data[j][k];
                tst_no++;
            }
            else{
                for(int k=0;k<COL;k++) training_data[trn_no][k]=input_data[j][k];
                trn_no++;
            }
        }

        tree[0]=Decision_Tree(training_data,trn_no,COL,att_list,COL-1,0);
        cout<<i<<"-th block output: "<<endl;

        for(int j=0;j<tst_no;j++){
            int input[COL];
            for(int k=0;k<COL-1;k++) input[k]=testing_data[j][k];

            node n=tree[0];
            while(n.att_no!=LEAF){
                int child_address=n.id*ATT_MAX_VALUE+input[n.att_no];
                n=tree[child_address];
            }

            if(n.level==YES){
                //cout<<"YES"<<endl;
                if(testing_data[j][COL-1]==1) true_pos++;
                else false_pos++;
            }
            else if(n.level==NO){
             //       cout<<"NO"<<endl;
                if(testing_data[j][COL-1]==1) false_neg++;
                else true_neg++;
            }
        }

        ///recall=1.0*true_pos/(true_pos+false_neg);
        ///precision=1.0*true_pos/(true_pos+false_pos);
        accuracy=1.0*(true_pos+true_neg)/(true_pos+true_neg+false_neg+false_pos);
        ///f1score=2*precision*recall/(precision+recall);

        cout<<recall<<" "<<precision<<" "<<accuracy<<" "<<f1score<<endl;

        tree.clear();

        ///total_recall+=recall;
        ///total_precision+=precision;
        total_accuracy+=accuracy;
        ///total_f1score+=f1score;
    }
    ///cout<<"avg_recall = "<<total_recall/ROW<<" avg_precision = "<<total_precision/ROW<<" avg_accuracy = "<<total_accuracy/ROW<<" avg_f1score= "<<total_f1score/ROW<<endl;
    cout<<" avg_accuracy = "<<total_accuracy/ROW<<endl;
}

void ensemble_learning_adaboost(int k){
    read_input_data();

    srand(time(NULL));

    int train_row=ROW*80/100;               ///80% as training data
    int test_row=ROW-train_row;

    double total_recall=0;
    double total_precision=0;
    double total_accuracy=0;
    double total_f1score=0;

    ///dividing training and testing samples
    for(int iter=0;iter<100;iter++){
        int training_data[ROW][COL];
        int testing_data[ROW][COL];
///cout<<"258"<<endl;
        ///---------------------preparing initial attribute list-----------------------------------------------------
        int true_pos=0;
        int false_neg=0;
        int false_pos=0;
        int true_neg=0;

        double recall=0;
        double precision=0;
        double accuracy=0;
        double f1score=0;
///cout<<"271"<<endl;
        int rand_list[ROW]={0};
        for(int j=0;j<train_row;){
            int ran_no=rand()%ROW;
            ///cout<<ran_no<<" ";
            if(rand_list[ran_no]!=1){
                rand_list[ran_no]=1;
                j++;
            }
        }
///cout<<"280"<<endl;
        int trn_no=0;
        int tst_no=0;
        for(int j=0;j<ROW;j++){
            if(rand_list[j]==1){
                for(int k=0;k<COL;k++) training_data[trn_no][k]=input_data[j][k];
                trn_no++;
            }
            else{
                for(int k=0;k<COL;k++) testing_data[tst_no][k]=input_data[j][k];
                tst_no++;
            }
        }
        int m=train_row;
        double w[k+1][m],p[k+1][m],eps[k+1],beta[k+1];
        for(int i=0;i<m;i++) w[0][i]=1.0/m;
        for(int r=0;r<k;r++){
///cout<<r<<"-th iteration"<<endl;
            double sum=0;
            for(int i=0;i<m;i++) sum+=w[r][i];

            for(int i=0;i<m;i++) p[r][i]=w[r][i]/sum;

            int train_subset[ROW][COL];
            int train_subset_sz=0;
            while(train_subset_sz<SUBSET){
                double random_no=((double) rand() / (RAND_MAX));
                ///cout<<random_no<<endl;
                double tmp_sum=0;
                for(int it=0;it<m;it++){
                    tmp_sum+=p[r][it];
                    if(random_no<=tmp_sum){
                        for(int it1=0;it1<COL;it1++) train_subset[train_subset_sz][it1]=training_data[it][it1];
                        train_subset_sz++;
                        break;
                    }
                }
            }
            int att_list[COL-1];                            ///it does not contain the target attribute i.e class
            for(int j=0;j<COL-1;j++) att_list[j]=j;         /// 0 indicates attribute no 0 etc.
            tree.clear();
            tree[0]=Decision_stump(train_subset,train_subset_sz,COL,att_list,COL-1,0);
            tree_arr[r]=tree;
            eps[r]=0;
            int check[ROW]={0};
            for(int i=0;i<m;i++){
                int input[COL];
                for(int kt=0;kt<COL-1;kt++) input[kt]=training_data[i][kt];

                node n=tree[0];
                while(n.att_no!=LEAF){
                    ///cout<<n.id<<endl;
                    int child_address=n.id*ATT_MAX_VALUE+input[n.att_no];
                    n=tree[child_address];
///cout<<"while"<<endl;
                }

                if(n.level==YES){
                    //cout<<"YES"<<endl;
                    if(training_data[i][COL-1]!=1) {eps[r]+=p[r][i];check[i]=1;}
                }
                else if(n.level==NO){
                 //       cout<<"NO"<<endl;
                    if(training_data[i][COL-1]!=0) {eps[r]+=p[r][i];check[i]=1;}
                }
            }
///cout<<"eps value: "<<eps[r]<<endl;
            if(eps[r]>0.50){
                k=r-1;
                break;
            }
            beta[r]=eps[r]/(1.0-eps[r]);
///cout<<"beta value: "<<beta[r]<<endl;
            for(int i=0;i<m;i++){
                if(check[i]==0) w[r+1][i]=w[r][i]*beta[r];
                else w[r+1][i]=w[r][i];
            }
        }
///cout<<"training completed"<<endl;
        for(int i=0;i<test_row;i++){
            int input[COL];
            for(int kt=0;kt<COL-1;kt++) input[kt]=testing_data[i][kt];

            double sum1=0,sum2=0;
            for(int it=0;it<k;it++){
                map<int,node> tmp;
                tmp.insert(tree_arr[it].begin(),tree_arr[it].end());
                node n=tmp[0];
                while(n.att_no!=LEAF){
                    int child_address=n.id*ATT_MAX_VALUE+input[n.att_no];
                    n=tmp[child_address];
                }
///cout<<beta[it]<<endl;
                if(n.level==YES){
                    sum1+=log(1.0/beta[it]);
                }
                else if(n.level==NO){
                    sum2+=log(1.0/beta[it]);
                }

            }
///cout<<sum1<<" "<<sum2<<endl;
            if(sum1>=sum2){          ///yes predicted
                if(testing_data[i][COL-1]==1) true_pos++;
                else false_pos++;
            }
            else{
                if(testing_data[i][COL-1]==1) false_neg++;
                else true_neg++;
            }
        }
///cout<<true_neg<<" "<<true_pos<<" "<<false_neg<<" "<<false_pos<<endl;
        ///recall=1.0*true_pos/(true_pos+false_neg);
        ///precision=1.0*true_pos/(true_pos+false_pos);
        accuracy=1.0*(true_pos+true_neg)/(true_pos+true_neg+false_neg+false_pos);
        ///f1score=2*precision*recall/(precision+recall);

        ///cout<<recall<<" "<<precision<<" "<<accuracy<<" "<<f1score<<endl;
cout<<accuracy<<endl;
        for(int i=0;i<k;i++) tree_arr[i].clear();

        total_recall+=recall;
        total_precision+=precision;
        total_accuracy+=accuracy;
        total_f1score+=f1score;
    }
    cout<<"avg_recall = "<<total_recall/100<<" avg_precision = "<<total_precision/100<<" avg_accuracy = "<<total_accuracy/100<<" avg_f1score= "<<total_f1score/100<<endl;

}

int main()
{
    ///supervised_learning();
    ///semi_supervised_learning();
    ///k_fold_cross_validation(20);
    ///leave_one_out_cross_validation();
    ///ensemble_learning_adaboost(30);
    return 0;
}
