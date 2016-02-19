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

public class MainActivity extends ActionBarActivity implements ImportantEventListener {
    static int MAX_EVENTS = 1000;

    ArrayList<String> events = new ArrayList<String>();
    ArrayAdapter<String> adapter;
    private Thread t;
    private Server server;
    private Handler importantEventsHandler;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        adapter=new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1,
                events);

        initImportantEventsHandler();

        ListView lv = (ListView)findViewById(R.id.listViewLatestEvents);
        lv.setAdapter(adapter);

        addEventToList("siema");

        setIpAndPort();

        server = new Server();
        server.addImportantEventHandler(importantEventsHandler);
        System.out.print("siema");

        t = new Thread(server);
        t.start();

        File file = new File(Environment.getExternalStorageDirectory() + "/HomeDiscServer/");
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

        tv.setText( String.format( "%s:%d", ip, port ) );
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void addEventToList( String eventDescription )
    {
        if( events.size() == MAX_EVENTS )
            events.remove( events.size() - 1 );

        ListView lv = (ListView)findViewById(R.id.listViewLatestEvents);

        ArrayAdapter<String> ad = (ArrayAdapter<String>) lv.getAdapter();

        ad.insert(eventDescription, 0);
        //events.add( 0, eventDescription );
    }

    @Override
    public void handleEvent(ImportantEvent event) {
        addEventToList(event.getMsg());
    }
}
