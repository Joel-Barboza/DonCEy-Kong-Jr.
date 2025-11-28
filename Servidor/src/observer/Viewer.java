package observer;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Viewer implements Subscriber {
    private final SubscriberType type = SubscriberType.VIEWER;
    private final Socket socket;
    private final BufferedReader reader;
    private final PrintWriter writer;

    public Viewer(Socket socket, BufferedReader reader,PrintWriter writer) throws IOException {
        this.socket = socket;

//        this.reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//        this.writer = new PrintWriter(socket.getOutputStream(), true);
        this.reader = reader;
        this.writer = writer;
    }

    @Override
    public String getName() { return null; }

    @Override
    public Socket getSocket() { return socket; }

    @Override
    public PrintWriter getWriter() { return writer; }

    @Override
    public BufferedReader getReader() { return reader; }

    @Override
    public SubscriberType getType() {
        return type;
    }
}


