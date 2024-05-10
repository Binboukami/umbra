#include "glad/glad.h"
#include "umath.h"
#include "umbragl.h"
#include <stb_image.h>
#include <file_system.h>
#include <string.h>

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

void U_ClearColor(f32 red, f32 green, f32 blue, f32 alpha)
{
  glClearColor(red, green, blue, alpha);
  glClear(GL_COLOR_BUFFER_BIT);
}

void U_UseShader(URenderer* renderer, ui32 shader_id)
{
	renderer->shader_id = shader_id;
  glUseProgram(shader_id);
};

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

  size_t new_len = fread(shad_src, sizeof(char), buff_size, file_ptr);
  
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

void U_InitRenderer(URenderer* renderer)
{
  renderer->vertex_count = 0;
  renderer->shader_id = 0;

  glGenBuffers(1, &renderer->vbo);
  glGenBuffers(1, &renderer->ebo);

  glGenVertexArrays(1, &renderer->vao);

  glBindVertexArray(renderer->vao);

  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->buffer), renderer->buffer, GL_DYNAMIC_DRAW);

  // Attrib 0: { x, y, z }
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(UVertex), (void*)0);
  glEnableVertexAttribArray(0);

  // Attrib 1: { r, g, b }
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(Vec3)));
  // glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void U_DrawTris(URenderer* renderer, UVec3 position, UVec3 color)
{
  // 3 vertices
	UVec3 v_position = { 0.0f, 0.0f, 0.0f };
  renderer->buffer[renderer->vertex_count].position = v_position;

  renderer->buffer[1 + renderer->vertex_count].position.x = v_position.x + 1.0f;
  renderer->buffer[1 + renderer->vertex_count].position.y = v_position.y + 0.0f;
  renderer->buffer[1 + renderer->vertex_count].position.z = v_position.z + 0.0f;

  renderer->buffer[2 + renderer->vertex_count].position.x = v_position.x + 0.5f;
  renderer->buffer[2 + renderer->vertex_count].position.y = v_position.y + 1.0f;
  renderer->buffer[2 + renderer->vertex_count].position.z = v_position.z + 0.0f;
	
  UMat4x4 mat4 = U_Mat4x4(1.0f);
  UMat4x4 view_mat = U_Mat4x4(1.0f);

  UVec3 transform = position;
  mat4 = U_Translate(mat4, transform);

  glUniformMatrix4fv(
    glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_MODEL"),
    1, GL_TRUE,
    &mat4.data[0][0]);

	// Apply offset to center on view matrix
	UVec3 view_translate = { -0.5f, -0.5f, -0.9f }; // Camera position?
	view_mat = U_Translate(view_mat, view_translate);

  glUniformMatrix4fv(
    glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_VIEW"),
    1, GL_TRUE,
    &view_mat.data[0][0]);

  glUniform4f(
    glGetUniformLocation(renderer->shader_id, "uMeshColor"),
    color.x, color.y, color.z, 1.0f);

  renderer->vertex_count += 3;
}

// TODO: Use element object buffers
void U_DrawQuad(URenderer* renderer, UVec3 position, UVec3 color)
{
  // first tris
	UVec3 v_position = { 0.0f, 0.0f, 0.0f };
  renderer->buffer[renderer->vertex_count].position = v_position;

  renderer->buffer[1 + renderer->vertex_count].position.x = v_position.x + 1.0f;
  renderer->buffer[1 + renderer->vertex_count].position.y = v_position.y + 0.0f;
  renderer->buffer[1 + renderer->vertex_count].position.z = v_position.z + 0.0f;

  renderer->buffer[2 + renderer->vertex_count].position.x = v_position.x + 1.0f;
  renderer->buffer[2 + renderer->vertex_count].position.y = v_position.y + 1.0f;
  renderer->buffer[2 + renderer->vertex_count].position.z = v_position.z + 0.0f;

  // second tris
  renderer->buffer[3 + renderer->vertex_count].position = v_position;

  renderer->buffer[4 + renderer->vertex_count].position.x = v_position.x + 1.0f;
  renderer->buffer[4 + renderer->vertex_count].position.y = v_position.y + 1.0f;
  renderer->buffer[4 + renderer->vertex_count].position.z = v_position.z + 0.0f;

  renderer->buffer[5 + renderer->vertex_count].position.x = v_position.x + 0.0f;
  renderer->buffer[5 + renderer->vertex_count].position.y = v_position.y + 1.0f;
  renderer->buffer[5 + renderer->vertex_count].position.z = v_position.z + 0.0f;

  UMat4x4 mat4 = U_Mat4x4(1.0f);
  UMat4x4 view_mat = U_Mat4x4(1.0f);

  UVec3 transform = position;
  mat4 = U_Translate(mat4, transform);

  glUniformMatrix4fv(
    glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_MODEL"),
    1, GL_TRUE,
    &mat4.data[0][0]);

	// Apply offset to center on view matrix
	UVec3 view_translate = { -0.5f, -0.5f, -0.9f }; // Camera position?
	view_mat = U_Translate(view_mat, view_translate);

  glUniformMatrix4fv(
    glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_VIEW"),
    1, GL_TRUE,
    &view_mat.data[0][0]);

  glUniform4f(
    glGetUniformLocation(renderer->shader_id, "uMeshColor"),
    color.x, color.y, color.z, 1.0f);

  renderer->vertex_count += 6;
}

void U_BindVertexArray(ui32 vao)
{
  glBindVertexArray(vao);
}

void U_BeginDrawing(URenderer* renderer)
{
  renderer->vertex_count = 0;
  U_BindVertexArray(renderer->vao);
}

void U_EndDrawing(URenderer* renderer)
{
  glClear(GL_COLOR_BUFFER_BIT);

  if(renderer->vertex_count > 0)
  {
    glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->buffer), renderer->buffer, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, renderer->vertex_count);

    U_BindVertexArray(0);
  }
}

void U_SetViewport(f32 left, f32 right, f32 bottom, f32 top, f32 clip_near, f32 clip_far)
{
  glViewport(left, top, right, bottom);
	glDepthRangef(clip_near, clip_far);

	// TODO: Set orthogonal matrix

}
