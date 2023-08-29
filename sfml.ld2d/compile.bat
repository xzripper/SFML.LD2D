@echo off
echo Starting compilation.
g++ imgui\include\imgui.cpp imgui\include\imgui_widgets.cpp imgui\include\imgui_draw.cpp imgui\include\imgui_tables.cpp imgui\include\imgui_stdlib.cpp imgui\include\imgui-SFML.cpp ld2d.cpp -o ld2d.exe -std=c++17 -Wall -Wextra -g -DSFML_STATIC -I SFML\include\ -I imgui\include\ -L SFML\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype
echo Compilation finished.
