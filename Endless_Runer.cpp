#include <raylib.h>

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    const int gravity{2'000}; // 1'000 pixels per second/second
    const int velocity_of_jump{-900}; // 600 pixels per second
    int velocity_y{0};
    bool is_in_air{false};

    InitWindow(screenWidth, screenHeight, "Endless Runner");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Texture2D player_texture = LoadTexture("textures/scarfy.png");
    Rectangle player_rect
    {
        0,
        0,
        player_texture.width / 6,
        player_texture.height
    };
    Vector2 player_pos
    {
        screenWidth / 2 - player_rect.width / 2,
        screenHeight - player_rect.height
    };
    const float animation_update_time{1.0f / 12.0f};
    float running_time{};

    int frame_counter{};


    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        const float delta_time{GetFrameTime()};

        running_time += delta_time;
        if (running_time >= animation_update_time)
        {
            running_time = 0.0f;
            player_rect.x = frame_counter * player_rect.width;
            frame_counter++;
            if (frame_counter > 5)
            {
                frame_counter = 0;
            }
        }
        
        if (player_pos.y >= screenHeight - player_rect.height)
        {
            velocity_y = 0;
            is_in_air = false;
        }
        else
        {
            velocity_y += gravity * delta_time;
        }

        if (IsKeyPressed(KEY_SPACE) && player_pos.y >= 0 && !is_in_air)
        {
            velocity_y += velocity_of_jump;
            is_in_air = true;
        }
        
        player_pos.y += velocity_y * delta_time;
       
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureRec(player_texture, player_rect, player_pos, WHITE);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(player_texture); // Unload texture
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}