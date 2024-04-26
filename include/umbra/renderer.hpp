#pragma once

#include <iostream>
#include <fileapi.h>
#include "types.hpp"
#include "utils.hpp"

#define U_FRAGMENT_SHADER 0x8B30 // GL_FRAGMENT_SHADER
#define U_VERTEX_SHADER 0x8B31 // GL_VERTEX_SHADER

enum UShaderType
{
  Vertex = U_FRAGMENT_SHADER,
  Fragment = U_VERTEX_SHADER,
};

namespace UECore
{
  struct Color
  {
    float r;
    float g;
    float b;
    float a;
  };

  struct Vec2
  {
    float x; float y;
  };

  struct Transform
  {
    Vec2 position;
    Vec2 size;
  };

  typedef ui32 TextureID;

  struct Texture
  {
    TextureID id;
  };

  class Renderer {

    private:

    protected:
      unsigned int VBO_QUAD = 0;
      unsigned int EBO_QUAD = 0;
      unsigned int VAO_QUAD = 0;

      unsigned int MULTIPLY_TEXTURE_ID = 0;

      // Todo: Warp shader funcionality
      // Todo: Compile multiple shader programs
      unsigned int SHADER_DEFAULT = 0;
      unsigned int CURRENT_SHADER_ID = 0;

      unsigned int
      LoadDefaultShaders(void);

      void
      InitQuad();

    public:

      Renderer()
      {
        //#region Init projection matrix
		    SHADER_DEFAULT = this->LoadDefaultShaders();
        this->SetCurrentShader(SHADER_DEFAULT);

        SetViewport(
            0.0f, 800.0f,
            600.0f, 0.0f,
            -1.0f, 1.0f
        );

        //#endregion

        this->InitQuad();

        this->MULTIPLY_TEXTURE_ID = this->LoadTexture("core/resources/multiply_text.jpg");
      };

      ~Renderer() {};

      /** Texture functions */
      Texture
      GenerateTexture();

      void
      BindTexture2D(TextureID id);

      void
      SetTextureImage2D(const void* image, ui32 width, ui32 height);

      /** State functions */
      int
      LoadShader(const char* filepath, UShaderType shader_type);

      ui32
      CompileShaderProgram(i32 vertex, i32 fragment);

      void
      EnableBlending();

      void
      DisableBlending();

      /** Rendering functions */
      void
      UseShader(ui32 shader_id);

      void
      SetCurrentShader(unsigned int shader_id)
      {
        this->CURRENT_SHADER_ID = shader_id;
      };

      unsigned int
      GetCurrentShader()
      {
        return this->CURRENT_SHADER_ID;
      };

      int
      LoadTexture(const char* filepath);

      void SetViewport(f32 left, f32 right, f32 bottom, f32 top, f32 clip_near, f32 clip_far);

      void
      ClearColor(f32 red, f32 green, f32 blue, f32 alpha);

      void
      DrawQuad(float x, float y, float width, float height, Color color);

      void
      DrawQuadTextured(float x, float y, float width, float height, unsigned int texture);
  };
}
