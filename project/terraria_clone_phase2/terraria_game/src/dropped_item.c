#include "dropped_item.h"
#include "constants.h"
#include "tile.h"
#include <math.h>

/*
    dropped_item.c

    Like particles, dropped items use a fixed pool. Unlike particles, they
    collide with the ground and stay active until collected.
*/

static DroppedItem *FindUnusedItem(DroppedItemSystem *system)
{
    for (int i = 0; i < MAX_DROPPED_ITEMS; i++)
    {
        if (!system->items[i].isActive)
        {
            return &system->items[i];
        }
    }

    return NULL;
}

static void ResolveGroundCollision(DroppedItem *item, const World *world)
{
    if (item->velocity.y <= 0.0f)
    {
        return;
    }

    const float edgeInset = 0.01f;
    int leftTile = (int)floorf((item->position.x + edgeInset) / TILE_SIZE);
    int rightTile = (int)floorf(
        (item->position.x + DROPPED_ITEM_SIZE - edgeInset) / TILE_SIZE);
    int bottomTile = (int)floorf(
        (item->position.y + DROPPED_ITEM_SIZE) / TILE_SIZE);

    for (int tileX = leftTile; tileX <= rightTile; tileX++)
    {
        if (Tile_IsSolid(World_GetTile(world, tileX, bottomTile)))
        {
            item->position.y = bottomTile * TILE_SIZE - DROPPED_ITEM_SIZE;
            item->velocity.y = 0.0f;
            item->velocity.x *= 0.8f;
            return;
        }
    }
}

static void KeepItemInsideWorld(DroppedItem *item)
{
    float worldWidth = WORLD_WIDTH_TILES * TILE_SIZE;
    float worldHeight = WORLD_HEIGHT_TILES * TILE_SIZE;

    if (item->position.x < 0.0f)
    {
        item->position.x = 0.0f;
        item->velocity.x = 0.0f;
    }
    if (item->position.x + DROPPED_ITEM_SIZE > worldWidth)
    {
        item->position.x = worldWidth - DROPPED_ITEM_SIZE;
        item->velocity.x = 0.0f;
    }
    if (item->position.y + DROPPED_ITEM_SIZE > worldHeight)
    {
        item->position.y = worldHeight - DROPPED_ITEM_SIZE;
        item->velocity.y = 0.0f;
    }
}

static bool TryCollectItem(DroppedItem *item, const Player *player,
                           Inventory *inventory)
{
    Rectangle itemBounds = {
        item->position.x,
        item->position.y,
        DROPPED_ITEM_SIZE,
        DROPPED_ITEM_SIZE
    };
    Rectangle playerBounds = {
        player->position.x,
        player->position.y,
        player->width,
        player->height
    };

    if (!CheckCollisionRecs(itemBounds, playerBounds))
    {
        return false;
    }

    item->quantity = Inventory_AddItem(inventory, item->type, item->quantity);
    return item->quantity == 0;
}

void DroppedItemSystem_Init(DroppedItemSystem *system)
{
    *system = (DroppedItemSystem){ 0 };
}

void DroppedItemSystem_Spawn(DroppedItemSystem *system, ItemType type,
                             int quantity, Vector2 centerPosition)
{
    if (type == ITEM_NONE || quantity <= 0)
    {
        return;
    }

    DroppedItem *item = FindUnusedItem(system);
    if (item == NULL)
    {
        return;
    }

    item->position = (Vector2){
        centerPosition.x - DROPPED_ITEM_SIZE / 2.0f,
        centerPosition.y - DROPPED_ITEM_SIZE / 2.0f
    };
    item->velocity = (Vector2){
        (float)GetRandomValue(-45, 45),
        (float)GetRandomValue(-120, -70)
    };
    item->type = type;
    item->quantity = quantity;
    item->pickupDelay = ITEM_PICKUP_DELAY;
    item->isActive = true;
}

void DroppedItemSystem_Update(DroppedItemSystem *system, const World *world,
                              const Player *player, Inventory *inventory,
                              float deltaTime)
{
    const float maxPhysicsStep = 1.0f / 120.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    for (int i = 0; i < MAX_DROPPED_ITEMS; i++)
    {
        DroppedItem *item = &system->items[i];
        if (!item->isActive)
        {
            continue;
        }

        item->pickupDelay -= deltaTime;

        float remainingTime = deltaTime;
        while (remainingTime > 0.0f)
        {
            float stepTime = remainingTime;
            if (stepTime > maxPhysicsStep)
            {
                stepTime = maxPhysicsStep;
            }

            item->velocity.y += DROPPED_ITEM_GRAVITY * stepTime;
            item->position.x += item->velocity.x * stepTime;
            item->position.y += item->velocity.y * stepTime;
            ResolveGroundCollision(item, world);

            remainingTime -= stepTime;
        }

        KeepItemInsideWorld(item);

        if (item->pickupDelay <= 0.0f &&
            TryCollectItem(item, player, inventory))
        {
            item->isActive = false;
        }
    }
}

void DroppedItemSystem_Draw(const DroppedItemSystem *system)
{
    for (int i = 0; i < MAX_DROPPED_ITEMS; i++)
    {
        const DroppedItem *item = &system->items[i];
        if (!item->isActive)
        {
            continue;
        }

        DrawRectangleV(item->position,
                       (Vector2){ DROPPED_ITEM_SIZE, DROPPED_ITEM_SIZE },
                       Item_GetColor(item->type));
        DrawRectangleLines((int)item->position.x, (int)item->position.y,
                           (int)DROPPED_ITEM_SIZE, (int)DROPPED_ITEM_SIZE,
                           BLACK);
    }
}
