package GUI;

import javax.print.attribute.standard.JobHoldUntil;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

abstract class TemplateScreen {


    // colors from the background image
    protected final Color YELLOW = new Color(255, 163, 0);
    protected final Color BLUE = new Color(0, 104, 163);
    protected final Color RED = new Color(251, 0, 0);
    protected JFrame mainFrame = null;
    protected JPanel leftPanel = null;
    protected JPanel rightPanel = null;

    public TemplateScreen() {


        // starts main window
        mainFrame = new JFrame();
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainFrame.setSize(1115, 600);
        mainFrame.setLocationRelativeTo(null);

        createLeftPanel();
        createRightPanel();

        leftPanel.setPreferredSize(new Dimension(300, 0)); // 1/3 de una ventana de 900px

        mainFrame.setLayout(new BorderLayout());
        mainFrame.add(leftPanel, BorderLayout.WEST);
        mainFrame.add(rightPanel, BorderLayout.CENTER);

        mainFrame.setVisible(true);
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


    protected abstract JPanel createLeftPanelButtons();

    protected abstract void createRightPanelContent();


}
