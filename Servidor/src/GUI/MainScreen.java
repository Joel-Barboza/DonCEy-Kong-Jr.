package GUI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Objects;

import static App.Main.mainFrame;

public class MainScreen extends TemplateScreen {


    protected JPanel createLeftPanelButtons() {
        // Container to stack buttons vertically
        JPanel buttonContainer = new JPanel();
        buttonContainer.setOpaque(false);          // keep background color from parent
        buttonContainer.setLayout(new BoxLayout(buttonContainer, BoxLayout.Y_AXIS));

        // Button style
        Font buttonFont = new Font("Arial", Font.BOLD, 20);    // bigger, bold text
        Dimension buttonSize = new Dimension(220, 50);         // fixed button size

        // Create buttons
//        JButton btnNewGame = new JButton("Nueva Partida");
        JButton btnSpectate = new JButton("Espectar");
        JButton btnAdmin = new JButton("Administrar Partidas");

//        JButton[] buttons = { btnNewGame, btnSpectate, btnAdmin };
        JButton[] buttons = { btnSpectate, btnAdmin };

        for (JButton b : buttons) {
            setButtonStyle(b);
        }

        // Add buttons with spacing between them
//        buttonContainer.add(btnNewGame);
//        buttonContainer.add(Box.createVerticalStrut(20));       // vertical spacing
        buttonContainer.add(btnSpectate);
        buttonContainer.add(Box.createVerticalStrut(20));
        buttonContainer.add(btnAdmin);


//        btnNewGame.addActionListener(new ActionListener() {
//            @Override
//            public void actionPerformed(ActionEvent e) {
//                System.out.println("pantalla nuevo juego");
//
//                mainFrame.remove(leftPanel);
//                mainFrame.remove(rightPanel);
//
//                new NewGameScreen(leftPanel, rightPanel);
//
//                mainFrame.revalidate();
//                mainFrame.repaint();
////                server.addSubscriber();
//            }
//        });

        btnSpectate.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("pantalla espectador");

                mainFrame.remove(leftPanel);
                mainFrame.remove(rightPanel);

                new SpectatorScreen(leftPanel, rightPanel);

                mainFrame.revalidate();
                mainFrame.repaint();

//                server.addSubscriber();
            }
        });

        btnAdmin.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("pantalla Admin");

                mainFrame.remove(leftPanel);
                mainFrame.remove(rightPanel);

                new AdminScreen(leftPanel, rightPanel);

                mainFrame.revalidate();
                mainFrame.repaint();
//                server.addSubscriber();
            }
        });
        return buttonContainer;
    }

    protected void createRightPanelContent() {
        ImageIcon icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/main_screen_img.png")));

//        ImageIcon icon = new ImageIcon("src/GUI/resources/main_screen_img.png");
        Image original = icon.getImage();
        Image scaled = original.getScaledInstance(
                original.getWidth(icon.getImageObserver())/2,
                original.getHeight(icon.getImageObserver())/2, Image.SCALE_SMOOTH);
        ImageIcon scaledIcon = new ImageIcon(scaled);

        JLabel imgLabel = new JLabel(scaledIcon, SwingConstants.CENTER);
        imgLabel.setFont(new Font("Arial", Font.PLAIN, 20));
        imgLabel.setForeground(Color.WHITE);

        rightPanel.add(imgLabel, BorderLayout.CENTER);

    }



}
