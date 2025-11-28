
package GUI;

import observer.Subscriber;
import observer.SubscriberType;
import org.json.JSONObject;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Objects;
import java.util.List;

import static App.Main.mainFrame;
import static App.Main.server;

public class AdminScreen extends TemplateScreen {

    private final JPanel previousLeftPanel;
    private final JPanel previousRightPanel;
    private Timer subscriberRefreshTimer;
    private ButtonGroup fruitGroup;
    private ButtonGroup crocoGroup;
    private ButtonGroup radioVinesGroup;
    private static JSlider selectedSlider;
    private static JButton btnSend;
    private static final ArrayList<JSlider> sliderHeightList = new ArrayList<>();
    private static final ArrayList<Integer[]> vineList = new ArrayList<>(List.of(
            new Integer[]{80, 78, 6, 300},
            new Integer[]{150, 78, 6, 300},
            new Integer[]{280, 200, 6, 200},
            new Integer[]{430, 98, 6, 300},
            new Integer[]{480, 98, 6, 200},
            new Integer[]{530, 98, 6, 300},
            new Integer[]{590, 98, 6, 260},
            new Integer[]{680, 30, 6, 380},
            new Integer[]{750, 30, 6, 380}
    ));


    public AdminScreen(JPanel leftPanel, JPanel rightPanel) {
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
//        JButton btnSend = new JButton("Enviar");
        JButton btnBack = new JButton("Volver");

        JButton[] buttons = { btnBack};

        for (JButton b : buttons) {
            b.setBackground(YELLOW);                            // custom background color
            b.setFont(BUTTON_FONT);                              // bold, large font
            b.setPreferredSize(BUTTON_SIZE);                     // enforce size
            b.setMaximumSize(BUTTON_SIZE);                       // required for BoxLayout
            b.setForeground(Color.BLACK);                       // black text
            b.setBorder(BorderFactory.createLineBorder(Color.BLACK, 2)); // 2px border
            b.setFocusable(false);                              // removes focus outline
        }

        buttonContainer.add(btnBack);


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
//        createRightPanelWithMapImage();
//        createLeftPanelAdminOptions();

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
                        System.out.println("Administrando a: " + subscriber.getName());
                        if (subscriberRefreshTimer != null) {
                            subscriberRefreshTimer.stop();
                        }
                        rightPanel.removeAll();
                        leftPanel.removeAll();
                        createRightPanelWithMapImage();
                        createLeftPanelAdminOptions(subscriber.getWriter());
                        rightPanel.revalidate();
                        rightPanel.repaint();
                        leftPanel.revalidate();
                        leftPanel.repaint();


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

    private void createLeftPanelAdminOptions(PrintWriter writer) {
        leftPanel.setLayout(new GridBagLayout());

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.weightx = 1.0;

        JLabel fruitsLabel = new JLabel("Select fruit:");
        fruitsLabel.setFont(BUTTON_FONT);
        fruitsLabel.setOpaque(true);
        fruitsLabel.setBackground(YELLOW);
        fruitsLabel.setBorder(BorderFactory.createLineBorder(Color.BLACK, 2));
        fruitsLabel.setForeground(Color.black);
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 3;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        leftPanel.add(fruitsLabel, gbc);

        gbc.gridwidth = 1;
        gbc.fill = GridBagConstraints.CENTER;
        ImageIcon appleIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/apple.png")));
        JRadioButton rbAppleImg = createRadioImage(appleIcon, "fruit");
        rbAppleImg.setName("Apple");
        gbc.gridx = 0;
        gbc.gridy = 1;
        leftPanel.add(rbAppleImg,gbc);

        ImageIcon peachIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/peach.png")));
        JRadioButton rbPeachImg = createRadioImage(peachIcon, "fruit");
        rbPeachImg.setName("Peach");
        gbc.gridx = 1;
        gbc.gridy = 1;
        leftPanel.add(rbPeachImg,gbc);

        ImageIcon bananaIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/banana.png")));
        JRadioButton rbBananaImg = createRadioImage(bananaIcon, "fruit");
        rbBananaImg.setName("Banana");
        gbc.gridx = 2;
        gbc.gridy = 1;
        leftPanel.add(rbBananaImg,gbc);

        fruitGroup = new ButtonGroup();
        fruitGroup.add(rbAppleImg);
        fruitGroup.add(rbPeachImg);
        fruitGroup.add(rbBananaImg);

        ActionListener listener = e -> {
            JRadioButton source = (JRadioButton) e.getSource();
            System.out.println("Selected fruit: " + source.getName());
            updateVinesVisibility();
        };

        rbAppleImg.setActionCommand("0");   // apple
        rbPeachImg.setActionCommand("1");   // peach
        rbBananaImg.setActionCommand("2");  // banana

        rbAppleImg.addActionListener(listener);
        rbPeachImg.addActionListener(listener);
        rbBananaImg.addActionListener(listener);

        btnSend = new JButton("Send");
        setButtonStyle(btnSend);
        btnSend.setEnabled(false);
        JButton btnBack = new JButton("Volver");
        setButtonStyle(btnBack);

        btnSend.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (fruitGroup.getSelection() == null || radioVinesGroup.getSelection() == null || selectedSlider == null) return;
                System.out.println("Send changes");
                Integer selectedVine = Integer.parseInt(radioVinesGroup.getSelection().getActionCommand());
                System.out.println("Selected vine " + selectedVine);
                System.out.println("Coords: " + vineList.get(selectedVine)[0]);
                Integer heightValue = selectedSlider.getValue();
                System.out.println(heightValue);

                JSONObject rectObject = new JSONObject();
                rectObject.put("x", vineList.get(selectedVine)[0]+3);
                rectObject.put("y", vineList.get(selectedVine)[1] + heightValue);
                rectObject.put("width", vineList.get(selectedVine)[2]);
                rectObject.put("height", vineList.get(selectedVine)[3]);
                JSONObject obj = new JSONObject();

                obj.put("type", Integer.parseInt(fruitGroup.getSelection().getActionCommand()));
                obj.put("vine", -1);
                obj.put("rectangle", rectObject);
                obj.put("msg_type", "place_fruit");

                writer.println(obj.toString());
                System.out.println(obj.toString());

                fruitGroup.clearSelection();
                selectedSlider.setVisible(false);
                selectedSlider = null;
                updateVinesVisibility();
                btnSend.setEnabled(false);
            }
        });

        gbc.fill = GridBagConstraints.CENTER;
        gbc.gridx = 0;
        gbc.gridy = 2;
        gbc.gridwidth = 3;
        leftPanel.add(btnSend, gbc);  // Add Send button at row 2

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



        crocodiles(gbc, writer);
        JPanel livesPanel = new JPanel(new FlowLayout());
        livesPanel.setOpaque(false);

        JButton btnAddLife = new JButton("+ Vida");
        JButton btnRemoveLife = new JButton("- Vida");

        setButtonStyle(btnAddLife);
        setButtonStyle(btnRemoveLife);

        livesPanel.add(btnAddLife);
        livesPanel.add(btnRemoveLife);

        gbc.gridx = 0;
        gbc.gridy = 7; // Ajustar según tu layout actual
        gbc.gridwidth = 3;
        leftPanel.add(livesPanel, gbc);

        gbc.gridx = 0;
        gbc.gridy = 8;
        gbc.gridwidth = 3;
        leftPanel.add(btnBack, gbc);
// Action Listeners
        btnAddLife.addActionListener(e -> {
            JSONObject obj = new JSONObject();
            obj.put("command", "ADD_LIFE");
            writer.println(obj.toString());
        });

        btnRemoveLife.addActionListener(e -> {
            JSONObject obj = new JSONObject();
            obj.put("command", "REMOVE_LIFE");
            writer.println(obj.toString());
        });// Add Back button at row 3
    }

    private void crocodiles(GridBagConstraints gbc, PrintWriter writer) {
        JLabel crocodilesLabel = new JLabel("Select Crocodile:");
        crocodilesLabel.setFont(BUTTON_FONT);
        crocodilesLabel.setOpaque(true);
        crocodilesLabel.setBackground(YELLOW);
        crocodilesLabel.setBorder(BorderFactory.createLineBorder(Color.BLACK, 2));
        crocodilesLabel.setForeground(Color.black);
        gbc.gridx = 0;
        gbc.gridy = 4;
        gbc.gridwidth = 3;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        leftPanel.add(crocodilesLabel, gbc);

        gbc.gridwidth = 1;
        gbc.fill = GridBagConstraints.CENTER;
        ImageIcon blueCrocIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/blue_crocodile.png")));
        JRadioButton rbBlueCrocImg = createRadioImage(blueCrocIcon, "crocodile");
        rbBlueCrocImg.setName("Blue");
        gbc.gridx = 0;
        gbc.gridy = 5;
        leftPanel.add(rbBlueCrocImg,gbc);

        ImageIcon redCrocIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/red_crocodile.png")));
        JRadioButton rbRedCrocImg = createRadioImage(redCrocIcon, "crocodile");
        rbRedCrocImg.setName("Red");
        gbc.gridx = 1;
        gbc.gridy = 5;
        leftPanel.add(rbRedCrocImg,gbc);



        crocoGroup = new ButtonGroup();
        crocoGroup.add(rbBlueCrocImg);
        crocoGroup.add(rbRedCrocImg);

        ActionListener listener = e -> {
            JRadioButton source = (JRadioButton) e.getSource();
            System.out.println("Selected Crocodile: " + source.getName());
            updateVinesVisibility();
        };

        rbBlueCrocImg.setActionCommand("0");   // blue
        rbRedCrocImg.setActionCommand("1");   // red

        rbBlueCrocImg.addActionListener(listener);
        rbRedCrocImg.addActionListener(listener);

        btnSend = new JButton("Send");
        setButtonStyle(btnSend);
        btnSend.setEnabled(false);


        btnSend.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (crocoGroup.getSelection() == null || radioVinesGroup.getSelection() == null) return;
                System.out.println("Send changes");
                Integer selectedVine = Integer.parseInt(radioVinesGroup.getSelection().getActionCommand());
                System.out.println("Selected vine " + selectedVine);
                System.out.println("Coords: " + vineList.get(selectedVine)[0]);

                JSONObject rectObject = new JSONObject();
                rectObject.put("x", vineList.get(selectedVine)[0]+3);
                rectObject.put("y", vineList.get(selectedVine)[1]);
                rectObject.put("width", vineList.get(selectedVine)[2]);
                rectObject.put("height", vineList.get(selectedVine)[3]);
                JSONObject obj = new JSONObject();

                obj.put("type", Integer.parseInt(crocoGroup.getSelection().getActionCommand()));
                obj.put("vine", selectedVine);
                obj.put("rectangle", rectObject);
                obj.put("msg_type", "place_crocodile");

                writer.println(obj.toString());
                System.out.println(obj.toString());

                fruitGroup.clearSelection();
                if (selectedSlider != null) {
                    selectedSlider.setVisible(false);
                    selectedSlider = null;
                }
                updateVinesVisibility();
                btnSend.setEnabled(false);
            }
        });

        gbc.fill = GridBagConstraints.CENTER;
        gbc.gridx = 0;
        gbc.gridy = 6;
        gbc.gridwidth = 3;
        leftPanel.add(btnSend, gbc);

    }

    private void updateVinesVisibility() {
        if (fruitGroup.getSelection() != null || crocoGroup.getSelection() != null) {
            System.out.println("There is a selection");
            radioVinesGroup.getElements().asIterator().forEachRemaining(rb -> rb.setVisible(true));
            //            if (radioVinesGroup.getSelection() != null) {
//                sliderHeightGroup.get(Integer.parseInt(radioVinesGroup.getSelection().getActionCommand())).setVisible(true);
//            }
        } else {
            System.out.println("No selection");
            radioVinesGroup.getElements().asIterator().forEachRemaining(rb -> rb.setVisible(false));
//            sliderHeightGroup.get(Integer.parseInt(radioVinesGroup.getSelection().getActionCommand())).setVisible(false);
//            sliderHeightGroup.getElements().asIterator().forEachRemaining(sld -> sld.setVisible(false));
        }
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

    protected void createRightPanelWithMapImage() {
        ImageIcon icon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/game_map.png")));

//        ImageIcon icon = new ImageIcon("src/GUI/resources/main_screen_img.png");
        Image original = icon.getImage();
        Image scaled = original.getScaledInstance(
                original.getWidth(icon.getImageObserver()),
                original.getHeight(icon.getImageObserver()), Image.SCALE_SMOOTH);
        ImageIcon scaledIcon = new ImageIcon(scaled);

        JLabel imgLabel = new JLabel(scaledIcon, SwingConstants.CENTER);
        imgLabel.setFont(new Font("Arial", Font.PLAIN, 20));
        imgLabel.setForeground(Color.WHITE);

        ActionListener listener = e -> {
            JRadioButton source = (JRadioButton) e.getSource();
            System.out.println("Selected vine: " + source.getName());
            for (JSlider slider: sliderHeightList) {
                slider.setVisible(false);
            }

            if (fruitGroup.getSelection() != null) {
                selectedSlider = sliderHeightList.get(Integer.parseInt(source.getName()));
                selectedSlider.setVisible(true);
            }
            btnSend.setEnabled(true);


        };

        for (Integer[] vine: vineList) {
            JSlider slider = new JSlider(SwingConstants.VERTICAL, 0, vine[3], vine[3]/2);
//            slider.setBounds(calcBounds(680, 30, 6, 380));
            slider.setBounds(calcBounds(
                    vine[0],    // x
                    vine[1],    // y
                    vine[2],    // width
                    vine[3]     //  height
            ));
            slider.setVisible(false);
            slider.setInverted(true);
            sliderHeightList.add(slider);
            // Add new slider to the panel
            rightPanel.add(slider);

        }


        radioVinesGroup = new ButtonGroup();
        for (int i = 0; i < vineList.size(); i++) {
            Integer[] v = vineList.get(i);   // [x, y, w, h]

            int x = v[0];
            int y = v[1];

            JRadioButton rb = new JRadioButton();
            rb.setBounds(calcBounds(x-7, y - 10, 20, 20));
            rb.setBackground(RED);
            rb.setForeground(Color.black);
            rb.setHorizontalAlignment(SwingConstants.CENTER);
            rb.setName(String.valueOf(i));
            rb.setActionCommand(String.valueOf(i));
            rb.addActionListener(listener);
//            radioButton.setHorizontalTextPosition(SwingConstants.CENTER);
//            radioButton.setVerticalTextPosition(SwingConstants.BOTTOM);

            radioVinesGroup.add(rb);
            rightPanel.add(rb);
            rb.setVisible(false);
        }



        rightPanel.add(imgLabel, BorderLayout.CENTER);

    }

    private JRadioButton createRadioImage(ImageIcon fruitIcon, String type) {
        Image originalFruitImg = fruitIcon.getImage();
        Image fruitImgScaled = null;
        if (Objects.equals(type, "fruit")) {
            fruitImgScaled = originalFruitImg.getScaledInstance(40, 40, Image.SCALE_SMOOTH);
        } else if (Objects.equals(type, "crocodile")) {
            fruitImgScaled = originalFruitImg.getScaledInstance(60, 20, Image.SCALE_SMOOTH);
        }
        assert fruitImgScaled != null;
        ImageIcon fruitScaledIcon = new ImageIcon(fruitImgScaled);

        JRadioButton radio = new JRadioButton();
        radio.setIcon(fruitScaledIcon);

        // Remove text and default styling
        radio.setText(null);
        radio.setOpaque(false);

        radio.setFocusPainted(false);
        radio.setBorderPainted(false);

        radio.setHorizontalAlignment(SwingConstants.CENTER);
        radio.setVerticalAlignment(SwingConstants.CENTER);

        radio.addItemListener(e -> {
            if (radio.isSelected()) {
                radio.setOpaque(true);
//                radio.setBackground(new Color(180, 130, 170)); // selected color
//                radio.setOpaque(true);
                radio.setBackground(YELLOW);
                radio.setBorderPainted(true);
//                radio.setMargin(new Insets(5, 10, 5, 10));
                radio.setBorder(BorderFactory.createLineBorder(Color.black, 3));
            } else {
                radio.setOpaque(false);
                radio.setBackground(new Color(0, 0, 0, 0)); // normal color
                radio.setBorderPainted(true);
//                radio.setMargin(new Insets(5, 10, 5, 10));
                radio.setBorder(BorderFactory.createLineBorder(new Color(0, 0, 0, 0), 3));
            }
        });


        return radio;
    }


    private Rectangle calcBounds(Integer x, Integer y, Integer width, Integer height){
        return new Rectangle(
                x-1,
                y-21,
                width,
                height
        );
    };






}
