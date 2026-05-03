#include "raylib.h"
#include "fileutils.h"
#include <string>
#include <vector>

using namespace std;

int main()
{

    string currentPath = "C:\\Users\\Haytham\\Desktop\\A";

    vector<string> files = listFiles(currentPath);

    InitWindow(1000, 600, "File Explorer");
    SetTargetFPS(60);

    int scrollY = 0;
    const int rowHeight = 25;
    const int listStartY = 45;

    const int btnX = 950;    // back button position and size
    const int delbtnX = 890; // this is for delete button
    const int crtbtnX = 830; // this is for the create folder button
    const int renbtnX = 770; // this is for the rename button
    const int btnY = 0;
    const int btnW = 50;
    const int btnH = 34;

    bool delConfirm = false;
    bool crtConfirm = false;
    bool renConfirm = false;

    string newName = "";

    int selectedIndex = 0;      // first item highlighted by default
    double lastClickTime = 0.0; // time of last click
    int lastClickedIndex = -1;  // which index was last clicked

    while (!WindowShouldClose())
    {
        // --- UPDATE ---

        // scroll
        int wheel = GetMouseWheelMove();
        scrollY -= wheel * rowHeight;

        if (scrollY < 0)
            scrollY = 0;
        int maxScroll = (int)files.size() * rowHeight - (600 - listStartY);
        if (maxScroll < 0)
            maxScroll = 0;
        if (scrollY > maxScroll)
            scrollY = maxScroll;

        if (crtConfirm)
        {
            int key = GetCharPressed();
            if (key > 0)
            {
                newName += (char)key;
            }
            if (IsKeyPressed(KEY_BACKSPACE) && newName.size() > 0)
            {
                newName.pop_back();
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                createFolder(currentPath, newName);
                files = listFiles(currentPath);
                newName = "";
                crtConfirm = false;
            }
        }
        if (renConfirm)
        {
            int key = GetCharPressed();
            if (key > 0)
            {
                newName += (char)key;
            }
            if (IsKeyPressed(KEY_BACKSPACE) && newName.size() > 0)
            {
                newName.pop_back();
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                renameFile(currentPath, files[selectedIndex], newName);
                files = listFiles(currentPath);
                newName = "";
                renConfirm = false;
            }
        }
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
            else if (mouseX >= delbtnX && mouseX <= delbtnX + btnW &&
                     mouseY >= btnY && mouseY <= btnY + btnH)
            {
                delConfirm = true;
            }
            else if (mouseX >= 320 && mouseX <= 360 && mouseY >= 310 && mouseY <= 330)
            {
                deleteFile(currentPath, files[selectedIndex]);
                files = listFiles(currentPath);
                delConfirm = false;
            }
            else if (mouseX >= 520 && mouseX <= 560 && mouseY >= 310 && mouseY <= 330)
            {
                delConfirm = false;
            }
            else if (mouseX >= crtbtnX && mouseX <= crtbtnX + btnW &&
                     mouseY >= btnY && mouseY <= btnY + btnH)
            {
                crtConfirm = true;
            }
            else if (mouseX >= renbtnX && mouseX <= renbtnX + btnW &&
                     mouseY >= btnY && mouseY <= btnY + btnH)
            {
                renConfirm = true;
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
                            // double click — enter folder or open file
                            if (isFolder(currentPath, files[i]))
                            {
                                currentPath = enterFolder(files[i], currentPath);
                                files = listFiles(currentPath);
                                scrollY = 0;
                                selectedIndex = 0;
                                lastClickedIndex = -1;
                            }
                            else
                            {
                                openFile(files[i], currentPath);
                            }
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
        DrawLine(0, 35, 500, 35, LIGHTGRAY);

        // back button
        DrawRectangle(btnX, btnY, btnW, btnH, ORANGE);
        DrawText("<-", btnX + 10, btnY + 5, 32, WHITE);
        DrawRectangle(delbtnX, btnY, btnW + 5, btnH, RED);
        DrawText("DEL", delbtnX + 2, btnY + 5, 28, WHITE);
        DrawRectangle(crtbtnX, btnY, btnW + 5, btnH, ORANGE);
        DrawText("CRT", crtbtnX + 3, btnY + 5, 28, WHITE);
        DrawRectangle(renbtnX, btnY, btnW + 5, btnH, LIME);
        DrawText("REN", renbtnX + 3, btnY + 5, 28, WHITE);

        // ONE loop only
        for (int i = 0; i < (int)files.size(); i++)
        {
            int y = listStartY + i * rowHeight - scrollY;
            if (y < listStartY || y > 600)
                continue;

            if (i == selectedIndex)
            {
                DrawRectangleLines(10, y - 2, 980, rowHeight, ORANGE);
                DrawText(files[i].c_str(), 20, y, 18, ORANGE);
                string size = sizeSorter(fileSize(currentPath, files[i]));
                DrawText(size.c_str(), 900, y, 18, RED);
                string type = fileType(files[i]);
                DrawText(type.c_str(), 775, y, 18, YELLOW);
            }
            else
            {
                DrawText(files[i].c_str(), 20, y, 18, GREEN);
            }
        }

        if (delConfirm)
        {
            DrawRectangle(250, 200, 400, 150, DARKGRAY);
            DrawText("Are you sure you want to delete?", 270, 230, 18, WHITE);
            DrawText("YES", 320, 310, 20, RED);
            DrawText("NO", 520, 310, 20, GREEN);
        }
        if (crtConfirm)
        {
            DrawRectangle(250, 200, 400, 150, DARKGRAY);
            DrawText("Name your New Folder:", 270, 230, 18, WHITE);
            DrawText(newName.c_str(), 270, 270, 18, GREEN);
            DrawText("Press ENTER to confirm", 270, 310, 16, GRAY);
        }
        if (renConfirm)
        {
            DrawRectangle(250, 200, 400, 150, DARKGRAY);
            DrawText("Rename your Item:", 270, 230, 18, WHITE);
            DrawText(newName.c_str(), 270, 270, 18, GREEN);
            DrawText("Press ENTER to confirm", 270, 310, 16, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}