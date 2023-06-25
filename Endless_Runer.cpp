#include <raylib.h>
#include <vector>
#include <tuple>

struct BgPlane
{
    Texture2D texture;
    float x_pos;
    float speed;
    float scale;
    Color tint;
};

struct AnimData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float running_time{0};
    int sprite_length;
};

struct EnemyNebula
{
    AnimData anim_data;
    int spawn_distance;
    Color color;
    int speed;
};

const float animation_update_time{1.0f / 12.0f};

AnimData updateAnimData(AnimData& data, float delta_time)
{
    data.running_time += delta_time;
    if (data.running_time >= animation_update_time)
    {
        data.running_time = 0.0f;

        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if (data.frame > data.sprite_length)
        {
            data.frame = 0;
        }
    }
    return data;
}

void drawScrollingBackground(BgPlane& bg_plane, float delta_time)
{
    bg_plane.x_pos -= bg_plane.speed * delta_time;
    if (bg_plane.x_pos <= -bg_plane.texture.width * bg_plane.scale)
    {
        bg_plane.x_pos = 0.0f;
    }
    DrawTextureEx(bg_plane.texture, Vector2{bg_plane.x_pos, 0}, 0.0f, bg_plane.scale, bg_plane.tint);
    DrawTextureEx(bg_plane.texture, Vector2{bg_plane.x_pos + bg_plane.texture.width * bg_plane.scale, 0}, 0.0f, bg_plane.scale, bg_plane.tint);
}

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int windowDimensions[2];
    windowDimensions[0] = 1920;//GetScreenWidth(); // why it's not working properly?
    windowDimensions[1] = 1080;//GetScreenHeight();
    
    const int gravity{2'500}; // 1'000 pixels per second/second
    const int velocity_of_jump{-1'200}; // 600 pixels per second
    int velocity_y{0};
    bool is_in_air{false};
    bool double_jump_enable{true};

    
    InitWindow(windowDimensions[0], windowDimensions[1], "Endless Runner");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    
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
        windowDimensions[0] / 4 - player_anim.rect.width / 2,
        windowDimensions[1] - player_anim.rect.height
    };
    player_anim.frame = 0;
    player_anim.sprite_length = 5;
    // float running_time{};

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
                Vector2{windowDimensions[0], windowDimensions[1] - single_sprite_height},
                0, 0, 7
            },
            windowDimensions[0],
            WHITE,
            enemy_nebula_speed
        },
        {
            {
                Rectangle{0, single_sprite_height, single_sprite_width, single_sprite_height},
                Vector2{windowDimensions[0] * 2 + 450, windowDimensions[1] * 0.5 - single_sprite_height},
                0, 0, 7
            },
            windowDimensions[0] * 2 + 450,
            RED,
            enemy_nebula_speed * 1.125
        },
        {
            {
                Rectangle{0, single_sprite_height, single_sprite_width, single_sprite_height},
                Vector2{windowDimensions[0] * 3, windowDimensions[1] - single_sprite_height * 1.25},
                0, 0, 7
            },
            windowDimensions[0] * 3,
            BLUE,
            enemy_nebula_speed * 0.65
        }
    };

    std::vector<BgPlane> bg_planes
    {
        {
            LoadTexture("textures/far-buildings.png"),
            0.0f,
            20.0f,
            6.0f,
            WHITE
        },
        {
            LoadTexture("textures/back-buildings.png"),
            0.0f,
            50.0f,
            6.0f,
            WHITE
        },
        {
            LoadTexture("textures/foreground.png"),
            0.0f,
            80.0f,
            6.0f,
            WHITE
        }
    };

    float finish_line{static_cast<float>(windowDimensions[0] * 10.0f)};
    bool collision{false};

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        const float delta_time{GetFrameTime()};

        { // sprite animation    
            if (!is_in_air)
            {        
                player_anim = updateAnimData(player_anim, delta_time);
            }
            for (auto& nebula : enemy_nebulas)
            {
                nebula.anim_data = updateAnimData(nebula.anim_data, delta_time);
            }
        }

        Rectangle player_collision
        {
            player_anim.pos.x,
            player_anim.pos.y,
            player_anim.rect.width,
            player_anim.rect.height
        };
        for (auto& nebula : enemy_nebulas)
        {
            float pad{50};
            Rectangle nebula_collision
            {
                nebula.anim_data.pos.x + pad,
                nebula.anim_data.pos.y + pad,
                nebula.anim_data.rect.width - 2 * pad,
                nebula.anim_data.rect.height - 2 * pad
            };
            if (CheckCollisionRecs(player_collision, nebula_collision))
            {
                collision = true;
                break;
            }
        }
        
        // player position logic and update
        if (player_anim.pos.y >= windowDimensions[1] - player_anim.rect.height)
        {
            velocity_y = 0;
            is_in_air = false;
            double_jump_enable = true;
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
        if (IsKeyPressed(KEY_D) && is_in_air && double_jump_enable)
        {
            velocity_y += velocity_of_jump * 0.75;
            double_jump_enable = false;
        }
        
        player_anim.pos.y += velocity_y * delta_time;

        // respawning enemies off-screen
        for (auto& nebula : enemy_nebulas)
        {
            if (nebula.anim_data.pos.x + nebula.anim_data.rect.width < 0)
            {
                nebula.anim_data.pos.y = GetRandomValue(windowDimensions[1] * 0.5, windowDimensions[1] - nebula.anim_data.rect.height);
                nebula.anim_data.pos.x = nebula.spawn_distance;
            }
            // movement from right to left
            nebula.anim_data.pos.x += nebula.speed * delta_time;
        }
        finish_line += enemy_nebula_speed * delta_time;
       
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(WHITE);
        
        for (auto& bg_plane : bg_planes)
        {
            drawScrollingBackground(bg_plane, delta_time);
        }
        if (collision)
        {
            DrawText("GAME OVER!", windowDimensions[0] * 0.5 - 100, windowDimensions[1] * 0.5, 50, RED);
        }
        else if (player_anim.pos.x >= finish_line)
        {
            DrawText("YOU WIN!", windowDimensions[0] * 0.5 - 100, windowDimensions[1] * 0.5, 50, BLUE);

        }
        else
        {
            DrawTextureRec(player_texture, player_anim.rect, player_anim.pos, WHITE);
            
            for (auto& nebula : enemy_nebulas)
            {
                DrawTextureRec(enemy_nebula_texture, nebula.anim_data.rect, nebula.anim_data.pos, nebula.color);
            }
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(player_texture);
    UnloadTexture(enemy_nebula_texture);
    for (auto& bg_plane : bg_planes)
    {
        UnloadTexture(bg_plane.texture);
    }
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}