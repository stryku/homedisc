package stryku.homediscserver;

import android.os.Handler;
import android.util.Log;

/**
 * Created by stryku on 29.02.16.
 */
public class ServerManager {
    private Server server = new Server();
    private Thread t = new Thread(server);
    private Handler importantEventsHandler;

    public ServerManager() {}

    public void addImportantEventHandler(Handler handler) {
        server.addImportantEventHandler(handler);
    }

    public boolean isRunning() {
        return t.isAlive();
    }

    public void turnOn() {
        if(isRunning())
            return;

        t.start();
    }

    public void turnOff() {
        if(!isRunning())
            return;

        t.interrupt();
        try {
            t.join();
            Log.d("MOJEDEB", "succeed server thread join");
        } catch (InterruptedException e) {
            Log.d("MOJEDEB", "server thread join interrupted");
            e.printStackTrace();
        }
    }
}