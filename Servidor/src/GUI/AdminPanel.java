package GUI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class AdminPanel {

    public static void createAndShowGUI() {
        JFrame frame = new JFrame("My Frame");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setBounds(100, 100, 800, 600); // Set location to (100,100) and size to 500x400

        JButton button = new JButton("Administrate games");
        frame.add(button);

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("Por ahora no hace nada el botón");
            }
        });


        frame.setVisible(true);
    }


}
