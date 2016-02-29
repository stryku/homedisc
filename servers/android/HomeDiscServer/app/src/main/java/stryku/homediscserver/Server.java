package stryku.homediscserver;


import android.os.Handler;
import android.util.Log;

import org.zeromq.ZContext;
import org.zeromq.ZMQ;
import org.zeromq.ZMQ.PollItem;
import org.zeromq.ZMQ.Socket;
import org.zeromq.ZMQ.Poller;
import org.zeromq.ZMsg;

/**
 * Created by stryku on 14.02.16.
 */
public class Server implements Runnable {
    private final static int HEARTBEAT_INTERVAL = 1000;

    private ZContext context = new ZContext( 1 );
    private Socket router;
    private MessageSender sender = new MessageSender();
    private RequestHandler requestHandler = new RequestHandler();

    public Server()
    {
        router = context.createSocket(ZMQ.ROUTER);
    }

    @Override
    public void run()
    {
        Log.d("MYDEB", "Server started");
        router.bind( String.format( "tcp://*:%d", Settings.getPort() ) );
        ZMsg identity;
        Thread senderThread;
        Thread requestHandlerThread;

        sender.setRouter(router);
        requestHandler.setSender(sender);

        senderThread = new Thread(sender);
        requestHandlerThread = new Thread(requestHandler);

        senderThread.start();
        requestHandlerThread.start();

        while(!Thread.interrupted()) {
            PollItem items[] = {new PollItem(router, Poller.POLLIN)};
            int rc = ZMQ.poll(items, HEARTBEAT_INTERVAL);
            if(rc == -1)
                break;

            if(items[0].isReadable()) {
                identity = ZMsg.recvMsg(router);

                if(identity == null)
                    Log.d("MYDEB", "Received null message");
                else {
                    Log.d("MYDEB", identity.toString());

                    PersonalMessage request = new PersonalMessage(identity.duplicate());

                    requestHandler.newRequest(request);
                }
            }
        }

        Log.d("MYDEB", "Interrupting sender and request handler threads");
        senderThread.interrupt();
        requestHandlerThread.interrupt();

        try {
            senderThread.join();
            Log.d("MYDEB", "Succeed joining sender thread");
        } catch (InterruptedException e) {
            Log.d("MYDEB", "Failed joining sender thread");
            e.printStackTrace();
        }
        try {
            requestHandlerThread.join();
            Log.d("MYDEB", "Succeed joining request handler thread");
        } catch (InterruptedException e) {
            Log.d("MYDEB", "Failed joining request handler thread");
            e.printStackTrace();
        }

        Log.d("MYDEB", "Server thread stopped correctly");
    }

    public void addImportantEventHandler(Handler handler) {
        requestHandler.addEventHandler(handler);
    }
}
