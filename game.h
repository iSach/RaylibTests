#ifndef UNTITLED1_GAME_H
#define UNTITLED1_GAME_H

#endif //UNTITLED1_GAME_H

#define JUMP_VALUE 18
#define MARIO_HEIGHT 50
#define MARIO_WIDTH 40

enum vert_direction{up, down};
enum hor_direction{left, right};

typedef struct {
    int x, y, width, height, mot_x, mot_y;
    bool facing_right, on_ground;
} Mario;

typedef struct {
    char *name;
    int x, y, width, height;
} Hitbox;

typedef struct {
    int x, y;
    char * text;
    int lifetime;
} TempText;

typedef struct {
    enum vert_direction vert;
    enum hor_direction hor;
    Hitbox h1, h2;
    bool do_collide;
} Collision;

Collision get_collision(Hitbox h1, Hitbox h2);
void exec_controls(Mario *mario);