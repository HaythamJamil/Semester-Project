#include "raylib.h"
#include "fileutils.h"
#include <string>
#include <vector>

using namespace std;

int main()
{
    const string startPath = "Desktop";
    vector<string> files = listFiles(startPath);

    InitWindow(800, 600, "File Explorer");
    SetTargetFPS(60);

    int scrollY = 0; // how many pixels we've scrolled down
    const int rowHeight = 25;
    const int listStartY = 45;

    while (!WindowShouldClose())
    {
        // --- UPDATE ---
        int wheel = GetMouseWheelMove();         // returns 1, -1, or 0
        scrollY = scrollY - (wheel * rowHeight); // scroll down = negative wheel

        // clamp so you can't scroll above the top
        if (scrollY < 0)
        {
            scrollY = 0;
        }

        // clamp so you can't scroll past the last item
        int maxScroll = (int)files.size() * rowHeight - (600 - listStartY); // dirent built-in function to get number of files in directory
        if (maxScroll < 0)
            maxScroll = 0;
        if (scrollY > maxScroll)
            scrollY = maxScroll;

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("File Explorer", 10, 10, 20, DARKGRAY);
        DrawLine(0, 35, 800, 35, LIGHTGRAY);

        for (int i = 0; i < (int)files.size(); i++)
        {
            int y = listStartY + i * rowHeight - scrollY; // offset by scroll

            // only draw if the row is visible on screen
            if (y < listStartY || y > 600)
                continue;

            DrawText(files[i].c_str(), 20, y, 18, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}