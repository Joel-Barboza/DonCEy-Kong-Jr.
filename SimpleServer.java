import java.io.*;
import java.net.*;

public class SimpleServer {
    private static final int PORT = 8080;
    
    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Servidor iniciado en puerto " + PORT);
            System.out.println("Esperando conexiones...");
            
            while (true) {
                try (Socket clientSocket = serverSocket.accept();
                     BufferedReader in = new BufferedReader(
                         new InputStreamReader(clientSocket.getInputStream()));
                     PrintWriter out = new PrintWriter(
                         clientSocket.getOutputStream(), true)) {
                    
                    System.out.println("Cliente conectado: " + 
                                     clientSocket.getInetAddress().getHostAddress());
                    
                    // Enviar mensaje de bienvenida
                    out.println("Bienvenido al servidor! Escribe 'quit' para salir");
                    
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
                    
                    System.out.println("Cliente desconectado");
                    
                } catch (IOException e) {
                    System.out.println("Error con cliente: " + e.getMessage());
                }
            }
            
        } catch (IOException e) {
            System.err.println("Error al iniciar servidor: " + e.getMessage());
        }
    }
}

