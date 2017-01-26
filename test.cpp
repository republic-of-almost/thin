
#include <SDL2/SDL.h>

#define THIN_DEVICE_IMPL
#include <thin/ogl_device.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

Device gl;

auto t_start = std::chrono::high_resolution_clock::now();

// Shader sources
const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec3 position;"
    "in vec3 color;"
    "in vec2 texcoord;"
    "out vec3 Color;"
    "out vec2 Texcoord;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 proj;"
    "uniform vec3 overrideColor;"
    "void main()"
    "{"
    "    Color = overrideColor * color;"
    "    Texcoord = texcoord;"
    "    gl_Position = proj * view * model * vec4(position, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 150 core\n"
    "in vec3 Color;"
    "in vec2 Texcoord;"
    "out vec4 outColor;"
    "uniform sampler2D texKitten;"
    "uniform sampler2D texPuppy;"
    "void main()"
    "{"
    "    outColor = vec4(Color, 1.0) * mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), 0.5);"
    "}";

int
main()
{
  // SDL Window
  SDL_Init(SDL_INIT_EVERYTHING);

  auto sdl_window = SDL_CreateWindow("GL",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                800,
                480,
                SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);

  // GL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 2);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  auto gl_context = SDL_GL_CreateContext(sdl_window);

  SDL_GL_MakeCurrent(sdl_window, gl_context);

  gl.initialize();

  gl.errorCallback([](const char *str){
    printf("%s\n", str);
  });

  gl.enable(GL_DEPTH_TEST);

  // Create Vertex Array Object

  const uintptr_t vao = gl.genVertexArray();
  gl.bindVertexArray(vao);

  // Create a Vertex Buffer Object and copy the vertex data to it
  const uintptr_t vbo = gl.genBuffer();

  GLfloat vertices[] = {
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

       0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
       0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
       0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
       0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
       0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
       0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
       0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

      -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
       1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
       1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
       1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
      -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
  };

  gl.bindBuffer(GL_ARRAY_BUFFER, vbo);
  gl.bufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create and compile the vertex shader
  const uintptr_t shader_program = gl.createProgram(vertexSource, nullptr, fragmentSource);
  gl.useProgram(shader_program);

  GLuint shaderProgram = (GLuint)shader_program;

  // Specify the layout of the vertex data
  const intptr_t pos_index = gl.getAttribLocation(shader_program, "position");
  gl.enableVertexAttribArrayPointer(pos_index, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

  const intptr_t col_index = gl.getAttribLocation(shader_program, "color");
  gl.enableVertexAttribArrayPointer(col_index, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  const intptr_t tex_index = gl.getAttribLocation(shader_program, "texcoord");
  gl.enableVertexAttribArrayPointer(tex_index, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

  // Load textures
  uintptr_t textures[2];
  gl.genTextures(2, textures);

  unsigned char* image;

  gl.bindActiveTexture(GL_TEXTURE0, GL_TEXTURE_2D, textures[0]);

  int x,y,n;
  auto data = stbi_load("/Users/phil0005/Desktop/slim/img.png", &x, &y, &n, 0);

  gl.uniform1i(gl.getUniformLocation(shader_program, "texKitten"), 0);
  // glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  gl.bindActiveTexture(GL_TEXTURE1, GL_TEXTURE_2D, textures[1]);

  // glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);
  gl.uniform1i(gl.getUniformLocation(shader_program, "texPuppy"), 0);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLint uniModel = (GLint)gl.getUniformLocation(shader_program, "model");
  GLint uniProj = (GLint)gl.getUniformLocation(shaderProgram, "proj");
  GLint uniView = (GLint)gl.getUniformLocation(shader_program, "view");
  GLint uniColor = (GLint)gl.getUniformLocation(shader_program, "overrideColor");

  // Set up projection
  glm::mat4 view = glm::lookAt(
      glm::vec3(2.5f, 2.5f, 2.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f)
  );

  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

  glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);

  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

  // Game loop
  bool is_running = true;
  while(is_running)
  {
    // Process Events
    SDL_Event sdl_event;

    while (SDL_PollEvent(&sdl_event))
    {
      if (sdl_event.type == SDL_QUIT)
      {
        is_running = false;
      }
    }

    // *** Do Your Stuff.

    // Clear the screen to black
    gl.clearColor(1,1,1,1);
    gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate transformation
    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
    time *= 0.125f;

    glm::mat4 model;
    model = glm::rotate(
       model,
       time * glm::radians(180.0f),
       glm::vec3(0.0f, 0.0f, 1.0f)
    );
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

    // Draw cube
    gl.drawArrays(GL_TRIANGLES, 0, 36);

    gl.enable(GL_STENCIL_TEST);

    // Draw floor
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);

    gl.clear(GL_STENCIL_BUFFER_BIT);

    gl.drawArrays(GL_TRIANGLES, 36, 6);

    // Draw cube reflection
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDepthMask(GL_TRUE);

    model = glm::scale(
       glm::translate(model, glm::vec3(0, 0, -1)),
       glm::vec3(1, 1, -1)
    );
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);
    gl.drawArrays(GL_TRIANGLES, 0, 36);
    glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

    gl.disable(GL_STENCIL_TEST);

    SDL_GL_SwapWindow(sdl_window);
  }

  gl.deleteTextures(2, textures);
  gl.deleteProgram(shaderProgram);
  gl.deleteBuffer(vbo);
  gl.deleteVertexArray(vao);

  return 0;
}
