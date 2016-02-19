package stryku.homediscserver;

import java.util.HashMap;

/**
 * Created by stryku on 14.02.16.
 */
public class Request {
    public enum Type {
        LIST_REQ,
        FILE_REQ,
        REMOVE_FILE,
        REMOVE_DIR,
        NEW_FILE,
        NEW_DIR,
        UNDEF
    }

    public Type type;
    HashMap<String, String> values;
}
