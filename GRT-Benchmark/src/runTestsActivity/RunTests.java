package runTestsActivity;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import storage.Storage;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.app.ActionBarActivity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;

import com.licenta.grt_benchmark.GRTwrapper;
import com.licenta.grt_benchmark.R;

public class RunTests extends ActionBarActivity {
	private static boolean startedTesting = false;
	
	/* Tags for intent parameters */ 
	public static String datasetTag = "datasetTag";
	public static String usecaseTag = "usecaseTag";
	public static String pipelineTag = "pipelineTag";
	
	/* Input parameters for test */
	String pipeline = null;
	String usecase = null;
	String dataset = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_run_tests);

		if (savedInstanceState == null) {
			getSupportFragmentManager().beginTransaction()
					.add(R.id.container, new PlaceholderFragment()).commit();
		}
		
		if (RunTests.startedTesting) {
			return;
		}
		startedTesting = true;
		
		/* Get intent parameters */
		if ( (pipeline == null) || (usecase == null) || (dataset == null) ) {
			Intent testIntent = getIntent();
			this.pipeline = testIntent.getStringExtra(pipelineTag);
			this.usecase = testIntent.getStringExtra(usecaseTag);
			this.dataset = testIntent.getStringExtra(datasetTag);
		}
		
		/* Use full path for input and output */
		/* Generate filename for output*/
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd-HH:mm:ss", Locale.ROOT);
		String currentDateandTime = dateFormat.format(new Date());
		final String ResultFolderPath = Storage.getRootPath() + this.usecase +
				"/" + this.pipeline + "/" + currentDateandTime;
		
		this.dataset = Storage.getDatasetPath() + this.dataset;
		
		/* Run test on another thread */
		new Thread(new Runnable() {
			  @Override
			  public void run()
			  {
				  GRTwrapper.test(
						  RunTests.this.dataset,
						  ResultFolderPath,
						  RunTests.this.usecase,
						  RunTests.this.pipeline
				  );
				  RunTests.startedTesting = false;
			  }
		}).start();
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.run_tests, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {

		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_run_tests,
					container, false);
			return rootView;
		}
	}

}
