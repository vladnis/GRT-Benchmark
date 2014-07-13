package dialogFragments;

import java.util.ArrayList;
import java.util.List;

import manageDatasets.CreateDatasetActivity;
import manageDatasets.ManageDatasetsActivity;
import runTestsActivity.RunTests;
import storage.Storage;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;

import com.licenta.grt_benchmark.R;

public class manageDatasetDialog extends DialogFragment {
	
	final ProgressDialog progress = null;
	
	@Override
	public Dialog onCreateDialog(Bundle savedInstanceState) {
		
		int title = 0, negativeButton = 0;
		String[] listItems = null;

		title = R.string.choose_dataset;
		listItems = Storage.getDatasets(0, getActivity());
		negativeButton = R.string.cancel;
		
		List<String> list = new ArrayList<String>();
		for (int i = 0; i < listItems.length; i++) {
			list.add(listItems[i]);
		}
		list.add("Create new dataset");
		final String[] listItemsAux =  new String[ list.size() ];
		list.toArray(listItemsAux);
		
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());

        builder.setTitle(title)
               .setNegativeButton(negativeButton, new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface dialog, int id) {
                   }
               })
               .setItems(listItemsAux, new DialogInterface.OnClickListener() {
            	   public void onClick(DialogInterface dialog, int which) {
            		   if (which == (listItemsAux.length - 1)) {
            			   Intent testIntent = new Intent(getActivity(), CreateDatasetActivity.class);
            			   startActivity(testIntent);
            		   }
            		   else {
            			   String datasetFile = Storage.getDatasets(0, getActivity())[which];
            			   Intent testIntent = new Intent(getActivity(), ManageDatasetsActivity.class);
            			   testIntent.putExtra(RunTests.datasetTag, datasetFile);
            			   startActivity(testIntent);
            		   }
		           }
			    });

        // Create the AlertDialog object and return it
        return builder.create();
	}
}
