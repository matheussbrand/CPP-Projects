#include <iostream>
#include <thread>
#include <vector>

using namespace std;

#include <stdio.h>
#include <Windows.h>

int nScreenWidth = 80;      // Screen Size X (columns)
int nScreenHeight = 30;     // Screen Size Y (rows)
wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;

int Rotate(int px, int py, int r){
    int pi = 0;
    switch (r % 4){
        
        case 0:     // 0 degrees
            pi = py * 4 + px;
            break;
        
        case 1:     // 90 degrees
            pi = 12 + py - (px * 4);
            break;

        case 2: // 180 degrees
            pi = 15 - (py *4 ) - px;
            break;

        case 3: // 270 degrees
            pi = 3 - py + (px * 4);
            break;
    }
    return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY){
    for (int px = 0; px < 4; px++)      // All Field cells >0 are occupied
        for (int py = 0; py < 4; py++)
        {
            int pi = Rotate(px, py, nRotation);     // Get index into piece
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);     // Get index into field
        
            if (nPosX + px >= 0 && nPosX + py < nFieldWidth){

                if(nPosY + py >= 0 && nPosY + py < nFieldHeight){
                    if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)       // Colision Check
                        return false;       // Fail on first hit
                }
            }
        }
    return true;
}

int main() {

    // Create Screen Buffer
    wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
    for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    // Tetronimos 4X4
    tetromino[0].append(L"..X...X...X...X.");
    tetromino[1].append(L"..X..XX...X.....");
    tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");

    pField = new unsigned char[nFieldWidth*nFieldHeight]; // Create play field buffer
    for (int x = 0; x < nFieldWidth; x++) // Board Boundary
        for (int y = 0; y < nFieldHeight; y++)
            pField[y*nFieldWidth + x] = (x == 0 || y == nFieldHeight -1) ? 9 : 0;

    // Game Logic
    bool bKey[4];
    int nCurrentPiece = 0;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth / 2;
    int nCurrentY = 0;
    int nSpeed = 20;
    int nSpeedCount = 0;
    bool bForceDown = false;
    bool bRotateHold = true;
    int nPieceCount = 0;
    int nScore = 0;
    vector<int> vLines;
    bool bGameOver = false;

    while(!bGameOver){ // Main Loop
        
        // Timing -----------------------------
        this_thread::sleep_for(50ms);   // small Step = 1 Game Tick
        nSpeedCount++;
        bForceDown = (nSpeedCount == nSpeed);

        // Input -------------------------------
        for (int k = 0; k < 4; k++)
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

        // Game Logic ---------------------------

        // Handle player movement
        nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
        nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
        nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

        // Rotate
        if (bKey[3]){
            nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
            bRotateHold = false;
        } else 
            bRotateHold = true;
        
        if (bForceDown){
            nSpeedCount = 0;
            nPieceCount++;
            if (nPieceCount % 50 == 0)
                if (nSpeed >= 10) nSpeed--;
            
            // Test if piece can be moved down
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
                nCurrentY++;
            else {
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

                // Check for lines
                for (int py = 0; py < 4; py++)
                    if(nCurrentY + py < nFieldHeight - 1){
                        bool bLine = true;
                        for (int px = 1; px < nFieldWidth - 1; px++)
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

                        if (bLine){
                            // Remove Line
                            for (int px = 1; px < nFieldWidth - 1; px++)
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                            vLines.push_back(nCurrentY + py);
                        }
                    }
                
                nScore += 25;
                if(!vLines.empty())     nScore += (1 << vLines.size()) * 100;

                // Pick new piece
                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;
                nCurrentRotation = 0;
                nCurrentPiece = rand() % 7;

                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);

            }               
        }

        // Display ------------

        // Draw Field
        for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
                screen[(y + 2)*nScreenWidth + (x + 2)] = L"ABCDEFG=#"[pField[y*nFieldWidth + x]];

        // Draw Current Piece
        for(int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                    screen[(nCurrentY + py + 2)*nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;    
        
        // Draw Score
        swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

        // Animate Line Completion
        if (!vLines.empty()){

            // Display Frame
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
            this_thread::sleep_for(400ms);      // Delay a bit

            for (auto &v : vLines)
                for (int px = 1; px < nFieldWidth - 1; px++) {
                    for (int py = v; py > 0; py--)
                        pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                    pField[px] = 0;
                }
            vLines.clear();
        }

        // Display Frame
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
           
    } 

    CloseHandle(hConsole);
    cout << "Game Over!! Score:" << nScore << endl;
    system("pause");   
    return 0;
}