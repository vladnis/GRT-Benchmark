package manageDatasets;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.licenta.grt_benchmark.R;

public class ShowDrawing extends Activity {
	private InveresedDrawView inveredView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_show_drawing);

	}
	
	@Override
	protected void onStart() {
		super.onStart();
		RelativeLayout drawLayout = (RelativeLayout)findViewById(R.id.activityDraw);
		if (drawLayout != null) {
			inveredView = new InveresedDrawView(this);
			inveredView.setBackgroundColor(Color.WHITE);
			drawLayout.addView(inveredView);
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		return true;
	}
	
	public void restartCapture(View view) {
		finish();
	}

	public void saveGesture(View view) {
		String datasetName = ManageDatasetsActivity.datasetName;
		String classId = ((EditText)this.findViewById(R.id.classId)).getText().toString();
		
		try {
		    PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(datasetName, true)));
		    
			out.println("************TIME_SERIES************");
			out.println("ClassID: " + classId);
			out.println("TimeSeriesLength: " + Integer.toString(DrawView.coordinates.size()));
			out.println("TimeSeriesData:");
			
			for (int i = 0; i < DrawView.coordinates.size(); i++) {
				Float x = DrawView.coordinates.get(i).x;
				Float y = DrawView.coordinates.get(i).y;
				out.println(Float.toString(x) + "\t" + Float.toString(y));
			}

		    out.close();
		} catch (IOException e) {
			Toast toast = Toast.makeText(this, "There was a problem adding gestures", Toast.LENGTH_SHORT);
			toast.show();
		    return;
		}
        
        String line;
        FileReader fr;
		try {
	        File file = new File(datasetName + ".aux");
	        file.createNewFile();
	        FileWriter fw = new FileWriter(file);
	        BufferedWriter bw = new BufferedWriter(fw);
			
			fr = new FileReader(datasetName);
	        BufferedReader br = new BufferedReader(fr);

	        Integer pos = 1;
	        while( (line = br.readLine()) != null ){
	            if(line != null){
	            	if (pos == 5) {
	            		String[] line_split = line.split(" ");
	            		Integer numSample = Integer.valueOf(line_split[1]) + 1;
	            		Log.d("GRT TEST", Integer.toString(numSample));
	            		line = "TotalNumTrainingExamples: " + Integer.toString(numSample);
	            	}
	            }
	            pos++;
	            bw.write(line + "\n");
	        }
	        
	        bw.flush();
	        bw.close();
	        br.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		File fileDelete = new File(datasetName);
		fileDelete.delete();
		
		File file = new File(datasetName + ".aux");

	    // File (or directory) with new name
	    File file2 = new File(datasetName);
	    // Rename file (or directory)
	    file.renameTo(file2);
		
		Toast toast = Toast.makeText(this, "Gesture saved to dataset " + datasetName, Toast.LENGTH_SHORT);
		toast.show();
		
		finish();
	}
}
