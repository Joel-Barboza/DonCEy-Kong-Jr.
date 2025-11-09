import GUI.AdminPanel;
import observer.Server;

public class Main {
    public static void main(String[] args) {
        AdminPanel panel = new AdminPanel();
        panel.createAndShowGUI();
        Server server = Server.getInstance();
        server.startServer();

//        server.addSubscriber();
//        server.addSubscriber();

//        server.stopServer();

    }
}