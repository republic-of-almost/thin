#ifndef THIN_OGL_DEVICE_INCLUDED_
#define THIN_OGL_DEVICE_INCLUDED_


#define THIN_EXTRA_PARAM_CHECKS
#define THIN_EXTRA_ERROR_CHECKS


#include <stdint.h>
#include <stddef.h>

#include <OpenGL/gl3.h>


struct Device
{

using error_callback_fn = void(*)(const char *);

error_callback_fn curr_error_callback = nullptr;

// ---------------------------------------------------------------[ General ]--

void
initialize();

void
errorCallback(const error_callback_fn err_cb);

void
getError(const char *msg);

// ------------------------------------------------------------------[ Misc ]--

void
enable(const GLenum cap);

void
enable(const size_t count, const GLenum to_enable[]);

void
disable(const GLenum cap);

void
disable(const size_t count, const GLenum to_disable[]);

// -------------------------------------------------------------------[ VAO ]--

uintptr_t
genVertexArray();

void
genVertexArrays(const size_t count, uintptr_t vaos[]);

uintptr_t
createVertexArray();

void
createVertexArrays(const size_t count, uintptr_t vaos[]);

void
bindVertexArray(const uintptr_t vao);

void
deleteVertexArray(const uintptr_t vao);

void
deleteVertexArrays(const size_t count, const uintptr_t vaos[]);

// -----------------------------------------------------------------[ Clear ]--

void
clearColor(const float r, const float g, const float b, const float a = 1.f);

void
clearColor(const float color[4]);

void
clear(const GLbitfield mask);

void
clear(const bool color, const bool depth, const bool stencil = false);

void
clearColorBuffer();

void
clearDepthBuffer();

void
clearStencilBuffer();

void
getClearColor(float out_color[4]);

// ---------------------------------------------------------------[ Texture ]--

uintptr_t
genTexture();

void
genTextures(const size_t count, uintptr_t textures[]);

uintptr_t
createTexture();

void
createTextures(const size_t count, uintptr_t textures[]);

void
bindTexture(const GLenum target, const uintptr_t texture);

void
bindActiveTexture(const GLuint texture_slot,
                  const GLenum target,
                  const uintptr_t texture);

void
bindTextures(const GLuint first_target,
             const size_t count,
             const uintptr_t textures[]);

void
deleteTexture(const uintptr_t texture);

void
deleteTextures(const size_t count, const uintptr_t textures[]);

// ---------------------------------------------------------------[ Shaders ]--

uintptr_t
createProgram(const char *vs, const char *gs, const char *fs);

void
useProgram(const uintptr_t program);

void
deleteProgram(const uintptr_t program);

void
bindFragDataLocation(const uintptr_t program,
                     const GLuint color_number,
                     const char *name);

// --------------------------------------------------------------[ Uniforms ]--

intptr_t
getUniformLocation(const uintptr_t shader, const char *name);

void
uniform1i(const intptr_t location, const GLint v0);

// ---------------------------------------------------------------[ Buffers ]--

uintptr_t
genBuffer();

void
genBuffers(const size_t count, uintptr_t buffers[]);

uintptr_t
createBuffer();

void
createBuffers(const size_t count, uintptr_t buffers[]);

void
bindBuffer(const GLenum target, const uintptr_t buffer);

void
bufferData(const GLenum target,
           const GLsizeiptr size,
           const GLvoid *data,
           const GLenum useage);

void
deleteBuffer(const uintptr_t buffer);

void
deleteBuffers(const size_t count, const uintptr_t buffers[]);

// --------------------------------------------------------[ Vertex Attribs ]--

intptr_t
getAttribLocation(const intptr_t shader, const char *name);

void
enableVertexAttribArray(const intptr_t attrib);

void
vertexAttribPointer(const intptr_t attr_index,
                    const GLint size,
                    const GLenum type,
                    const GLboolean norm,
                    const GLsizei stride,
                    const GLvoid *pointer);

void
enableVertexAttribArrayPointer(const intptr_t attr_index,
                               const GLint size,
                               const GLenum type,
                               const GLboolean norm,
                               const GLsizei stride,
                               const GLvoid *pointer);

// ---------------------------------------------------------------[ Drawing ]--

void
drawArrays(const GLenum mode, const GLint first, const GLsizei count);

void
drawElements(const GLenum mode,
             const GLsizei count,
             const GLenum type,
             const GLvoid *index);

// -----------------------------------------------[ Debug Markers Extension ]--

bool
getHasDebugMarkers();

void
insertEventMarker(const char *msg);

void
pushGroupMarker(const char *debug);

void
popGroupMarker();

};


#endif // inc guard


#ifdef THIN_DEVICE_IMPL

// ---------------------------------------------------------------[ General ]--

void
Device::initialize()
{

}

void
Device::errorCallback(const error_callback_fn err_cb)
{
  curr_error_callback = err_cb;
}

void
Device::getError(const char *msg)
{
  const GLenum err = glGetError();

  if(err && curr_error_callback)
  {
    // TODO: Prefix with error code.
    curr_error_callback(msg);
  }
}

// ------------------------------------------------------------------[ Misc ]--

void
Device::enable(const GLenum cap)
{
  glEnable(cap);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Enable");
  #endif
}

void
Device::enable(const size_t count, const GLenum to_enable[])
{
  for(size_t i = 0; i < count; ++i)
  {
    enable(to_enable[i]);
  }
}

void
Device::disable(const GLenum cap)
{
  glDisable(cap);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Enable");
  #endif
}

void
Device::disable(const size_t count, const GLenum to_disable[])
{
  for(size_t i = 0; i < count; ++i)
  {
    disable(to_disable[i]);
  }
}


// -------------------------------------------------------------------[ VAO ]--

uintptr_t
Device::genVertexArray()
{
  uintptr_t vao = 0;
  genVertexArrays(1, &vao);

  return vao;
}

void
Device::genVertexArrays(const size_t count, uintptr_t out_vaos[])
{
  GLuint *vaos = (GLuint*)malloc(count * sizeof(GLuint));

  glGenVertexArrays(count, vaos);

  // Convert back to uintptrs
  for(size_t i = 0; i < count; ++i)
  {
    out_vaos[i] = (uintptr_t)vaos[i];
  }

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Generating VAO");
  #endif

  free(vaos);
}

uintptr_t
Device::createVertexArray()
{
  // TODO: Need to check to see if glCreateVertexArrays exists.
  return genVertexArray();
}

void
Device::createVertexArrays(const size_t count, uintptr_t vaos[])
{
  // TODO: Need to check to see if glCreateVertexArrays exists.
  genVertexArrays(count, vaos);
}

void
Device::bindVertexArray(const uintptr_t vao)
{
  glBindVertexArray(vao);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Binding VAO");
  #endif
}

void
Device::deleteVertexArray(const uintptr_t vao)
{
  deleteVertexArrays(1, &vao);
}

void
Device::deleteVertexArrays(const size_t count, const uintptr_t vaos_to_destroy[])
{
  GLuint *vaos = (GLuint*)malloc(count * sizeof(GLuint));

  // Convert to GLuint
  for(size_t i = 0; i < count; ++i)
  {
    vaos[i] = (GLuint)vaos_to_destroy[i];
  }

  glDeleteVertexArrays(count, vaos);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Destroying VAO");
  #endif

  free(vaos);
}


// -----------------------------------------------------------------[ Clear ]--

void
Device::clearColor(const float r, const float g, const float b, const float a)
{
  glClearColor(r,g,b,a);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Clearing Color");
  #endif
}

void
Device::clearColor(const float color[4])
{
  enum {r = 0,g,b,a};

  clearColor(color[r], color[g], color[b], color[a]);
}

void
Device::clear(const GLbitfield mask)
{
  glClear(mask);
}

void
Device::clear(const bool color, const bool depth, const bool stencil)
{
  GLbitfield mask = 0;
  if(color)   { mask |= GL_COLOR_BUFFER_BIT;   }
  if(depth)   { mask |= GL_DEPTH_BUFFER_BIT;   }
  if(stencil) { mask |= GL_STENCIL_BUFFER_BIT; }

  clear(mask);
}

void
Device::clearColorBuffer()
{
  clear(GL_COLOR_BUFFER_BIT);
}

void
Device::clearDepthBuffer()
{
  clear(GL_DEPTH_BUFFER_BIT);
}

void
Device::clearStencilBuffer()
{
  clear(GL_STENCIL_BUFFER_BIT);
}

void
Device::getClearColor(float out_color[4])
{
}


// ---------------------------------------------------------------[ Texture ]--

uintptr_t
Device::genTexture()
{
  uintptr_t tex;
  genTextures(1, &tex);

  return tex;
}

void
Device::genTextures(const size_t count, uintptr_t out_textures[])
{
  GLuint *textures = (GLuint*)malloc(sizeof(GLuint) * count);
  glGenTextures(count, textures);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Gen Textures");
  #endif

  for(size_t i = 0; i < count; ++i)
  {
    out_textures[i] = (uintptr_t)textures[i];
  }

  free(textures);
}

uintptr_t
Device::createTexture()
{
  return genTexture();
}

void
Device::createTextures(const size_t count, uintptr_t textures[])
{
  return genTextures(count, textures);
}

void
Device::bindTexture(const GLenum target, const uintptr_t texture)
{
  glBindTexture(target, (GLuint)texture);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Bind Texture");
  #endif
}

void
Device::bindActiveTexture(const GLuint texture_slot,
                          const GLenum target,
                          const uintptr_t texture)
{
  glActiveTexture(texture_slot);
  glBindTexture(target, (GLuint)texture);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Bind Active Texture");
  #endif
}

void
Device::bindTextures(const GLuint first_target,
                     const size_t count,
                     const uintptr_t in_textures[])
{
  // GLuint *textures = (GLuint*)malloc(sizeof(GLuint) * count);
  //
  // for(size_t i = 0; i < count; ++i)
  // {
  //   textures[i] = (GLuint)in_textures[i];
  // }
  //
  // glBindTextures(first_target, count, textures);
  //
  // free(textures);
  //
  // #ifdef THIN_EXTRA_ERROR_CHECKS
  // getError("Bind Textures");
  // #endif
}

void
Device::deleteTexture(const uintptr_t texture)
{
  deleteTextures(1, &texture);
}

void
Device::deleteTextures(const size_t count, const uintptr_t in_textures[])
{
  GLuint *textures = (GLuint*)malloc(sizeof(GLuint) * count);

  for(size_t i = 0; i < count; ++i)
  {
    textures[i] = (GLuint)in_textures[i];
  }

  glDeleteTextures(count, textures);

  free(textures);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Delete Texture");
  #endif
}


// ---------------------------------------------------------------[ Shaders ]--

uintptr_t
Device::createProgram(const char *vs, const char *gs, const char *fs)
{
  GLuint vert_shd = 0;
  GLuint geo_shd = 0;
  GLuint frag_shd = 0;

  vert_shd = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shd, 1, &vs, NULL);
  glCompileShader(vert_shd);

  if(gs && strlen(gs) > 1)
  {
    geo_shd = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geo_shd, 1, &gs, NULL);
    glCompileShader(geo_shd);
  }

  frag_shd = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shd, 1, &fs, NULL);
  glCompileShader(frag_shd);

  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert_shd);

  if(geo_shd)
  {
    glAttachShader(prog, geo_shd);
  }

  glAttachShader(prog, frag_shd);
  glLinkProgram(prog);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Compiling Program");
  #endif

  return (uintptr_t)prog;
}

void
Device::useProgram(const uintptr_t program)
{
  glUseProgram((GLuint)program);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Use Program");
  #endif
}

void
Device::deleteProgram(const uintptr_t program)
{
  constexpr GLsizei max_out = 3;
  GLuint out_shaders[max_out];
  GLsizei out_count = 0;

  glGetAttachedShaders((GLuint)program, max_out, &out_count, out_shaders);

  for(GLsizei i = 0; i < out_count; ++i)
  {
    glDeleteShader(out_shaders[i]);
  }

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Deleting Shaders");
  #endif

  glDeleteProgram((GLuint)program);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Deleting Program");
  #endif
}


void
Device::bindFragDataLocation(const uintptr_t program,
                             const GLuint color_number,
                             const char *name)
{
  glBindFragDataLocation((GLuint)program, color_number, name);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Deleting Program");
  #endif
}

// --------------------------------------------------------------[ Uniforms ]--

intptr_t
Device::getUniformLocation(const uintptr_t shader, const char *name)
{
  const GLint loc = glGetUniformLocation((GLuint)shader, name);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Getting location");
  #endif

  return (intptr_t)loc;
}

void
Device::uniform1i(const intptr_t location, const GLint v0)
{
  glUniform1i((GLint)location, v0);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Setting location");
  #endif
}

// ---------------------------------------------------------------[ Buffers ]--

uintptr_t
Device::genBuffer()
{
  uintptr_t buffer = 0;
  genBuffers(1, &buffer);

  return buffer;
}

void
Device::genBuffers(const size_t count, uintptr_t out_buffers[])
{
  GLuint *buffers = (GLuint*)malloc(count * sizeof(GLuint));

  glGenBuffers(count, buffers);

  // Convert back to uintptrs
  for(size_t i = 0; i < count; ++i)
  {
    out_buffers[i] = (uintptr_t)buffers[i];
  }

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Generating Buffers");
  #endif

  free(buffers);
}

uintptr_t
Device::createBuffer()
{
  // TODO: Check gl version for this.
  return genBuffer();
}

void
Device::createBuffers(const size_t count, uintptr_t buffers[])
{
  // TODO: Check gl version for this.
  return genBuffers(count, buffers);
}

void
Device::bindBuffer(const GLenum target, const uintptr_t buffer)
{
  glBindBuffer(target, (GLuint)buffer);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Binding Buffer");
  #endif
}

void
Device::bufferData(const GLenum target,
                   const GLsizeiptr size,
                   const GLvoid *data,
                   const GLenum use)
{
  glBufferData(target, size, data, use);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Adding Buffer Data");
  #endif
}

void
Device::deleteBuffer(const uintptr_t buffer)
{
  deleteBuffers(1, &buffer);
}

void
Device::deleteBuffers(const size_t count, const uintptr_t del_buffers[])
{
  GLuint *buffers = (GLuint*)malloc(count * sizeof(GLuint));

  // Convert to GLuint
  for(size_t i = 0; i < count; ++i)
  {
    buffers[i] = (GLuint)del_buffers[i];
  }

  glDeleteBuffers(count, buffers);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Destroying Buffers");
  #endif

  free(buffers);
}

// --------------------------------------------------------[ Vertex Attribs ]--

intptr_t
Device::getAttribLocation(const intptr_t shader, const char *name)
{
  intptr_t index = (intptr_t)glGetAttribLocation((GLuint)shader, name);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Get Attrib Location");
  #endif

  return index;
}

void
Device::enableVertexAttribArray(const intptr_t index)
{
  glEnableVertexAttribArray((GLint)index);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Enable Vertex Attrib Array");
  #endif
}

void
Device::vertexAttribPointer(const intptr_t attr_index,
                            const GLint size,
                            const GLenum type,
                            const GLboolean norm,
                            const GLsizei stride,
                            const GLvoid *pointer)
{
  glVertexAttribPointer((GLint)attr_index, size, type, norm, stride, pointer);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Attrib Pointer");
  #endif
}

void
Device::enableVertexAttribArrayPointer(const intptr_t attr_index,
                                       const GLint size,
                                       const GLenum type,
                                       const GLboolean norm,
                                       const GLsizei stride,
                                       const GLvoid *pointer)
{
  enableVertexAttribArray(attr_index);
  vertexAttribPointer(attr_index, size, type, norm, stride, pointer);
}


// ---------------------------------------------------------------[ Drawing ]--

void
Device::drawArrays(const GLenum mode, const GLint first, const GLsizei count)
{
  glDrawArrays(mode, first, count);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Draw Arrays");
  #endif
}

void
Device::drawElements(const GLenum mode,
                     const GLsizei count,
                     const GLenum type,
                     const GLvoid *index)
{
  glDrawElements(mode, count, type, index);

  #ifdef THIN_EXTRA_ERROR_CHECKS
  getError("Draw Elements");
  #endif
}


#endif // impl guard
