package me.maars.imguiglsurfaceview;

import static android.graphics.PixelFormat.TRANSPARENT;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewGroup;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class MyGLSurfaceView extends GLSurfaceView {
    private static final String TAG = "MyGLSurfaceView";
    private Context ctx;


    public MyGLSurfaceView(Context context) {
        super(context);

        ctx = context;

        setEGLContextClientVersion(3);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        getHolder().setFormat(TRANSPARENT);
        setZOrderOnTop(true);
        setRenderer(new GLRenderer());
    }


    @SuppressLint("ClickableViewAccessibility")
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        super.onTouchEvent(event);

        Logger.d(TAG, "onTouchEvent");


        NativeMethod.handleTouch(event.getX(), event.getY(), event.getAction());


        View rootView = ((Activity) getContext()).getWindow().getDecorView().getRootView();
        return dispatchTouchEventToRoot(rootView, event);
    }

    private boolean dispatchTouchEventToRoot(View rootView, MotionEvent event) {
        if (rootView instanceof ViewGroup) {
            ViewGroup rootViewGroup = (ViewGroup) rootView;
            MotionEvent eventCopy = MotionEvent.obtain(event);

            for (int i = 0; i < rootViewGroup.getChildCount(); i++) {
                View child = rootViewGroup.getChildAt(i);
                if (child != this && child.dispatchTouchEvent(eventCopy))
                    return true;
            }
        }
        return false;
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        NativeMethod.onSurfaceDestroyed(holder.getSurface());
    }

    private class GLRenderer implements Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            NativeMethod.onSurfaceCreated(getHolder().getSurface(), gl, config);
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            gl.glViewport(0, 0, width, height);
            NativeMethod.onSurfaceChanged(gl, width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
            NativeMethod.onDrawFrame(gl);
        }
    }

}
