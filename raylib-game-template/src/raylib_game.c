/*******************************************************************************************
*
*   Top-down survivor - simple raylib game
*
*   Player moves with WASD/arrows, shoots toward the mouse click.
*   Enemies spawn at the screen edges and chase the player.
*   Bullet vs enemy = kill + score. Enemy vs player = lose HP.
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define MAX_ENEMIES 15
#define MAX_BULLETS 3
#define PLAYER_SPEED 220.0f
#define PLAYER_RADIUS 16.0f
#define ENEMY_RADIUS 14.0f
#define BULLET_RADIUS 5.0f
#define BULLET_SPEED 420.0f
// seconds between enemy spawns
#define SPAWN_INTERVAL 1.0f 

//----------------------------------------------------------------------------------
// Types
//----------------------------------------------------------------------------------
typedef struct Player 
{
    Vector2 pos;
    int health;
    int speed;
} Player;

typedef struct Enemy 
{
    Vector2 pos;
    float speed;
    bool active;
} Enemy;

typedef struct Bullet 
{
    Vector2 pos;
    Vector2 vel;
    bool active;
} Bullet;

//----------------------------------------------------------------------------------
// Global game state (kept simple and file-local on purpose)
//----------------------------------------------------------------------------------
static Player player;
static Enemy enemies[MAX_ENEMIES];
static Bullet bullets[MAX_BULLETS];
static int score = 0;
static float spawnTimer = 0.0f;
static bool gameOver = false;

static const int screenWidth = 800;
static const int screenHeight = 450;

//----------------------------------------------------------------------------------
// Module functions
//----------------------------------------------------------------------------------
static void ResetGame(void)
{
    player.pos = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    player.health = 5;
    score = 0;
    spawnTimer = 0.0f;
    gameOver = false;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].active = false;
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].active = false;
    }
}


// spawn enemy at a random point just outside the screen edge
static void SpawnEnemy(void)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].active)
        {
            // built in raylib function
            int edge = GetRandomValue(0, 3); 
            Vector2 pos = { 0 };

            if (edge == 0) pos = (Vector2)
            { 
                (float)GetRandomValue(0, screenWidth), -20 
            };
            else if (edge == 1) pos = (Vector2)
            { 
                (float)GetRandomValue(0, screenWidth), screenHeight + 20 
            };
            else if (edge == 2) pos = (Vector2)
            { 
                -20, (float)GetRandomValue(0, screenHeight) 
            };
            else pos = (Vector2)
            { 
                screenWidth + 20, (float)GetRandomValue(0, screenHeight) 
            };

            enemies[i].pos = pos;
            enemies[i].speed = (float)GetRandomValue(60, 110);
            enemies[i].active = true;
            break;
        }
    }
}

// fire a bullet from the player toward a target point
static void FireBullet(Vector2 mousePosition)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
			// built in raylib functions
            Vector2 dir = Vector2Normalize(Vector2Subtract(mousePosition, player.pos));
            bullets[i].pos = player.pos;
            bullets[i].vel = Vector2Scale(dir, BULLET_SPEED);
            bullets[i].active = true;
            break;
        }
    }
}

static void UpdatePlayer(float dt)
{
	player.speed = 1;
    Vector2 move = { 0 };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        move.y -= player.speed;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        move.y += player.speed;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        move.x -= player.speed;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        move.x += player.speed;
    }

    if (Vector2Length(move) > 0)
    {
        move = Vector2Normalize(move);
        // add two vectors 
        player.pos = Vector2Add(player.pos, Vector2Scale(move, PLAYER_SPEED * dt));
    }

	// prevent the player from going out of screen bounds
    player.pos.x = Clamp(player.pos.x, PLAYER_RADIUS, screenWidth - PLAYER_RADIUS);
    player.pos.y = Clamp(player.pos.y, PLAYER_RADIUS, screenHeight - PLAYER_RADIUS);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        FireBullet(GetMousePosition());
    }
}

static void UpdateEnemies(float dt)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].active)
        {
            continue;
        }

        Vector2 dir = Vector2Normalize(Vector2Subtract(player.pos, enemies[i].pos));
        enemies[i].pos = Vector2Add(enemies[i].pos, Vector2Scale(dir, enemies[i].speed * dt));

        // if enemy collides with player, player loses health and enemy is deactivated
        if (CheckCollisionCircles(enemies[i].pos, ENEMY_RADIUS, player.pos, PLAYER_RADIUS))
        {
            enemies[i].active = false;
            player.health--;
            if (player.health <= 0)
            {
                gameOver = true;
            }
        }
    }
}

static void UpdateBullets(float dt)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            continue;
        }

        bullets[i].pos = Vector2Add(bullets[i].pos, Vector2Scale(bullets[i].vel, dt));

        // deactivate bullets that out of bounds
        if (bullets[i].pos.x < -10 || bullets[i].pos.x > screenWidth + 10 ||
            bullets[i].pos.y < -10 || bullets[i].pos.y > screenHeight + 10)
        {
            bullets[i].active = false;
        }
    }
}

// bullet and enemy collisions
static void CheckCollisions()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            continue;
        }

        for (int j = 0; j < MAX_ENEMIES; j++)
        {
            if (!enemies[j].active)
            {
                continue;
            }

            if (CheckCollisionCircles(bullets[i].pos, BULLET_RADIUS, enemies[j].pos, ENEMY_RADIUS))
            {
                bullets[i].active = false;
                enemies[j].active = false;
                score += 10;
                break;
            }
        }
    }
}

static void DrawGame()
{
    ClearBackground(RAYWHITE);

    DrawCircleV(player.pos, PLAYER_RADIUS, BLUE);

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].active)
        {
            DrawCircleV(enemies[i].pos, ENEMY_RADIUS, RED);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            DrawCircleV(bullets[i].pos, BULLET_RADIUS, DARKGRAY);
        }
    }

    DrawText(TextFormat("Score: %d", score), 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("HP: %d", player.health), 10, 35, 20, MAROON);
    DrawText("WASD/arrows to move, click to shoot", 10, screenHeight - 25, 16, GRAY);

    if (gameOver)
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));
        DrawText("GAME OVER", screenWidth/2 - 100, screenHeight/2 - 20, 30, RAYWHITE);
        DrawText("Press ENTER to restart", screenWidth/2 - 110, screenHeight/2 + 20, 18, RAYWHITE);
    }
}

//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main()
{
    InitWindow(screenWidth, screenHeight, "Top-down survivor");
    SetTargetFPS(60);

    ResetGame();

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (gameOver)
        {
            if (IsKeyPressed(KEY_ENTER)) ResetGame();
        }
        else
        {
            spawnTimer += dt;
            if (spawnTimer >= SPAWN_INTERVAL)
            {
                spawnTimer = 0.0f;
                SpawnEnemy();
            }

            UpdatePlayer(dt);
            UpdateEnemies(dt);
            UpdateBullets(dt);
            CheckCollisions();
        }

        BeginDrawing();
        DrawGame();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
