#include "raylib.h"
#include "fileutils.h"
#include <string>
#include <vector>

using namespace std;

int main()
{

    string currentPath = "";
    string startPath = "";

    vector<string> files = listFiles(currentPath);

    InitWindow(1000, 600, "File Explorer");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    int scrollY = 0;
    const int rowHeight = 25;
    const int listStartY = 45;

    bool delConfirm = false;
    bool crtConfirm = false;
    bool renConfirm = false;

    string newName = "";

    int selectedIndex = 0;      // first item highlighted by default
    double lastClickTime = 0.0; // time of last click
    int lastClickedIndex = -1;  // which index was last clicked

    // Matrix Rain Effect variables, this is only for aesthetics
    const int COLS = 200; // one column every 10px
    int rainY[COLS];      // current Y of each column head
    int rainSpeed[COLS];  // speed of each column
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
    Texture2D driveIcon = LoadTexture("harddrive.png");

    bool onHomeScreen = true;

    while (!WindowShouldClose())
    { // matrix rain update
        for (int i = 0; i < COLS; i++)
        {
            rainY[i] += rainSpeed[i];
            if (rainY[i] > GetScreenHeight())
            {
                rainY[i] = GetRandomValue(-300, 0);
                rainSpeed[i] = GetRandomValue(2, 8);
            }
        }
        // UI buttons variables
        const int btnX = GetScreenWidth() - 50;     // back button position and size
        const int delbtnX = GetScreenWidth() - 120; // this is for delete button
        const int crtbtnX = GetScreenWidth() - 190; // this is for the create folder button
        const int renbtnX = GetScreenWidth() - 260; // this is for the rename button
        const int btnY = 0;
        const int btnW = 28;
        const int btnH = 34;

        if (onHomeScreen)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                int mouseX = GetMouseX();
                int mouseY = GetMouseY();

                if (mouseX >= 50 && mouseX <= 950 && mouseY >= 150 && mouseY <= 230)
                {
                    currentPath = "C:\\";
                    startPath = currentPath;
                    files = listFiles(currentPath);
                    onHomeScreen = false;
                }
                else if (mouseX >= 50 && mouseX <= 950 && mouseY >= 250 && mouseY <= 330)
                {
                    currentPath = "D:\\";
                    startPath = currentPath;
                    files = listFiles(currentPath);
                    onHomeScreen = false;
                }
                else if (mouseX >= 50 && mouseX <= 950 && mouseY >= 350 && mouseY <= 430)
                {
                    currentPath = "C:\\Users\\Haytham\\Desktop";
                    startPath = currentPath;
                    files = listFiles(currentPath);
                    onHomeScreen = false;
                }
            }

            // home screen draw
            BeginDrawing();
            ClearBackground(BLACK);

            // matrix rain
            for (int i = 0; i < COLS; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    int x = i * 10;
                    int y = rainY[i] - j * 15;
                    int alpha = 180 - (j * 15);
                    if (alpha < 0)
                        alpha = 0;
                    Color rainColor = {0, (unsigned char)(180 - j * 8), 0, (unsigned char)alpha};
                    char ch[2] = {(char)GetRandomValue(33, 126), '\0'};
                    DrawTextEx(matrixFont, ch, {(float)x, (float)y}, 14, 1, rainColor);
                }
            }

            DrawRectangle(0, 0, 1000, 600, {0, 0, 0, 80});
            Vector2 textSize = MeasureTextEx(uiFont, "SELECT A DRIVE", 48, 1);
            float textX = (1000 - textSize.x) / 2;
            DrawTextEx(uiFont, "SELECT A DRIVE", {textX, 80}, 48, 1, WHITE);

            // drive data
            long long cTotal = getDriveTotal("C:\\");
            long long cFree = getDriveFree("C:\\");
            float cFill = (float)(cTotal - cFree) / (float)cTotal;

            long long dTotal = getDriveTotal("D:\\");
            long long dFree = getDriveFree("D:\\");
            float dFill = (float)(dTotal - dFree) / (float)dTotal;

            const int rowX = 50;
            const int rowW = 900;
            const int rowH = 80;
            const int barW = 400;
            const int barX = 200;

            // C drive row
            DrawRectangleLines(rowX, 150, rowW, rowH, GREEN);
            DrawTextureEx(driveIcon, {(float)rowX + 5, 167}, 0, 0.08f, GREEN);
            DrawTextEx(uiFont, "C:\\", {(float)rowX + 48, 170}, 32, 1, GREEN);
            DrawRectangle(barX, 175, barW, 20, DARKGREEN);
            DrawRectangle(barX, 175, (int)(barW * cFill), 20, GREEN);
            string cInfo = sizeSorter(cTotal - cFree) + " / " + sizeSorter(cTotal);
            DrawTextEx(uiFont, cInfo.c_str(), {(float)barX + barW + 15, 170}, 24, 1, GREEN);

            // D drive row
            DrawRectangleLines(rowX, 250, rowW, rowH, GREEN);
            DrawTextureEx(driveIcon, {(float)rowX + 5, 267}, 0, 0.08f, GREEN);
            DrawTextEx(uiFont, "D:\\", {(float)rowX + 48, 270}, 32, 1, GREEN);
            DrawRectangle(barX, 275, barW, 20, DARKGREEN);
            DrawRectangle(barX, 275, (int)(barW * dFill), 20, GREEN);
            string dInfo = sizeSorter(dTotal - dFree) + " / " + sizeSorter(dTotal);
            DrawTextEx(uiFont, dInfo.c_str(), {(float)barX + barW + 15, 270}, 24, 1, GREEN);

            // Desktop row
            DrawRectangleLines(rowX, 350, rowW, rowH, GREEN);
            DrawTextureEx(folderIcon, {(float)rowX + 5, 365}, 0, 0.08f, GREEN);
            DrawTextEx(uiFont, "Desktop", {(float)rowX + 48, 370}, 32, 1, GREEN);

            EndDrawing();
        }

        else
        {
            // --- UPDATE ---

            // scroll
            int wheel = GetMouseWheelMove();
            scrollY -= wheel * rowHeight;

            if (scrollY < 0)
                scrollY = 0;
            int maxScroll = (int)files.size() * rowHeight - (GetScreenHeight() - listStartY);
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
                    if (currentPath == "C:\\" || currentPath == "D:\\" || currentPath == startPath)
                    {
                        onHomeScreen = true;
                    }
                    else
                    {
                        currentPath = goBack(currentPath);
                        files = listFiles(currentPath);
                        scrollY = 0;
                        selectedIndex = 0;
                        lastClickedIndex = -1;
                    }
                }
                else if (mouseX >= delbtnX && mouseX <= delbtnX + btnW &&
                         mouseY >= btnY && mouseY <= btnY + btnH)
                {
                    delConfirm = true;
                }
                else if (delConfirm)
                {
                  int popX = GetScreenWidth() / 2 - 200;
                  int popY = GetScreenHeight() / 2 - 75;
    
                  // YES button
                    if (mouseX >= popX + 70 && mouseX <= popX + 130 && mouseY >= popY + 90 && mouseY <= popY + 120)
                        {
                            deleteFile(currentPath, files[selectedIndex]);
                            files = listFiles(currentPath);
                            delConfirm = false;
                        }
                  // NO button
                    else if (mouseX >= popX + 270 && mouseX <= popX + 310 && mouseY >= popY + 90 && mouseY <= popY + 120)
                        {
                            delConfirm = false;
                        }
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
            DrawRectangle(0, 35, GetScreenWidth()-230, GetScreenHeight(), {0, 0, 0, 120});

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
                if (y < listStartY || y > GetScreenHeight())
                    {continue;}

                if (i == selectedIndex)
                {
                    DrawRectangleLines(10, y - 2, GetScreenWidth() - 320, rowHeight, ORANGE);
                    if (isFolder(currentPath, files[i]))
                    {
                        DrawTextureEx(folderIcon, {2, (float)y}, 0, 0.035f, ORANGE);
                    }
                    else
                    {
                        DrawTextureEx(fileIcon, {4, (float)y + 2}, 0, 0.028f, ORANGE);
                    }
                    DrawTextEx(uiFont, files[i].c_str(), {(float)20, (float)y}, 24, 1, ORANGE);
                    string size = sizeSorter(fileSize(currentPath, files[i]));
                    DrawTextEx(uiFont, size.c_str(), {(float)GetScreenWidth() - 400, (float)y}, 24, 1, RED);
                    string type = fileType(files[i]);
                    DrawTextEx(uiFont, type.c_str(), {(float)GetScreenWidth() - 525, (float)y}, 24, 1, YELLOW);
                }
                else
                {
                    if (isFolder(currentPath, files[i]))
                    {
                        DrawTextureEx(folderIcon, {2, (float)y}, 0, 0.035f, GREEN);
                    }
                    else
                    {
                        DrawTextureEx(fileIcon, {4, (float)y + 2}, 0, 0.028f, GREEN);
                    }
                    DrawTextEx(uiFont, files[i].c_str(), {(float)20, (float)y}, 22, 1, GREEN);
                }
            }

            if (delConfirm)
            {
                int popX = GetScreenWidth() / 2 - 200;
                int popY = GetScreenHeight() / 2 - 75;
                DrawRectangle(popX, popY, 400, 150, DARKGRAY);
                DrawTextEx(uiFont, "Are you sure you want to delete?", {(float)popX + 20, (float)popY + 20}, 25, 1, WHITE);
                DrawText("YES", popX + 70, popY + 100, 20, RED);
                DrawText("NO", popX + 270, popY + 100, 20, GREEN);
            }
            if (crtConfirm)
            {
                int popX = GetScreenWidth() / 2 - 200;
                int popY = GetScreenHeight() / 2 - 75;
                DrawRectangle(popX, popY, 400, 150, DARKGRAY);
                DrawTextEx(uiFont, "Name your New Folder:", {(float)popX + 20, (float)popY + 20}, 25, 1, WHITE);
                DrawTextEx(uiFont, newName.c_str(), {(float)popX + 20, (float)popY + 60}, 22, 1, GREEN);
                DrawText("Press ENTER to confirm", popX + 20, popY + 110, 16, GRAY);
            }
            if (renConfirm)
            {
                int popX = GetScreenWidth() / 2 - 200;
                int popY = GetScreenHeight() / 2 - 75;
                DrawRectangle(popX, popY, 400, 150, DARKGRAY);
                DrawTextEx(uiFont, "Rename your Item:", {(float)popX + 20, (float)popY + 20}, 25, 1, WHITE);
                DrawTextEx(uiFont, newName.c_str(), {(float)popX + 20, (float)popY + 60}, 22, 1, GREEN);
                DrawText("Press ENTER to confirm", popX + 20, popY + 110, 16, GRAY);
            }

            EndDrawing();
        }
    }
    UnloadTexture(folderIcon);
    UnloadTexture(fileIcon);
    UnloadFont(uiFont);
    UnloadFont(matrixFont);

    CloseWindow();
    return 0;
}