#include <raylib.h>

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    const int gravity{2'500}; // 1'000 pixels per second/second
    const int velocity_of_jump{-1'200}; // 600 pixels per second
    int velocity_y{0};
    bool is_in_air{false};

    InitWindow(screenWidth, screenHeight, "Endless Runner");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    const float animation_update_time{1.0f / 12.0f};
    
    // player
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
    float running_time{};

    // enemy
    const int enemy_nebula_speed{-600};
    Texture2D enemy_nebula_texture = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle enemy_nebula_rect
    {
        0,
        0,
        enemy_nebula_texture.width / 8,
        enemy_nebula_texture.height / 8
    };
    Vector2 enemy_nebula_pos
    {
        screenWidth,
        screenHeight - enemy_nebula_rect.height
    };


    int player_frame{};
    int nebula_frame{};

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
            
            if (!is_in_air)
            {        
                player_rect.x = player_frame * player_rect.width;
                player_frame++;
                if (player_frame > 5)
                {
                    player_frame = 0;
                }
            }
            enemy_nebula_rect.x = nebula_frame * enemy_nebula_rect.width;
            nebula_frame++;
            if (nebula_frame > 7)
            {
                nebula_frame = 0;
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

        if (enemy_nebula_pos.x + enemy_nebula_rect.width < 0)
        {
            enemy_nebula_pos.y = GetRandomValue(screenHeight * 0.5, screenHeight - enemy_nebula_rect.height);
            enemy_nebula_pos.x = screenWidth;
        }

        enemy_nebula_pos.x += enemy_nebula_speed * delta_time;
       
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureRec(player_texture, player_rect, player_pos, WHITE);
        DrawTextureRec(enemy_nebula_texture, enemy_nebula_rect, enemy_nebula_pos, WHITE);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(player_texture); // Unload texture
    UnloadTexture(enemy_nebula_texture); // Unload texture
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}