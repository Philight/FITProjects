

package ija.gui;

import ija.gui.BackgroundJFrame;
import ija.gui.ShapeMover;

import java.awt.*;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

public class CardTable implements ActionListener{

	private BackgroundJFrame frame;
	private JTextField textField;
	private JButton b1;
	private JLabel l1;

	public CardTable(){
		initialize();
	}

	private void initialize() {
		frame = new BackgroundJFrame();

	    frame.setLayout(new BorderLayout());

		frame.add(new NorthPanel(), BorderLayout.NORTH);
		frame.add(new SouthPanel(), BorderLayout.SOUTH);

		String imagepath = "C:\\repository\\git\\FIT\\Projects\\6. sem\\IJA\\u3\\xprudk06\\lib\\images\\background.png";
		DragPanel centerPanel = new DragPanel(new ImageIcon(imagepath).getImage());
		frame.add(centerPanel, BorderLayout.CENTER);
/*
		frame = new JFrame();
		frame.setBounds(100, 100, 450, 300);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[] {0, 0, 0, 0, 0, 0, 0};
		gridBagLayout.rowHeights = new int[] {0, 0, 0, 0, 0, 0};
		gridBagLayout.columnWeights = new double[]{1.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
		gridBagLayout.rowWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
		frame.getContentPane().setLayout(gridBagLayout);
		
		textField = new JTextField();
		textField.setEditable(true);
		textField.setFont(new Font("Dialog", Font.BOLD, 22));
		textField.setHorizontalAlignment(SwingConstants.RIGHT);
		GridBagConstraints gbc_textField = new GridBagConstraints();
		gbc_textField.weighty = 1.0;
		gbc_textField.weightx = 1.0;
		gbc_textField.fill = GridBagConstraints.BOTH;
		gbc_textField.gridwidth = 6;
		gbc_textField.insets = new Insets(5, 5, 5, 5);
		gbc_textField.gridx = 0;
		gbc_textField.gridy = 0;
		frame.getContentPane().add(textField, gbc_textField);
		textField.setColumns(10);
		
		JButton btnPi = new JButton("PI");
		btnPi.setActionCommand("PI");
		btnPi.addActionListener(this);
	    GridBagConstraints gbc_btnPi = new GridBagConstraints();
		gbc_btnPi.fill = GridBagConstraints.BOTH;
		gbc_btnPi.weighty = 1.0;
		gbc_btnPi.weightx = 1.0;
		gbc_btnPi.insets = new Insets(5, 5, 5, 5);
		gbc_btnPi.gridx = 5;
		gbc_btnPi.gridy = 2;
		frame.getContentPane().add(btnPi, gbc_btnPi);
		
	*/

		frame.setVisible(true);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		textField.setText("1.5");
		try {
			int num = Integer.parseInt(textField.getText());
		} catch (Exception e1) {textField.setText("2");}
	}

	public class NorthPanel extends JPanel {
		public NorthPanel() {
		    add(new JLabel("Add Games"));
	    	add(new JButton("+"));
		}
	}

    public class SouthPanel extends JPanel {

        public SouthPanel() {
            for (int index = 0; index < 5; index++) {
                add(new JLabel(Integer.toString(index)));
                add(new JTextField(5));
            }
            add(new JButton("Button"));
        }            
    }  
}
