package observer;

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import org.json.JSONObject;

public class Server {
    // singleton instance
    private static Server instance = null;
    // Observer subscribers
    private final ArrayList<Subscriber> subscribers = new ArrayList<Subscriber>();

    // server parameters
    private static final int PORT = 8080;
    private ServerSocket serverSocket;
    private boolean isRunning;


    //-----------------------------------------------------
    //                 Singleton server
    //-----------------------------------------------------
    public Server() {
        System.out.println("Iniciando server");
        isRunning = false;
    }

    public static Server getInstance() {
        if (instance == null) {
            instance = new Server();
        }
        return instance;
    }


    //-----------------------------------------------------
    //                 Observer methods
    //-----------------------------------------------------
    public void addSubscriber(Subscriber subscriber) {
        subscribers.add(subscriber);
    }

    public void removeSubscriber(Subscriber subscriber) {
        subscribers.remove(subscriber);
    }

    public void notifySubscribers() {
//        subscribers.forEach(subscriber -> subscriber.update(String.valueOf(new ArrayList<>())));
    }



    //-----------------------------------------------------
    //                 Server methods
    //-----------------------------------------------------
    public void startServer() {
        if (isRunning) {
            System.out.println("El servidor ya está en ejecución");
            return;
        }

        try {
            serverSocket = new ServerSocket(PORT);
            isRunning = true;
            System.out.println("Servidor iniciado en puerto " + PORT);
            System.out.println("Esperando conexiones...");

            runServerLoop();

        } catch (IOException e) {
            System.err.println("Error al iniciar servidor: " + e.getMessage());
        }
    }

    public void stopServer() {
        if (!isRunning) {
            System.out.println("El servidor no está en ejecución");
            return;
        }

        try {
            isRunning = false;
            if (serverSocket != null && !serverSocket.isClosed()) {
                serverSocket.close();
            }
            System.out.println("Servidor detenido");
        } catch (IOException e) {
            System.err.println("Error al detener servidor: " + e.getMessage());
        }
    }

    public boolean isRunning() {
        return isRunning;
    }

    private void runServerLoop() {
        while (isRunning) {
            try {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Cliente conectado: " +
                        clientSocket.getInetAddress().getHostAddress());

                // Create new thread for this client
                new Thread(() -> handleClient(clientSocket)).start();

            } catch (IOException e) {
                if (isRunning) {
                    System.out.println("Error con cliente: " + e.getMessage());
                }
            }
        }
    }


//    private void handleClientCommunication(BufferedReader in, PrintWriter out) {
//        try {
//            String inputLine;
//            while ((inputLine = in.readLine()) != null) {
//                System.out.println("Cliente dice: " + inputLine);
//
//                if ("quit".equalsIgnoreCase(inputLine)) {
//                    out.println("Adios!");
//                    break;
//                }
//
//                // Responder al cliente
//                out.println("Servidor recibio: " + inputLine);
//            }
//        } catch (IOException e) {
//            System.out.println("Error en comunicación con cliente: " + e.getMessage());
//        }
//    }

    private void handleClient(Socket clientSocket) {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
             PrintWriter writer = new PrintWriter(clientSocket.getOutputStream(), true)) {

            writer.println("Bienvenido al servidor! Escribe 'quit' para salir");

            String inputLine;
            while ((inputLine = reader.readLine()) != null) {
                System.out.println("Cliente dice: " + inputLine);
                JSONObject obj = new JSONObject(inputLine);

                if (obj.has("name") && Objects.equals(obj.getString("type"), "PLAYER")) {
                    String name = obj.getString("name");
                    System.out.println("Name = " + name);
                    Player player = new Player(name, clientSocket, reader, writer);
                    subscribers.add(player);
                }

                if ("quit".equalsIgnoreCase(inputLine)) {
                    writer.println("Adios!");
                    break;
                }

                writer.println("Servidor recibió: " + inputLine);
            }

        } catch (IOException e) {
            System.out.println("Error en comunicación con cliente: " + e.getMessage());
        }
    }

    public ArrayList<Subscriber> getSubsList() {
        return subscribers;
    }


//    public static void main(String[] args) {
//        observer.Server server = observer.Server.getInstance();
//        server.startServer();
//
//        // server.stopServer();
//    }
}