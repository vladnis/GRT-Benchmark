package manageDatasets;

public class Helpers {
    
    /**
     * Genereaza culoare in functie de valoarea presiunii
     * 
     * @param pressure
     * @return Culoarea generata
     */
    public static int getColorFromPressure(float pressure) {
    	int color = 255 - (int)(255 * pressure);
		if (color > 255) {
			color = 255;
		}
		if (color < 0) {
			color = 0;
		}
		return color;
    }
}
