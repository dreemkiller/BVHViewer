package com.DreemKiller.BVHViewer;

import java.io.FileNotFoundException;
import java.io.IOException;

import android.app.Activity;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.widget.Toast;

public class BVHGLActivity extends Activity {
    private GLSurfaceView mGLView;
    
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = getIntent();
        String filename = intent.getStringExtra(SpinnerListener.EXTRA_MESSAGE);
        try {
            BVHLoader myBVHLoader = new BVHLoader(this, filename);
            mGLView = new BVHViewerGLSurfaceView(this, myBVHLoader.root, myBVHLoader.channelValues);
            setContentView(mGLView);
        } catch (FileNotFoundException fnfe) {
            Toast.makeText(this, fnfe.getMessage(), Toast.LENGTH_LONG);
        } catch (BVHParseException bpe) {
            Toast.makeText(this, bpe.toString(), Toast.LENGTH_LONG);
        } catch (IOException ioe) {
            Toast.makeText(this, ioe.getMessage(), Toast.LENGTH_LONG);
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
