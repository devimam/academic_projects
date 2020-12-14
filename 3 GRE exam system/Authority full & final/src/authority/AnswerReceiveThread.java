/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package authority;

import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author iBoss
 */
public class AnswerReceiveThread extends Thread {
    
    Socket soc=null;
    ObjectInputStream in=null;
    ObjectOutputStream out=null;
    DefaultTableModel model;
    JTable table;
    
    int no;
    boolean flag;
    String name=null,id=null;
    
    public AnswerReceiveThread(Socket soc,ObjectInputStream in,ObjectOutputStream out,int no,JTable txt,DefaultTableModel model) { 
        super("demo");
        this.soc=soc;
        this.in=in;
        this.out=out;  
        this.no=no;
        this.table=txt;
        this.flag=false;
        this.model=model;
        
        
        System.out.println("Child thread: " + this); 
        start();
    } 
    
    public void run() { 
        boolean check=true;
        while(check){
            try {
                name=(String)in.readObject();
                System.out.println(name);
                
                id=(String)in.readObject();
                System.out.println(id);
                model=(DefaultTableModel)this.table.getModel();
                model.insertRow(table.getRowCount(),new Object[]{name,id,"running"});
                model.fireTableDataChanged();
                
                System.out.println(name);
                
                
                    FileOutputStream fw=new FileOutputStream(String.format("src/Resources/ans"+no));
                    BufferedOutputStream bw=new BufferedOutputStream(fw);

                    byte[] ans=((String)in.readObject()).getBytes();
                    bw.write(ans);
                    bw.flush();

                    System.out.println("accepted final answer sheet");
                    
                    model.setValueAt("Completed",no, 2);
                    model.fireTableDataChanged();
                    check=false;
                    break;
                
            } catch (IOException ex) {
                Logger.getLogger(TeacherThread.class.getName()).log(Level.SEVERE, null, ex);
            } catch (ClassNotFoundException ex) {
                Logger.getLogger(TeacherThread.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}
    
    
     
