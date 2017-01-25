
#include <SDL2/SDL.h>

#define THIN_DEVICE_IMPL
#include <thin/ogl_device.hpp>

Device gl;

// Shader sources
const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec2 position;"
    "in vec3 color;"
    "out vec3 Color;"
    "void main()"
    "{"
    "    Color = color;"
    "    gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 150 core\n"
    "in vec3 Color;"
    "out vec4 outColor;"
    "void main()"
    "{"
    "    outColor = vec4(Color, 1.0);"
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

  gl.errorCallback([](const char *str){
    printf("%s\n", str);
  });

  uintptr_t vao = gl.createVertexArray();
  gl.bindVertexArray(vao);

  enum { VBO, EBO, };
  uintptr_t buffers[2];
  gl.genBuffers(2, buffers);

  // GLuint vbo;
  // glGenBuffers(1, &vbo);

  GLfloat vertices[] = {
      -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
       0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
      -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
  };

  gl.bindBuffer(GL_ARRAY_BUFFER, buffers[VBO]);
  gl.bufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create an element array

  GLuint elements[] = {
      0, 1, 2,
      2, 3, 0
  };

  gl.bindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[EBO]);
  gl.bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  // Create and compile the vertex shader
  // GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // glShaderSource(vertexShader, 1, &vertexSource, NULL);
  // glCompileShader(vertexShader);
  //
  // // Create and compile the fragment shader
  // GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  // glCompileShader(fragmentShader);
  //
  // // Link the vertex and fragment shader into a shader program
  // GLuint shaderProgram = glCreateProgram();
  // glAttachShader(shaderProgram, vertexShader);
  // glAttachShader(shaderProgram, fragmentShader);
  // glBindFragDataLocation(shaderProgram, 0, "outColor");
  // glLinkProgram(shaderProgram);
  // glUseProgram(shaderProgram);

  uintptr_t program = gl.createProgram(vertexSource, nullptr, fragmentSource);
  gl.useProgram(program);

  glBindFragDataLocation((GLuint)program, 0, "outColor");

  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation((GLuint)program, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

  GLint colAttrib = glGetAttribLocation((GLuint)program, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

  printf("Shader: %zu\n", program);

  gl.getError("Shader Setup");

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
    gl.clearColor(1,0,0);
    gl.clear(GL_COLOR_BUFFER_BIT);

    // Clear the screen to black
     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT);

     // Draw a rectangle from the 2 triangles using 6 indices
     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(sdl_window);
  }

  gl.deleteProgram(program);
  // glDeleteProgram(shaderProgram);
  //
  // glDeleteShader(fragmentShader);
  // glDeleteShader(vertexShader);

  gl.deleteBuffers(2, buffers);

  GLuint destroy_vao = (GLuint)vao;
  glDeleteVertexArrays(1, &destroy_vao);

  return 0;
}
