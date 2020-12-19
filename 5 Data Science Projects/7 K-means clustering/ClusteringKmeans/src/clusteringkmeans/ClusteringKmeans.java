/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package clusteringkmeans;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;
import javax.swing.JPanel;

/**
 *
 * @author Imam
 */

class Cluster{
    public int c_size;
    public double points[][]=new double[1000][2];
    public double cent_x,cent_y;
    
    Cluster(){
        c_size=0;
    }
    
    double error(){
        double err=0;
        for(int i=0;i<c_size;i++){
            err+=(cent_x-points[i][0])*(cent_x-points[i][0])+(cent_y-points[i][1])*(cent_y-points[i][1]);
        }
        return err;
    }
    
    void update_center(){
        double x_sum=0,y_sum=0;
        for(int i=0;i<c_size;i++){
            x_sum+=points[i][0];
            y_sum+=points[i][1];
        }
        cent_x=x_sum/c_size;
        cent_y=y_sum/c_size;
    }
}

public class ClusteringKmeans extends JPanel{

    /**
     * @param args the command line arguments
     */
    
    ClusteringKmeans(){
        
    }
    
    public int total_points;                               ///total no of input points
    public double points[][]=new double[1000][2];          ///all the points are stored into this array
    
    void readPoints(String file){
        try {
            BufferedReader br=new BufferedReader(new FileReader(file));
            total_points=0;
            String s;
            while((s=br.readLine())!=null){
                Scanner sc=new Scanner(s);
                points[total_points][0]=sc.nextDouble();
                points[total_points][1]=sc.nextDouble();
                total_points++;
                
                ///System.out.println(points[total_points][0]);
            }
            
        } catch (FileNotFoundException ex) {
            ///Logger.getLogger(ClusteringKmeans.class.getName()).log(Level.SEVERE, null, ex);
            System.out.println("Can't read file ");
        } catch (IOException ex) {
            ///Logger.getLogger(ClusteringKmeans.class.getName()).log(Level.SEVERE, null, ex);
            System.out.println("IO exception occured");
        }
    }
    
    ArrayList k_means(int k,double cent[][],double cluster_points[][],int num_points){
        ///forming the arraylist and initializing with the initial centeroids
        ArrayList c_list=new ArrayList();
        for(int i=0;i<k;i++){
            Cluster c=new Cluster();
            c.cent_x=cent[i][0];
            c.cent_y=cent[i][1];
            c_list.add(c);
        }
        ///System.out.println(num_points);
        ///assigning points to nearest cluster
        int iter=0;
        while(true){
            ///clearing the cluster points for checking the new points
            for(int i=0;i<k;i++){
                Cluster c=(Cluster) c_list.get(i);
                c.c_size=0;
            }
            
            for(int i=0;i<num_points;i++){
                ///System.out.println(i);
                int track_cluster=-1;
                double min_dist=Double.POSITIVE_INFINITY;
                for(int j=0;j<k;j++){
                    Cluster c=(Cluster) c_list.get(j);
                    double dist=(c.cent_x-cluster_points[i][0])*(c.cent_x-cluster_points[i][0])+(c.cent_y-cluster_points[i][1])*(c.cent_y-cluster_points[i][1]);
                    if(dist<min_dist){
                        min_dist=dist;
                        track_cluster=j;
                    }
                }
                ///System.out.println(i+" "+track_cluster);
                Cluster c=(Cluster) c_list.get(track_cluster);
                ///System.out.println(c.c_size);
                c.points[c.c_size][0]=cluster_points[i][0];
                c.points[c.c_size][1]=cluster_points[i][1];
                c.c_size+=1;
            }
            ///recomputing the center
            int cnt=0;
            for(int j=0;j<k;j++){
                Cluster c=(Cluster) c_list.get(j);
                double old_x=c.cent_x,old_y=c.cent_y;
                c.update_center();
                if(Math.abs(c.cent_x-old_x)<0.0000000000001 && Math.abs(c.cent_y-old_y)<0.00000000000001) cnt++;
            }
            if(cnt==k) break;
            
            iter++;
            if(iter==1000) break;
        }
        
        return c_list;
    }
    
    int select_cluster(ArrayList a){                ///selecting cluster with the highest error
        double max_dist=Double.NEGATIVE_INFINITY;
        int cell_no=-1;
        for(int i=0;i<a.size();i++){
            Cluster c=(Cluster) a.get(i);
            double tmp_dist=c.error();
            /*
            for(int j=0;j<c.c_size;j++){
                tmp_dist+=(c.cent_x-c.points[j][0])*(c.cent_x-c.points[j][0])+(c.cent_y-c.points[j][1])*(c.cent_y-c.points[j][1]);
            }
            */
            if(tmp_dist>max_dist){
                max_dist=tmp_dist;
                cell_no=i;
            }
        }
        return cell_no;
    }
    
    public ArrayList final_list;
    
    void bisecting_algo(int k){
        ArrayList list=new ArrayList();
        ///initializing the cluster list
        Cluster c=new Cluster();
        c.c_size=total_points;
        for(int i=0;i<total_points;i++){
            c.points[i][0]=points[i][0];
            c.points[i][1]=points[i][1];
        }
        c.update_center();
        ///System.out.println(c.cent_x+" "+c.cent_y);
        list.add(c);
        
        ///starting the repetition steps
        while(list.size()!=k){
            ///select a cluster from the list of clusters and remove it from the list
            int index=select_cluster(list);
            Cluster tmp=(Cluster) list.get(index);
            list.remove(index);
            
            ///System.out.println(tmp.c_size+" "+tmp.cent_x+" "+tmp.cent_y);
            
            ///iterate fixed no of times for the bisecting 2 clusters
            Cluster c1=new Cluster();
            Cluster c2=new Cluster();
            
            int no_of_iter=10;
            
            double lowest_sse=Double.POSITIVE_INFINITY;                     ///finding the bisection pair with the lowest sse
            for(int i=0;i<no_of_iter;i++){
                Random rand=new Random();
                int rand_index1=rand.nextInt(tmp.c_size);
                int rand_index2=rand.nextInt(tmp.c_size);
                ///System.out.println(rand_index1+" "+rand_index2);
                double center[][]=new double[1000][2];
                center[0][0]=tmp.points[rand_index1][0];center[0][1]=tmp.points[rand_index1][1];
                center[1][0]=tmp.points[rand_index2][0];center[1][1]=tmp.points[rand_index2][1];
                
                ///System.out.println(tmp.c_size);
                ArrayList cluster_pair=k_means(2,center,tmp.points,tmp.c_size);
                
                double sse=0;
                sse+=((Cluster)cluster_pair.get(0)).error();
                sse+=((Cluster)cluster_pair.get(1)).error();
                
                if(sse<lowest_sse){
                    lowest_sse=sse;
                    c1=(Cluster)cluster_pair.get(0);
                    c2=(Cluster)cluster_pair.get(1);
                }
            }
            list.add(c1);
            list.add(c2);
            
        }
        System.out.println("Bisection Completed");
        double final_center[][]=new double[1000][2];
        for(int i=0;i<k;i++){
            Cluster tmp=(Cluster) list.get(i);
            final_center[i][0]=tmp.cent_x;
            final_center[i][1]=tmp.cent_y;
        }
        final_list=k_means(k,final_center,points,total_points);
        for(int i=0;i<k;i++){
            Cluster tmp=(Cluster) final_list.get(i);
            System.out.println(tmp.cent_x+" "+tmp.cent_y);
        }
    }
   /* 
    protected void paintComponent(Graphics g){
        super.paintComponent(g);
        Graphics2D g2=(Graphics2D)g;
        
        ///g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
        ///int w = getWidth();
        ///int h = getHeight();
        // Draw ordinate.
        //g2.draw(new Line2D.Double(PAD, PAD, PAD, h-PAD));
        // Draw abcissa.
        //g2.draw(new Line2D.Double(PAD, h-PAD, w-PAD, h-PAD));
        // Mark data points.
        g2.setPaint(Color.red);
        System.out.println(total_points);
        
        for(int i = 0; i < total_points; i++) {
            double x = points[i][0];
            double y = points[i][1];
            g2.drawRect((int)x*20, (int)y*10, 5, 5);
        }
    }
    */
    public static void main(String[] args) {
        // TODO code application logic here
        
        ClusteringKmeans ob=new ClusteringKmeans();
        ob.readPoints("bisecting.txt");
        ob.bisecting_algo(12);
       /*
        JFrame f=new JFrame();
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.add(ob);
        f.setSize(1000,800);
        f.setLocation(200,200);
        f.setVisible(true);
               */
        GuiPrint gui=new GuiPrint(ob);
        
    }
    
}
