package com.DreemKiller.BVHViewer;
import android.content.Context;
import android.content.Intent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.Spinner;


public class SpinnerListener implements OnItemSelectedListener {

    private Context context;
    private Spinner BVHSpinner;
    public final static String EXTRA_MESSAGE = "com.DreemKiller.BVHViewer.MESSAGE";
    private int callCount = 0;
    
    public SpinnerListener(Context context, Spinner BVHSpinner) {
        this.context = context;
        this.BVHSpinner = BVHSpinner;
    }
    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
        if (callCount != 0) {
            Intent spinnerToGLIntent = new Intent(context, BVHGLActivity.class);
            String filename = parent.getItemAtPosition(pos).toString();
            spinnerToGLIntent.putExtra(EXTRA_MESSAGE, filename);
            context.startActivity(spinnerToGLIntent);
        }
        callCount++;
    }

    public void onNothingSelected(AdapterView<?> arg0) {

    }

}
