package authority;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;


public class TeacherThread extends Thread{
    Socket soc=null;
    ObjectInputStream in=null;
    ObjectOutputStream out=null;
    DefaultTableModel model;
    JTable table;
    
    int no;
    boolean flag;
    String name=null,id=null;
    
    public TeacherThread(Socket soc,ObjectInputStream in,ObjectOutputStream out,int no,JTable txt,DefaultTableModel model){
        this.soc=soc;
        this.in=in;
        this.out=out;  
        this.no=no;
        this.table=txt;
        this.flag=false;
        this.model=model;
        File f=new File("src/Questions");
        //model=(DefaultTableModel) table.getModel();
        
        String[] filenames=f.list();
        for(String temp:filenames){
            String content="";
            try{
                FileReader fr=new FileReader(String.format("src/Questions/%s",temp));
                BufferedReader br=new BufferedReader(fr);
                String t=null;
                while((t=br.readLine())!=null){
                    content+=t;
                    content+="\n";
                }
                out.writeObject(content);
                out.flush();
                System.out.println("Successful\n");
            }
            catch(Exception e){
                ;
            }
        }
        
        
        
    }
    
    public void run(){
        new AnswerReceiveThread(soc,in,out,no,table,model);
    }
}
