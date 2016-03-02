package stryku.homediscserver;

import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Messenger;
import android.os.Parcelable;
import android.support.v7.app.ActionBarActivity;
import android.text.format.Formatter;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.ToggleButton;

import java.io.File;
import java.util.ArrayList;

public class MainActivity extends ActionBarActivity {
    static int MAX_EVENTS = 1000;

    ArrayList<String> events = new ArrayList<String>();
    ArrayAdapter<String> adapter;

    private final Messenger messenger = new Messenger(new IncomingHandler());


    class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case ServerService.MSG_EVENT:
                    addEventToList(msg.getData().getString("data"));
                default:
                    super.handleMessage(msg);
            }
        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        adapter= new ArrayAdapter<>(this,
                                    android.R.layout.simple_list_item_1,
                                    events);


        initSwitchOnOffServerListener();

        ListView lv = (ListView)findViewById(R.id.listViewLatestEvents);
        lv.setAdapter(adapter);

        setIpAndPort();

        File file = new File(Settings.getMainFolderPath());
        file.mkdirs();

    }

    private void startServerService() {

        Context context = getApplicationContext();
        Intent i= new Intent(context, ServerService.class);

        ArrayList<Parcelable> messengerArray= new ArrayList<>();
        messengerArray.add(messenger);

        i.putParcelableArrayListExtra("MESSENGER", messengerArray);
        context.startService(i);
    }

    private void stopServerService() {
        Context context = getApplicationContext();
        Intent i= new Intent(context, ServerService.class);
        context.stopService(i);
    }

    private void initSwitchOnOffServerListener() {
        ToggleButton toggle = (ToggleButton) findViewById(R.id.toggleBtnOnOffServer);
        toggle.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    startServerService();
                } else {
                    stopServerService();
                }
            }
        });
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
