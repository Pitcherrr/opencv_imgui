#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <atomic>
#include <thread>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

std::atomic<bool> stopCapture(false);
std::atomic<cv::Mat*> sharedFrame(nullptr);
std::atomic<bool> frameAvailable(false);
GLuint textureId = 0;

struct cam_params{
    int width;
    int height;
};

cam_params camera = {};

void captureThread()
{
    cv::VideoCapture cap(0);
    double des_fps = 30;
    cap.set(cv::CAP_PROP_FPS, des_fps);

    if (!cap.isOpened())
    {
        stopCapture = true;
        return;
    }

    camera.width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    camera.height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    std::cout << camera.width << "x" << camera.height << std::endl;

    cv::Mat frame;
    while (!stopCapture)
    {
        cap >> frame;

        if (frame.empty())
            continue;

        cv::Mat* sharedFramePtr = new cv::Mat(frame); // Create a new cv::Mat to share

        cv::Mat* oldFrame = sharedFrame.exchange(sharedFramePtr); // Atomic exchange
        if (oldFrame)
            delete oldFrame;

        frameAvailable = true;
    }
}

int main(int, char**)
{
    glfwSetErrorCallback([](int error, const char* description) {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    });

    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Webcam Viewer", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    std::thread captureThreadObj(captureThread);

    cv::Mat* framePtr = nullptr;
    bool newFrameDisplayed = false;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Webcam Image");

        ImVec2 window_size = ImGui::GetWindowSize();

        float image_scale = window_size.x / camera.width;

        ImVec2 imageSize(static_cast<int>(camera.width*image_scale), static_cast<int>(camera.height*image_scale));

        // ImVec2 imageSize(camera.width, camera.height);

        if (frameAvailable)
        {
            framePtr = sharedFrame.exchange(nullptr);
            frameAvailable = false;
            newFrameDisplayed = false;
        }

        if (framePtr && !newFrameDisplayed)
        {
            cv::Mat& frame = *framePtr;

            if (!frame.empty())
            {
                if (textureId == 0)
                {
                    glGenTextures(1, &textureId);
                    glBindTexture(GL_TEXTURE_2D, textureId);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, textureId);
                }

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);

                newFrameDisplayed = true;
            }
        }

        ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(textureId)), imageSize);

        ImGui::End();

        ImGui::Begin("Performance"); 
        ImGui::SetWindowFontScale(1.0f);                         // Create a window called "Hello, world!" and append into it.
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    stopCapture = true;
    captureThreadObj.join();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
