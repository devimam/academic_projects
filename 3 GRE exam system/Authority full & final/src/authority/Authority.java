package authority;

import javax.swing.JFrame;

public class Authority {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
                Teacher te=new Teacher();
                te.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                te.setVisible(true);
                te.execute();

    }
    
}
