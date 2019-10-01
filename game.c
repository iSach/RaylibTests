#include "raylib.h"
#include "game.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Disgusting code. TODO Proper physics and hitboxes system.
 * @return
 */
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "yeet");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Image img = LoadImage("resources/graphics/background.png");
    ImageResize(&img, screenWidth, screenHeight);
    Texture2D background_texture = LoadTextureFromImage(img);
    UnloadImage(img);

    Image mario_img = LoadImage("resources/graphics/mario.png");
    ImageResize(&mario_img, MARIO_WIDTH, MARIO_HEIGHT);
    Texture2D mario_texture_right = LoadTextureFromImage(mario_img);
    ImageFlipHorizontal(&mario_img);
    Texture2D mario_texture_left = LoadTextureFromImage(mario_img);
    UnloadImage(mario_img);

    Mario mario;
    mario.x = screenWidth / 2;
    mario.y = 468 - MARIO_HEIGHT;
    mario.mot_x = 0;
    mario.mot_y = 0;
    mario.on_ground = true;
    mario.facing_right = 1;
    mario.width = MARIO_WIDTH;
    mario.height = MARIO_HEIGHT;

    const int min_y = screenHeight - 110;

    int hitboxes_amount = 4;

    Hitbox *hitboxes = malloc(hitboxes_amount * sizeof(Hitbox));
    hitboxes[0].x = 751;
    hitboxes[0].y = 325;
    hitboxes[0].height = 37;
    hitboxes[0].width = 205;
    hitboxes[0].name = "plat_1";

    hitboxes[1].x = 0;
    hitboxes[1].y = 468;
    hitboxes[1].height = screenHeight - 469;
    hitboxes[1].width = screenWidth;
    hitboxes[1].name = "ground";

    hitboxes[2].x = 589;
    hitboxes[2].y = 324;
    hitboxes[2].height = 37;
    hitboxes[2].width = 41;
    hitboxes[2].name = "box_1";

    hitboxes[3].x = 834;
    hitboxes[3].y = 180;
    hitboxes[3].height = 38;
    hitboxes[3].width = 41;
    hitboxes[3].name = "box_2";

    Hitbox mario_hitbox;
    mario_hitbox.width = mario.width;
    mario_hitbox.height = mario.height;

    SetMouseScale(1, 1);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Controls
        exec_controls(&mario);

        // Physics
        if (!mario.on_ground)
            mario.mot_y -= 1; // Gravity acceleration

        int new_x = mario.x + mario.mot_x;
        int new_y = mario.y - mario.mot_y;
        mario_hitbox.x = new_x;
        mario_hitbox.y = new_y;

        if (mario.mot_y != 0 || mario.mot_x != 0)
            for (int i = 0; i < hitboxes_amount; i++) {
                Hitbox h = *(hitboxes + i);

                if ((h.x < mario.x && mario.x < h.x + h.width) ||
                    (h.x < mario.x + mario.width && mario.x + mario.width < h.x + h.width)) {
                    if (new_y + mario_hitbox.height > h.y && mario.y + mario.height <= h.y) { // Up
                        printf("%s: UP\n", h.name);
                        new_y = h.y - mario.height;
                        mario.on_ground = true;
                        mario.mot_y = 0;
                        break;
                    } else if (new_y < h.y + h.height && mario.y >= h.y + h.height) { // Down
                        printf("%s: DOWN\n", h.name);
                        new_y = h.y + h.height;
                        mario.mot_y = 0;
                    }
                } else {
                    if (mario.on_ground) mario.on_ground = false;
                }


                if ((h.y < mario.y && mario.y < h.y + h.height) ||
                    (h.y < mario.y + mario.height && mario.y + mario.height < h.y + h.height)) {
                    // Left
                    if (new_x + mario_hitbox.width > h.x && mario.x + mario.width <= h.x) {
                        printf("%s: Left\n", h.name);
                        new_x = h.x - mario.width;
                        mario.mot_x = 0;
                    }

                    // Right
                    if (new_x < h.x + h.width && mario.x >= h.x + h.width) {
                        printf("%s: Right\n", h.name);
                        new_x = h.x + h.width;
                        mario.mot_x = 0;
                    }
                }
            }

        // Screen borders
        if (new_x < 0) new_x = 0;
        if (new_x > screenWidth - mario.width) new_x = screenWidth - mario.width;
        if (new_y < 0) new_y = 0;
        if (new_y > screenHeight - mario.height) {
            mario.on_ground = true;
            new_y = screenHeight - mario.height;
        }

        mario_hitbox.x = new_x;
        mario_hitbox.y = new_y;
        mario.x = new_x; // Apply New X
        mario.y = new_y; // Apply New Y
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        // Draw Background
        DrawTexture(background_texture, 0, 0, WHITE);
        // Draw character
        DrawTexture(mario.facing_right ? mario_texture_right : mario_texture_left, mario.x, mario.y, WHITE);

        // --------- Debug ---------
        char debug_text[100];
        sprintf(debug_text, "FPS: %d\nX: %d\nY: %d\nMot_X: %d\nMot_Y: %d\nOn_Ground: %s", GetFPS(), mario.x, mario.y,
                mario.mot_x, mario.mot_y, mario.on_ground ? "true" : "false");
        DrawText(debug_text, 50, 50, 25, BLACK);
        sprintf(debug_text, "X: %d\nY: %d", GetMouseX(), GetMouseY());
        DrawText(debug_text, GetMouseX() - 50, GetMouseY() - 50, 15, BLACK);
        // Draw hitboxes
        for (int i = 0; i < hitboxes_amount; i++) {
            Hitbox h = *(hitboxes + i);
            DrawRectangleLines(h.x, h.y, h.width, h.height, WHITE);
        }
        DrawRectangleLines(mario_hitbox.x, mario_hitbox.y, mario_hitbox.width, mario_hitbox.height, BLACK);
        // --------- End Debug ---------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void exec_controls(Mario *mario) {
    // Right/Left moves
    if (IsKeyDown(KEY_LEFT)) {
        mario->facing_right = 0;
        mario->mot_x = -4;
    } else if (IsKeyDown(KEY_RIGHT)) {
        mario->facing_right = 1;
        mario->mot_x = 4;
    } else if (mario->on_ground) {
        mario->mot_x = 0;
    }

    // Jump
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_SPACE)) && mario->on_ground) {
        mario->on_ground = false;
        mario->mot_y = JUMP_VALUE;
    }
}

/**
 *
 * @param h1
 * @param h2
 * @return
 */
Collision get_collision(Hitbox h1, Hitbox h2) {
    Collision to_return;
    to_return.vert = NULL;
    to_return.hor = NULL;
    to_return.h1 = h1;
    to_return.h2 = h2;

    bool collide_x, collide_y;

    // Check for h1 on top of h2
    if (h1.y + h1.height > h2.y && h1.y + h1.height < h2.y + h2.height) {
        to_return.vert = up;
        collide_y = true;
    } else if (h1.y > h2.y && h1.y < h2.y + h2.height) {
        to_return.vert = down;
        collide_y = true;
    }

    // Check for h1 is left to h2.
    if (h1.x + h1.width > h2.x && h1.x + h1.width < h2.x + h2.width) {
        to_return.hor = left;
        collide_x = true;
    } else if (h1.x > h2.x && h1.x < h2.x + h2.width) {
        to_return.hor = right;
        collide_x = true;
    }

    to_return.do_collide = collide_x && collide_y;

    if (to_return.do_collide) {
        printf("[%s]: vert(%d) & hor(%d)\n", h2.name, to_return.vert, to_return.hor);
    }

    return to_return;
}
