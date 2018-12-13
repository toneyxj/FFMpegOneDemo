package com.xj.ffmpegonedemo;

import android.app.Activity;
import android.os.Bundle;

import com.xj.ffmpegonedemo.TestOpenGl.MyGLSurfaceView;

public class OpenGlTestActivity extends Activity {

    private MyGLSurfaceView myGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_open_gl_test);
        myGLSurfaceView = new MyGLSurfaceView(this);
        setContentView(myGLSurfaceView);
    }

    @Override
    protected void onPause() {
        super.onPause();
        myGLSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        myGLSurfaceView.onResume();
    }
}
