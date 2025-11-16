package GUI;

import javax.print.attribute.standard.JobHoldUntil;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

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
        JButton btnNewGame = new JButton("Nueva Partida");
        JButton btnSpectate = new JButton("Espectar");
        JButton btnAdmin = new JButton("Administrar Partidas");

        JButton[] buttons = { btnNewGame, btnSpectate, btnAdmin };

        for (JButton b : buttons) {
            b.setBackground(YELLOW);                            // custom background color
            b.setFont(buttonFont);                              // bold, large font
            b.setPreferredSize(buttonSize);                     // enforce size
            b.setMaximumSize(buttonSize);                       // required for BoxLayout
            b.setForeground(Color.BLACK);                       // black text
            b.setBorder(BorderFactory.createLineBorder(Color.BLACK, 2)); // 2px border
            b.setFocusable(false);                              // removes focus outline
        }

        // Add buttons with spacing between them
        buttonContainer.add(btnNewGame);
        buttonContainer.add(Box.createVerticalStrut(20));       // vertical spacing
        buttonContainer.add(btnSpectate);
        buttonContainer.add(Box.createVerticalStrut(20));
        buttonContainer.add(btnAdmin);


        btnNewGame.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("pantalla nuevo juego");

                NewGameScreen screen = new NewGameScreen(leftPanel, rightPanel);
//                server.addSubscriber();
            }
        });

        btnSpectate.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("Por ahora no hace nada el botón");

                SpectatorScreen screen = new SpectatorScreen(leftPanel, rightPanel);
//                server.addSubscriber();
            }
        });

        btnAdmin.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("Por ahora no hace nada el botón");
                AdminPanel screen = new AdminPanel(leftPanel, rightPanel);
//                server.addSubscriber();
            }
        });
        return buttonContainer;
    }

    protected void createRightPanelContent() {
        ImageIcon icon = new ImageIcon("src/GUI/resources/main_screen_img.png");
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
