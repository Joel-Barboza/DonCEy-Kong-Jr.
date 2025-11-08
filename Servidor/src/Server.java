import java.io.*;
import java.net.*;

public class Server {
    private static Server instance = null;
    private static final int PORT = 8080;
    private ServerSocket serverSocket;
    private boolean isRunning;

    private Server() {
        System.out.println("Iniciando server");
        isRunning = false;
    }

    public static Server getInstance() {
        if (instance == null) {
            instance = new Server();
        }
        return instance;
    }

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
            try (Socket clientSocket = serverSocket.accept();
                 BufferedReader in = new BufferedReader(
                         new InputStreamReader(clientSocket.getInputStream()));
                 PrintWriter out = new PrintWriter(
                         clientSocket.getOutputStream(), true)) {

                System.out.println("Cliente conectado: " +
                        clientSocket.getInetAddress().getHostAddress());

                // Enviar mensaje de bienvenida
                out.println("Bienvenido al servidor! Escribe 'quit' para salir");

                handleClientCommunication(in, out);

                System.out.println("Cliente desconectado");

            } catch (IOException e) {
                if (isRunning) {
                    System.out.println("Error con cliente: " + e.getMessage());
                }
            }
        }
    }

    private void handleClientCommunication(BufferedReader in, PrintWriter out) {
        try {
            String inputLine;
            while ((inputLine = in.readLine()) != null) {
                System.out.println("Cliente dice: " + inputLine);

                if ("quit".equalsIgnoreCase(inputLine)) {
                    out.println("Adios!");
                    break;
                }

                // Responder al cliente
                out.println("Servidor recibio: " + inputLine);
            }
        } catch (IOException e) {
            System.out.println("Error en comunicación con cliente: " + e.getMessage());
        }
    }


//    public static void main(String[] args) {
//        Server server = Server.getInstance();
//        server.startServer();
//
//        // server.stopServer();
//    }
}