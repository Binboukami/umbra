#include <file_system.h>
#include "glad/glad.h"
#include "umbragl.h"
#include <stdlib.h>
#include <string.h>

GLState G_GL_STATE = {0}; // TODO: What happens for multiple umbragl instances?

void U_PollWindowEvents()
{
	glfwPollEvents();
}

void U_SetVertexAttribute(const ui64 idx, const i64 size, const ui64 type, const bool normalized, const ui64 stride, const ui64 offset)
{
  UVertexArrayObject current_vao = G_GL_STATE.vaos[G_GL_STATE.current_vao];

  UVertexAttribute new_attr = { idx, size, type, normalized, stride, offset };

  glVertexAttribPointer(idx, size, type, normalized, stride, (void*)(offset));

  current_vao._attributes[idx] = new_attr;

  if(current_vao._attrs_counter + 1 > U_MAX_VERTEX_ATTRIBUTES)
  {
    // todo: error out
  }
  
  current_vao._attrs_counter++;

  U_EnableVertexAttribute(idx);
}

void U_EnableVertexAttribute(const ui64 idx)
{
  UVertexArrayObject current_vao = G_GL_STATE.vaos[G_GL_STATE.current_vao];

  if(!current_vao.enabled_attributes[idx])
  {
    current_vao.enabled_attributes[idx] = true;
    current_vao._enabled_attrs_counter++;
  }

  glEnableVertexAttribArray(idx);
}

void U_GenerateVAO(ui32* buffer)
{
  glGenVertexArrays(1, buffer);

  const UVertexArrayObject new_vao = { .id = *buffer };

  G_GL_STATE._vaos_counter++;
  G_GL_STATE.vaos[G_GL_STATE._vaos_counter] = new_vao;
};

void U_BindVertexArray(const ui32 vao)
{
  glBindVertexArray(vao);
  G_GL_STATE.current_vao = vao;
}

void U_BindVBO(const ui32 vbo)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  G_GL_STATE.current_vbo = vbo;
}

void U_BindEBO(const ui32 ebo)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void U_BindTexture2D(const TextureID id)
{
  glBindTexture(GL_TEXTURE_2D, id);
}

/** This function consumes the shader src, making them unusable after call */
ui32 U_CompileShaderProgram(i32 vertex_shader, i32 fragment_shader)
{
  ui32
  shader = glCreateProgram();

  glAttachShader(shader, vertex_shader);
  glAttachShader(shader, fragment_shader);

  glLinkProgram(shader);

  int success;
  char infoLog[512];

  glGetProgramiv(shader, GL_LINK_STATUS, &success);

  if(!success) {
      glGetProgramInfoLog(shader, 512, NULL, infoLog);
      fprintf(stderr, "ERROR::SHADER::PROGRAM_COMPILATION_FAILED\n%s", infoLog);
  }

  glDetachShader(shader, vertex_shader);
  glDeleteShader(vertex_shader);

  glDetachShader(shader, fragment_shader);
  glDeleteShader(fragment_shader);

  return shader;
}

i32 U_LoadShader(const char* filepath, const U_SHADER_TYPE shader_type)
{
  /** Load shader source from file */
  FILE *file_ptr = fopen(filepath, "r");

  if(file_ptr == NULL)
  {
    fprintf(stderr, "Could not open shader file: '%s'\n", filepath);
    return -1;
  }

  const i128 buff_size = file_size(file_ptr);

  if(buff_size == -1)
  {
    fprintf(stderr, "Could not read file size from '%s'\n", filepath);
    return -2;
  }

  /** Intialize memory to prevent erros when writing to the buffer */
  char *shad_src = memset((char*)malloc(sizeof(char) * (buff_size + 1)), '\0', buff_size + 1);

  if(shad_src == NULL)
  {
    /** Error intializing memory */
		return -3;
  }

  fread(shad_src, sizeof(char), buff_size, file_ptr);

  if(ferror(file_ptr) != 0)
  {
    fclose(file_ptr);
    free(shad_src);

    fprintf(stderr, "Unexpected error while reading file");
    return -3;
  }

  /** Compilation step */

  ui32 gl_shader = glCreateShader(shader_type);

  glShaderSource(gl_shader, 1, &shad_src, NULL);
  glCompileShader(gl_shader);

  free(shad_src);

  int  success;
  glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    char infoLog[512];
    glGetShaderInfoLog(gl_shader, 512, NULL, infoLog);
		char* print_shader_type = (shader_type == U_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";

    fprintf(stderr, "ERROR::%s_SHADER::COMPILATION_FAILED\n%s", print_shader_type, infoLog);

    return -4;
  }

  return gl_shader; // gl_shader should always be > 0 at this point
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

void U_EnableScissor()
{
  glEnable(GL_SCISSOR_TEST);
}

void U_DisableScissor()
{
  glDisable(GL_SCISSOR_TEST);
}

void U_SwapBuffers(UWindow window)
{
  glfwSwapBuffers(window._glfw_handler);
}

void U_ClearColor(const f32 red, const f32 green, const f32 blue, const f32 alpha)
{
  glClearColor(red, green, blue, alpha);
  glClear(GL_COLOR_BUFFER_BIT);
}

void U_SetViewport(const f32 left, const f32 top, const f32 right, const f32 bottom)
{
  glViewport(left, top, right, bottom);
}

void U_SetViewport3D(const f32 left, const f32 top, const f32 right, const f32 bottom, const f32 clip_near, const f32 clip_far)
{
  U_SetViewport(left, top, right, bottom);
  U_SetDepthRangeF(clip_near, clip_far);
}

void U_SetDepthRangeF(const f32 near, const f32 far)
{
  glDepthRangef(near, far);
}

void U_Scissor(f32 x, f32 y, f32 width, f32 height)
{
  glScissor(x, y, width, height);
}
