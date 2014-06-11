package async;

import android.content.Context;
import storage.Storage;

public class Maintenance implements Runnable {
	Context ctx;
	
	public Maintenance(Context ctx) {
		this.ctx = ctx;
	}
	
	@Override
	public void run() {
		Storage.createDirStruct(ctx);
	}
	
}
