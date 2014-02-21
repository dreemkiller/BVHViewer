package com.DreemKiller.BVHViewer;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class BVHViewerActivity extends Activity {
    
    private SpinnerListener BVHSpinnerListener;
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        Spinner spinner = (Spinner)  findViewById(R.id.bvh_spinner);
        // Create an ArrayAdapter using the string array and a default spinner layout
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, R.array.bvh_files, android.R.layout.simple_spinner_item);
        // Specify the layout to use when the list of choices appears
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // Apply the adapter to the spinner
        spinner.setAdapter(adapter);
        BVHSpinnerListener = new SpinnerListener(this, spinner);
        spinner.setOnItemSelectedListener(BVHSpinnerListener);
    }
}