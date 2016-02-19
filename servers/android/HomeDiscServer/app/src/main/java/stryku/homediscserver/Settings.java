package stryku.homediscserver;

/**
 * Created by stryku on 14.02.16.
 */
public class Settings {
    static private int port = 2393;
    static private String mainFolderPath = "";

    static public int getPort() {
        return port;
    }


    public static String getMainFolderPath() {
        return mainFolderPath;
    }
}
