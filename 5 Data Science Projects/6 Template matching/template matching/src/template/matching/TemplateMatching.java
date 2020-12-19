/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package template.matching;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;


/**
 *
 * @author Imam
 */
public class TemplateMatching {

    /**
     * @param args the command line arguments
     */
    int ref1h,ref1w,ref2h,ref2w,testh,testw;
    double ref1g[][]=new double[2000][2000];
    double ref2g[][]=new double[2000][2000];
    double testg[][]=new double[2000][2000];
    double c[][]=new double[2000][2000];

    double tlevel1[][]=new double[512][512];
    double tlevel2[][]=new double[256][256];
    double ref1level1[][]=new double[200][200];
    double ref1level2[][]=new double[200][200];
    double ref2level1[][]=new double[200][200];
    double ref2level2[][]=new double[200][200];
    
    public void read_images(){
        try {
            // TODO code application logic here

            BufferedImage ref1=ImageIO.read(new File("ref1.bmp"));
            ref1h=ref1.getHeight();
            ref1w=ref1.getWidth();
            System.out.println(ref1w+" "+ref1h);
            
            for(int i=0;i<ref1w;i++){
                for(int j=0;j<ref1h;j++){
                    Color clr=new Color(ref1.getRGB(i,j));
                    
                    ref1g[i][j]=(clr.getRed()+clr.getGreen()+clr.getBlue())/(3*255);
                }
            }
            
            BufferedImage ref2=ImageIO.read(new File("ref2.bmp"));
            ref2h=ref2.getHeight();
            ref2w=ref2.getWidth();
            System.out.println(ref2w+" "+ref2h);
            
            for(int i=0;i<ref2w;i++){
                for(int j=0;j<ref2h;j++){
                    Color clr=new Color(ref2.getRGB(i,j));
                    
                    ref2g[i][j]=(clr.getRed()+clr.getGreen()+clr.getBlue())/(3*255);
                }
            }
         
            BufferedImage test=ImageIO.read(new File("test.bmp"));
            testh=test.getHeight();
            testw=test.getWidth();
            System.out.println(testw+" "+testh);
            
            for(int i=0;i<testw;i++){
                for(int j=0;j<testh;j++){
                    Color clr=new Color(test.getRGB(i,j));
                    
                    testg[i][j]=(clr.getRed()+clr.getGreen()+clr.getBlue())/(3*255);
                }
            }
        } catch (IOException ex) {
            ///Logger.getLogger(TemplateMatching.class.getName()).log(Level.SEVERE, null, ex);
            System.out.println("Can't open the image file");
        } 
        
    }
    
    public void exhaustive_search1(){
        int minw=-1,minh=-1;
        double minval=Double.POSITIVE_INFINITY;
        double tmp;
        for(int m=0;m<testw-ref1w+1;m++){
            for(int n=0;n<testh-ref1h+1;n++){
                tmp=0;
                for(int i=m;i<m+ref1w;i++){
                    for(int j=n;j<n+ref1h;j++){
                       tmp+=Math.abs(testg[i][j]-ref1g[i-m][j-n]);
                    }
                }
                if(tmp<minval){
                    minw=m;
                    minh=n;
                    minval=tmp;
                }
            }
        }

        System.out.println(minw+" "+minh+" "+minval);
    }
    
    public void exhaustive_search2(){
        int minw2=-1,minh2=-1;
        double minval2=Double.POSITIVE_INFINITY;
        double tmp;
        for(int m=0;m<testw-ref2w+1;m++){
            for(int n=0;n<testh-ref2h+1;n++){
                tmp=0;
                for(int i=m;i<m+ref2w;i++){
                    for(int j=n;j<n+ref2h;j++){
                        tmp+=Math.abs(testg[i][j]-ref2g[i-m][j-n]);
                    }
                }
                if(tmp<minval2){
                    minw2=m;
                    minh2=n;
                    minval2=tmp;
                }
            }
        }
        System.out.println(minw2+" "+minh2+" "+minval2);
    }
    
    public void log_2d_search1(){
        ///performing 2d logarithmic approach
            int p=Math.min(testh,testw)/2;
            int k=(int)Math.ceil(Math.log10(p)/Math.log10(2));
            int d=(int) Math.pow(2, k-1);
            int cx=(testw/2);
            int cy=(testh/2);
            
            double minval=Double.POSITIVE_INFINITY;
            int maxw=-1,maxh=-1;
            double tmp=0;
    
            while(d>=1){
                int pos_x[]={-1,0,+1,-1,0,+1,-1,0,+1};
                int pos_y[]={-1,-1,-1,0,0,0,+1,+1,+1};
                ///System.out.println(d+" "+cx+" "+cy);
                
                for(int it=0;it<9;it++){
                    int tmp_cx=cx+pos_x[it]*d;
                    int tmp_cy=cy+pos_y[it]*d;
                    
                    int m=tmp_cx-(ref1w/2);
                    int n=tmp_cy-(ref1h/2);

                    tmp=0;
                    for(int i=m;i<m+ref1w;i++){
                        for(int j=n;j<n+ref1h;j++){
                            tmp+=Math.abs(testg[i][j]-ref1g[i-m][j-n]);
                        }
                    }
                    if(tmp<=minval){
                        maxw=tmp_cx;
                        maxh=tmp_cy;
                        minval=tmp;
                    }
                }
                
                cx=maxw;
                cy=maxh;
                
                p=p/2;
                k=(int)Math.ceil(Math.log10(p)/Math.log10(2));
                d=(int) Math.pow(2, k-1);
                
                if(minval<0.0000000000000000000001) break;
            }
            System.out.println((cx-(ref1w/2))+" "+(cy-(ref1h/2)));
    }
    
    public void log_2d_search2(){
        int p=Math.min(testh,testw)/2;
        int k=(int)Math.ceil(Math.log10(p)/Math.log10(2));
        int d=(int) Math.pow(2, k-1);
        int cx=(testw/2);
        int cy=(testh/2);
        
        
        
        while(d>=1){
            double minval=Double.POSITIVE_INFINITY;
            int maxw=-1,maxh=-1;
            double tmp;
            
            int pos_x[]={-1,0,+1,-1,0,+1,-1,0,+1};
            int pos_y[]={-1,-1,-1,0,0,0,+1,+1,+1};

            for(int it=0;it<9;it++){
                int tmp_cx=cx+pos_x[it]*d;
                int tmp_cy=cy+pos_y[it]*d;

                int m=tmp_cx-(ref2w/2);
                int n=tmp_cy-(ref2h/2);

                tmp=0;
                for(int i=m;i<m+ref2w;i++){
                    for(int j=n;j<n+ref2h;j++){
                        tmp+=Math.abs(testg[i][j]-ref2g[i-m][j-n]);
                    }
                }
                if(tmp<=minval){
                    maxw=tmp_cx;
                    maxh=tmp_cy;
                    minval=tmp;
                }
            }
            ///System.out.println(d+" "+cx+" "+cy+" "+minval);
            
            cx=maxw;
            cy=maxh;

            p=p/2;
            k=(int)Math.ceil(Math.log10(p)/Math.log10(2));
            d=(int) Math.pow(2, k-1);

            if(minval<=0.00001) break;
        }
        System.out.println((cx-(ref2w/2))+" "+(cy-(ref2h/2)));
    }
    
    public void hierarchical_search1(){
        ///Hierarchical search
            ///Making blur at level 1
            for(int i=1;i+1<testw;i+=2){
                for(int j=1;j+1<testh;j+=2){
                    tlevel1[i/2][j/2]=(testg[i][j]+testg[i-1][j-1]+testg[i+1][j-1]+testg[i+1][j+1]+testg[i-1][j+1]+testg[i][j-1]+testg[i][j+1]+testg[i-1][j]+testg[i+1][j])/9.0;
                }
            }
            for(int i=1;i+1<ref1w;i+=2){
                for(int j=1;j+1<ref1h;j+=2){
                    ref1level1[i/2][j/2]=(ref1g[i][j]+ref1g[i-1][j-1]+ref1g[i+1][j-1]+ref1g[i+1][j+1]+ref1g[i-1][j+1]+ref1g[i][j-1]+ref1g[i][j+1]+ref1g[i-1][j]+ref1g[i+1][j])/9.0;
                }
            }
            
            
            ///making blur at level2
            for(int i=1;i+1<testw/2;i+=2){
                for(int j=1;j+1<testh/2;j+=2){
                    tlevel2[i/2][j/2]=(tlevel1[i][j]+tlevel1[i-1][j-1]+tlevel1[i+1][j-1]+tlevel1[i+1][j+1]+tlevel1[i-1][j+1]+tlevel1[i][j-1]+tlevel1[i][j+1]+tlevel1[i-1][j]+tlevel1[i+1][j])/9.0;
                }
            }
            for(int i=1;i+1<ref1w/2;i+=2){
                for(int j=1;j+1<ref1h/2;j+=2){
                    ref1level2[i/2][j/2]=(ref1level1[i][j]+ref1level1[i-1][j-1]+ref1level1[i+1][j-1]+ref1level1[i+1][j+1]+ref1level1[i-1][j+1]+ref1level1[i][j-1]+ref1level1[i][j+1]+ref1level1[i-1][j]+ref1level1[i+1][j])/9.0;
                }
            }
            
            ///iteration in level 2
            int x,y,u2=-1,v2=-1;
            double minval=Double.POSITIVE_INFINITY;
            for(int m=0;m<(testw/4)-(ref1w/4)+1;m++){
                for(int n=0;n<(testh/4)-(ref1h/4)+1;n++){
                    double tmp=0;
                    for(int i=m;i<m+ref1w/4;i++){
                        for(int j=n;j<n+ref1h/4;j++){
                            
                            tmp+=Math.abs(tlevel2[i][j]-ref1level2[i-m][j-n]);
                            
                        }
                    }
                    if(tmp<=minval){
                        u2=m;
                        v2=n;
                        minval=tmp;
                    }
                
                }
            }
            System.out.println(u2+" "+v2);
            ///calculating center
            
            
            int xb4=testw/8;
            int yb4=testh/8;
            
            int x1=u2+(ref1w/8)-xb4;        
            int y1=v2+(ref1h/8)-yb4;
            
            int xb2=testw/4;
            int yb2=testh/4;
            
            //x=2*(x+u2);
            //y=2*(y+v2);
            x=xb2+2*x1;
            y=yb2+2*y1;
            
            ///System.out.println(xb4+" "+yb4+" "+x1+" "+y1+" "+xb2+" "+yb2+" "+x+" "+y);
            
            ///x=2*u2;
            ///y=2*v2;
            int pos_x[]={-1,0,+1,-1,0,+1,-1,0,+1};
            int pos_y[]={-1,-1,-1,0,0,0,+1,+1,+1};
            
            int u1=-1,v1=-1;
            double minval2=Double.POSITIVE_INFINITY;
            for(int it=0;it<9;it++){
                
                int tmp_x=x+pos_x[it];
                int tmp_y=y+pos_y[it];
                
                int m=tmp_x-ref1w/4;
                int n=tmp_y-ref1h/4;
                
                ///int m=x+pos_x[it];
                ///int n=y+pos_y[it];
                
                double tmp=0;
                for(int i=m;i<ref1w/2;i++){
                    for(int j=n;j<ref1h/2;j++){
                        tmp+=Math.abs(tlevel1[i][j]-ref1level1[i-m][j-n]);
                    }
                }
                if(tmp<minval2){
                    minval2=tmp;
                    u1=m;
                    v1=n;
                }
                
            }
            System.out.println(u1+" "+v1);
            
            int x2=u1+(ref1w/4)-xb2;
            int y2=v1+(ref1h/4)-yb2;
            
            int xb1=testw/2;
            int yb1=testh/2;
            
            x=xb1+2*x2;
            y=yb1+2*y2;
            
            ///x=2*u1;
            ///y=2*v1;
            
            int u=-1,v=-1;
            minval2=Double.POSITIVE_INFINITY;
            for(int it=0;it<9;it++){
                
                int tmp_x=x+pos_x[it];
                int tmp_y=y+pos_y[it];
                
                int m=tmp_x-ref1w/2;
                int n=tmp_y-ref1h/2;
                
                ///int m=x+pos_x[it];
                ///int n=y+pos_y[it];
                double tmp=0;
                for(int i=m;i<ref1w;i++){
                    for(int j=n;j<ref1h;j++){
                        tmp+=Math.abs(testg[i][j]-ref1g[i-m][j-n]);
                    }
                }
                if(tmp<minval2){
                    minval2=tmp;
                    u=m;
                    v=n;
                }
                
            }
            System.out.println(u+" "+v);
    }
    
    public void hierarchical_search2(){
        for(int i=1;i+1<testw;i+=2){
                for(int j=1;j+1<testh;j+=2){
                    tlevel1[i/2][j/2]=(testg[i][j]+testg[i-1][j-1]+testg[i+1][j-1]+testg[i+1][j+1]+testg[i-1][j+1]+testg[i][j-1]+testg[i][j+1]+testg[i-1][j]+testg[i+1][j])/9.0;
                }
            }
            for(int i=1;i+1<ref2w;i+=2){
                for(int j=1;j+1<ref2h;j+=2){
                    ref2level1[i/2][j/2]=(ref2g[i][j]+ref2g[i-1][j-1]+ref2g[i+1][j-1]+ref2g[i+1][j+1]+ref2g[i-1][j+1]+ref2g[i][j-1]+ref2g[i][j+1]+ref2g[i-1][j]+ref2g[i+1][j])/9.0;
                }
            }
            
            
            ///making blur at level2
            for(int i=1;i+1<testw/2;i+=2){
                for(int j=1;j+1<testh/2;j+=2){
                    tlevel2[i/2][j/2]=(tlevel1[i][j]+tlevel1[i-1][j-1]+tlevel1[i+1][j-1]+tlevel1[i+1][j+1]+tlevel1[i-1][j+1]+tlevel1[i][j-1]+tlevel1[i][j+1]+tlevel1[i-1][j]+tlevel1[i+1][j])/9.0;
                }
            }
            for(int i=1;i+1<ref2w/2;i+=2){
                for(int j=1;j+1<ref2h/2;j+=2){
                    ref2level2[i/2][j/2]=(ref2level1[i][j]+ref2level1[i-1][j-1]+ref2level1[i+1][j-1]+ref2level1[i+1][j+1]+ref2level1[i-1][j+1]+ref2level1[i][j-1]+ref2level1[i][j+1]+ref2level1[i-1][j]+ref2level1[i+1][j])/9.0;
                }
            }
            
            ///iteration in level 2
            int x,y,u2=-1,v2=-1;
            double minval=Double.POSITIVE_INFINITY;
            for(int m=0;m<(testw/4)-(ref1w/4)+1;m++){
                for(int n=0;n<(testh/4)-(ref1h/4)+1;n++){
                    double tmp=0;
                    for(int i=m;i<m+ref1w/4;i++){
                        for(int j=n;j<n+ref1h/4;j++){
                            
                            tmp+=Math.abs(tlevel2[i][j]-ref2level2[i-m][j-n]);
                            
                        }
                    }
                    if(tmp<=minval){
                        u2=m;
                        v2=n;
                        minval=tmp;
                    }
                
                }
            }
            System.out.println(u2+" "+v2);
            ///calculating center
            
            
            int xb4=testw/8;
            int yb4=testh/8;
            
            int x1=u2+(ref2w/8)-xb4;        
            int y1=v2+(ref2h/8)-yb4;
            
            int xb2=testw/4;
            int yb2=testh/4;
            
            //x=2*(x+u2);
            //y=2*(y+v2);
            x=xb2+2*x1;
            y=yb2+2*y1;
            
            ///System.out.println(xb4+" "+yb4+" "+x1+" "+y1+" "+xb2+" "+yb2+" "+x+" "+y);
            
            ///x=2*u2;
            ///y=2*v2;
            int pos_x[]={-1,0,+1,-1,0,+1,-1,0,+1};
            int pos_y[]={-1,-1,-1,0,0,0,+1,+1,+1};
            
            int u1=-1,v1=-1;
            double minval2=Double.POSITIVE_INFINITY;
            for(int it=0;it<9;it++){
                
                int tmp_x=x+pos_x[it];
                int tmp_y=y+pos_y[it];
                
                int m=tmp_x-ref2w/4;
                int n=tmp_y-ref2h/4;
                
                ///int m=x+pos_x[it];
                ///int n=y+pos_y[it];
                
                double tmp=0;
                for(int i=m;i<ref2w/2;i++){
                    for(int j=n;j<ref2h/2;j++){
                        tmp+=Math.abs(tlevel1[i][j]-ref2level1[i-m][j-n]);
                    }
                }
                if(tmp<=minval2){
                    minval2=tmp;
                    u1=m;
                    v1=n;
                }
                
            }
            System.out.println(u1+" "+v1);
            
            int x2=u1+(ref2w/4)-xb2;
            int y2=v1+(ref2h/4)-yb2;
            
            int xb1=testw/2;
            int yb1=testh/2;
            
            x=xb1+2*x2;
            y=yb1+2*y2;
            
            ///x=2*u1;
            ///y=2*v1;
            
            int u=-1,v=-1;
            minval2=Double.POSITIVE_INFINITY;
            for(int it=0;it<9;it++){
                
                int tmp_x=x+pos_x[it];
                int tmp_y=y+pos_y[it];
                
                int m=tmp_x-ref2w/2;
                int n=tmp_y-ref2h/2;
                
                ///int m=x+pos_x[it];
                ///int n=y+pos_y[it];
                double tmp=0;
                for(int i=m;i<ref2w;i++){
                    for(int j=n;j<ref2h;j++){
                        tmp+=Math.abs(testg[i][j]-ref2g[i-m][j-n]);
                    }
                }
                if(tmp<=minval2){
                    minval2=tmp;
                    u=m;
                    v=n;
                }
                
            }
            System.out.println(u+" "+v);
    }
    
    public static void main(String[] args) {
        
        ///reading the image pixels into array
        TemplateMatching ob=new TemplateMatching();
        ob.read_images();
        ob.exhaustive_search1();
        ob.exhaustive_search2();
        ob.log_2d_search1();
        ob.log_2d_search2();
        ob.hierarchical_search1();
        ob.hierarchical_search2();
   
    }
    
}
