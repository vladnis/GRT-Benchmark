package manageDatasets;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PointF;
import android.support.v4.view.MotionEventCompat;
import android.view.MotionEvent;
import android.view.View;

public class DrawView extends View{
	private Paint paint = new Paint();
	
	// Linia generata de miscarea curenta (intre ACTION_UP si ACTION_DOWN)
	private Path mPath = new Path();
	
	// Toate "partile" miscarii
	public static ArrayList<Path> Paths = new ArrayList<Path>();
	
	// Date legate de presiune asociate "partilor" de miscare
	public static ArrayList<Double> presurePoints = new ArrayList<Double>();
	
	// Coordonatele punctelor de miscare
	public static ArrayList<PointF> coordinates = new ArrayList<PointF>();
	
	// Ultima valoare a presiunii aplicata pe ecran
	float lastPressure;
	
	// Ultimile valori ale coordonatelor inregistrate
	float lastx;
	float lasty;

	
    public DrawView(Context context) {
        super(context);
        paint.setColor(Color.BLACK);
        paint.setStyle(Paint.Style.STROKE);	

    	DrawView.presurePoints = new ArrayList<Double>();
    	DrawView.Paths = new ArrayList<Path>();
    }

    @Override
    public void onDraw(Canvas canvas) {
    	super.onDraw(canvas);
    	
    	float pathPressure;
    	int  pathColor;
    	
    	// Deseneaza "miscarea" aplicata pana in momentul afisarii
    	for (int i = 0; i < Paths.size(); i++) {
    		pathPressure = presurePoints.get(i).floatValue();
    		paint.setStrokeWidth(pathPressure * 20);
    		pathColor = Helpers.getColorFromPressure(pathPressure);
    		paint.setARGB(255, pathColor, pathColor, pathColor);
            canvas.drawPath(Paths.get(i), paint);
    	}
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
    	float x = event.getX();
        float y = event.getY();
 
        PointF newCoordinate = new PointF();
        newCoordinate.x = x;
        newCoordinate.y = y;
        coordinates.add(newCoordinate);
 
        float pressure = event.getPressure();
        int action = MotionEventCompat.getActionMasked(event);
    	switch(action) {
		    case (MotionEvent.ACTION_DOWN) :
		    	mPath.reset();
		        mPath.moveTo(x, y);
		     	mPath.lineTo(x, y);

		        lastPressure = pressure;
		        lastx = x;
		        lasty = y;
		        return true;
		    case (MotionEvent.ACTION_MOVE) :
		    	mPath.quadTo(lastx, lasty, (x+lastx) / 2, (y+lasty) / 2);
		    	lastx = x;
		    	lasty = y;
		        mPath.lineTo(x, y);
		    
		    	/* La schimbarea presiunii se fragmenteaza linia
		    	 * pe bucati ce au grosimea si culoarea diferita
		    	 */
		    	if (pressure != lastPressure) {
		    		Paths.add(mPath);
		    		presurePoints.add((double) pressure);
		    		
		    		mPath = new Path();
		    		mPath.moveTo(x, y);
		    		
		    		lastPressure = pressure;
		    	}
		    	// Redeseneaza view
	    		invalidate();

		        return true;
		    case (MotionEvent.ACTION_UP):
		        return true;
		    default :
		        return super.onTouchEvent(event);
    	}
    	
    }

}