#include <stb_image.h>
#include <file_system.h>
#include <string.h>

#include "glad/glad.h"
#include "umath.h"
#include "umbragl.h"
#include "umbra.h"

#define U_CONFIG_SHADERS_DIR U_SHADERS_DIR

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

void U_InitRenderer(URenderer* renderer, ui8 use_ebo)
{
  renderer->vertex_count = 0;
  renderer->index_count = 0;
  renderer->shader_id = 0;

  renderer->ebo = 0;
  renderer->use_ebo = use_ebo;

  glGenBuffers(1, &renderer->vbo);
  glGenVertexArrays(1, &renderer->vao);

  glBindVertexArray(renderer->vao);
  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

	if(use_ebo)
	{
		glGenBuffers(1, &renderer->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->buffer), &renderer->buffer, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderer->index_buffer), &renderer->index_buffer, GL_DYNAMIC_DRAW);

  // Attrib 0: { x, y, z }
  glEnableVertexAttribArray(U_DEFAULT_VERTEX_ATTR_POSITION_IDX);
  glVertexAttribPointer(U_DEFAULT_VERTEX_ATTR_POSITION_IDX, 3, GL_FLOAT, GL_FALSE, sizeof(UVertex), (void*)0);

  // Attrib 1: { r, g, b }
  glEnableVertexAttribArray(U_DEFAULT_VERTEX_ATTR_COLOR_IDX);
  glVertexAttribPointer(U_DEFAULT_VERTEX_ATTR_COLOR_IDX, 3, GL_FLOAT, GL_FALSE, sizeof(UVertex), (void*)(sizeof(UVec3)));

  glBindVertexArray(0);

	/** Set up shader **/
	i32 vert_shader = U_LoadShader(U_CONFIG_SHADERS_DIR"/default.vert.glsl", U_VERTEX_SHADER);
	i32 frag_shader = U_LoadShader(U_CONFIG_SHADERS_DIR"/default.frag.glsl", U_FRAGMENT_SHADER);

	if(vert_shader < 0 || frag_shader < 0)
	{
		// TODO: Add error return value
		printf("%s\n", U_CONFIG_SHADERS_DIR);
	}

	i32 shader = U_CompileShaderProgram(vert_shader, frag_shader);

	U_UseShader(renderer, shader);
}

void U_DrawTris(const UVec3 position, const f32 size, const UVec3 color)
{
	URenderer *renderer = &UCORE.renderer;

	ui32 prev_vert_count = renderer->vertex_count;
  renderer->buffer[renderer->vertex_count].position = position;

  renderer->buffer[1 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[1 + renderer->vertex_count].position.y = position.y + 0.0f;
  renderer->buffer[1 + renderer->vertex_count].position.z = position.z + 0.0f;

  renderer->buffer[2 + renderer->vertex_count].position.x = position.x + size / 2;
  renderer->buffer[2 + renderer->vertex_count].position.y = position.y + size;
  renderer->buffer[2 + renderer->vertex_count].position.z = position.z + 0.0f;

	if(renderer->use_ebo)
	{
		renderer->index_buffer[renderer->index_count] = renderer->vertex_count;
		renderer->index_buffer[1 + renderer->index_count] = 1 + renderer->vertex_count;
		renderer->index_buffer[2 + renderer->index_count] = 2 + renderer->vertex_count;

		renderer->index_count += 3;
	}

  renderer->vertex_count += 3;

	/** Set vertices color **/
	ui32 n = (renderer->vertex_count - prev_vert_count);

	for (ui32 i = 0; i < n; i++)
	{
		renderer->buffer[i + prev_vert_count].color = color;
	}

	UMat4x4 model_mat = U_Mat4x4(1.0f);
	glUniformMatrix4fv(
		glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_MODEL"),
		1, GL_TRUE,
		&model_mat.data[0][0]);
}

// TODO: Use element object buffers
void U_DrawQuad(const UVec3 position, const f32 size, const UVec3 color)
{
	URenderer* renderer = &UCORE.renderer;

	ui32 prev_vert_count = renderer->vertex_count;

  renderer->buffer[renderer->vertex_count].position = position;

  renderer->buffer[1 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[1 + renderer->vertex_count].position.y = position.y + 0.0f;
  renderer->buffer[1 + renderer->vertex_count].position.z = position.z + 0.0f;

  renderer->buffer[2 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[2 + renderer->vertex_count].position.y = position.y + size;
  renderer->buffer[2 + renderer->vertex_count].position.z = position.z + 0.0f;

	if(renderer->use_ebo)
	{
		// Define 4th vertex
		renderer->buffer[3 + renderer->vertex_count].position.x = position.x + 0.0f;
		renderer->buffer[3 + renderer->vertex_count].position.y = position.y + size;
		renderer->buffer[3 + renderer->vertex_count].position.z = position.z + 0.0f;

		renderer->index_buffer[renderer->index_count] = renderer->vertex_count;
		renderer->index_buffer[1 + renderer->index_count] = renderer->vertex_count + 1;
		renderer->index_buffer[2 + renderer->index_count] = renderer->vertex_count + 2;

		renderer->index_buffer[3 + renderer->index_count] = renderer->vertex_count;
		renderer->index_buffer[4 + renderer->index_count] = renderer->vertex_count + 2;
		renderer->index_buffer[5 + renderer->index_count] = renderer->vertex_count + 3;

		renderer->vertex_count += 4;
		renderer->index_count += 6;
	}
	else
	{
		renderer->buffer[3 + renderer->vertex_count].position = position;

		renderer->buffer[4 + renderer->vertex_count].position.x = position.x + size;
		renderer->buffer[4 + renderer->vertex_count].position.y = position.y + size;
		renderer->buffer[4 + renderer->vertex_count].position.z = position.z + 0.0f;

		renderer->buffer[5 + renderer->vertex_count].position.x = position.x + 0.0f;
		renderer->buffer[5 + renderer->vertex_count].position.y = position.y + size;
		renderer->buffer[5 + renderer->vertex_count].position.z = position.z + 0.0f;

		renderer->vertex_count += 6;
	}

	/** Set vertices color **/
	ui32 n = (renderer->vertex_count - prev_vert_count);

	for (ui32 i = 0; i < n; i++)
	{
		renderer->buffer[i + prev_vert_count].color = color;
	}

	UMat4x4 model_mat = U_Mat4x4(1.0f);
	glUniformMatrix4fv(
		glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_MODEL"),
		1, GL_TRUE,
		&model_mat.data[0][0]);
}

void U_DrawRect(const UVec3 position, const f32 width, const f32 height, const UVec3 color)
{
	URenderer* renderer = &UCORE.renderer;

	ui32 prev_vert_count = renderer->vertex_count;

  renderer->buffer[renderer->vertex_count].position = position;

  renderer->buffer[1 + renderer->vertex_count].position.x = position.x + width;
  renderer->buffer[1 + renderer->vertex_count].position.y = position.y + 0.0f;
  renderer->buffer[1 + renderer->vertex_count].position.z = position.z + 0.0f;

  renderer->buffer[2 + renderer->vertex_count].position.x = position.x + width;
  renderer->buffer[2 + renderer->vertex_count].position.y = position.y + height;
  renderer->buffer[2 + renderer->vertex_count].position.z = position.z + 0.0f;

	if(renderer->use_ebo)
	{
		// Define 4th vertex
		renderer->buffer[3 + renderer->vertex_count].position.x = position.x + 0.0f;
		renderer->buffer[3 + renderer->vertex_count].position.y = position.y + height;
		renderer->buffer[3 + renderer->vertex_count].position.z = position.z + 0.0f;

		renderer->index_buffer[renderer->index_count] = renderer->vertex_count;
		renderer->index_buffer[1 + renderer->index_count] = renderer->vertex_count + 1;
		renderer->index_buffer[2 + renderer->index_count] = renderer->vertex_count + 2;

		renderer->index_buffer[3 + renderer->index_count] = renderer->vertex_count;
		renderer->index_buffer[4 + renderer->index_count] = renderer->vertex_count + 2;
		renderer->index_buffer[5 + renderer->index_count] = renderer->vertex_count + 3;

		renderer->vertex_count += 4;
		renderer->index_count += 6;
	}
	else
	{
		renderer->buffer[3 + renderer->vertex_count].position = position;

		renderer->buffer[4 + renderer->vertex_count].position.x = position.x + width;
		renderer->buffer[4 + renderer->vertex_count].position.y = position.y + height;
		renderer->buffer[4 + renderer->vertex_count].position.z = position.z + 0.0f;

		renderer->buffer[5 + renderer->vertex_count].position.x = position.x + 0.0f;
		renderer->buffer[5 + renderer->vertex_count].position.y = position.y + height;
		renderer->buffer[5 + renderer->vertex_count].position.z = position.z + 0.0f;

		renderer->vertex_count += 6;
	}

	/** Set vertices color **/
	ui32 n = (renderer->vertex_count - prev_vert_count);

	for (ui32 i = 0; i < n; i++)
	{
		renderer->buffer[i + prev_vert_count].color = color;
	}

	UMat4x4 model_mat = U_Mat4x4(1.0f);
	glUniformMatrix4fv(
		glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_MODEL"),
		1, GL_TRUE,
		&model_mat.data[0][0]);
}

void U_BindVertexArray(ui32 vao)
{
  glBindVertexArray(vao);
}

void U_BeginDrawing()
{
	URenderer* renderer = &UCORE.renderer;

  renderer->vertex_count = 0;
  renderer->index_count = 0;
  U_BindVertexArray(renderer->vao);
}

void U_EndDrawing()
{
	URenderer* renderer = &UCORE.renderer;

  glClear(GL_COLOR_BUFFER_BIT);

  if(renderer->vertex_count > 0)
  {
		UMat4x4 view_mat = U_Mat4x4(1.0f);

		// Apply offset to center on view matrix
		UVec3 view_translate = { 0.0f, 0.0f, -0.9f }; // Camera position?
		view_mat = U_Translate(view_mat, view_translate);

		glUniformMatrix4fv(
			glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_VIEW"),
			1, GL_TRUE,
			&view_mat.data[0][0]);

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(renderer->buffer), &renderer->buffer);

		if(renderer->use_ebo)
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(renderer->index_buffer), &renderer->index_buffer);
			glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, renderer->vertex_count);
		}

    U_BindVertexArray(0);
  }
}

void U_SetViewport(URenderer* renderer, f32 left, f32 right, f32 bottom, f32 top, f32 clip_near, f32 clip_far)
{
  glViewport(left, top, right, bottom);
  glDepthRangef(clip_near, clip_far);

  UMat4x4 projection = U_MatOrtho(left, right, bottom, top, clip_near, clip_far);

  glUniformMatrix4fv(
      glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_PROJECTION"),
      1, GL_TRUE,
      &projection.data[0][0]);
}
