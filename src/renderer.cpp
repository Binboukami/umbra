#include "renderer.hpp"

#include "gl_include.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

using namespace UECore;

void
Renderer::BindTexture2D(TextureID id)
{
  glBindTexture(GL_TEXTURE_2D, id);
}

void
Renderer::EnableBlending()
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
Renderer::DisableBlending()
{
  glDisable(GL_BLEND);
}

void
Renderer::ClearColor(f32 red, f32 green, f32 blue, f32 alpha)
{
  glClearColor(red, green, blue, alpha);
  glClear(GL_COLOR_BUFFER_BIT);
}

void
Renderer::UseShader(ui32 shader_id)
{
  glUseProgram(shader_id);
};

unsigned int
Renderer::LoadDefaultShaders(void)
{
  int vertex_shader = LoadShader(
    "core/shaders/default.vert.glsl",
    UShaderType::Vertex);
  int fragment_shader = LoadShader(
    "core/shaders/default.frag.glsl",
    UShaderType::Fragment);

  // other shaders

  // if shader > 0 // ok
  // else error:
  ui32
  shader_program = this->CompileShaderProgram(vertex_shader, fragment_shader);

  return shader_program;
}

/** This function consumes the shader src, making them unusable after call */
ui32
Renderer::CompileShaderProgram(i32 vertex_shader, i32 fragment_shader)
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

  this->SHADER_DEFAULT = shader;

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return shader;
}

int
Renderer::LoadShader(const char* filepath, UShaderType shader_type)
{
  FILE *file_ptr;

  /** Load shader source from file */

  file_ptr = fopen(filepath, "r");

  if(file_ptr == NULL)
  {
    fprintf(stderr, "Could not open shader file: '%s'", filepath);
    return -1;
  }

  long buff_size = SizeOfFile(file_ptr);

  if(buff_size == -1)
  {
    fprintf(stderr, "Could not read file size from '%s'", filepath);
    return -2;
  }

  /** Intialize memory to prevent erros when writing to the buffer */
  char *shad_src = memset((char*)malloc(sizeof(char) * (buff_size + 1)), '\0', buff_size);

  if(shad_src == NULL)
  {
    /** Error intializing memory */
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

  unsigned int gl_shader;
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
    fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s", infoLog);

    return -4;
  }

  return gl_shader;
}

void
Renderer::InitQuad()
{
  glGenBuffers(1, &this->VBO_QUAD);
  glGenBuffers(1, &this->EBO_QUAD);

  glGenVertexArrays(1, &this->VAO_QUAD);

  glBindVertexArray(this->VAO_QUAD);

  float vertices[] = {
    /** coords xyz */   /** color data */   /** texture coords*/
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f
  };

  unsigned int indexes[] = {
    1, 3, 0,
    1, 2, 3
  };

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO_QUAD);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO_QUAD);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

  float size_of_vertex = 8 * sizeof(float);

  // Attrib 0: { x, y, z }
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size_of_vertex, (void*)0);
  glEnableVertexAttribArray(0);

  // Attrib 1: { r, g, b }
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size_of_vertex, (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Attrib 2: { s, t }
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size_of_vertex, (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void
Renderer::DrawQuad(float x, float y, float width, float height, Color color)
{
  unsigned int current_shader = this->GetCurrentShader();

  glUseProgram(current_shader);

  glm::mat4x4 matrix_model = glm::mat4(1.0f);

  matrix_model = glm::translate(
      matrix_model, glm::vec3(x, y, 1.0f));
  matrix_model = glm::scale(
      matrix_model, glm::vec3(width, height, 1.0f));

  glUniformMatrix4fv(
    glGetUniformLocation(current_shader, "UNIFORM_MATRIX_MODEL"),
    1,
    GL_FALSE,
    glm::value_ptr(matrix_model)
  );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->MULTIPLY_TEXTURE_ID);

  glUniform4f(
    glGetUniformLocation(current_shader, "meshColor"),
    color.r, color.g, color.b, 1.0f
  );

  glBindVertexArray(this->VAO_QUAD);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void
Renderer::DrawQuadTextured(
  float x, 
  float y, 
  float width, 
  float height,
  unsigned int texture)
{
  unsigned int current_shader = this->GetCurrentShader();

  glUseProgram(current_shader);

  glm::mat4x4 matrix_model = glm::mat4(1.0f);

  matrix_model = glm::translate(
      matrix_model, glm::vec3(x, y, 1.0f));
  matrix_model = glm::scale(
      matrix_model, glm::vec3(width, height, 1.0f));

  glUniformMatrix4fv(
    glGetUniformLocation(current_shader, "UNIFORM_MATRIX_MODEL"),
    1,
    GL_FALSE,
    glm::value_ptr(matrix_model)
  );

  glUniform4f(
    glGetUniformLocation(current_shader, "meshColor"),
    1.0f, 1.0f, 1.0f, 1.0f
  );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glBindVertexArray(this->VAO_QUAD);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

/**
 * TODO: Add support for RGBA (png) images. Currently seg faults
*/
int
Renderer::LoadTexture(const char* filepath)
{
  /** Fixes upside down image due to how opengl expects textures coordinates */
  stbi_set_flip_vertically_on_load(true);

  int width, height, nr_channels;
  unsigned char *data = stbi_load(
    filepath,
    &width, &height,
    &nr_channels, 0);

  if(!data)
  {
    std::cerr << "Failed to load texture from file: '" << filepath << "'." << std::endl;
    stbi_image_free(data);

    return -1;
  }

  /** Load in the first samples GL_TEXTURE0 */
  glUniform1i(glGetUniformLocation(this->GetCurrentShader(), "meshTexture"), 0);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    width, height,
    0,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}

Texture
Renderer::GenerateTexture()
{
  ui32 tex_id;
  glGenTextures(1, &tex_id);
  Texture texture = { tex_id };

  return texture;
}

void
Renderer::SetTextureImage2D(const void* image, ui32 width, ui32 height)
{
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RED,
    width, height,
    0,
    GL_RED,
    GL_UNSIGNED_BYTE,
    image
  );
}

void 
Renderer::SetViewport(f32 left, f32 right, f32 bottom, f32 top, f32 clip_near, f32 clip_far)
{
  glViewport(left, top, right, bottom);

  glm::mat4x4 projection = glm::ortho(
      left, right,
      bottom, top,
      clip_near, clip_far);

  glm::mat4 view = glm::lookAt(
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f)
  );

  glUseProgram(this->GetCurrentShader());

  glUniformMatrix4fv(
      glGetUniformLocation(this->SHADER_DEFAULT, "UNIFORM_MATRIX_PROJECTION"),
      1,
      GL_FALSE,
      glm::value_ptr(projection));

  glUniformMatrix4fv(
      glGetUniformLocation(this->SHADER_DEFAULT, "UNIFORM_MATRIX_VIEW"),
      1,
      GL_FALSE,
      glm::value_ptr(view));
};