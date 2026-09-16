#include "player.h"
#include "constants.h"
#include "tile.h"
#include <math.h>

/*
    player.c

    Player movement is resolved one axis at a time. First the player moves
    horizontally and is pushed out of any wall, then the same happens
    vertically. Separating the axes keeps tile collision easy to follow and
    prevents a floor collision from incorrectly stopping horizontal motion.
*/

static int PixelToTile(float pixelPosition)
{
    return (int)floorf(pixelPosition / TILE_SIZE);
}

static bool IsSolidAt(const World *world, int tileX, int tileY)
{
    return Tile_IsSolid(World_GetTile(world, tileX, tileY));
}

static void ResolveHorizontalCollision(Player *player, const World *world)
{
    const float edgeInset = 0.01f;
    int topTile = PixelToTile(player->position.y + edgeInset);
    int bottomTile = PixelToTile(player->position.y + player->height - edgeInset);

    if (player->velocity.x > 0.0f)
    {
        int rightTile = PixelToTile(player->position.x + player->width);

        for (int tileY = topTile; tileY <= bottomTile; tileY++)
        {
            if (IsSolidAt(world, rightTile, tileY))
            {
                player->position.x = rightTile * TILE_SIZE - player->width;
                player->velocity.x = 0.0f;
                break;
            }
        }
    }
    else if (player->velocity.x < 0.0f)
    {
        int leftTile = PixelToTile(player->position.x);

        for (int tileY = topTile; tileY <= bottomTile; tileY++)
        {
            if (IsSolidAt(world, leftTile, tileY))
            {
                player->position.x = (leftTile + 1) * TILE_SIZE;
                player->velocity.x = 0.0f;
                break;
            }
        }
    }
}

static void ResolveVerticalCollision(Player *player, const World *world)
{
    const float edgeInset = 0.01f;
    int leftTile = PixelToTile(player->position.x + edgeInset);
    int rightTile = PixelToTile(player->position.x + player->width - edgeInset);

    player->isOnGround = false;

    if (player->velocity.y > 0.0f)
    {
        int bottomTile = PixelToTile(player->position.y + player->height);

        for (int tileX = leftTile; tileX <= rightTile; tileX++)
        {
            if (IsSolidAt(world, tileX, bottomTile))
            {
                player->position.y = bottomTile * TILE_SIZE - player->height;
                player->velocity.y = 0.0f;
                player->isOnGround = true;
                break;
            }
        }
    }
    else if (player->velocity.y < 0.0f)
    {
        int topTile = PixelToTile(player->position.y);

        for (int tileX = leftTile; tileX <= rightTile; tileX++)
        {
            if (IsSolidAt(world, tileX, topTile))
            {
                player->position.y = (topTile + 1) * TILE_SIZE;
                player->velocity.y = 0.0f;
                break;
            }
        }
    }
}

static void KeepPlayerInsideWorld(Player *player)
{
    float worldWidth = WORLD_WIDTH_TILES * TILE_SIZE;
    float worldHeight = WORLD_HEIGHT_TILES * TILE_SIZE;

    if (player->position.x < 0.0f)
    {
        player->position.x = 0.0f;
        player->velocity.x = 0.0f;
    }
    if (player->position.x + player->width > worldWidth)
    {
        player->position.x = worldWidth - player->width;
        player->velocity.x = 0.0f;
    }
    if (player->position.y < 0.0f)
    {
        player->position.y = 0.0f;
        player->velocity.y = 0.0f;
    }
    if (player->position.y + player->height > worldHeight)
    {
        player->position.y = worldHeight - player->height;
        player->velocity.y = 0.0f;
        player->isOnGround = true;
    }
}

void Player_Init(Player *player, Vector2 spawnPosition)
{
    player->position = spawnPosition;
    player->velocity = (Vector2){ 0.0f, 0.0f };
    player->width = PLAYER_WIDTH;
    player->height = PLAYER_HEIGHT;
    player->isOnGround = false;
}

void Player_Update(Player *player, const World *world,
                   PlayerInput input, float deltaTime)
{
    const float maxPhysicsStep = 1.0f / 120.0f;

    /* Ignore an unusually large pause so the simulation does not try to
       catch up by moving the player a huge distance in one frame. */
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    player->velocity.x = input.moveDirection * PLAYER_MOVE_SPEED;

    if (input.jumpPressed && player->isOnGround)
    {
        player->velocity.y = -PLAYER_JUMP_SPEED;
        player->isOnGround = false;
    }

    /* Small physics steps keep the player from passing through a one-tile
       floor when the frame rate briefly drops. */
    float remainingTime = deltaTime;
    while (remainingTime > 0.0f)
    {
        float stepTime = remainingTime;
        if (stepTime > maxPhysicsStep)
        {
            stepTime = maxPhysicsStep;
        }

        player->velocity.y += PLAYER_GRAVITY * stepTime;
        if (player->velocity.y > PLAYER_MAX_FALL_SPEED)
        {
            player->velocity.y = PLAYER_MAX_FALL_SPEED;
        }

        player->position.x += player->velocity.x * stepTime;
        ResolveHorizontalCollision(player, world);

        player->position.y += player->velocity.y * stepTime;
        ResolveVerticalCollision(player, world);

        remainingTime -= stepTime;
    }

    KeepPlayerInsideWorld(player);
}

void Player_Draw(const Player *player)
{
    Rectangle body = {
        player->position.x,
        player->position.y,
        player->width,
        player->height
    };

    DrawRectangleRec(body, (Color){ 70, 130, 210, 255 });
    DrawRectangleLinesEx(body, 1.0f, DARKBLUE);
}

Vector2 Player_GetCenter(const Player *player)
{
    return (Vector2){
        player->position.x + player->width / 2.0f,
        player->position.y + player->height / 2.0f
    };
}
