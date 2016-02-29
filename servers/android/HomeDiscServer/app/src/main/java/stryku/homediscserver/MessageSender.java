package stryku.homediscserver;

import android.util.Log;

import org.zeromq.ZMQ;
import org.zeromq.ZMsg;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * Created by stryku on 14.02.16.
 */
public class MessageSender implements Runnable {
    ZMQ.Socket router;
    LinkedBlockingQueue<PersonalMessage> messagesToSend = new LinkedBlockingQueue<>();

    @Override
    public void run()
    {
        try {
            while(true) {
                ZMsg msg = new ZMsg();
                Log.d("MYDEB", "Taking message to send from queue");
                PersonalMessage pmsg = messagesToSend.take();

                msg.push(pmsg.msg);
                msg.wrap(pmsg.identity);

                msg.send( router );
            }
        } catch (InterruptedException e) {
            Log.d("MYDEB", "MessageSender taking message interrupted");
            return;
        }
    }

    public void setRouter(ZMQ.Socket router) {
        this.router = router;
    }

    public void newMessageToSend( PersonalMessage msg ) {
        try {
            messagesToSend.put( msg );
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

}
