package stryku.homediscserver;

import android.app.Service;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

public class ServerService extends Service {
    public static final int MSG_EVENT = 1;

    private boolean started = false;

    private Messenger messenger = null;
    private ServerManager serverManager = new ServerManager();
    private Handler importantEventsHandler;

    public ServerService() {
    }

    private void initImportantEventsHandler() {
        importantEventsHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                if(messenger != null) {
                    String msgFromServer = (String)msg.obj;
                    Bundle b = new Bundle();
                    b.putString("data", msgFromServer);
                    Message msgToUI = Message.obtain(null, MSG_EVENT);
                    msgToUI.setData(b);

                    try {
                        messenger.send(msgToUI);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                        Log.d("MYDEB", "Activity aka client is dead");
                    }
                }
                //todo messenger
                super.handleMessage(msg);
            }
        };
    }

    @Override
    public void onCreate() {
        initImportantEventsHandler();
        serverManager.addImportantEventHandler(importantEventsHandler);
        Log.d("MYDEB", "ServerService created");
        onStartCommand(null, 0, 0);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if(!started) {
            if(intent != null)
                messenger = (Messenger)intent.getParcelableArrayListExtra("MESSENGER").get(0);
            
            serverManager.turnOn();
            Log.d("MYDEB", "ServerService started");
            started = true;
        }

        return Service.START_STICKY;
    }

    public void onDestroy() {
        serverManager.turnOff();
        started = false;
        Log.d("MYDEB", "ServerService stopped");
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }
}
