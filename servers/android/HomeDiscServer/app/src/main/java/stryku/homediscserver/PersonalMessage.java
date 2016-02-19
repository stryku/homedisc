package stryku.homediscserver;

import org.zeromq.ZFrame;
import org.zeromq.ZMQ;
import org.zeromq.ZMsg;

/**
 * Created by stryku on 14.02.16.
 */
public class PersonalMessage {

    public ZFrame identity;
    public String msg;

    PersonalMessage(ZMsg msg) {
        this.identity = msg.unwrap();

       // ZFrame strMsg = msg.pop();

        //if(strMsg != null)
        this.msg = new String(msg.getFirst().getData(), ZMQ.CHARSET);
    }

    PersonalMessage(ZFrame identity, String msg) {
        this.identity = identity;
        this.msg = msg;
    }

    public String toString()
    {
        return "Id: " + identity.toString() + ", msg: " + msg;
    }
}
