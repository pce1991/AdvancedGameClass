
MosaicMem *Mosaic = NULL;

void ComputeGridSize(uint8 newWidth, uint8 newHeight) {
    MosaicMem *mosaic = Mosaic;
    
    mosaic->gridWidth = Clamp(newWidth, 1, 255);
    mosaic->gridHeight = Clamp(newHeight, 1, 255);

    // @TODO: free? 
    mosaic->tileCapacity = mosaic->gridWidth * mosaic->gridHeight;
    mosaic->tiles = (Tile *)malloc(sizeof(Tile) * mosaic->tileCapacity);

    memset(mosaic->tiles, 0, mosaic->tileCapacity * sizeof(Tile));

    mosaic->tileSize = (9.0f - mosaic->padding) / mosaic->gridWidth;

    // @TODO: add the line sizes
    mosaic->gridSize.x = mosaic->tileSize * mosaic->gridWidth;
    mosaic->gridSize.y = mosaic->tileSize * mosaic->gridHeight;
    
    mosaic->gridOrigin = V2(0) + V2(-mosaic->gridSize.x * 0.5f, mosaic->gridSize.y * 0.5f);
}

void MosaicInit(GameMemory *mem) {
    MosaicMem *mosaic = &mem->mosaic;
    Mosaic = mosaic;

    mosaic->gridWidth = 16;
    mosaic->gridHeight = 16;

    mosaic->tileCapacity = mosaic->gridWidth * mosaic->gridHeight;
    mosaic->tiles = (Tile *)malloc(sizeof(Tile) * mosaic->tileCapacity);

    memset(mosaic->tiles, 0, mosaic->tileCapacity * sizeof(Tile));

    mosaic->padding = 1.0f;

    AllocateRectBuffer(Mosaic->gridWidth * Mosaic->gridHeight, &mosaic->rectBuffer);

    real32 screenAspect = 16.0f / 9.0f;
    real32 levelAspect = mosaic->gridWidth / (mosaic->gridHeight * 1.0f);

    if (levelAspect > screenAspect) {
        
    }

    mosaic->tileSize = (9.0f - mosaic->padding) / mosaic->gridWidth;

    // Note: proportional to tileSize so the grid doesn't take up more room proportionally
    mosaic->lineThickness = mosaic->tileSize * 0.04f;

    // @TODO: add the line sizes
    mosaic->gridSize.x = mosaic->tileSize * mosaic->gridWidth;
    mosaic->gridSize.y = mosaic->tileSize * mosaic->gridHeight;
    
    mosaic->gridOrigin = V2(0) + V2(-mosaic->gridSize.x * 0.5f, mosaic->gridSize.y * 0.5f);

    mosaic->screenColor = V4(0.2f, 0.2f, 0.2f, 1.0f);
    mosaic->boardColor = V4(0, 0, 0, 1.0f);
    mosaic->lineColor = V4(0.8f, 0.8f, 0.8f, 1.0f);

    mosaic->onlyDrawBorder = true;

    Tile *tiles = Mosaic->tiles;
    for (int y = 0; y < Mosaic->gridHeight; y++) {
        for (int x = 0; x < Mosaic->gridWidth; x++) {
            int32 index = (y * Mosaic->gridWidth) + x;

            Tile *tile = &tiles[index];

            tile->position = V2i(x, y);
        }
    }

    Mosaic->guyDir = 1;
    Mosaic->guyUp = 1;
}

void RandomizeTiles() {
    Tile *tiles = Mosaic->tiles;
    for (int y = 0; y < Mosaic->gridHeight; y++) {
        for (int x = 0; x < Mosaic->gridWidth; x++) {
            int32 index = (y * Mosaic->gridWidth) + x;

            Tile *tile = &tiles[index];

            tile->position = V2i(x, y);
        }
    }
    
    for (int i = 0; i < Mosaic->tileCapacity; i++) {
        Tile *tile = &tiles[i];

        //int32 r = RandiRange(0, 2);
        //int32 r = Randi();
        int32 r = RandfRange(0, 2.0f);

        vec4 color = V4(RandfRange(0, 2.0f), RandfRange(0, 2.0f), RandfRange(0, 2.0f), 1.0f);

        if (r < 0.5f) {
            tile->active = true;
        }
        else {
            tile->active = false;
        }

        tile->color = color;
    }
}

vec2 GridPositionToWorldPosition(vec2i gridPosition) {
    vec2 worldPos = Mosaic->gridOrigin;
    real32 size = Mosaic->tileSize;
    worldPos.x += (size * gridPosition.x) + (size * 0.5f);
    worldPos.y += (-size * gridPosition.y) + (-size * 0.5f);
    return worldPos;
}

void DrawTile(vec2i position, vec4 color) {
    vec2 worldPos = GridPositionToWorldPosition(position);
    DrawRect(worldPos, V2(Mosaic->tileSize * 0.5f), color);
    // Instancing
    //DrawRect(&Mosaic->rectBuffer, worldPos, V2(Mosaic->tileSize * 0.5f), color);
}

void DrawBorder() {
    for (int y = 0; y < Mosaic->gridHeight + 1; y++) {

        if (y > 0 && y < Mosaic->gridHeight) { continue; }
        vec2 rowLineCenter = Mosaic->gridOrigin + V2((Mosaic->gridSize.x * 0.5f), 0) + V2(0, -y * Mosaic->tileSize);
        DrawRect(rowLineCenter, V2(Mosaic->gridSize.x * 0.5f, Mosaic->lineThickness), Mosaic->lineColor);
        
    }

    for (int x = 0; x < Mosaic->gridWidth + 1; x++) {
        if (x > 0 && x < Mosaic->gridWidth) { continue; }
        
        vec2 colLineCenter = Mosaic->gridOrigin + V2(0, (-Mosaic->gridSize.y * 0.5f)) + V2(x * Mosaic->tileSize, 0);
        DrawRect(colLineCenter, V2(Mosaic->lineThickness, Mosaic->gridSize.y * 0.5f), Mosaic->lineColor);
    }
}

void DrawGrid() {
    for (int y = 0; y < Mosaic->gridHeight + 1; y++) {

        vec2 rowLineCenter = Mosaic->gridOrigin + V2((Mosaic->gridSize.x * 0.5f), 0) + V2(0, -y * Mosaic->tileSize);
        DrawRect(rowLineCenter, V2(Mosaic->gridSize.x * 0.5f, Mosaic->lineThickness), Mosaic->lineColor);
        
    }

    for (int x = 0; x < Mosaic->gridWidth + 1; x++) {
        vec2 colLineCenter = Mosaic->gridOrigin + V2(0, (-Mosaic->gridSize.y * 0.5f)) + V2(x * Mosaic->tileSize, 0);
        DrawRect(colLineCenter, V2(Mosaic->lineThickness, Mosaic->gridSize.y * 0.5f), Mosaic->lineColor);
    }
}

Tile *GetHoveredTile() {
    InputQueue *input = &Game->inputQueue;

    vec2 mousePos = input->mousePosNormSigned;
    mousePos.x *= 8;
    mousePos.y *= 4.5f;

    real32 xDistFromOrig = mousePos.x - Mosaic->gridOrigin.x;
    real32 yDistFromOrig = Mosaic->gridOrigin.y - mousePos.y;

    if (xDistFromOrig < 0 || yDistFromOrig < 0) { return NULL; }

    int32 xCoord = xDistFromOrig / Mosaic->tileSize;
    int32 yCoord = yDistFromOrig / Mosaic->tileSize;

    if (xCoord >= Mosaic->gridWidth || yCoord >= Mosaic->gridHeight) {
        return NULL;
    }

    int32 index = (yCoord * Mosaic->gridWidth) + xCoord;
    return &Mosaic->tiles[index];
}

Tile *GetTile(int32 x, int32 y) {
    if (x < 0 || x >= Mosaic->gridWidth) {
        return NULL;
    }
    if (y < 0 || y >= Mosaic->gridHeight) {
        return NULL;
    }
    
    // TODO: clamp these in case they're out of bounds, so we don't crash
    int32 index = (y * Mosaic->gridWidth) + x;
    return &Mosaic->tiles[index];
}


void MosaicUpdate(GameMemory *mem) {
    MosaicMem *mosaic = &mem->mosaic;

    InputQueue *input = &Game->inputQueue;

    Mosaic->hoveredTilePrev = Mosaic->hoveredTile;
    Mosaic->hoveredTile = GetHoveredTile();

    Tile *tiles = Mosaic->tiles;

    for (int y = 0; y < Mosaic->gridHeight; y++) {
        for (int x = 0; x < Mosaic->gridWidth; x++) {
            Tile *tile = GetTile(x, y);

            float t = x / (Mosaic->gridWidth  * 1.0f);
            float t2 = y / (Mosaic->gridHeight  * 1.0f);

            // tile->color = Lerp(V4(1.0f, 0.8f, 0.8f, 1.0f),
            //                    V4(0.6f, 0.0f, 0.0f, 1.0f), t) * Lerp(1.0f, 0.6f, t2);

            //tile->color = V4(0.54f, 1.0f, 0.37f, 1.0f);
            //tile->color = V4(0.674f, 0.13f, 0.82, 1.0f);
            tile->color = V4(0.4444444444f, 1.0f, 0.681953145f, 1.0f) +
                V4(sinf(Game->time * 6) * 0.25f, 0.0f, 0.1f, 1.0f);
            
            // tile->color =
            //     Lerp(V4(1.0f, 0.5f, 0.0f, 1.0f),
            //          Lerp(V4(0.2f, 0.6f, 1.0f, 1.0f),
            //               V4(0.15f, 0.4f, 1.0f, 1.0f), t),
            //          t2);

            // tile->color = tile->color +
            //     Lerp(V4(0.05f, 1.0f, 1.0f, 1.0f),
            //          V4(1.0f, 0.5f, 0.0f, 1.0f), t) * 0.5f;
        
            tile->active = true;
        }
    }

    vec4 guyColor = V4(0.5f + (Mosaic->guyPos.x / Mosaic->gridWidth), 0.5f * (Mosaic->guyPos.x / Mosaic->gridWidth), 0.9f, 1.0f);

    Mosaic->guyPos.x += (0.5f + cosf(Game->time * 5) * 0.25f) * Mosaic->guyDir;

    if (Mosaic->guyPos.x > 16) {
        Mosaic->guyDir = -1;
        Mosaic->guyPos.y += Mosaic->guyUp;
        //Mosaic->guyPos.y = 11;
    }
    if (Mosaic->guyPos.x < 0) {
        Mosaic->guyDir = 1;
        Mosaic->guyPos.y += Mosaic->guyUp;
    }

    if (Mosaic->guyPos.y <= 0) {
        Mosaic->guyUp = 1;
    }
    if (Mosaic->guyPos.y >= 15) {
        Mosaic->guyUp = -1;
    }

    vec2 guyPos = Mosaic->guyPos;
    
    Tile *guyTile = GetTile(guyPos.x, guyPos.y);
    if (guyTile) {
        guyTile->color = guyColor;
    }
    
    Tile* hoveredTile = Mosaic->hoveredTile;

    if (hoveredTile != NULL) {
        hoveredTile->active = true;
        hoveredTile->color = V4(1);
    }
    if (Mosaic->hoveredTilePrev !=  NULL && Mosaic->hoveredTilePrev != hoveredTile) {
        //Mosaic->hoveredTilePrev->active = false;
    }
    // store previous hovered tile and set it to inactive

    
    
    glClearColor(Mosaic->screenColor.r, Mosaic->screenColor.g, Mosaic->screenColor.b, 1.0f);
    Mosaic->rectBuffer.count = 0;
    {
        DrawRect(V2(0), Mosaic->gridSize * 0.5f, Mosaic->boardColor);
    }

    for (int i = 0; i < mosaic->tileCapacity; i++) {
        Tile *tile = &tiles[i];

        if (tile->active) {
            DrawTile(tile->position, tile->color);
        }
    }

    if (Mosaic->onlyDrawBorder) {
        DrawBorder();    
    }
    else {
        DrawGrid();        
    }

    //Instancing
    //RenderRectBuffer(&Mosaic->rectBuffer);
}
