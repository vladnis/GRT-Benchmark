package manageDatasets;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.View;

public class InveresedDrawView extends View{
	private Paint paint = new Paint();

    public InveresedDrawView(Context context) {
    	super(context);
        paint.setColor(Color.BLACK);
        paint.setStyle(Paint.Style.STROKE);
    }

    @Override
    public void onDraw(Canvas canvas) {
    	super.onDraw(canvas);
    	
    	float pathPressure;
    	int  pathColor;
    	
    	// Genereaza micarea completa din "partile" de linie cu grosimi si culori diferite
    	for (int i = 0; i < DrawView.Paths.size(); i++) {
    		pathPressure = DrawView.presurePoints.get(i).floatValue();
    		paint.setStrokeWidth(pathPressure * 20);
    		pathColor = Helpers.getColorFromPressure(pathPressure);
    		paint.setARGB(255, pathColor, pathColor, pathColor);
            canvas.drawPath(DrawView.Paths.get(i), paint);
    	}
    }

}
