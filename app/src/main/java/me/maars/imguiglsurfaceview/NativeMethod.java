package me.maars.imguiglsurfaceview;

import android.view.Surface;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class NativeMethod {

    public static native void onSurfaceCreated(Surface surface, GL10 gl, EGLConfig config);

    public static native void onSurfaceDestroyed(Surface surface);

    public static native void onSurfaceChanged(GL10 gl, int width, int height);

    public static native void onDrawFrame(GL10 gl);

    public static native boolean handleTouch(float x, float y, int action);
}
