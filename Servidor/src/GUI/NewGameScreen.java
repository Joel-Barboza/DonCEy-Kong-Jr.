package GUI;

import observer.Player;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import static App.Main.mainFrame;
import static App.Main.server;


public class NewGameScreen extends TemplateScreen {

    private final JPanel previousLeftPanel;
    private final JPanel previousRightPanel;
    private JTextField textField;
    private JButton btnStart;



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
        btnStart = new JButton("Iniciar");
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
                System.out.println("Iniciooooooo");


                String playerNamer = textField.getText();
//                Player player = new Player(playerNamer, );
//                System.out.println(player.getClass());
                try {
                    String command = "..\\Cliente\\cmake-build-debug\\cliente_tcp.exe";
                    Process process = Runtime.getRuntime().exec(command);

//                    int exitCode = process.waitFor();
//                    System.out.println("Exited with code: " + exitCode);

                } catch (IOException error) {
//                } catch (IOException | InterruptedException error) {
                    error.printStackTrace();
                }

//                server.addSubscriber(player);
            }
        });

        btnBack.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("Volviendo a Inicio");

                mainFrame.remove(leftPanel);
                mainFrame.remove(rightPanel);

                mainFrame.add(previousLeftPanel, BorderLayout.WEST);
                mainFrame.add(previousRightPanel, BorderLayout.CENTER);

                mainFrame.revalidate();
                mainFrame.repaint();
            }
        });

        return buttonContainer;
    }

    protected void createRightPanelContent() {

        rightPanel.setLayout(new GridBagLayout());

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = GridBagConstraints.RELATIVE;
        gbc.insets = new Insets(15, 0, 15, 0);
        gbc.anchor = GridBagConstraints.CENTER;

        JLabel labelInsertName = new JLabel("Ingrese el nombre del jugador:");
        labelInsertName.setFont(new Font("Arial", Font.BOLD, 18));
        labelInsertName.setForeground(Color.WHITE);

        textField = new JTextField();
        textField.setPreferredSize(new Dimension(250, 40));
        textField.setBackground(YELLOW);
        textField.setForeground(Color.BLACK);
        textField.setFont(new Font("Arial", Font.BOLD, 18));
        textField.setBorder(BorderFactory.createLineBorder(Color.BLACK, 2));
        textField.setCaretColor(Color.BLACK);

        textField.addActionListener(e -> btnStart.doClick());


        rightPanel.add(labelInsertName, gbc);
        rightPanel.add(textField, gbc);
    }





}
