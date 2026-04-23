#include "raylib.h"
#include "fileutils.h"
#include <string>
#include <vector>

using namespace std;

int main()
{
    string currentPath = "Desktop";
    vector<string> files = listFiles(currentPath);

    InitWindow(800, 600, "File Explorer");
    SetTargetFPS(60);

    int scrollY = 0;
    const int rowHeight = 25;
    const int listStartY = 45;

    while (!WindowShouldClose())
    {
        // --- UPDATE ---

        // scroll
        int wheel = GetMouseWheelMove();
        scrollY -= wheel * rowHeight;
        if (scrollY < 0) scrollY = 0;
        int maxScroll = (int)files.size() * rowHeight - (600 - listStartY);
        if (maxScroll < 0) maxScroll = 0;
        if (scrollY > maxScroll) scrollY = maxScroll;

        // mouse click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int mouseX = GetMouseX();
            int mouseY = GetMouseY();

            for (int i = 0; i < (int)files.size(); i++)
            {
                int y = listStartY + i * rowHeight - scrollY;

                // check if click is within this row
                if (mouseY >= y && mouseY < y + rowHeight && mouseX < 800)
                {
                    // enter the folder and refresh the file list
                    currentPath = enterFolder(files[i], currentPath);
                    files = listFiles(currentPath);
                    scrollY = 0; // reset scroll when entering a new folder
                    break;
                }
            }
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(BLACK);

        // show current path at top
        DrawText(currentPath.c_str(), 10, 10, 16, RED);
        DrawLine(0, 35, 800, 35, LIGHTGRAY);

        for (int i = 0; i < (int)files.size(); i++)
        {
            int y = listStartY + i * rowHeight - scrollY;
            if (y < listStartY || y > 600) continue;

            DrawText(files[i].c_str(), 20, y, 18, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}