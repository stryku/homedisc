package stryku.homediscserver;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.EventObject;

/**
 * Created by stryku on 14.02.16.
 */
public class ImportantEvent {
    private String msg;

    ImportantEvent(String msg) {
        SimpleDateFormat sdf = new SimpleDateFormat("dd.MM.yyyy HH:mm:ss");
        this.msg = sdf.format(new Date()) + " " +  msg;
    }

    public String getMsg() {
        return msg;
    }
}
