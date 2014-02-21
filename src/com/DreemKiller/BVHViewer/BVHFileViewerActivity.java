package com.DreemKiller.BVHViewer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.concurrent.ExecutionException;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.os.Bundle;
import android.widget.Toast;

public class BVHFileViewerActivity extends Activity {
    private GLSurfaceView mGLView;
    String stringData;
    BVHLoader loader;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        loader = null;
        // Get the intent that started this activity
        Intent intent = getIntent();
        Uri data = intent.getData();
        
        // Figure out what to do based on the intent type
        String urlName = data.getHost();
        String path = data.getEncodedPath();
        String filename = data.getScheme() + "://" + urlName + path;

        // Read all of the text returned by the server
        WebReader reader = new WebReader();

        reader.execute(filename);
        try {
            stringData = reader.get();
            loader = new BVHLoader(this, stringData, 1);
            mGLView = new BVHViewerGLSurfaceView(this, loader.root, loader.channelValues);
            setContentView(mGLView);
        }catch (InterruptedException e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG);
        }catch (ExecutionException e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG);
        } catch (BVHParseException e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG);
        } catch (IOException e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG);
        }
    }
    
    class WebReader extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String...params) {
            String result = new String("");
            try {
                URL url = new URL(params[0]);
                InputStreamReader isr = new InputStreamReader(url.openStream());
                BufferedReader reader = new BufferedReader(isr);
                String line = reader.readLine();
                while(line != null) {
                    result += line;
                    result += "\n";
                    line = reader.readLine();
                }
            } catch (Exception e) {
                //Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG);
            }
            return result;
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }
    
    @Override
    protected void onResume() {
        super.onResume();
       mGLView.onResume();
    }
}