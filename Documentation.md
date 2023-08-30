Coming in v1.1-v1.3.

How to run application with SFML.LD2D API:
```
g++ test.cpp -o test.exe -std=c++17 -Wall -Wextra -g -DSFML_STATIC -I PATH_TO_SFML_INCLUDES -L PATH_TO_SFML_LIBS -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype
```
