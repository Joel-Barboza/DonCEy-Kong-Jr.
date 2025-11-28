
package GUI;

import observer.Subscriber;
import observer.SubscriberType;

import javax.swing.*;
        import java.awt.*;
        import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import static App.Main.mainFrame;
import static App.Main.server;

public class SpectatorScreen extends TemplateScreen {

    private final JPanel previousLeftPanel;
    private final JPanel previousRightPanel;
    private Timer subscriberRefreshTimer;


    public SpectatorScreen(JPanel leftPanel, JPanel rightPanel) {
        previousLeftPanel = leftPanel;
        previousRightPanel = rightPanel;
        startRefreshing();
    }

    protected JPanel createLeftPanelButtons() {
        // Container to stack buttons vertically
        JPanel buttonContainer = new JPanel();
        buttonContainer.setOpaque(false);          // keep background color from parent
        buttonContainer.setLayout(new BoxLayout(buttonContainer, BoxLayout.Y_AXIS));


        // Create buttons
        JButton btnStart = new JButton("Ver");
        JButton btnBack = new JButton("Volver");

        JButton[] buttons = {btnStart, btnBack};

        for (JButton b : buttons) {
            setButtonStyle(b);
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
                System.out.println("Volviendo a Inicio");

                // Stops refreshing when leaving this screen
                if (subscriberRefreshTimer != null) {
                    subscriberRefreshTimer.stop();
                }

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

        // Panel that will contain the subscriber items
        JPanel listPanel = new JPanel();
        listPanel.setLayout(new BoxLayout(listPanel, BoxLayout.Y_AXIS));
        listPanel.setBackground(BLUE);

        ArrayList<Subscriber> subscriberArrayList = server.getSubsList();

        subscriberArrayList.forEach(subscriber -> {
            if (subscriber.getType() == SubscriberType.PLAYER) {

                JButton button = new JButton(subscriber.getName());
                button.setBackground(YELLOW);
                button.setFont(BUTTON_FONT);
                button.setPreferredSize(BUTTON_SIZE);
                button.setMaximumSize(BUTTON_SIZE);
                button.setForeground(Color.BLACK);
                button.setBorder(BorderFactory.createLineBorder(Color.BLACK, 2));
                button.setFocusable(false);


                button.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        System.out.println("Espectando a: " + subscriber.getName());


                    }
                });

                // Add some spacing between buttons
                listPanel.add(Box.createVerticalStrut(8));
                listPanel.add(button);
            }
        });

        // Make list scrollable
        JScrollPane scrollPane = new JScrollPane(listPanel);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        scrollPane.setBorder(BorderFactory.createEmptyBorder());

        rightPanel.setLayout(new BorderLayout());
        rightPanel.add(scrollPane, BorderLayout.CENTER);
    }
    private void startRefreshing() {
        subscriberRefreshTimer = new Timer(1000, e -> refreshSubscribers());
        subscriberRefreshTimer.start();
    }

    private void refreshSubscribers() {
        rightPanel.removeAll();        // Clear old content
        createRightPanelContent();     // Rebuild the subscriber list
        rightPanel.revalidate();
        rightPanel.repaint();
    }







}
