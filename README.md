# 📝 Todo GUI (C++ & ImGui)

A simple cross-platform GUI Todo App built with **C++**, **GLFW**, and **Dear ImGui**.  
Designed to demonstrate GUI programming using ImGui + task persistence using file I/O.

## 📸 Screenshot

![screenshot](screenshots/todo.png)

## 🎯 Features

- Add tasks
- Mark as done (with strike-through and reorder)
- Two categories: Today / Important
- Save/load from local file `todo.txt`

## 🚀 Build & Run

```bash
make
./todo-gui
```

## 🔧 Requirements
g++

GLFW

OpenGL (macOS: enable forward-compatible)

```bash
git init
git add .
git commit -m "Initial commit: Todo GUI app with ImGui"
git remote add origin https://github.com/USERNAME/todo-gui.git
git push -u origin main
```