#include "umbra.h"

int main()
{
    UWindow* window = U_InitWindow("umbra", 800, 600);

    URenderer renderer {};
    U_InitRenderer(&renderer);

    // Compile shader
    i32 vert_shader = U_LoadShader("C:/Dev/umbra/resources/shaders/basic.vert.glsl", U_VERTEX_SHADER);
    i32 frag_shader = U_LoadShader("C:/Dev/umbra/resources/shaders/basic.frag.glsl", U_FRAGMENT_SHADER);

    if(vert_shader < 0 || frag_shader < 0)
    {
        // error
    }

    ui32 shader = U_CompileShaderProgram(vert_shader, frag_shader);

    renderer.shader_id = shader;

    U_UseShader(shader);
    U_BindVertexArray(renderer.vao);

    U_SetViewport(0, 800, 600, 0, -1.0f, 1.0f);

    while(!U_ShouldCloseWindow(window))
    {
        U_BeginDrawing(&renderer);

            U_DrawTris(&renderer, { -0.5f, -0.5f, 0.0f }, { 0.5f, 1.0f, 1.0f });

        U_EndDrawing(&renderer);
    }

    U_CloseWindow(window);
    return 0;
}
