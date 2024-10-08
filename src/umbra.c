#include "umbra.h"

UCoreContext UCORE = {0};

void U_BeginDrawing(UCamera camera)
{
	UWindow* window = &UCORE.window;
	URenderer* renderer = &UCORE.renderer;

	renderer->vertex_count = 0;
  renderer->index_count = 0;

	U_BindVBO(renderer->vbo);
  U_BindVertexArray(renderer->vao);

	U_UseShader(renderer, renderer->shader_id);

  UMat4x4 projection;

  if(camera.projection == U_ORTHO)
  {
  	projection = U_MatOrtho(0, window->width, window->height, 0, 1, -1);
  }
  else
  {
  	projection = U_MatPerspective((window->width / window->height), camera.fov, camera.position.z, 1);
  }

  glUniformMatrix4fv(
      glGetUniformLocation(renderer->shader_id, "UNIFORM_MATRIX_PROJECTION"),
      1, GL_TRUE,
      &projection.data[0][0]);
}

void U_EndDrawing()
{
	URenderer* renderer = &UCORE.renderer;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glClearDepth(0.0f);

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