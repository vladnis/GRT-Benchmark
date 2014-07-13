package storage;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

public class Storage {
	
	private static String DatasetsFolder = "datasets";
	
	private static String RootFolder = "/GRT";
	
	/* Use cases */
	private static String[] useCaseNames = {
		"Segmented gestures", "Continous stream"
	};

	/* Associated pipelines */
	private static final  String[] pipelines = {
		"Dynamic-time-warping",
		"Hidden-Markov-Models"
	};
	
	private static void copyFile(InputStream in, OutputStream out) throws IOException {
	    byte[] buffer = new byte[1024];
	    int read;
	    while((read = in.read(buffer)) != -1){
	      out.write(buffer, 0, read);
	    }
	}
	
	private static void copyAssets(Context ctx, String FolderPath) {
	    AssetManager assetManager = ctx.getAssets();
	    String[] files = null;
	    try {
	        files = assetManager.list(DatasetsFolder);
	    } catch (IOException e) {
	        Log.e("tag", "Failed to get asset file list.", e);
	    }
	    for(String filename : files) {
	        InputStream in = null;
	        OutputStream out = null;
	        try {
	          in = assetManager.open(DatasetsFolder + "/" + filename);
	          File outFile = new File(FolderPath, filename);
	          out = new FileOutputStream(outFile);
	          copyFile(in, out);
	          in.close();
	          in = null;
	          out.flush();
	          out.close();
	          out = null;
	        } catch(IOException e) {
	            Log.e("tag", "Failed to copy asset file: " + filename, e);
	        }       
	    }
	}
	
	/**
	 * Get absolute path to GRT folder root on sdcard
	 */
	public static String getRootPath() {
		return Environment.getExternalStorageDirectory().getAbsolutePath() + RootFolder + "/";
	}
	
	/**
	 * Get absolute path to datasets folder
	 */
	public static String getDatasetPath() {
		return getRootPath() + DatasetsFolder + "/";
	}
	
	/**
	 * Create GRT folder structure if it does not already exists.
	 */
	public static boolean createDirStruct(Context ctx) {
		Log.d("GRT", "Creating folder structure.");	

		String absolutePath = getRootPath();
		File file = null;

		// Create results folders
		for (int i = 0; i < pipelines.length; i++) {
			String useCaseFolderPath = absolutePath + useCaseNames[i] + "/";
			for (int j = 0; j < pipelines.length; j++) {
				file = new File(useCaseFolderPath + pipelines[j]);
			    if (!file.mkdirs()) {
			        Log.d("Build dir struct", "Folder: " + file + " was not created.");
			    }
			    else {
			    	Log.d("Build dir struct", "Folder: " + file + " was created.");
			    }
			}
	    }
		
		// Create datasets folder
		String datasetFolderPath = absolutePath + "datasets";
		file = new File(datasetFolderPath);
		if (!file.mkdir()) {
			Log.d("Build dir struct", "Dataset folder not created");
		}
		
		// Copy datasets from assets to sdcard
		copyAssets(ctx, datasetFolderPath);
		
	    return true;
	}

	/**
	 * Get list of all Use cases defined.
	 */
	public static String[] getUseCases() {
		return useCaseNames;
	}
	
	/**
	 * Get list of all pipelines defined for Use case
	 * @param useCaseKey The position in "useCaseNames" of the use case.
	 */
	public static String[] getPepelines() {
		return Storage.pipelines;
	}
	
	/**
	 * Get all the datasets for use case.
	 */
	public static String[] getDatasets(Integer useCaseKey, Context ctx) {
		ArrayList<String> datasetNames = new ArrayList<String>();
		
		
		String DatasetExternPath = getRootPath() + DatasetsFolder;
		
		File f = new File(DatasetExternPath);        
		File file[] = f.listFiles();
		
		for (int i=0; i < file.length; i++)
		{
			datasetNames.add(file[i].getName());
		}
		String[] auxArray = new String[datasetNames.size()];
		auxArray = datasetNames.toArray(auxArray);
		return auxArray;
	}
	
	public static boolean createNewDataset(String datasetName, String description, Integer usecase, Context context) {
		if (datasetName.length() == 0) {
			Toast toast = Toast.makeText(context, "Invalid dataset name", Toast.LENGTH_SHORT);
			toast.show();
			return false;
		}
		String filePath = getRootPath() + DatasetsFolder + "/" + Integer.toString(usecase) + "-" + datasetName;
		File file = new File(filePath);
	    try {
			file.createNewFile();
			FileWriter fw;
			try {
				fw = new FileWriter(file);
		        BufferedWriter bw = new BufferedWriter(fw);
		        bw.write("GRT_LABELLED_CLASSIFICATION_DATA_FILE_V1.0\n");
		        bw.write("DatasetName: "  + datasetName + "\n");
		        bw.write("InfoText: "  + description + "\n");
		        bw.write("NumDimensions: 3\n");
		        bw.write("TotalNumTrainingExamples: 0\n");
		        bw.write("NumberOfClasses: 5\n");
		        bw.write("LabelledTimeSeriesTrainingData:\n");
		        bw.flush();
		        bw.close();
			} catch (IOException e) {
				e.printStackTrace();
				return false;
			}
			Toast toast = Toast.makeText(context, "New dataset was created!", Toast.LENGTH_SHORT);
			toast.show();
		} catch (IOException e) {
			Toast toast = Toast.makeText(context, "Dataset name is already used", Toast.LENGTH_SHORT);
			toast.show();
			return false;
		}
	    return true;
	}
	
}
