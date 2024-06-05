package me.maars.imguiglsurfaceview;

import static android.graphics.PixelFormat.TRANSLUCENT;
import static android.view.ViewGroup.LayoutParams.MATCH_PARENT;
import static android.view.WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
import static android.view.WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL;
import static android.view.WindowManager.LayoutParams.TYPE_APPLICATION;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.view.Gravity;
import android.view.WindowManager;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    static {
        System.loadLibrary("imguiglsurfaceview");
    }

    private Context ctx;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Logger.d(TAG, "onCreate");

        ctx = this;

        if (!supportsOpenGLES3()) {
            Logger.e(TAG, String.valueOf(R.string.opengles3_not_supported));

            setContentView(R.layout.opengles3_not_supported);
            findViewById(R.id.exitButton).setOnClickListener(v -> finish());

            return;
        }

        setContentView(R.layout.activity_main);
        startMenu();
    }


    private void startMenu() {
        Logger.d(TAG, "startMenu()");

        WindowManager wm = this.getWindowManager();

        WindowManager.LayoutParams params = new WindowManager.LayoutParams(
                MATCH_PARENT,
                MATCH_PARENT,
                TYPE_APPLICATION,
                FLAG_NOT_TOUCH_MODAL | FLAG_NOT_FOCUSABLE,
                TRANSLUCENT);

//        WindowManager.LayoutParams params = new WindowManager.LayoutParams(
//                MATCH_PARENT,
//                MATCH_PARENT,
//                0, 0,
//                TYPE_APPLICATION,
//                FLAG_NOT_TOUCHABLE | FLAG_NOT_FOCUSABLE,
//                TRANSPARENT);

        params.gravity = Gravity.TOP | Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL;
        wm.addView(new MyGLSurfaceView(ctx), params);
    }

    private boolean supportsOpenGLES3() {
        ActivityManager am = (ActivityManager) ctx.getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = am.getDeviceConfigurationInfo();
        return (configurationInfo.reqGlEsVersion >= 0x30000);
    }

}