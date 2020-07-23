package ija.gui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.Dimension;

public class BackgroundJFrame extends JFrame
{
	JButton b1;
	JLabel l1;
	int frameWidth = 1000;
	int frameHeight = 600;

    public BackgroundJFrame()
    {
	    setTitle("Solitaire");
	    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

	    setPreferredSize(new Dimension(frameWidth, frameHeight));
   		pack();
   		setLocationRelativeTo(null);

	    setResizable(false);
	
	/////// One way
/*	  
	    setLayout(new BorderLayout());
	    JLabel background=new JLabel(new ImageIcon("C:\\repository\\git\\FIT\\Projects\\6. sem\\IJA\\u3\\xprudk06\\lib\\images\\background.png"));
	    add(background);
		
	    l1=new JLabel("Here is a button");
	    b1=new JButton("I am a button");
*/

/*
	    background.setLayout(new FlowLayout());

	    background.add(l1);
	    background.add(b1);

	    background.setLayout(new BorderLayout());
		background.add(b1, BorderLayout.NORTH);
		background.add(l1, BorderLayout.WEST);
*/
	////// Another way

	    setLayout(new BorderLayout());
	    setContentPane(new JLabel(new ImageIcon("C:\\repository\\git\\FIT\\Projects\\6. sem\\IJA\\u3\\xprudk06\\lib\\images\\background.png")));


	

    }

}