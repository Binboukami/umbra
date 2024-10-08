#include "umbragl.h"
#include "glad/glad.h"

void U_PollWindowEvents()
{
	glfwPollEvents();
}

void U_BindVertexArray(ui32 vao)
{
  glBindVertexArray(vao);
}

void U_BindVBO(ui32 vbo)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void U_BindEBO(ui32 ebo)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void U_BindTexture2D(TextureID id)
{
  glBindTexture(GL_TEXTURE_2D, id);
}

void U_EnableBlending()
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void U_DisableBlending()
{
  glDisable(GL_BLEND);
}

void U_EnableDepthTest()
{
  glEnable(GL_DEPTH_TEST);
}

void U_DisableDepthTest()
{
  glDisable(GL_DEPTH_TEST);
}

void U_SwapBuffers(UWindow window)
{
  glfwSwapBuffers(window._glfw_handler);
}

void U_ClearColor(f32 red, f32 green, f32 blue, f32 alpha)
{
  glClearColor(red, green, blue, alpha);
  glClear(GL_COLOR_BUFFER_BIT);
}

void U_SetViewport(f32 left, f32 right, f32 bottom, f32 top, f32 clip_near, f32 clip_far)
{
  glViewport(left, top, right, bottom);
  U_SetDepthRangeF(clip_near, clip_far);
}

void U_SetDepthRangeF(f32 near, f32 far)
{
  glDepthRangef(near, far);
}
