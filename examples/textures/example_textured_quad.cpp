#include "umbra.h"

int main()
{
  (void)U_InitContext(); // Ignore result
  U_InitWindow("hello, world", 800, 600);

  constexpr UCamera camera = { 0.0f, 0.0f, 0.0f, UProjectionType::U_ORTHO, 90.0f };
  const TextureID tex = LoadTexture("../../../examples/textures/sample_texture.jpg");
  
  while(!U_ShouldCloseWindow())
  {
    U_BeginDrawing(camera);

      U_DrawTextureQuad({0.0f, 0.0f, 0.0f}, 100.0f, tex);

    U_EndDrawing();
  }

  U_CloseWindow();
  U_DestroyContext();

  return 0;
}