package storage;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;

public class Storage {
	
	private static String DatasetsFolder = "datasets";
	
	private static String RootFolder = "/GRT";
	
	/* Use cases */
	private static String[] useCaseNames = {"Static-postures", "Temporal-gestures", "Regression"};


	/* Pipelines for every use case */
	private static final String[] temporalGetsuresPipelines = {
		"Dynamic-time-warping",
		"Hidden-Markov-Models"
	};

	private static final String[] staticGetsuresPipelines = {
	};
	
	private static final String[] RegressionPipelines = {
	};

	/* Associated pipelines */
	private static final  ArrayList<String[]> pipelines = new  ArrayList<String[]>() {/**
		 * 
		 */
		private static final long serialVersionUID = -5242154802737194462L;

	{
	    add(staticGetsuresPipelines);
	    add(temporalGetsuresPipelines);
	    add(RegressionPipelines);
	}};
	
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
		for (int i = 0; i < pipelines.size(); i++) {
			String useCaseFolderPath = absolutePath + useCaseNames[i] + "/";
		    String[] algorithms = pipelines.get(i);
			for (int j = 0; j < algorithms.length; j++) {
				file = new File(useCaseFolderPath + algorithms[j]);
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
	public static String[] getPepelines(Integer useCaseKey) {
		return Storage.pipelines.get(useCaseKey);
	}
	
	/**
	 * Get all the datasets for use case.
	 */
	public static String[] getDatasets(Integer useCaseKey, Context ctx) {
		ArrayList<String> datasetNames = new ArrayList<String>();
		String [] list = null;
		try {
		   list = ctx.getAssets().list(DatasetsFolder);
           for (String fileName : list) {
        	   String[] fileNameTokens = fileName.split("-");

        	   if (Integer.parseInt(fileNameTokens[0]) == useCaseKey) {
        		   datasetNames.add(fileName);
        	   }
           }
		} catch (IOException e) {
			e.printStackTrace();
		}
		String[] auxArray = new String[datasetNames.size()];
		auxArray = datasetNames.toArray(auxArray);
		return auxArray;
	}
	
}
