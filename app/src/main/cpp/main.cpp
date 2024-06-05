#include <jni.h>
#include <string>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android_native_app_glue.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_android.h"
#include "imgui/imgui_impl_opengl3.h"

#define LOG_TAG "ImGuiWrapper"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

static ANativeWindow *g_Window = nullptr;
static bool g_Initialized = false;
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static void renderDemoWindow();

extern "C"
JNIEXPORT void JNICALL
Java_me_maars_imguiglsurfaceview_NativeMethod_onSurfaceCreated(JNIEnv *env, jclass clazz,
                                                               jobject surface, jobject gl,
                                                               jobject config) {
    if (g_Initialized)
        return;

    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (!window) {
        LOGE("ANativeWindow_fromSurface failed");
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplAndroid_Init(window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    ImFontConfig font_cfg;
    font_cfg.SizePixels = 26.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    ImGui::GetStyle().ScaleAllSizes(3.0f);
    io.FontGlobalScale = 1.2f;

    g_Initialized = true;
    g_Window = window;

    LOGD("ImGui initialized successfully");
}

extern "C"
JNIEXPORT void JNICALL
Java_me_maars_imguiglsurfaceview_NativeMethod_onSurfaceChanged(JNIEnv *env, jclass clazz,
                                                               jobject gl, jint width,
                                                               jint height) {
    LOGD("onSurfaceChanged: %d, %d", width, height);

    if (!g_Initialized)
        return;

    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float) width, (float) height);
}

extern "C"
JNIEXPORT void JNICALL
Java_me_maars_imguiglsurfaceview_NativeMethod_onDrawFrame(JNIEnv *env, jclass clazz, jobject gl) {
    // TODO: implement onDrawFrame()

    if (!g_Initialized)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    renderDemoWindow();

    ImGui::Render();
    glViewport(0, 0, (int) ImGui::GetIO().DisplaySize.x, (int) ImGui::GetIO().DisplaySize.y);
//    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_me_maars_imguiglsurfaceview_NativeMethod_handleTouch(JNIEnv *env, jclass clazz, jfloat x,
                                                          jfloat y, jint action) {
    if (!g_Initialized)
        return JNI_FALSE;

    LOGD("handleTouch: %.2f, %.2f, %d", x, y, action);


    ImGuiIO &io = ImGui::GetIO();
    switch (action) {
        case 0: // ACTION_DOWN
            io.AddMousePosEvent(x, y);
            io.AddMouseButtonEvent(0, true);
            break;
        case 1: // ACTION_UP
            io.AddMouseButtonEvent(0, false);
            io.AddMousePosEvent(-1, -1);
            break;
        case 2: // ACTION_MOVE
            io.AddMousePosEvent(x, y);
            break;
        default:
            return false;
    }

    return JNI_TRUE;
}

extern "C"
JNIEXPORT void JNICALL
Java_me_maars_imguiglsurfaceview_NativeMethod_onSurfaceDestroyed(JNIEnv *env, jclass clazz,
                                                                 jobject surface) {

    if (!g_Initialized)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
}

static void renderDemoWindow() {
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGuiIO &io = ImGui::GetIO();
        io.AddInputCharacter('a');

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text(
                "This is some useful text."); // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window",
                        &show_demo_window); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f,
                           1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color",
                          (float *) &clear_color); // Edit 3 floats representing a color

        if (ImGui::Button(
                "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate,
                    io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
        ImGui::Begin("Another Window",
                     &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}


