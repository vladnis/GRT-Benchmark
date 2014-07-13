package manageDatasets;


import runTestsActivity.RunTests;
import storage.Storage;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;

import com.licenta.grt_benchmark.R;

public class ManageDatasetsActivity extends Activity {
	private DrawView drawView;
	static final int DRAW_REQUEST = 1;
	public static String datasetName = null;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);        
        setContentView(R.layout.activity_manage_datasets_acticity);

        Intent testIntent = getIntent();
        datasetName = Storage.getDatasetPath() + testIntent.getStringExtra(RunTests.datasetTag);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        return true;
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // Check which request we're responding to
        if (requestCode == DRAW_REQUEST) {
			Button startButton = (Button)findViewById(R.id.startButton);
			startButton.setVisibility(View.VISIBLE);
			
			drawView.setVisibility(View.INVISIBLE);
			
			Button showButton = (Button)findViewById(R.id.showButton);
			showButton.setVisibility(View.INVISIBLE);
        }
    }
    

    public void startCapture(View view) {
		RelativeLayout mainLayout = (RelativeLayout)findViewById(R.id.activityMain);
		if (mainLayout != null) {

			drawView = new DrawView(this);
			drawView.setBackgroundColor(Color.WHITE);

			mainLayout.addView(drawView);
			
			Button startButton = (Button)findViewById(R.id.startButton);
			startButton.setVisibility(View.INVISIBLE);
			
			Button showButton = (Button)findViewById(R.id.showButton);
			showButton.setVisibility(View.VISIBLE);
		}
    }
    
    public void showCapture(View view) {
    	Intent intent = new Intent(this, ShowDrawing.class);
    	startActivityForResult(intent, DRAW_REQUEST);
    }
    
}