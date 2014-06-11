package dialogFragments;

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

public class ListDialog extends DialogFragment {
	public final static String useCaseTag = "use-case-dialog";
	public final static String pipelineTag = "pipeline-dialog";
	public final static String datasetTag = "dataset-dialog";

	public static Integer selectedUseCase = null;
	public static Integer selectedPipeline = null;
	
	final ProgressDialog progress = null;
	
	@Override
	public Dialog onCreateDialog(Bundle savedInstanceState) {
		final String currentTag =  this.getTag();
		
		int title = 0, negativeButton = 0;
		String[] listItems = null;
		
		switch (currentTag) {
			case ListDialog.useCaseTag:
				title = R.string.choose_use_case;
				listItems = Storage.getUseCases();
				negativeButton = R.string.cancel;
				break;
			case ListDialog.pipelineTag:
				title = R.string.choose_pipeline;
				listItems = Storage.getPepelines(ListDialog.selectedUseCase);
				negativeButton = R.string.back;
				break;
			case ListDialog.datasetTag:
				title = R.string.choose_dataset;
				listItems = Storage.getDatasets(ListDialog.selectedUseCase, getActivity());
				negativeButton = R.string.back;
				break;
			default:
				this.dismiss();
		}
		
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());

        builder.setTitle(title)
               .setNegativeButton(negativeButton, new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface dialog, int id) {
                	   switch (currentTag) {
	           			case ListDialog.pipelineTag:
	           			    ListDialog d1 = new ListDialog();
	                 	    d1.show(getFragmentManager(), ListDialog.useCaseTag);
	           				break;
	           			case ListDialog.datasetTag:
	           			    ListDialog d2 = new ListDialog();
	                 	    d2.show(getFragmentManager(), ListDialog.pipelineTag);
	           				break;
                	   }
                   }
               })
               .setItems(listItems, new DialogInterface.OnClickListener() {
            	   public void onClick(DialogInterface dialog, int which) {
            			switch (currentTag) {
            				case ListDialog.useCaseTag:
            					selectedUseCase = which;
            					ListDialog d1 = new ListDialog();
                     		   	d1.show(getFragmentManager(), ListDialog.pipelineTag);
            					break;
            				case ListDialog.pipelineTag:
                     		   	selectedPipeline  = which;
                     		   	ListDialog d2 = new ListDialog();
                     		   	d2.show(getFragmentManager(), ListDialog.datasetTag);
            					break;
            				case ListDialog.datasetTag:
            					
            					String usecase = Storage.getUseCases()[selectedUseCase];
            					String pipeline = Storage.getPepelines(selectedUseCase)[selectedPipeline];
            					String datasetFile = Storage.getDatasets(selectedUseCase, getActivity())[which];
            					
            					/* Start new test */
            					Intent testIntent = new Intent(getActivity(), RunTests.class);
            					testIntent.putExtra(RunTests.datasetTag, datasetFile);
            					testIntent.putExtra(RunTests.usecaseTag, usecase);
            					testIntent.putExtra(RunTests.pipelineTag, pipeline);
            					startActivity(testIntent);
            					break;
            			}
		           }
			    });

        // Create the AlertDialog object and return it
        return builder.create();
	}
}
