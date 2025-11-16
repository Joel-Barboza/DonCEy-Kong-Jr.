package GUI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class NewGameScreen extends TemplateScreen {

    private final JPanel previousLeftPanel;
    private final JPanel previousRightPanel;

    public NewGameScreen (JPanel leftPanel, JPanel rightPanel) {
        previousLeftPanel = leftPanel;
        previousRightPanel = rightPanel;
    }

    protected JPanel createLeftPanelButtons() {
        // Container to stack buttons vertically
        JPanel buttonContainer = new JPanel();
        buttonContainer.setOpaque(false);          // keep background color from parent
        buttonContainer.setLayout(new BoxLayout(buttonContainer, BoxLayout.Y_AXIS));

        // Button style
        Font buttonFont = new Font("Arial", Font.BOLD, 20);    // bigger, bold text
        Dimension buttonSize = new Dimension(220, 50);         // fixed button size

        // Create buttons
        JButton btnStart = new JButton("Iniciar");
        JButton btnBack = new JButton("Volver");

        JButton[] buttons = {btnStart, btnBack};

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
        buttonContainer.add(btnStart);
        buttonContainer.add(Box.createVerticalStrut(20));       // vertical spacing
        buttonContainer.add(btnBack);


        btnStart.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("pantalla nuevo juego");

//                NewGameScreen screen = new NewGameScreen(leftPanel, rightPanel);
//                server.addSubscriber();
            }
        });

        btnBack.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("Por ahora no hace nada el botón");
                if (mainFrame != null) {
                    mainFrame.remove(leftPanel);
                    mainFrame.remove(rightPanel);
                    mainFrame.dispose();   // destroys native resources
                }

                leftPanel = null;
                rightPanel = null;
                mainFrame = null;
                previousLeftPanel.setVisible(true);
                previousRightPanel.setVisible(true);
            }
        });

        return buttonContainer;
    }

    protected void createRightPanelContent() {
//        ImageIcon icon = new ImageIcon("src/GUI/resources/main_screen_img.png");
//        Image original = icon.getImage();
//        Image scaled = original.getScaledInstance(
//                original.getWidth(icon.getImageObserver())/2,
//                original.getHeight(icon.getImageObserver())/2, Image.SCALE_SMOOTH);
//        ImageIcon scaledIcon = new ImageIcon(scaled);
//
//        JLabel imgLabel = new JLabel(scaledIcon, SwingConstants.CENTER);
//        imgLabel.setFont(new Font("Arial", Font.PLAIN, 20));
//        imgLabel.setForeground(Color.WHITE);
//
//        rightPanel.add(imgLabel, BorderLayout.CENTER);

    }



}
