#include "umbra.h"

int main()
{
  U_InitWindow("hello, world", 800, 600);

  UCamera camera = { 0.0f, 0.0f, 0.0f, UProjectionType::U_ORTHO, 90.0f };

  TextureID tex = LoadTexture("/home/scylla/repos/umbra/examples/textures/sample_texture.jpg");
  
  while(!U_ShouldCloseWindow())
  {
    U_BeginDrawing(camera);

      U_DrawTextureQuad(&UCORE.renderer, {0.0f, 0.0f, 0.0f}, 100.0f, tex);

    U_EndDrawing();
  }

  U_CloseWindow();

  return 0;
}