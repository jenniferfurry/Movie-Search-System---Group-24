//
// Created by Kristina Despotuli  on 11/5/26.
//

#include <cstdio>
#include <fstream>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "MovieApp.h"

// CMake passes in
#ifndef DATA_FILE
#define DATA_FILE "tmdb_movies_100000.csv"
#endif
#ifndef FONT_FILE
#define FONT_FILE ""
#endif

static bool fileExists(const std::string& path) {
    return !path.empty() && std::ifstream(path).good();
}

static std::string findDataFile() {
    const char* candidates[] = {"tmdb_movies_100000.csv", "../tmdb_movies_100000.csv", DATA_FILE};
    for (const char* c : candidates) {
        if (fileExists(c)) return c;
    }
    return "tmdb_movies_100000.csv";
}

static void applyStyle() {
    ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(24, 18);
    style.FramePadding = ImVec2(12, 7);
    style.ItemSpacing = ImVec2(10, 10);
    style.FrameRounding = 8.0f;
    style.FrameBorderSize = 1.0f;
    style.CellPadding = ImVec2(12, 8);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(1, 1, 1, 1);
    style.Colors[ImGuiCol_Button] = ImVec4(0.95f, 0.95f, 0.95f, 1);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.85f, 0.90f, 1.0f, 1);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.83f, 1.0f, 1);
    style.Colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);
}

// loads roboto from the ImGui download
static void loadFonts(MovieApp& app) {
    ImGuiIO& io = ImGui::GetIO();
    ImFont* body = nullptr;
    ImFont* heading = nullptr;
    if (fileExists(FONT_FILE)) {
        body = io.Fonts->AddFontFromFileTTF(FONT_FILE, 19.0f);
        heading = io.Fonts->AddFontFromFileTTF(FONT_FILE, 30.0f);
    } else {
        ImFontConfig cfg;
        cfg.SizePixels = 16.0f;
        body = io.Fonts->AddFontDefault(&cfg);
        cfg.SizePixels = 26.0f;
        heading = io.Fonts->AddFontDefault(&cfg);
    }
    io.FontDefault = body;
    app.setFonts(body, heading);
}

int main() {
    if (!glfwInit()) {
        std::fprintf(stderr, "Could not start GLFW\n");
        return 1;
    }

#ifdef __APPLE__
    const char* glslVersion = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char* glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    GLFWwindow* window = glfwCreateWindow(1100, 720, "Movie Search System", nullptr, nullptr);
    if (window == nullptr) {
        std::fprintf(stderr, "Could not open window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr; // don't write imgui.ini
    applyStyle();

    MovieApp app;
    loadFonts(app);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    app.load(findDataFile()); // builds both trees

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        app.draw();

        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}