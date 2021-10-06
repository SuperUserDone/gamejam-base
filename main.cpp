#include "raylib.h"

#define BUBBLE_MAX   100
#define GRAVITY      320
#define FLOOR_HEIGHT 80

#define MAX_HEIGHT 12

#define PLAYER_SPEED 200
#define PLAYER_SIZE 30

#define ROPE_SPEED 300

struct Bubbles
{
    Vector2 positions[BUBBLE_MAX];
    Vector2 velocities[BUBBLE_MAX];

    int sizes[BUBBLE_MAX];
};

struct rope
{
    int pos_x = 1;
    int length = 1;

    bool active = false;
};

struct player
{
    int pos_x = 300;
};

int main()
{
    Bubbles bubs{};
    rope player_rope;
    player p;


    int size_x = 800, size_y = 600;

    p.pos_x = size_x/2;

    for (int i = 0; i < BUBBLE_MAX; i++) {
        bubs.positions[i] = {0.f, 0.f};
        bubs.sizes[i] = 0;
        bubs.velocities[i] = {0.f, 0.f};
    }

    bubs.positions[0] = {size_x * 0.5f, size_y * 0.25f};
    bubs.positions[1] = {size_x * 0.5f, size_y * 0.25f};
    bubs.velocities[0] = {100.f, 0.f};
    bubs.velocities[1] = {-100.f, 0.f};
    bubs.sizes[0] = 4;
    bubs.sizes[1] = 4;

    InitWindow(size_x, size_y, "Bubble game");

    SetExitKey(0);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GRAY);

        for (int i = 0; i < BUBBLE_MAX; i++) {
            DrawRectangle(0, size_y - FLOOR_HEIGHT, size_x, FLOOR_HEIGHT, DARKGRAY);

            if (bubs.sizes[i] > 0)
                DrawCircle((int)bubs.positions[i].x,
                           (int)bubs.positions[i].y,
                           (float)bubs.sizes[i] * 10,
                           WHITE);
        }

        if (player_rope.active)
            DrawRectangle((float)player_rope.pos_x,
                          (float)size_y - FLOOR_HEIGHT - (float)player_rope.length,
                          2,
                          (float)player_rope.length,
                          RED);

        DrawRectangle(p.pos_x, size_y-FLOOR_HEIGHT-PLAYER_SIZE, PLAYER_SIZE,PLAYER_SIZE, ORANGE);
        EndDrawing();

        float dt = GetFrameTime();

        if(IsKeyDown(KEY_SPACE) && !player_rope.active){
           player_rope.active = true;
           player_rope.length = 0;
           player_rope.pos_x = p.pos_x + PLAYER_SIZE/2;
        }

        if(IsKeyDown(KEY_A) && p.pos_x > 0)
            p.pos_x -= PLAYER_SPEED * dt;

        if(IsKeyDown(KEY_D) && p.pos_x+PLAYER_SIZE < size_x)
            p.pos_x += PLAYER_SPEED * dt;


        if(player_rope.active)
        {
            player_rope.length += ROPE_SPEED * dt;

            if(player_rope.length > size_y - FLOOR_HEIGHT)
                player_rope.active = false;
        }

        for (int i = 0; i < BUBBLE_MAX; i++) {
            if (bubs.sizes[i] > 0) {

                bubs.positions[i].x += bubs.velocities[i].x * dt;
                bubs.positions[i].y += bubs.velocities[i].y * dt;

                bubs.velocities[i].y += GRAVITY * dt;

                if (bubs.positions[i].x - (bubs.sizes[i] * 10) < 0 ||
                    bubs.positions[i].x + (bubs.sizes[i] * 10) > size_x)
                    bubs.velocities[i].x *= -1.0;

                if (bubs.positions[i].y < size_y - (bubs.sizes[i] * 10 * MAX_HEIGHT) &&
                    bubs.velocities[i].y < 0)
                    bubs.velocities[i].y *= 0.5;

                if (bubs.positions[i].y >= size_y - FLOOR_HEIGHT - (bubs.sizes[i] * 10)) {
                    bubs.positions[i].y = size_y - FLOOR_HEIGHT - (bubs.sizes[i] * 10);
                    bubs.velocities[i].y *= -1.0;
                }

                if (CheckCollisionCircleRec(
                        {bubs.positions[i].x, bubs.positions[i].y},
                        (float)bubs.sizes[i] * 10,
                        {(float)player_rope.pos_x,
                         (float)size_y - FLOOR_HEIGHT - (float)player_rope.length,
                         2,
                         (float)player_rope.length}) && player_rope.active) {
                    player_rope.active = false;
                    bubs.velocities[i].y = 0;
                    bubs.velocities[i].x *= -1;
                    bubs.sizes[i]--;

                    for(int j = 0; j < BUBBLE_MAX; j++)
                        if(bubs.sizes[j] == 0)
                        {
                            bubs.velocities[j] = bubs.velocities[i];
                            bubs.velocities[j].x *= -1;
                            bubs.positions[j] = bubs.positions[i];
                            bubs.sizes[j] = bubs.sizes[i];
                            break;
                        }

                }


            }
        }
    }
}
