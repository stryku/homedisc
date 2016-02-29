package stryku.homediscserver;

import android.media.audiofx.BassBoost;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.ActionBarActivity;
import android.text.format.Formatter;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import java.io.File;
import java.util.ArrayList;

public class MainActivity extends ActionBarActivity {
    static int MAX_EVENTS = 1000;

    ArrayList<String> events = new ArrayList<String>();
    ArrayAdapter<String> adapter;
    private Thread t;
    private Server server;
    private Handler importantEventsHandler;


    @Override
    protected void onCreate(Bundle savedInstanceState) {//TODO move server to another method, thread joining
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        adapter= new ArrayAdapter<>(this,
                                    android.R.layout.simple_list_item_1,
                                    events);

        initImportantEventsHandler();

        ListView lv = (ListView)findViewById(R.id.listViewLatestEvents);
        lv.setAdapter(adapter);

        setIpAndPort();

        server = new Server();
        server.addImportantEventHandler(importantEventsHandler);

        t = new Thread(server);
        t.start();

        File file = new File(Settings.getMainFolderPath());
        file.mkdirs();
    }

    private void initImportantEventsHandler() {
        importantEventsHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                addEventToList((String)msg.obj);
                super.handleMessage(msg);
            }
        };
    }


    public String getLocalIpAddress()
    {
        try
        {
            WifiManager wifiMgr = (WifiManager) getSystemService(WIFI_SERVICE);
            WifiInfo wifiInfo = wifiMgr.getConnectionInfo();
            int ip = wifiInfo.getIpAddress();

            return Formatter.formatIpAddress(ip);
        }
        catch (Exception ex)
        {
            ex.printStackTrace();
        }
        return null;
    }

    private void setIpAndPort()
    {
        String ip = getLocalIpAddress();
        int port = Settings.getPort();
        TextView tv = (TextView) findViewById(R.id.textViewIpAndPort);

        tv.setText(String.format("%s:%d", ip, port));
    }

    public void addEventToList( String eventDescription )
    {
        if( events.size() == MAX_EVENTS )
            events.remove( events.size() - 1 );

        ListView lv = (ListView)findViewById(R.id.listViewLatestEvents);

        ArrayAdapter<String> ad = (ArrayAdapter<String>) lv.getAdapter();

        ad.insert(eventDescription, 0);
    }
}
