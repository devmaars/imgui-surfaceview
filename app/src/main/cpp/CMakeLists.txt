cmake_minimum_required(VERSION 3.22.1)

project("imguiglsurfaceview")


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB IMGUI_SRC
        imgui/*.cpp
)

add_library(${CMAKE_PROJECT_NAME} SHARED
        main.cpp
        ${IMGUI_SRC}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${IMGUI_DIR}
        ${ANDROID_NDK}/sources/android/native_app_glue
)

find_library(log-lib log)
find_library(android-lib android)
find_library(GLESv3-lib GLESv3)
find_library(EGL-lib EGL)

target_link_libraries(${CMAKE_PROJECT_NAME}
        ${log-lib}
        ${android-lib}
        ${GLESv3-lib}
        ${EGL-lib})
