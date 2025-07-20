CXX = g++
CXXFLAGS = -std=c++11 -Iimgui -I/usr/local/include -I/opt/homebrew/include
LDFLAGS = -L/usr/local/lib -L/opt/homebrew/lib -lglfw -framework OpenGL

SOURCES = main.cpp imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp \
          imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp
TARGET = todo-gui

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
