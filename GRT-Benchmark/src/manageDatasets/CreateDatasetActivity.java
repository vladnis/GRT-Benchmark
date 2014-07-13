package manageDatasets;

import storage.Storage;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.app.ActionBarActivity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.RadioButton;

import com.licenta.grt_benchmark.R;

public class CreateDatasetActivity extends ActionBarActivity {
	Integer usecase = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setTitle("Create new dataset");
		setContentView(R.layout.activity_create_dataset);

		if (savedInstanceState == null) {
			getSupportFragmentManager().beginTransaction()
					.add(R.id.container, new PlaceholderFragment()).commit();
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		// getMenuInflater().inflate(R.menu.create_dataset, menu);
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
			View rootView = inflater.inflate(R.layout.fragment_create_dataset,
					container, false);
			return rootView;
		}
	}
	
	public void saveDataset(View view) {
		String datasetName = ((EditText)this.findViewById(R.id.datasetName)).getText().toString();
		String description = ((EditText)this.findViewById(R.id.description)).getText().toString();
		
		/* Create new assets file */
		boolean isSuccesfull = Storage.createNewDataset(datasetName, description, usecase, this);
		
		if (!isSuccesfull) {
			return;
		}
		
		this.finish();
	}
	
	public void onRadioButtonClicked(View view) {
	    // Is the button now checked?
	    boolean checked = ((RadioButton) view).isChecked();
	    
	    switch(view.getId()) {
	        case R.id.continous:
	            if (checked)
	            	usecase = 1; 
	            break;
	        case R.id.segmented:
	            if (checked)
	            	usecase = 0;
	            break;
	    }
	}

}
