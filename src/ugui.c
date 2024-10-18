#include "ugui.h"
#include "glad/glad.h"
#include "umath.h"
#include "umbra.h"
#include "umbragl.h"

UGuiContext UGUI = {0};

void InitGUI()
{
  U_InitRenderer(&UGUI.renderer, true);
}

void DoButton(const UVec3 position, const f32 size, const UVec3 color)
{
  U_DrawQuad(&UGUI.renderer, position, size, color);
}

void BeginGUI(ui32 width, ui32 height)
{
  URenderer* renderer = &UGUI.renderer;

  renderer->vertex_count = 0;
	renderer->index_count = 0;

	U_BindVBO(renderer->vbo);
  U_BindVertexArray(renderer->vao);
	U_UseShader(renderer, renderer->shader_id);

  UMat4x4 projection = U_MatOrtho(0, width, height, 0, -ORTHO_CLIPPING_SPACE, ORTHO_CLIPPING_SPACE);

	// Apply perspective divide, maps 0 to top left corner of the current viewport
	// TODO: Test for viewports that do not start at (0,0)
	UVec3 origin = {
		((0 + width)/(width - 0)) * -1.0f,												// x
		((0 + height) / (height - 0)),														// y
		-(ORTHO_CLIPPING_SPACE + (-ORTHO_CLIPPING_SPACE) ) / 2		// z
		};

	projection = U_PerspectiveDivide(projection, origin);

  U_DisableDepthTest();

	UMat4x4 view_mat = U_Mat4x4(1.0f);

	// Apply offset to center on view matrix
	UVec3 view_translate = { 0.0f, 0.0f, 0.0f }; // Camera position
	view_mat = U_Translate(view_mat, view_translate);

	glUniformMatrix4fv(
		glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_VIEW"),
		1, GL_TRUE,
		&view_mat.data[0][0]);

  glUniformMatrix4fv(
      glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_PROJECTION"),
      1, GL_TRUE,
      &projection.data[0][0]);
}

void EndGUI()
{
  URenderer* renderer = &UGUI.renderer;

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
  }

  U_BindVertexArray(0);
}