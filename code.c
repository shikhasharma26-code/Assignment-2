#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h> // for sound

/* Function to remove flicker by resetting the cursor position */
void clear_screen_fast() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(h, pos);
}

int main() {
    // === Initialization ===
    system("color 4F"); // Set console color
    PlaySound(TEXT("bg.wav"), NULL, SND_ASYNC | SND_LOOP); // Start looping BG music
    srand(time(0));

    int x = 1;             // Player position (0 to 2, 1 is center)
    int step = 0;          // Obstacle vertical movement (0 to 10)
    int obstaclePos = rand() % 3; // Obstacle lane (0, 1, 2)
    int score = 0;         // New: Game score
    int gameSpeed = 120;   // New: Base delay time (ms)
    
    // === Game Loop ===
    while (1) {

        // ---- INPUT ----
        if (_kbhit()) {
            char ch = getch();

            if ((ch == 75 || ch == 'a' || ch == 'A') && x > 0) // LEFT arrow or 'A'
                x--;

            if ((ch == 77 || ch == 'd' || ch == 'D') && x < 2) // RIGHT arrow or 'D'
                x++;
        }

        // ---- DRAW ----
        clear_screen_fast();
        printf("|--- --- ---| SCORE: %d\n", score); // Display score

        for (int i = 0; i < 10; i++) {
            if (i == step) {
                // Draw Obstacle (char 1 = smiley)
                if (obstaclePos == 0)
                    printf("| %c         |\n", 1);
                else if (obstaclePos == 1)
                    printf("|     %c     |\n", 1);
                else if (obstaclePos == 2)
                    printf("|         %c |\n", 1);
            } else {
                printf("|           |\n");
            }
        }

        // ---- PLAYER ----
        // Draw Player (char 6 = spade/card symbol)
        if (x == 0)
            printf("| %c         |\n", 6);
        else if (x == 1)
            printf("|     %c     |\n", 6);
        else if (x == 2)
            printf("|         %c |\n", 6);
            
        printf("|--- --- ---|\n"); // Bottom separator
        
        // ---- COLLISION ----
        if (step == 10) { 
            if (x == obstaclePos) { // Collision detected
                PlaySound(NULL, NULL, 0); // Stop background music
                PlaySound(TEXT("impact.wav"), NULL, SND_ASYNC); // Play collision sound
                Sleep(2500);
                
                clear_screen_fast();
                printf("\n|--- --- ---|\n");
                printf("| GAME OVER!|\n");
                printf("| Final Score: %d |\n", score); // Display final score
                printf("|--- --- ---|\n");
                break; // End game loop
            } else { // Obstacle dodged successfully
                // Optional: Play a sound for a successful dodge
                // PlaySound(TEXT("ding.wav"), NULL, SND_ASYNC); 
                score++;
                
                // New: Difficulty increase based on score
                if (score % 5 == 0 && gameSpeed > 50) {
                    gameSpeed -= 10; // Reduce delay, increase speed every 5 points
                }
            }
        }

        // ---- Game Update ----
        Sleep(gameSpeed); // Use the variable gameSpeed
        step++;

        // Reset when reaches bottom
        if (step > 10) {
            step = 0;
            obstaclePos = rand() % 3; // new lane
        }
    }

    return 0;
}
