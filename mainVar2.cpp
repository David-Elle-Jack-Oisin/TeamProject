#include "raylib.h"

#include <math.h>

#define PLAYER_BASE_SIZE 20.0f
#define PLAYER_SPEED 5.0f
#define PLAYER_MAX_SHOOTS 1

#define MAX_BIG_SLIMES 2
#define SLIMES_SPEED 2.0f


typedef struct Player
{
    Vector2 position;
    Vector2 speed;
    Vector3 collider;
    float rotation;
} Player;

typedef struct Shoot
{
    Vector2 position;
    Vector2 speed;
    float radius;
    float rotation;
    int lifeSpawn;
    bool active;
} Shoot;

typedef struct Slime
{
    Vector2 position;
    Vector2 speed;
    float radius;
    bool active;
} Slime;

static const int screenWidth = 1920;
static const int screenHeight = 1080;

static int framesCounter = 0;
static bool gameOver = false;
static bool pause = false;

static Player player = {0};
static Shoot shoot[PLAYER_MAX_SHOOTS] = {0};
static Slime bigSlimes[MAX_BIG_SLIMES] = {0};
static Slime mediumSlimes[MAX_BIG_SLIMES * 2] = {0};
static Slime smallSlimes[MAX_BIG_SLIMES * 4] = {0};

static float shipHeight = 0.0f;
static float gravity = 0.0f;

static int countmediumSlimes = 0;
static int countsmallSlimes = 0;
static int slimesDestroyed = 0;
static Vector2 linePosition = {0};

static bool victory = false;
static bool lose = false;
static bool awake = false;

static void InitGame(void);        // Initialize game
static void UpdateGame(void);      // Update game (one frame)
static void DrawGame(void);        // Draw game (one frame)
static void UnloadGame(void);      // Unload game
static void UpdateDrawFrame(void); // Update and Draw (one frame)


int main(void)
{
    InitWindow(screenWidth, screenHeight, "Quest for Moisture");

    InitGame();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();

    }

    UnloadGame(); // Unload loaded data (textures, sounds, models...)

    CloseWindow(); // Close window and OpenGL context

    return 0;
}


static void InitGame(void)
{
    int posx = 0;
    int posy = 0;
    int velx = 0;
    int vely = 0;

    framesCounter = 0;
    gameOver = false;
    pause = false;

    victory = false;
    lose = false;
    awake = true;
    gravity = 0.25f;

    linePosition = Vector2{0.0f, 0.0f};
    shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);

    // Initialization player
    player.position = Vector2{screenWidth / 2, screenHeight};
    player.speed = Vector2{PLAYER_SPEED, PLAYER_SPEED};
    player.rotation = 0;
    player.collider = Vector3{player.position.x, player.position.y - shipHeight / 2.0f, 12.0f};

    slimesDestroyed = 0;

    // Initialize shoots
    for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
    {
        shoot[i].position = Vector2{0, 0};
        shoot[i].speed = Vector2{0, 0};
        shoot[i].radius = 2;
        shoot[i].active = false;
        shoot[i].lifeSpawn = 0;
    }

    // Initialize big slimes
    for (int i = 0; i < MAX_BIG_SLIMES; i++)
    {
        bigSlimes[i].radius = 40.0f;
        posx = GetRandomValue(0 + bigSlimes[i].radius, screenWidth - bigSlimes[i].radius);
        posy = GetRandomValue(0 + bigSlimes[i].radius, screenHeight / 2);

        bigSlimes[i].position = Vector2{234, 464};

        while ((velx == 0) || (vely == 0))
        {
            velx = GetRandomValue(-SLIMES_SPEED, SLIMES_SPEED);
            vely = GetRandomValue(-SLIMES_SPEED, SLIMES_SPEED);
        }

        bigSlimes[i].speed = Vector2{10, 12};
        bigSlimes[i].active = true;
    }

    // Initialize medium slimes
    for (int i = 0; i < MAX_BIG_SLIMES * 2; i++)
    {
        mediumSlimes[i].position = Vector2{-100, -100};
        mediumSlimes[i].speed = Vector2{0, 0};
        mediumSlimes[i].radius = 20.0f;
        mediumSlimes[i].active = false;
    }

    // Initialize small slimes
    for (int i = 0; i < MAX_BIG_SLIMES * 4; i++)
    {
        smallSlimes[i].position = Vector2{-100, -100};
        smallSlimes[i].speed = Vector2{0, 0};
        smallSlimes[i].radius = 10.0f;
        smallSlimes[i].active = false;
    }

    countmediumSlimes = 0;
    countsmallSlimes = 0;
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver && !victory)
    {
        if (IsKeyPressed('P'))
            pause = !pause;

        if (!pause)
        {
            if (IsKeyDown(KEY_LEFT))
            {
                player.position.x -= player.speed.x;

            }
            if (IsKeyDown(KEY_RIGHT))
            {
                player.position.x += player.speed.x;

            }
            if (IsKeyDown(KEY_UP))
            {
                player.position.y -= player.speed.y;

            }
            if (IsKeyDown(KEY_DOWN))
            {
                player.position.y += player.speed.y;

            }

            // Player vs wall collision logic
            if (player.position.x + PLAYER_BASE_SIZE / 2 > screenWidth)
                player.position.x = screenWidth - PLAYER_BASE_SIZE / 2;
            else if (player.position.x - PLAYER_BASE_SIZE / 2 < 0)
                player.position.x = 0 + PLAYER_BASE_SIZE / 2;

            // Player shot logic
            if (IsKeyPressed(KEY_SPACE))
            {
                for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
                {
                    if (!shoot[i].active)
                    {
                        shoot[i].position = Vector2{player.position.x, player.position.y - shipHeight};
                        shoot[i].speed.y = PLAYER_SPEED;
                        shoot[i].active = true;

                        linePosition = Vector2{player.position.x, player.position.y};

                        break;
                    }
                }
            }

            // Shoot life timer
            for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
            {
                if (shoot[i].active)
                    shoot[i].lifeSpawn++;
            }

            // Shot logic
            for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
            {
                if (shoot[i].active)
                {
                    shoot[i].position.y -= shoot[i].speed.y;

                    // Shot vs walls collision logic
                    if ((shoot[i].position.x > screenWidth + shoot[i].radius) || (shoot[i].position.x < 0 - shoot[i].radius) ||
                        (shoot[i].position.y > screenHeight + shoot[i].radius) || (shoot[i].position.y < 0 - shoot[i].radius))
                    {
                        shoot[i].active = false;
                        shoot[i].lifeSpawn = 0;
                    }

                    // Player shot life spawn
                    if (shoot[i].lifeSpawn >= 120)
                    {
                        shoot[i].position = Vector2{0.0f, 0.0f};
                        shoot[i].speed = Vector2{0.0f, 0.0f};
                        shoot[i].lifeSpawn = 0;
                        shoot[i].active = false;
                    }
                }
            }

            // Player vs slimes collision logic
            player.collider = Vector3{player.position.x, player.position.y - shipHeight / 2, 12};

            for (int i = 0; i < MAX_BIG_SLIMES; i++)
            {
                if (CheckCollisionCircles(Vector2{player.collider.x, player.collider.y}, player.collider.z, bigSlimes[i].position, bigSlimes[i].radius) && bigSlimes[i].active)
                {
                    gameOver = true;
                }
            }

            for (int i = 0; i < MAX_BIG_SLIMES * 2; i++)
            {
                if (CheckCollisionCircles(Vector2{player.collider.x, player.collider.y}, player.collider.z, mediumSlimes[i].position, mediumSlimes[i].radius) && mediumSlimes[i].active)
                {
                    gameOver = true;
                }
            }

            for (int i = 0; i < MAX_BIG_SLIMES * 4; i++)
            {
                if (CheckCollisionCircles(Vector2{player.collider.x, player.collider.y}, player.collider.z, smallSlimes[i].position, smallSlimes[i].radius) && smallSlimes[i].active)
                {
                    gameOver = true;
                }
            }

            // slimes logic (big)
            for (int i = 0; i < MAX_BIG_SLIMES; i++)
            {
                if (bigSlimes[i].active)
                {
                    // slime movement logic
                    bigSlimes[i].position.x += bigSlimes[i].speed.x;
                    bigSlimes[i].position.y += bigSlimes[i].speed.y;

                    // slime vs wall collision logic
                    if (((bigSlimes[i].position.x + bigSlimes[i].radius) >= screenWidth) || ((bigSlimes[i].position.x - bigSlimes[i].radius) <= 0))
                        bigSlimes[i].speed.x *= -1;
                    if ((bigSlimes[i].position.y - bigSlimes[i].radius) <= 0)
                        bigSlimes[i].speed.y *= -1.5;

                    if ((bigSlimes[i].position.y + bigSlimes[i].radius) >= screenHeight)
                    {
                        bigSlimes[i].speed.y *= -1;
                        bigSlimes[i].position.y = screenHeight - bigSlimes[i].radius;
                    }

                    bigSlimes[i].speed.y += gravity;
                }
            }

            // slimes logic (medium)
            for (int i = 0; i < MAX_BIG_SLIMES * 2; i++)
            {
                if (mediumSlimes[i].active)
                {
                    // slime movement logic
                    mediumSlimes[i].position.x += mediumSlimes[i].speed.x;
                    mediumSlimes[i].position.y += mediumSlimes[i].speed.y;

                    // slime vs wall collision logic
                    if (mediumSlimes[i].position.x + mediumSlimes[i].radius >= screenWidth || mediumSlimes[i].position.x - mediumSlimes[i].radius <= 0)
                        mediumSlimes[i].speed.x *= -1;
                    if (mediumSlimes[i].position.y - mediumSlimes[i].radius <= 0)
                        mediumSlimes[i].speed.y *= -1;
                    if (mediumSlimes[i].position.y + mediumSlimes[i].radius >= screenHeight)
                    {
                        mediumSlimes[i].speed.y *= -1;
                        mediumSlimes[i].position.y = screenHeight - mediumSlimes[i].radius;
                    }

                    mediumSlimes[i].speed.y += gravity + 0.12f;
                }
            }

            // slimes logic (small)
            for (int i = 0; i < MAX_BIG_SLIMES * 4; i++)
            {
                if (smallSlimes[i].active)
                {
                    // slime movement logic
                    smallSlimes[i].position.x += smallSlimes[i].speed.x;
                    smallSlimes[i].position.y += smallSlimes[i].speed.y;

                    // slime vs wall collision logic
                    if (smallSlimes[i].position.x + smallSlimes[i].radius >= screenWidth || smallSlimes[i].position.x - smallSlimes[i].radius <= 0)
                        smallSlimes[i].speed.x *= -1;
                    if (smallSlimes[i].position.y - smallSlimes[i].radius <= 0)
                        smallSlimes[i].speed.y *= -1;
                    if (smallSlimes[i].position.y + smallSlimes[i].radius >= screenHeight)
                    {
                        smallSlimes[i].speed.y *= -1;
                        smallSlimes[i].position.y = screenHeight - smallSlimes[i].radius;
                    }

                    smallSlimes[i].speed.y += gravity + 0.25f;
                }
            }

            // Player-shot vs slimes logic
            for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
            {
                if ((shoot[i].active))
                {
                    for (int a = 0; a < MAX_BIG_SLIMES; a++)
                    {
                        if (bigSlimes[a].active && (bigSlimes[a].position.x - bigSlimes[a].radius <= linePosition.x && bigSlimes[a].position.x + bigSlimes[a].radius >= linePosition.x) && (bigSlimes[a].position.y + bigSlimes[a].radius >= shoot[i].position.y))
                        {
                            shoot[i].active = false;
                            shoot[i].lifeSpawn = 0;
                            bigSlimes[a].active = false;
                            slimesDestroyed++;

                            for (int j = 0; j < 2; j++)
                            {
                                if ((countmediumSlimes % 2) == 0)
                                {
                                    mediumSlimes[countmediumSlimes].position = Vector2{bigSlimes[a].position.x, bigSlimes[a].position.y};
                                    mediumSlimes[countmediumSlimes].speed = Vector2{-1 * SLIMES_SPEED, SLIMES_SPEED};
                                }
                                else
                                {
                                    mediumSlimes[countmediumSlimes].position = Vector2{bigSlimes[a].position.x, bigSlimes[a].position.y};
                                    mediumSlimes[countmediumSlimes].speed = Vector2{SLIMES_SPEED, SLIMES_SPEED};
                                }

                                mediumSlimes[countmediumSlimes].active = true;
                                countmediumSlimes++;
                            }

                            a = MAX_BIG_SLIMES;
                        }
                    }
                }

                if ((shoot[i].active))
                {
                    for (int b = 0; b < MAX_BIG_SLIMES * 2; b++)
                    {
                        if (mediumSlimes[b].active && (mediumSlimes[b].position.x - mediumSlimes[b].radius <= linePosition.x && mediumSlimes[b].position.x + mediumSlimes[b].radius >= linePosition.x) && (mediumSlimes[b].position.y + mediumSlimes[b].radius >= shoot[i].position.y))
                        {
                            shoot[i].active = false;
                            shoot[i].lifeSpawn = 0;
                            mediumSlimes[b].active = false;
                            slimesDestroyed++;

                            for (int j = 0; j < 2; j++)
                            {
                                if (countsmallSlimes % 2 == 0)
                                {
                                    smallSlimes[countsmallSlimes].position = Vector2{mediumSlimes[b].position.x, mediumSlimes[b].position.y};
                                    smallSlimes[countsmallSlimes].speed = Vector2{SLIMES_SPEED * -1, SLIMES_SPEED * -1};
                                }
                                else
                                {
                                    smallSlimes[countsmallSlimes].position = Vector2{mediumSlimes[b].position.x, mediumSlimes[b].position.y};
                                    smallSlimes[countsmallSlimes].speed = Vector2{SLIMES_SPEED, SLIMES_SPEED * -1};
                                }

                                smallSlimes[countsmallSlimes].active = true;
                                countsmallSlimes++;
                            }

                            b = MAX_BIG_SLIMES * 2;
                        }
                    }
                }

                if ((shoot[i].active))
                {
                    for (int c = 0; c < MAX_BIG_SLIMES * 4; c++)
                    {
                        if (smallSlimes[c].active && (smallSlimes[c].position.x - smallSlimes[c].radius <= linePosition.x && smallSlimes[c].position.x + smallSlimes[c].radius >= linePosition.x) && (smallSlimes[c].position.y + smallSlimes[c].radius >= shoot[i].position.y))
                        {
                            shoot[i].active = false;
                            shoot[i].lifeSpawn = 0;
                            smallSlimes[c].active = false;
                            slimesDestroyed++;

                            c = MAX_BIG_SLIMES * 4;
                        }
                    }
                }
            }

            if (slimesDestroyed == (MAX_BIG_SLIMES + MAX_BIG_SLIMES * 2 + MAX_BIG_SLIMES * 4))
                victory = true;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }

}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!gameOver)
    {
        // Draw player
        Vector2 v1 = {player.position.x + sinf(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cosf(player.rotation * DEG2RAD) * (shipHeight)};
        Vector2 v2 = {player.position.x - cosf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.position.y - sinf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2)};
        Vector2 v3 = {player.position.x + cosf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.position.y + sinf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2)};
        DrawTriangle(v1, v2, v3, MAROON);

        // Draw slimes (big)
        for (int i = 0; i < Slimes; i++)
        {
            if (bigSlimes[i].active)
                DrawCircleV(bigSlimes[i].position, bigSlimes[i].radius, DARKGRAY);
            else
                DrawCircleV(bigSlimes[i].position, bigSlimes[i].radius, Fade(LIGHTGRAY, 0.3f));
        }

        // Draw slimes (medium)
        for (int i = 0; i < MAX_BIG_SLIMES * 2; i++)
        {
            if (mediumSlimes[i].active)
                DrawCircleV(mediumSlimes[i].position, mediumSlimes[i].radius, GRAY);
            else
                DrawCircleV(mediumSlimes[i].position, mediumSlimes[i].radius, Fade(LIGHTGRAY, 0.3f));
        }

        // Draw slimes (small)
        for (int i = 0; i < MAX_BIG_SLIMES * 4; i++)
        {
            if (smallSlimes[i].active)
                DrawCircleV(smallSlimes[i].position, smallSlimes[i].radius, GRAY);
            else
                DrawCircleV(smallSlimes[i].position, smallSlimes[i].radius, Fade(LIGHTGRAY, 0.3f));
        }

        // Draw shoot
        for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
        {
            if (shoot[i].active)
                DrawLine(linePosition.x, linePosition.y, shoot[i].position.x, shoot[i].position.y, RED);
        }
    }
    else
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, LIGHTGRAY);

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}