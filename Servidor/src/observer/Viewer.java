package observer;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Viewer implements Subscriber {

    private final String name;
    private final Socket socket;
    private final BufferedReader reader;
    private final PrintWriter writer;

    public Viewer(String name, Socket socket) throws IOException {
        this.name = name;
        this.socket = socket;

        this.reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        this.writer = new PrintWriter(socket.getOutputStream(), true);
    }

    @Override
    public String getName() { return name; }

    @Override
    public Socket getSocket() { return socket; }

    @Override
    public PrintWriter getWriter() { return writer; }

    @Override
    public BufferedReader getReader() { return reader; }
}


