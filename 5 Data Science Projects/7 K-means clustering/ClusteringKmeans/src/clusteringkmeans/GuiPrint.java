package clusteringkmeans;


import java.awt.Color;
import java.awt.Graphics;
import java.awt.geom.Point2D;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.swing.JFrame;
import javax.swing.JPanel;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author acer
 */
public class GuiPrint extends JFrame{
    List points1=new ArrayList();
    List centroids=new ArrayList();
    
    
    GuiPrint(ClusteringKmeans ob){
        super("Ploting");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        ArrayList final_list=ob.final_list;
        for(int i=0;i<final_list.size();i++){
            Cluster c=(Cluster) ob.final_list.get(i);
            float x=(float) (c.cent_x*20);
            float y=(float) (c.cent_y*20);
            
            centroids.add(new Point2D.Float(x,y));
           
            
        }
        for(int i=0;i<ob.total_points;i++){
            float x=(float) ob.points[i][0]*20;
            float y=(float) ob.points[i][1]*20;
            
            points1.add(new Point2D.Float(x,y));
        }
        
        
        JPanel panel=new JPanel(){
            public void paintComponent(Graphics g){
                for(Iterator i=points1.iterator();i.hasNext();){
                    Point2D.Float pt=(Point2D.Float)i.next();
                    g.setColor(Color.red);
                    ///g.drawString(".",(int)pt.x,(int)pt.y);
                    g.drawOval((int)pt.x,(int)pt.y,5,5);
                }
                for(Iterator i=centroids.iterator();i.hasNext();){
                    Point2D.Float pt=(Point2D.Float)i.next();
                    g.setColor(Color.black);
                    ///g.drawString("*",(int)pt.x,(int)pt.y);
                    ///g.drawOval((int)pt.x,(int)pt.y, 10, 10);
                    g.drawRect((int)pt.x,(int)pt.y, 5, 5);
                }
            }
        };
        
        setContentPane(panel);
        setBounds(20,20,600,300);
        
        setVisible(true);
    }
}
