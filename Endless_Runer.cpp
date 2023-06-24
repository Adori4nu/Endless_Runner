#include <raylib.h>
#include <vector>

struct AnimData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
};

struct EnemyNebula
{
    AnimData anim_data;
    int spawn_distance;
    Color color;
    int speed;
};

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
    AnimData player_anim;
    player_anim.rect = {
        0,
        0,
        player_texture.width / 6,
        player_texture.height
    };
    player_anim.pos = {
        screenWidth / 2 - player_anim.rect.width / 2,
        screenHeight - player_anim.rect.height
    };
    player_anim.frame = 0;
    float running_time{};

    // enemy
    int enemy_nebula_speed{-600};
    Texture2D enemy_nebula_texture = LoadTexture("textures/12_nebula_spritesheet.png");
    int single_sprite_width{enemy_nebula_texture.width / 8};
    int single_sprite_height{enemy_nebula_texture.height / 8};

    std::vector<EnemyNebula> enemy_nebulas
    {
        {
            {
                Rectangle{0, 0, single_sprite_width, single_sprite_height},
                Vector2{screenWidth, screenHeight - single_sprite_height},
                0
            },
            screenWidth,
            WHITE,
            enemy_nebula_speed
        },
        {
            {
                Rectangle{0, single_sprite_height, single_sprite_width, single_sprite_height},
                Vector2{screenWidth * 2 + 450, screenHeight * 0.5 - single_sprite_height},
                0
            },
            screenWidth * 2 + 450,
            RED,
            enemy_nebula_speed * 1.125
        },
        {
            {
                Rectangle{0, single_sprite_height, single_sprite_width, single_sprite_height},
                Vector2{screenWidth * 3, screenHeight - single_sprite_height * 1.25},
                0
            },
            screenWidth * 3,
            BLUE,
            enemy_nebula_speed * 0.65
        }
    };

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
                player_anim.rect.x = player_anim.frame * player_anim.rect.width;
                player_anim.frame++;
                if (player_anim.frame > 5)
                {
                    player_anim.frame = 0;
                }
            }
            for (auto& nebula : enemy_nebulas)
            {
                nebula.anim_data.rect.x = nebula.anim_data.frame * nebula.anim_data.rect.width;
                nebula.anim_data.frame++;
                if (nebula.anim_data.frame > 7)
                {
                    nebula.anim_data.frame = 0;
                }
            }
        }
        
        if (player_anim.pos.y >= screenHeight - player_anim.rect.height)
        {
            velocity_y = 0;
            is_in_air = false;
        }
        else
        {
            velocity_y += gravity * delta_time;
        }

        if (IsKeyPressed(KEY_SPACE) && player_anim.pos.y >= 0 && !is_in_air)
        {
            velocity_y += velocity_of_jump;
            is_in_air = true;
        }
        
        player_anim.pos.y += velocity_y * delta_time;

        for (auto& nebula : enemy_nebulas)
        {
            if (nebula.anim_data.pos.x + nebula.anim_data.rect.width < 0)
            {
                nebula.anim_data.pos.y = GetRandomValue(screenHeight * 0.5, screenHeight - nebula.anim_data.rect.height);
                nebula.anim_data.pos.x = nebula.spawn_distance;
            }
            nebula.anim_data.pos.x += nebula.speed * delta_time;
        }
       
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureRec(player_texture, player_anim.rect, player_anim.pos, WHITE);
        DrawTextureRec(enemy_nebula_texture, enemy_nebulas[0].anim_data.rect, enemy_nebulas[0].anim_data.pos, enemy_nebulas[0].color);
        DrawTextureRec(enemy_nebula_texture, enemy_nebulas[1].anim_data.rect, enemy_nebulas[1].anim_data.pos, enemy_nebulas[1].color);
        DrawTextureRec(enemy_nebula_texture, enemy_nebulas[2].anim_data.rect, enemy_nebulas[2].anim_data.pos, enemy_nebulas[2].color);

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