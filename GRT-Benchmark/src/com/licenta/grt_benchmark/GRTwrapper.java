package com.licenta.grt_benchmark;

public class GRTwrapper {
	static {
		System.loadLibrary("grt");
		System.loadLibrary("TestModel");
		System.loadLibrary("TestModelFactory");
		System.loadLibrary("GRTwrapper");	
	}
	
	/**
	 * Ruleaza nativ teste pentru GRT.
	 */
	public native static void test(String inputFilePath,
			String outputFolderPath, String usecase, String pipeline);
	
}
