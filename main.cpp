#define GL_SILENCE_DEPRECATION

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

struct Task {
    std::string text;
    bool done = false;
    std::string category = "Today"; // or "Important"
};

std::vector<Task> tasks;
char input[256] = "";

void loadTasks() {
    tasks.clear();
    std::ifstream file("todo.txt");
    std::string line;
    while (std::getline(file, line)) {
        if (line.size() < 5) continue;
        Task task;
        task.done = (line.substr(0, 3) == "[X]");
        size_t sep = line.find("::");
        if (sep != std::string::npos) {
            task.text = line.substr(4, sep - 4);
            task.category = line.substr(sep + 2);
        } else {
            task.text = line.substr(4);
        }
        tasks.push_back(task);
    }
    file.close();
}

void saveTasks() {
    std::ofstream file("todo.txt");
    for (const auto& task : tasks) {
        file << (task.done ? "[X] " : "[ ] ") << task.text << "::" << task.category << "\n";
    }
    file.close();
}

void sortTasks() {
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return !a.done && b.done;
    });
}

void renderGUI() {
    static int tab = 0;

    ImGui::Begin("📝 Todo GUI App");

    // Tabs for task categories
    if (ImGui::BeginTabBar("Tabs")) {
        if (ImGui::BeginTabItem("Today")) {
            tab = 0;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Important")) {
            tab = 1;
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    // New task input and Add button
    ImGui::InputText("New Task", input, IM_ARRAYSIZE(input));
    ImGui::SameLine();
    if (ImGui::Button("Add")) {
        if (strlen(input) > 0) {
            Task newTask;
            newTask.text = input;
            newTask.done = false;
            newTask.category = tab == 0 ? "Today" : "Important";
            tasks.push_back(newTask);
            input[0] = '\0';
            sortTasks();
            saveTasks();
        }
    }

    ImGui::Separator();

    // Task list with delete buttons
    for (int i = 0; i < tasks.size();) {
        Task& task = tasks[i];

        // Show only tasks of the current tab
        if ((tab == 0 && task.category != "Today") || (tab == 1 && task.category != "Important")) {
            ++i;
            continue;
        }

        bool checked = task.done;
        ImGui::PushID(i); // Ensure unique widget IDs

        if (ImGui::Checkbox("##checkbox", &checked)) {
            task.done = checked;
            sortTasks();
            saveTasks();
        }
        ImGui::SameLine();

        if (task.done) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 128, 128, 255));
            ImGui::TextDisabled("~ %s ~", task.text.c_str());
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("%s", task.text.c_str());
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); // spacing before delete button
        if (ImGui::Button("[X]")) {
            tasks.erase(tasks.begin() + i);
            saveTasks();
            ImGui::PopID();
            continue; // Don't increment i because the item was erased
        }

        ImGui::PopID();
        ++i;
    }

    ImGui::End();
}

int main() {
    if (!glfwInit()) return -1;

    // macOS-specific OpenGL context setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS

    GLFWwindow* window = glfwCreateWindow(800, 600, "Todo GUI", nullptr, nullptr);
    if (!window) return -1;
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 150"; // Compatible with macOS
    ImGui_ImplOpenGL3_Init(glsl_version);

    loadTasks();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderGUI();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
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
