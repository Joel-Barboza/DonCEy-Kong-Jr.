package observer;

import java.io.BufferedReader;
import java.io.PrintWriter;
import java.net.Socket;


public interface Subscriber {
    String getName();
    Socket getSocket();
    PrintWriter getWriter();   // For sending messages
    BufferedReader getReader(); // Optional, if needed
    SubscriberType getType();
}


