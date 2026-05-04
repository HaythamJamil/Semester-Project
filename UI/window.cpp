#include "raylib.h"
#include "fileutils.h"
#include <string>
#include <vector>

using namespace std;

int main()
{

    string currentPath = "C:\\Users\\Haytham\\Desktop";

    vector<string> files = listFiles(currentPath);

    InitWindow(1000, 600, "File Explorer");
    SetTargetFPS(60);

    int scrollY = 0;
    const int rowHeight = 25;
    const int listStartY = 45;

    const int btnX = 950;    // back button position and size
    const int delbtnX = 880; // this is for delete button
    const int crtbtnX = 810; // this is for the create folder button
    const int renbtnX = 740; // this is for the rename button
    const int btnY = 0;
    const int btnW = 28;
    const int btnH = 34;

    bool delConfirm = false;
    bool crtConfirm = false;
    bool renConfirm = false;

    string newName = "";

    int selectedIndex = 0;      // first item highlighted by default
    double lastClickTime = 0.0; // time of last click
    int lastClickedIndex = -1;  // which index was last clicked

    // Matrix Rain Effect variables, this is only for aesthetics
    const int COLS = 1000 / 10; // one column every 10px
    int rainY[COLS];            // current Y of each column head
    int rainSpeed[COLS];        // speed of each column
    // loop for the same effect to get random values
    for (int i = 0; i < COLS; i++)
    {
        rainY[i] = GetRandomValue(-600, 0);
        rainSpeed[i] = GetRandomValue(2, 8);
    }

    // Font Loading
    Font matrixFont = LoadFont("MatrixCodeNFI.ttf");
    Font uiFont = LoadFont("VT323-Regular.ttf");

    // Icons Loading
    Texture2D folderIcon = LoadTexture("folder.png");
    Texture2D fileIcon = LoadTexture("paper.png");

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

        // matrix rain update
        for (int i = 0; i < COLS; i++)
        {
            rainY[i] += rainSpeed[i];
            if (rainY[i] > 600)
            {
                rainY[i] = GetRandomValue(-300, 0);
                rainSpeed[i] = GetRandomValue(2, 8);
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

        // matrix rain draw
        for (int i = 0; i < COLS; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                int x = i * 10;
                int y = rainY[i] - j * 15;
                int alpha = 255 - (j * 15);
                if (alpha < 0)
                    alpha = 0;
                Color rainColor = {0, (unsigned char)(180 - j * 8), 0, (unsigned char)alpha};
                char ch[2] = {(char)GetRandomValue(33, 126), '\0'};
                DrawTextEx(matrixFont, ch, {(float)x, (float)y}, 14, 1, rainColor);
            }
        }

        // this makes the rain effect damper
        DrawRectangle(0, 35, 770, 600, {0, 0, 0, 160});

        // current path
        DrawText(currentPath.c_str(), 10, 10, 16, RED);
        DrawLine(0, 35, renbtnX - 2, 35, LIGHTGRAY);

        // back button
        DrawRectangle(btnX, btnY, btnW + 5, btnH, BLUE);
        DrawTextEx(uiFont, "<-", {(float)btnX + 5, (float)btnY + 5}, 32, 1, WHITE);
        DrawRectangle(delbtnX, btnY, btnW + 9, btnH, RED);
        DrawTextEx(uiFont, "DEL", {(float)delbtnX + 2, (float)btnY + 5}, 28, 1, WHITE);
        DrawRectangle(crtbtnX, btnY, btnW + 10, btnH, ORANGE);
        DrawTextEx(uiFont, "CRT", {(float)crtbtnX + 2, (float)btnY + 5}, 28, 1, WHITE);
        DrawRectangle(renbtnX, btnY, btnW + 9, btnH, LIME);
        DrawTextEx(uiFont, "REN", {(float)renbtnX + 2, (float)btnY + 5}, 28, 1, WHITE);

        // ONE loop only
        for (int i = 0; i < (int)files.size(); i++)
        {
            int y = listStartY + i * rowHeight - scrollY;
            if (y < listStartY || y > 600)
                continue;

            if (i == selectedIndex)
            {
                DrawRectangleLines(10, y - 2, 980, rowHeight, ORANGE);
                if (isFolder(currentPath, files[i]))
                    DrawTextureEx(folderIcon, {2, (float)y}, 0, 0.035f, ORANGE);
                else
                    DrawTextureEx(fileIcon, {4, (float)y}, 0, 0.028f, ORANGE);
                DrawTextEx(uiFont, files[i].c_str(), {(float)20, (float)y}, 24, 1, ORANGE);
                string size = sizeSorter(fileSize(currentPath, files[i]));
                DrawTextEx(uiFont, size.c_str(), {(float)900, (float)y}, 24, 1, RED);
                string type = fileType(files[i]);
                DrawTextEx(uiFont, type.c_str(), {(float)775, (float)y}, 24, 1, YELLOW);
            }
            else
            {
                if (isFolder(currentPath, files[i]))
                    DrawTextureEx(folderIcon, {2, (float)y}, 0, 0.035f, GREEN);
                else
                    DrawTextureEx(fileIcon, {4, (float)y}, 0, 0.028f, GREEN);
                DrawTextEx(uiFont, files[i].c_str(), {(float)20, (float)y}, 22, 1, GREEN);
            }
        }

        if (delConfirm)
        {
            DrawRectangle(250, 200, 400, 150, DARKGRAY);
            DrawTextEx(uiFont, "Are you sure you want to delete?", {(float)270, (float)230}, 25, 1, WHITE);
            DrawText("YES", 320, 310, 20, RED);
            DrawText("NO", 520, 310, 20, GREEN);
        }
        if (crtConfirm)
        {
            DrawRectangle(250, 200, 400, 150, DARKGRAY);
            DrawTextEx(uiFont, "Name your New Folder:", {(float)270, (float)230}, 25, 1, WHITE);
            DrawTextEx(uiFont, newName.c_str(), {(float)270, (float)270}, 22, 1, GREEN);
            DrawText("Press ENTER to confirm", 270, 310, 16, GRAY);
        }
        if (renConfirm)
        {
            DrawRectangle(250, 200, 400, 150, DARKGRAY);
            DrawTextEx(uiFont, "Rename your Item:", {(float)270, (float)230}, 25, 1, WHITE);
            DrawTextEx(uiFont, newName.c_str(), {(float)270, (float)270}, 22, 1, GREEN);
            DrawText("Press ENTER to confirm", 270, 310, 16, GRAY);
        }
        UnloadTexture(folderIcon);
        UnloadTexture(fileIcon);
        UnloadFont(uiFont);
        UnloadFont(matrixFont);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}