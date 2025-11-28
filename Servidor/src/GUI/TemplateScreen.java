package GUI;

import javax.swing.*;
import java.awt.*;

import static App.Main.mainFrame;

abstract class TemplateScreen {


    // colors from the background image
    protected final Color YELLOW = new Color(255, 163, 0);
    protected final Color BLUE = new Color(0, 104, 163);
    protected final Color RED = new Color(251, 0, 0);
    protected final Font BUTTON_FONT = new Font("Arial", Font.BOLD, 20);
    protected final Dimension BUTTON_SIZE = new Dimension(220, 50);

    protected JPanel leftPanel = null;
    protected JPanel rightPanel = null;

    public TemplateScreen() {


        createLeftPanel();
        createRightPanel();

        leftPanel.setPreferredSize(new Dimension(300, 0)); // 1/3 de una ventana de 900px

        mainFrame.add(leftPanel, BorderLayout.WEST);
        mainFrame.add(rightPanel, BorderLayout.CENTER);

    }


    private void createLeftPanel() {
        leftPanel = new JPanel();
        leftPanel.setBackground(RED);

        // Center content vertically and horizontally
        leftPanel.setLayout(new GridBagLayout());

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = GridBagConstraints.RELATIVE;   // automatic vertical stacking
        gbc.insets = new Insets(10, 0, 10, 0);     // spacing between rows
        gbc.anchor = GridBagConstraints.CENTER;

        JPanel buttonContainer = createLeftPanelButtons();

        // Center the container inside the left panel
        leftPanel.add(buttonContainer, gbc);

    }

    private void createRightPanel() {
        rightPanel = new JPanel(new BorderLayout());
        rightPanel.setBackground(BLUE);
        createRightPanelContent();
    }

    public void setButtonStyle(JButton button) {
        button.setBackground(YELLOW);                            // custom background color
        button.setFont(BUTTON_FONT);                              // bold, large font
        button.setPreferredSize(BUTTON_SIZE);                     // enforce size
        button.setMaximumSize(BUTTON_SIZE);                       // required for BoxLayout
        button.setForeground(Color.BLACK);                       // black text
        button.setBorder(BorderFactory.createLineBorder(Color.BLACK, 2)); // 2px border
        button.setFocusable(false);
    }

    protected abstract JPanel createLeftPanelButtons();

    protected abstract void createRightPanelContent();


}
