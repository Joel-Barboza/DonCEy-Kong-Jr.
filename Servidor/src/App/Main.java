package App;

import GUI.MainScreen;
import observer.Server;

import javax.swing.*;
import java.awt.*;

public class Main {
    public static JFrame mainFrame = new JFrame();
    public static Server server = null;

    public static void main(String[] args) {
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainFrame.setSize(1115, 600);
        mainFrame.setLocationRelativeTo(null);
        mainFrame.setLayout(new BorderLayout());

        new MainScreen();
        mainFrame.setVisible(true);

        server = new Server();
        server.startServer();
    }
}
