#include <file_system.h>
#include <string.h>

#include "umbra.h"
#include "umbragl.h"
#include "urenderer.h"

#define U_CONFIG_SHADERS_DIR U_SHADERS_DIR

void U_InitRenderer(URenderer* renderer, ui8 use_ebo)
{
  renderer->vertex_count = 0;
  renderer->index_count = 0;
  renderer->shader_id = 0;

  renderer->ebo = 0;
  renderer->use_ebo = use_ebo;

  // Initialize vbo data
  glGenBuffers(1, &renderer->vbo);
  U_BindVBO(renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->buffer), &renderer->buffer, GL_DYNAMIC_DRAW);

  glGenVertexArrays(1, &renderer->vao);

  U_BindVertexArray(renderer->vao);

	if(use_ebo)
	{
		glGenBuffers(1, &renderer->ebo);
    U_BindEBO(renderer->ebo);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderer->index_buffer), &renderer->index_buffer, GL_DYNAMIC_DRAW);

  // Attrib 0: { x, y, z }
  glEnableVertexAttribArray(U_DEFAULT_VERTEX_ATTR_POSITION_IDX);
  glVertexAttribPointer(U_DEFAULT_VERTEX_ATTR_POSITION_IDX, 3, GL_FLOAT, GL_FALSE, sizeof(UVertex), (void*)0);

  // Attrib 1: { r, g, b }
  glEnableVertexAttribArray(U_DEFAULT_VERTEX_ATTR_COLOR_IDX);
  glVertexAttribPointer(U_DEFAULT_VERTEX_ATTR_COLOR_IDX, 3, GL_FLOAT, GL_FALSE, sizeof(UVertex), (void*)(sizeof(UVec3)));

  U_BindVertexArray(0);

	/** Set up shader **/
	i32 vert_shader = U_LoadShader(U_CONFIG_SHADERS_DIR"/default.vert.glsl", U_VERTEX_SHADER);
	i32 frag_shader = U_LoadShader(U_CONFIG_SHADERS_DIR"/default.frag.glsl", U_FRAGMENT_SHADER);

	if(vert_shader < 0 || frag_shader < 0)
	{
		// TODO: Add error return value
		printf("%s\n", U_CONFIG_SHADERS_DIR);
	}

	i32 shader = U_CompileShaderProgram(vert_shader, frag_shader);

  renderer->shader_id = shader;
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

void U_DrawQuad(URenderer *renderer, const UVec3 position, const f32 size, const UVec3 color)
{
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

	for (ui32 i = 1; i < n; i++)
	{
		renderer->buffer[i + prev_vert_count].color = color;
	}

	UMat4x4 model_mat = U_Mat4x4(1.0f);
	glUniformMatrix4fv(
		glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_MODEL"),
		1, GL_TRUE,
		&model_mat.data[0][0]);
}

void U_DrawCube(const UVec3 position, const f32 size, const UVec3 color)
{
	URenderer* renderer = &UCORE.renderer;

	ui32 prev_vert_count = renderer->vertex_count;

	// front face
	renderer->buffer[1 + renderer->vertex_count].position = position;

 	renderer->buffer[2 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[2 + renderer->vertex_count].position.y = position.y + 0.0f;
  renderer->buffer[2 + renderer->vertex_count].position.z = position.z + 0.0f;

 	renderer->buffer[3 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[3 + renderer->vertex_count].position.y = position.y + size;
  renderer->buffer[3 + renderer->vertex_count].position.z = position.z + 0.0f;

	renderer->buffer[4 + renderer->vertex_count].position.x = position.x + 0.0f;
	renderer->buffer[4 + renderer->vertex_count].position.y = position.y + size;
	renderer->buffer[4 + renderer->vertex_count].position.z = position.z + 0.0f;

	f32 size_z = size / 2;

	//back face
	renderer->buffer[5 + renderer->vertex_count].position.x = position.x + 0.0f;
  renderer->buffer[5 + renderer->vertex_count].position.y = position.y + 0.0f;
  renderer->buffer[5 + renderer->vertex_count].position.z = position.z + size_z;

 	renderer->buffer[6 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[6 + renderer->vertex_count].position.y = position.y + 0.0f;
  renderer->buffer[6 + renderer->vertex_count].position.z = position.z + size_z;

	renderer->buffer[7 + renderer->vertex_count].position.x = position.x + size;
	renderer->buffer[7 + renderer->vertex_count].position.y = position.y + size;
	renderer->buffer[7 + renderer->vertex_count].position.z = position.z + size_z;

	renderer->buffer[8 + renderer->vertex_count].position.x = position.x + 0.0f;
	renderer->buffer[8 + renderer->vertex_count].position.y = position.y + size;
	renderer->buffer[8 + renderer->vertex_count].position.z = position.z + size_z;

	// Vertex Indexing

	// back face
  renderer->index_buffer[renderer->index_count] = renderer->vertex_count + 5;
  renderer->index_buffer[1 + renderer->index_count] = renderer->vertex_count + 6;
  renderer->index_buffer[2 + renderer->index_count] = renderer->vertex_count + 7;

  renderer->index_buffer[3 + renderer->index_count] = renderer->vertex_count + 5;
  renderer->index_buffer[4 + renderer->index_count] = renderer->vertex_count + 7;
  renderer->index_buffer[5 + renderer->index_count] = renderer->vertex_count + 8;

  // left face
  renderer->index_buffer[6 + renderer->index_count] = renderer->vertex_count + 2;
  renderer->index_buffer[7 + renderer->index_count] = renderer->vertex_count + 6;
  renderer->index_buffer[8 + renderer->index_count] = renderer->vertex_count + 7;

  renderer->index_buffer[9 + renderer->index_count] = renderer->vertex_count + 2;
  renderer->index_buffer[10 + renderer->index_count] = renderer->vertex_count + 7;
  renderer->index_buffer[11 + renderer->index_count] = renderer->vertex_count + 3;

  // right face
  renderer->index_buffer[12 + renderer->index_count] = renderer->vertex_count + 1;
  renderer->index_buffer[13 + renderer->index_count] = renderer->vertex_count + 5;
  renderer->index_buffer[14 + renderer->index_count] = renderer->vertex_count + 8;

  renderer->index_buffer[15 + renderer->index_count] = renderer->vertex_count + 1;
  renderer->index_buffer[16 + renderer->index_count] = renderer->vertex_count + 8;
  renderer->index_buffer[17 + renderer->index_count] = renderer->vertex_count + 4;

  // top face
  renderer->index_buffer[18 + renderer->index_count] = renderer->vertex_count + 4;
  renderer->index_buffer[19 + renderer->index_count] = renderer->vertex_count + 3;
  renderer->index_buffer[20 + renderer->index_count] = renderer->vertex_count + 7;

  renderer->index_buffer[21 + renderer->index_count] = renderer->vertex_count + 4;
  renderer->index_buffer[22 + renderer->index_count] = renderer->vertex_count + 7;
  renderer->index_buffer[23 + renderer->index_count] = renderer->vertex_count + 8;

  // bottom face
  renderer->index_buffer[24 + renderer->index_count] = renderer->vertex_count + 1;
  renderer->index_buffer[25 + renderer->index_count] = renderer->vertex_count + 2;
  renderer->index_buffer[26 + renderer->index_count] = renderer->vertex_count + 6;

  renderer->index_buffer[27 + renderer->index_count] = renderer->vertex_count + 1;
  renderer->index_buffer[28 + renderer->index_count] = renderer->vertex_count + 6;
  renderer->index_buffer[29 + renderer->index_count] = renderer->vertex_count + 5;

 	// front face
  renderer->index_buffer[30 + renderer->index_count] = renderer->vertex_count + 1;
  renderer->index_buffer[31 + renderer->index_count] = renderer->vertex_count + 2;
  renderer->index_buffer[32 + renderer->index_count] = renderer->vertex_count + 3;

  renderer->index_buffer[33 + renderer->index_count] = renderer->vertex_count + 1;
  renderer->index_buffer[34 + renderer->index_count] = renderer->vertex_count + 3;
  renderer->index_buffer[35 + renderer->index_count] = renderer->vertex_count + 4;

  renderer->vertex_count += 8;
  renderer->index_count += 36;

 	/** Set vertices color **/
	ui32 n = (renderer->vertex_count - prev_vert_count);

	for (ui32 i = 1; i <= n; i++)
	{
		renderer->buffer[i + prev_vert_count].color = color;
	}

	UMat4x4 model_mat = U_Mat4x4(1.0f);

	glUniformMatrix4fv(
		glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_MODEL"),
		1, GL_TRUE,
		&model_mat.data[0][0]);
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