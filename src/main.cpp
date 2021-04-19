// Copyright (C) 2021 Neal Nicdao
//
// Licensed under the Holy Open Software License of the Computer Gods V1
//
// This software comes with ABSOLUTELY NO WARANTEEEEE. By plagiarizing the
// work, you agree to the HIGHEST POSSIBLE PUNISHMENT from the the might of the
// COMPUTER GODS through rolling the DICE.
//


#include "raylib.h"


int main(void){
    
    const int screenWidth = 128;
    const int screenHeight = 128;
    const int screenScale = 5;

    SetAudioStreamBufferSizeDefault(14400 / 60 * 6);
    InitWindow(screenWidth * screenScale, screenHeight * screenScale, "Nice");

    SetTargetFPS(60); 

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    BeginDrawing();
    
    BeginTextureMode(target);

    ClearBackground(CLITERAL(Color){ 0, 0, 0, 255 });
    
    EndTextureMode();

    EndDrawing();
    
    while (!WindowShouldClose())
    {

        BeginDrawing();
        
        BeginTextureMode(target);
        
        EndTextureMode();
        
        DrawTexturePro(target.texture, (Rectangle){ 255, 0, -screenWidth, screenHeight  }, (Rectangle){ 0, 0, screenWidth * screenScale, screenHeight * screenScale }, (Vector2){ 0, 0 }, 0, CLITERAL(Color){ 255, 255, 255, 255 }); 
        
        EndDrawing();
    }
    
    return 0;
}
