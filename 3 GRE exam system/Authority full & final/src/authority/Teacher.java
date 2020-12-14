

package authority;

import java.awt.Color;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.table.DefaultTableModel;


public class Teacher extends javax.swing.JFrame {

   ServerSocket server;
   
   int MaxStudent=50;
   int serial;
   ExecutorService e;
   ObjectOutputStream out;
   ObjectInputStream in;
   int isactivated=0;
   DefaultTableModel model;
   
    public Teacher() {
        initComponents();
        this.setTitle("GRE Exam");
        serial=0;
        model=(DefaultTableModel) table.getModel();
        
        e=Executors.newFixedThreadPool(2);
       try {
           server=new ServerSocket(2000,100);
           System.out.println("activated server ");
       } catch (IOException ex) {
           Logger.getLogger(Teacher.class.getName()).log(Level.SEVERE, null, ex);
       }
       
    }

    
    
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane1 = new javax.swing.JScrollPane();
        jTable1 = new javax.swing.JTable();
        jLabel1 = new javax.swing.JLabel();
        activateb = new javax.swing.JButton();
        jScrollPane3 = new javax.swing.JScrollPane();
        table = new javax.swing.JTable();
        deactivate = new javax.swing.JButton();

        jTable1.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null}
            },
            new String [] {
                "Title 1", "Title 2", "Title 3", "Title 4"
            }
        ));
        jScrollPane1.setViewportView(jTable1);

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setResizable(false);

        jLabel1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/Resources/GRE.jpg"))); // NOI18N

        activateb.setBackground(new java.awt.Color(252, 0, 0));
        activateb.setFont(new java.awt.Font("Comic Sans MS", 1, 18)); // NOI18N
        activateb.setText("Activate");
        activateb.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                activatebActionPerformed(evt);
            }
        });

        table.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Student Name", "Student ID", "Exam Status"
            }
        ) {
            boolean[] canEdit = new boolean [] {
                false, false, false
            };

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        table.getTableHeader().setReorderingAllowed(false);
        table.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                tableMouseClicked(evt);
            }
        });
        jScrollPane3.setViewportView(table);
        if (table.getColumnModel().getColumnCount() > 0) {
            table.getColumnModel().getColumn(0).setResizable(false);
            table.getColumnModel().getColumn(1).setResizable(false);
            table.getColumnModel().getColumn(2).setResizable(false);
        }

        deactivate.setFont(new java.awt.Font("Comic Sans MS", 1, 18)); // NOI18N
        deactivate.setText("Deactivate");
        deactivate.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                deactivateActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jLabel1, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(activateb, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(deactivate, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jScrollPane3, javax.swing.GroupLayout.DEFAULT_SIZE, 462, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel1)
                        .addGap(18, 18, 18)
                        .addComponent(activateb, javax.swing.GroupLayout.PREFERRED_SIZE, 46, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(deactivate, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(147, 147, 147))
                    .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void activatebActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_activatebActionPerformed
       serial=0;
       activateb.setBackground(Color.GREEN);
       System.out.println("actibvated");
       isactivated=1;   
    }//GEN-LAST:event_activatebActionPerformed

    private void deactivateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_deactivateActionPerformed
       try {
           // TODO add your handling code here:
           server.close();
           deactivate.setBackground(Color.RED);
           activateb.setBackground(Color.RED);
       } catch (IOException ex) {
           Logger.getLogger(Teacher.class.getName()).log(Level.SEVERE, null, ex);
       }
        
    }//GEN-LAST:event_deactivateActionPerformed

    private void tableMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_tableMouseClicked
        // TODO add your handling code here:
        int row=table.getSelectedRow();
        if(table.getValueAt(row,2).equals("running")){
            JOptionPane.showMessageDialog(this,"Exam is not completed yet");
        }
        else{
            answerscript page=new answerscript();
//            page.value=row;
            System.out.println(row);
            page.getmarks(row);
            page.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
            page.setVisible(true);
        }
    }//GEN-LAST:event_tableMouseClicked

    public void execute(){ 
        while(serial<MaxStudent){
            System.out.println("entered");
            
            Socket connection=null;
            try {
                connection = server.accept();
                
                System.out.println("accepted");
                try {
                    in=new ObjectInputStream(connection.getInputStream());
                    out=new ObjectOutputStream(connection.getOutputStream());
                    out.flush(); 

                    int status=0;
                    TeacherThread t=new TeacherThread(connection,in,out,serial,table,model);
                    e.execute(t);

                    serial++;
                } catch (IOException ex) {
                    Logger.getLogger(Teacher.class.getName()).log(Level.SEVERE, null, ex);
                }
            } catch (IOException ex) {
                System.out.println("not accepted");
                Logger.getLogger(Teacher.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(Teacher.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Teacher.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Teacher.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Teacher.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Teacher().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    public javax.swing.JButton activateb;
    private javax.swing.JButton deactivate;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JTable jTable1;
    public static javax.swing.JTable table;
    // End of variables declaration//GEN-END:variables
}
