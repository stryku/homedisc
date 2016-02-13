package stryku.homediscserver;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import org.zeromq.ZContext;

import java.util.ArrayList;


public class MainActivity extends ActionBarActivity {

    static int MAX_EVENTS = 1000;

    ArrayList<String> events = new ArrayList<String>();
    ArrayAdapter<String> adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        adapter=new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1,
                events);

        ListView lv = (ListView)findViewById(R.id.listViewLatestEvents);
        lv.setAdapter( adapter );
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

        events.add( 0, eventDescription );
    }
}
