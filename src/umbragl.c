#include <file_system.h>
#include "umbragl.h"
#include "glad/glad.h"

GLState G_GL_STATE = {0}; // TODO: What happens for multiple umbragl instances?

void U_PollWindowEvents()
{
	glfwPollEvents();
}

void U_SetVertexAttribute(ui64 idx, i64 size, ui64 type, bool normalized, ui64 stride, ui64 offset)
{
  UVertextArrayObject current_vao = G_GL_STATE.vaos[G_GL_STATE.current_vao];

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

void U_EnableVertexAttribute(ui64 idx)
{
  UVertextArrayObject current_vao = G_GL_STATE.vaos[G_GL_STATE.current_vao];

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

  UVertextArrayObject new_vao = { .id = *buffer };

  G_GL_STATE._vaos_counter++;
  G_GL_STATE.vaos[G_GL_STATE._vaos_counter] = new_vao;
};

void U_BindVertexArray(ui32 vao)
{
  glBindVertexArray(vao);
  G_GL_STATE.current_vao = vao;
}

void U_BindVBO(ui32 vbo)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  G_GL_STATE.current_vbo = vbo;
}

void U_BindEBO(ui32 ebo)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void U_BindTexture2D(TextureID id)
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

i32 U_LoadShader(const char* filepath, U_SHADER_TYPE shader_type)
{
  FILE *file_ptr;

  /** Load shader source from file */

  file_ptr = fopen(filepath, "r");

  if(file_ptr == NULL)
  {
    fprintf(stderr, "Could not open shader file: '%s'\n", filepath);
    return -1;
  }

  long buff_size = file_size(file_ptr);

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

  ui32 gl_shader;
  gl_shader = glCreateShader(shader_type);

  glShaderSource(gl_shader, 1, &shad_src, NULL);
  glCompileShader(gl_shader);

  free(shad_src);

  int  success;
  char infoLog[512];
  glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(gl_shader, 512, NULL, infoLog);
		char* stype = shader_type == U_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";

    fprintf(stderr, "ERROR::%s_SHADER::COMPILATION_FAILED\n%s", stype, infoLog);

    return -4;
  }

  return gl_shader;
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
