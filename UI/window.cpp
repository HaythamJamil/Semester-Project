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

    // back button position and size
    const int btnX = 750;
    const int btnY = 0;
    const int btnW = 50;
    const int btnH = 34;

    int selectedIndex = 0;        // first item highlighted by default
    double lastClickTime = 0.0;   // time of last click
    int lastClickedIndex = -1;    // which index was last clicked

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

            // check back button click
            if (mouseX >= btnX && mouseX <= btnX + btnW &&
                mouseY >= btnY && mouseY <= btnY + btnH)
            {
                currentPath = goBack(currentPath);
                files = listFiles(currentPath);
                scrollY = 0;
                selectedIndex = 0;
                lastClickedIndex = -1;
            }
            else
            {
                // check file row click
                for (int i = 0; i < (int)files.size(); i++)
                {
                    int y = listStartY + i * rowHeight - scrollY;

                    if (mouseY >= y && mouseY < y + rowHeight && mouseX < 800)
                    {
                        double now = GetTime();
                        double timeSinceLastClick = now - lastClickTime;

                        if (lastClickedIndex == i && timeSinceLastClick < 0.4)
                        {
                            // double click — enter folder
                            currentPath = enterFolder(files[i], currentPath);
                            files = listFiles(currentPath);
                            scrollY = 0;
                            selectedIndex = 0;
                            lastClickedIndex = -1;
                        }
                        else
                        {
                            // single click — just highlight
                            selectedIndex = i;
                            lastClickedIndex = i;
                            lastClickTime = now;
                        }

                        break;
                    }
                }
            }
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(BLACK);

        // current path
        DrawText(currentPath.c_str(), 10, 10, 16, RED);
        DrawLine(0, 35, 800, 35, WHITE);

        // back button
        DrawRectangle(btnX, btnY, btnW, btnH, ORANGE);
        DrawText("<-", btnX + 10, btnY + 5, 32, WHITE);

        // file list
        for (int i = 0; i < (int)files.size(); i++)
        {
            int y = listStartY + i * rowHeight - scrollY;
            if (y < listStartY || y > 600) continue;

            if (i == selectedIndex)
            {
                // highlighted row — draw border rectangle
                DrawRectangleLines(10, y - 2, 780, rowHeight, ORANGE);
                DrawText(files[i].c_str(), 20, y, 18, ORANGE);
            }
            else
            {
                DrawText(files[i].c_str(), 20, y, 18, GREEN);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}