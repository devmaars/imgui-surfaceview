package me.maars.imguiglsurfaceview;

import android.util.Log;

public abstract class Logger {

    private static Boolean DEBUG = true;

    public static void d(String tag, String message) {
        if (DEBUG)
            Log.d(tag, message);
    }

    public static void e(String tag, String message) {
        if (DEBUG)
            Log.e(tag, message);
    }

    public static void i(String tag, String message) {
        if (DEBUG)
            Log.i(tag, message);
    }

    public static void v(String tag, String message) {
        if (DEBUG)
            Log.v(tag, message);
    }

    public static void w(String tag, String message) {
        if (DEBUG)
            Log.w(tag, message);
    }

    public static void wtf(String tag, String message) {
        if (DEBUG)
            Log.wtf(tag, message);
    }

}
