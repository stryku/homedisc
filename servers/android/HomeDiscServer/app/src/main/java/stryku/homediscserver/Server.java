package stryku.homediscserver;


import android.os.Handler;
import android.util.Log;

import org.zeromq.ZContext;
import org.zeromq.ZMQ;
import org.zeromq.ZMsg;

/**
 * Created by stryku on 14.02.16.
 */
public class Server implements Runnable {
    private ZContext context = new ZContext( 1 );
    private ZMQ.Socket router;
    private MessageSender sender = new MessageSender();
    private RequestHandler requestHandler = new RequestHandler();

    public Server()
    {
        router = context.createSocket(ZMQ.ROUTER);
    }

    @Override
    public void run()
    {
        System.out.print("siema");
        router.bind( String.format( "tcp://*:%d", Settings.getPort() ) );
        ZMsg identity = new ZMsg();
        Thread senderThread;
        Thread requestHandlerThread;
        String msg;

        sender.setRouter(router);
        requestHandler.setSender(sender);

        senderThread = new Thread(sender);
        requestHandlerThread = new Thread(requestHandler);

        senderThread.start();
        requestHandlerThread.start();

        while(true) {
            identity = ZMsg.recvMsg(router);
            //msg = router.recvStr();

            Log.d("DEB", identity.toString());

            PersonalMessage request = new PersonalMessage(identity.duplicate());

            requestHandler.newRequest(request);
        }
    }

    public void addImportantEventHandler(Handler handler) {
        requestHandler.addEventHandler(handler);
    }
}
