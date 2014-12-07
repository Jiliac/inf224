import java.awt.BorderLayout;

import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JRadioButton;
import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.AbstractAction;


public class Window extends JFrame {
	private static final long serialVersionUID = 1L;
  private Client client;
  private JPanel pan;
  JTextArea textArea; 
  private String selectedRadioButton = null;
  private String textEntry = null; //will be the file (or group) name to send to server

	public Window() {
		init_windows();

    textArea = new JTextArea(5, 5);
    textArea.setEditable(false);
    pan.add(textArea, BorderLayout.CENTER);

    pan.add(genSearchPanel(), BorderLayout.SOUTH);
   
		this.setContentPane(pan);
		this.setVisible(true);

    try {
      client = new Client(Client.DEFAULT_HOST, Client.DEFAULT_PORT);
    } catch (java.io.IOException e){
      client = null;
    }
	}
	
  /*
   * initialise la fenetre que sera ensuite agrementer par des panneaux
   */
	private void init_windows() {
		this.setTitle("First Window");
		this.setSize(300, 300);
		this.setLocationRelativeTo(null);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		pan = new JPanel();
		pan.setBackground(Color.BLACK);
		
		this.setLayout(new BorderLayout());
		
		JMenuBar menuBar = new JMenuBar();
		JMenu fileMenu = new JMenu("File");
		genFileMenu(fileMenu);
		JToolBar tool = new JToolBar(); 
		this.setJMenuBar(menuBar);
		menuBar.add(fileMenu);
		fileMenu.add(tool);
	}
  
  /*
   * s'occupe de generer le panneau inferieur destiner a la recherche
   */
  private JPanel genSearchPanel() {
    JPanel searchPanel = new JPanel();
    searchPanel.setLayout(new GridLayout(1, 3));
    searchPanel.add(genRadioButtons());

    JTextField textEntryArea = new JTextField();
    searchPanel.add(textEntryArea);

    JButton sendButton = new JButton("Send !");
    sendButton.addActionListener(new AbstractAction(){
      public void actionPerformed(ActionEvent e){
        String fileName = textEntryArea.getText();
        if (selectedRadioButton != null) {
          String response = send(selectedRadioButton + " " + fileName);
          textArea.append(response + "\n");
        } else {
          textArea.append("You must select an option\n");
        }
      }
    });
    searchPanel.add(sendButton);

    return searchPanel;
  }
	
  /*
   * genere le groupe de radio boutons ensuite utiliser dans le panneau de recherche
   */
	private JPanel genRadioButtons() {
		JPanel buttonsPanel = new JPanel();
		buttonsPanel.setLayout(new GridLayout(1, 3));
    ButtonGroup bg = new ButtonGroup();
		
    JRadioButton searchGroup = new JRadioButton("Search Group");
    JRadioButton searchFile = new JRadioButton("Search File");
    JRadioButton play = new JRadioButton("Play");

    searchGroup.addActionListener(new RadioButtonAction("search group"));
    searchFile.addActionListener(new RadioButtonAction("search file"));
    play.addActionListener(new RadioButtonAction("play"));

    bg.add(searchGroup);
    bg.add(searchFile);
    bg.add(play);

    buttonsPanel.add(searchGroup);
    buttonsPanel.add(searchFile);
    buttonsPanel.add(play);

		return buttonsPanel;
	}
	
  /*
   * genere le menu qui ne contient qu'un champ "exit" qui ferme la telecommande
   */
	private void genFileMenu(JMenu fileMenu){
		JButton exitButton = new JButton("Exit");
		exitButton.addActionListener(new ExitAction());
		fileMenu.add(exitButton);
	}

  class RadioButtonAction implements ActionListener {
    String name;
    public RadioButtonAction(String name) {
      this.name = name;
    }
     
    public void actionPerformed(ActionEvent e) {
      selectedRadioButton = name;
    }
  }
	
	class ExitAction implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			System.exit(0);			
		}
	}
	
  private String send(String command) {
    return client.send(command);
  }
}
