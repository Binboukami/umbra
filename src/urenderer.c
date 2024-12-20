#include <stdio.h>
#include <string.h>

#include "umbra.h"
#include "urenderer.h"

#include "stb_image.h"

#define U_CONFIG_SHADERS_DIR U_SHADERS_DIR

void U_InitRenderer(URenderer* renderer, ui8 use_ebo)
{
  renderer->vertex_count = 0;
  renderer->index_count = 0;
  renderer->shader_id = 0;
	renderer->samples_count = 0;

  renderer->ebo = 0;
  renderer->use_ebo = use_ebo;

  // Initialize vbo data
  glGenBuffers(1, &renderer->vbo);
  U_BindVBO(renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->buffer), &renderer->buffer, GL_DYNAMIC_DRAW);

  U_GenerateVAO(&renderer->vao);
  U_BindVertexArray(renderer->vao);

	if(use_ebo)
	{
		glGenBuffers(1, &renderer->ebo);
    U_BindEBO(renderer->ebo);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderer->index_buffer), &renderer->index_buffer, GL_DYNAMIC_DRAW);

  // Attrib 0: { x, y, z }
  U_SetVertexAttribute(U_DEFAULT_VERTEX_ATTR_POSITION_IDX, 3, GL_FLOAT, GL_FALSE, sizeof(UVertex), 0);

  // Attrib 1: { r, g, b }
  U_SetVertexAttribute(U_DEFAULT_VERTEX_ATTR_COLOR_IDX, 3, GL_FLOAT, GL_FALSE, sizeof(UVertex), sizeof(UVec3));

	// Attrib 2: { s, t }
	U_SetVertexAttribute(U_DEFAULT_VERTEX_ATTR_TEXTURE_COORD_IDX, 2, GL_FLOAT, GL_FALSE, sizeof(UVertex), (sizeof(UVec3) * 2));

	// Attrib 3: { idx }
	U_SetVertexAttribute(U_DEFAULT_VERTEX_ATTR_TEXTURE_IDX, 1, GL_INT, GL_FALSE, sizeof(UVertex), (sizeof(UVec3) * 2) + (sizeof(UVec2)));

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

TextureID LoadTexture(const char* path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0); 

	if(data)
	{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		fprintf(stdout, "erro ao carregar texture\n");
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return texture;
}

void U_BeginDrawing(UCamera camera)
{
	const UCoreContextRef u_instance = U_GetInstance();

	UWindow* window = &u_instance->window;
	URenderer* renderer = &u_instance->renderer;

	renderer->vertex_count = 0;
	renderer->index_count = 0;

	memset(renderer->samples, 0, sizeof(i64) * renderer->samples_count);
	renderer->samples_count = 0;

	U_BindVBO(renderer->vbo);
  U_BindVertexArray(renderer->vao);

	U_UseShader(renderer, renderer->shader_id);

  UMat4x4 projection;

	f32 clip_scaling = ORTHO_CLIPPING_SPACE;

  if(camera.projection == U_ORTHO)
  {
		glDisable(GL_DEPTH_TEST);
  	projection = U_MatOrtho(0, window->width, window->height, 0, -clip_scaling, clip_scaling);
  }
  else
  {
		glEnable(GL_DEPTH_TEST);
  	projection = U_MatPerspective((window->width / window->height), camera.fov, -clip_scaling, clip_scaling);
  }

	UMat4x4 view_mat = U_Mat4x4(1.0f);

	// Apply offset to center on view matrix
	UVec3 view_translate = { -camera.position.x, -camera.position.y, -camera.position.z }; // Camera position
	view_mat = U_Translate(view_mat, view_translate);

	glUniformMatrix4fv(
		glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_VIEW"),
		1, GL_TRUE,
		&view_mat.data[0][0]);

  glUniformMatrix4fv(
      glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_PROJECTION"),
      1, GL_TRUE,
      &projection.data[0][0]);

	UMat4x4 model_mat = U_Mat4x4(1.0f);
	glUniformMatrix4fv(
		glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_MODEL"),
		1, GL_TRUE,
		&model_mat.data[0][0]);
}

void U_EndDrawing()
{
	const UCoreContextRef u_instance = U_GetInstance();

	URenderer* renderer = &u_instance->renderer;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(renderer->samples_count > 0)
	{
		glUniform1iv(
			glGetUniformLocation(renderer->shader_id, "u_TextureArray"), renderer->samples_count,
			renderer->samples);

		for (int i = 0; i < renderer->samples_count; i++)
		{
			glBindTextureUnit(i, renderer->samples[i]);
		}
	}

  if(renderer->vertex_count > 0)
  {
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

void U_DrawTris(URenderer* renderer, const UVec3 position, const f32 size, const UVec3 color)
{
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
}

void U_DrawQuad(URenderer *renderer, const UVec3 position, const f32 size, const UVec3 color)
{
	ui32 prev_vert_count = renderer->vertex_count;

  renderer->buffer[renderer->vertex_count].position = position;
	renderer->buffer[renderer->vertex_count].text_coord.x = 0.0f;
	renderer->buffer[renderer->vertex_count].text_coord.y = 0.0f;

  renderer->buffer[1 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[1 + renderer->vertex_count].position.y = position.y + 0.0f;
  renderer->buffer[1 + renderer->vertex_count].position.z = position.z + 0.0f;
	renderer->buffer[1 + renderer->vertex_count].text_coord.x = 1.0f;
	renderer->buffer[1 + renderer->vertex_count].text_coord.y = 0.0f;

  renderer->buffer[2 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[2 + renderer->vertex_count].position.y = position.y + size;
  renderer->buffer[2 + renderer->vertex_count].position.z = position.z + 0.0f;
	renderer->buffer[2 + renderer->vertex_count].text_coord.x = 1.0f;
	renderer->buffer[2 + renderer->vertex_count].text_coord.y = 1.0f;

	if(renderer->use_ebo)
	{
		// Define 4th vertex
		renderer->buffer[3 + renderer->vertex_count].position.x = position.x + 0.0f;
		renderer->buffer[3 + renderer->vertex_count].position.y = position.y + size;
		renderer->buffer[3 + renderer->vertex_count].position.z = position.z + 0.0f;
		renderer->buffer[3 + renderer->vertex_count].text_coord.x = 0.0f;
		renderer->buffer[3 + renderer->vertex_count].text_coord.y = 1.0f;


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
		renderer->buffer[i + prev_vert_count].texture_id = -1;
	}
}


void 
U_DrawTextureQuad(const UVec3 position, const f32 size, TextureID texture)
{
	const UCoreContextRef u_instance = U_GetInstance();

	URenderer* renderer = &u_instance->renderer;

	ui32 prev_vert_count = renderer->vertex_count;

  renderer->buffer[renderer->vertex_count].position = position;
	renderer->buffer[renderer->vertex_count].text_coord.x = 0.0f;
	renderer->buffer[renderer->vertex_count].text_coord.y = 0.0f;

  renderer->buffer[1 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[1 + renderer->vertex_count].position.y = position.y + 0.0f;
  renderer->buffer[1 + renderer->vertex_count].position.z = position.z + 0.0f;
	renderer->buffer[1 + renderer->vertex_count].text_coord.x = 1.0f;
	renderer->buffer[1 + renderer->vertex_count].text_coord.y = 0.0f;

  renderer->buffer[2 + renderer->vertex_count].position.x = position.x + size;
  renderer->buffer[2 + renderer->vertex_count].position.y = position.y + size;
  renderer->buffer[2 + renderer->vertex_count].position.z = position.z + 0.0f;
	renderer->buffer[2 + renderer->vertex_count].text_coord.x = 1.0f;
	renderer->buffer[2 + renderer->vertex_count].text_coord.y = 1.0f;

	if(renderer->use_ebo)
	{
		// Define 4th vertex
		renderer->buffer[3 + renderer->vertex_count].position.x = position.x + 0.0f;
		renderer->buffer[3 + renderer->vertex_count].position.y = position.y + size;
		renderer->buffer[3 + renderer->vertex_count].position.z = position.z + 0.0f;
		renderer->buffer[3 + renderer->vertex_count].text_coord.x = 0.0f;
		renderer->buffer[3 + renderer->vertex_count].text_coord.y = 1.0f;


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

	// TODO: Store information that this set of vertices should use a certain texture

	/** Set vertices texture **/
	ui32 n = (renderer->vertex_count - prev_vert_count);

	bool texbuff_overflows = (renderer->samples_count + 1) > 32;
	if(!texbuff_overflows)
	{
		renderer->samples[renderer->samples_count] = texture;
		renderer->samples_count++;
	}
	else
	{
		// TODO: Error out of use another render batch
		// TODO: Set logging/debug 
		fprintf(stdout, "Overflow: Texture Buffer: Maximum samples count reached for current texture buffer\n");
	}

	// Index into texture buffer where actual gl texture buffer is stored
	TextureID tex_idx = texbuff_overflows ? 0 : renderer->samples_count;
	for (ui32 i = 0; i < n; i++)
	{
		renderer->buffer[i + prev_vert_count].texture_id = tex_idx;
	}
}

void U_DrawRect(URenderer* renderer, const UVec3 position, const f32 width, const f32 height, const UVec3 color)
{
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
}

void U_DrawCube(URenderer* renderer, const UVec3 position, const f32 size, const UVec3 color)
{
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
}

void U_UseShader(URenderer* renderer, ui32 shader_id)
{
	renderer->shader_id = shader_id;
  glUseProgram(shader_id);
};