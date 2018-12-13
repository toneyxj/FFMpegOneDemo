package com.xj.ffmpegonedemo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by xj on 2018/9/3.
 */

public class TestPlay extends GLSurfaceView implements GLSurfaceView.Renderer {
    public TestPlay(Context context) {
        this(context,null);
    }

    public TestPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
        setRenderer(this);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        gl.glViewport(0,0,width,height);
        float ratio=(float) width/height;
        gl.glMatrixMode(GL10.GL_PROJECTION);// set matrix to projection mode  是说我们现在改变的是坐标系与Surface的映射关系（投影矩阵）。
        gl.glLoadIdentity();// set matrix to projection mode 之前对投影矩阵的任何改变
        gl.glFrustumf(-ratio,ratio,-1,1,3,7);// apply the projection matrix
    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }
}
