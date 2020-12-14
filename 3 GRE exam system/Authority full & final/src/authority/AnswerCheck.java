package authority;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class AnswerCheck {
    FileReader fr;
    BufferedReader  br;
    String str=null;
    
    AnswerCheck(File f){ 
        try {
            fr = new FileReader(f);
            br=new BufferedReader(fr);  
        } catch (FileNotFoundException ex) {
            Logger.getLogger(AnswerCheck.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    int check(){
        int marks=10;
        try {
            FileReader file=new FileReader("src/Resources/CorrectAnswers.txt");
            BufferedReader buffer=new BufferedReader(file);
            String answer=buffer.readLine();  
        try{
            while((str=br.readLine())!=null){
                if(str.equals("Verbal Reasoning:") && answer.equals("Verbal Reasoning:")){
                    while((str=br.readLine())!=null && (answer=buffer.readLine())!=null){
                        if(str.equals(answer)==false) marks--;
                    }
                }
            }
        } catch (IOException ex) {
            Logger.getLogger(AnswerCheck.class.getName()).log(Level.SEVERE, null, ex);
        }
        } catch (FileNotFoundException ex) {
            Logger.getLogger(AnswerCheck.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(AnswerCheck.class.getName()).log(Level.SEVERE, null, ex);
        }
        return marks;
    }
}
