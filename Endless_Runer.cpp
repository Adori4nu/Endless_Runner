#include <raylib.h>

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Endless Runner");

    const int width = 50;
    const int height = 80;

    int pos_y{screenHeight - height};
    int velocity_y{0};
    const int velocity_of_jump{-10};
    int gravity{1};

    bool is_in_air{false};

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        if (pos_y >= screenHeight - height)
        {
            velocity_y = 0;
            is_in_air = false;
        }
        else
        {
            velocity_y += gravity;
        }

        if (IsKeyPressed(KEY_SPACE) && pos_y >= 0 && !is_in_air)
        {
            velocity_y += velocity_of_jump;
            is_in_air = true;
        }
        
        pos_y += velocity_y;
       
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangle(screenWidth / 2, pos_y, width, height, RED);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}