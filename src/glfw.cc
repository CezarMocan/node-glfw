#include "common.h"
#include <cstdio>
#include <cstdlib>

using namespace v8;
using namespace node;

#include <iostream>
using namespace std;

namespace glfw {

/* GLFW initialization, termination and version querying */

JS_METHOD(Init) {
  HandleScope scope;
  return scope.Close(JS_BOOL(glfwInit()==1));
}

JS_METHOD(Terminate) {
  HandleScope scope;
  glfwTerminate();
  return scope.Close(Undefined());
}

JS_METHOD(GetVersion) {
  HandleScope scope;
  int major, minor, rev;
  glfwGetVersion(&major,&minor,&rev);
  Local<Array> arr=Array::New(3);
  arr->Set(JS_STR("major"),JS_INT(major));
  arr->Set(JS_STR("minor"),JS_INT(minor));
  arr->Set(JS_STR("rev"),JS_INT(rev));
  return scope.Close(arr);
}

/* Window handling */
Persistent<Object> glfw_events;
int lastX=0,lastY=0;
bool windowCreated=false;

JS_METHOD(OpenWindow) {
  HandleScope scope;
  int width       = args[0]->Uint32Value();
  int height      = args[1]->Uint32Value();
  int redbits     = args[2]->Uint32Value();
  int greenbits   = args[3]->Uint32Value();
  int bluebits    = args[4]->Uint32Value();
  int alphabits   = args[5]->Uint32Value();
  int depthbits   = args[6]->Uint32Value();
  int stencilbits = args[7]->Uint32Value();
  int mode        = args[8]->Uint32Value();

  if(!windowCreated) {
    windowCreated=glfwOpenWindow(width,height,redbits,greenbits,bluebits,alphabits,depthbits,stencilbits,mode)!=0;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
  }
  else
    glfwSetWindowSize(width,height);

  return scope.Close(JS_BOOL(windowCreated));
}

JS_METHOD(OpenWindowHint) {
  HandleScope scope;
  glfwOpenWindowHint(args[0]->Int32Value(),args[1]->Int32Value());
  return scope.Close(Undefined());
}

JS_METHOD(CloseWindow) {
  HandleScope scope;
  glfwCloseWindow();
  return scope.Close(Undefined());
}

JS_METHOD(SetWindowTitle) {
  HandleScope scope;
  String::Utf8Value str(args[0]->ToString());
  glfwSetWindowTitle(*str);
  return scope.Close(Undefined());
}

JS_METHOD(GetWindowSize) {
  HandleScope scope;
  int w,h;
  glfwGetWindowSize(&w, &h);
  Local<Array> arr=Array::New(2);
  arr->Set(JS_STR("width"),JS_INT(w));
  arr->Set(JS_STR("height"),JS_INT(h));
  return scope.Close(arr);
}

JS_METHOD(SetWindowSize) {
  HandleScope scope;
  glfwSetWindowSize(args[0]->Uint32Value(),args[1]->Uint32Value());
  return scope.Close(Undefined());
}

JS_METHOD(SetWindowPos) {
  HandleScope scope;
  glfwSetWindowPos(args[0]->Uint32Value(),args[1]->Uint32Value());
  return scope.Close(Undefined());
}

JS_METHOD(IconifyWindow) {
  HandleScope scope;
  glfwIconifyWindow();
  return scope.Close(Undefined());
}

JS_METHOD(RestoreWindow) {
  HandleScope scope;
  glfwRestoreWindow();
  return scope.Close(Undefined());
}

JS_METHOD(SwapBuffers) {
  HandleScope scope;
  glfwSwapBuffers();
  return scope.Close(Undefined());
}

JS_METHOD(SwapInterval) {
  HandleScope scope;
  glfwSwapInterval(args[0]->Int32Value());
  return scope.Close(Undefined());
}

JS_METHOD(GetWindowParam) {
  HandleScope scope;
  return scope.Close(JS_INT(glfwGetWindowParam(args[0]->Int32Value())));
}

/* Input handling */
JS_METHOD(PollEvents) {
  HandleScope scope;
  glfwPollEvents();
  return scope.Close(Undefined());
}

JS_METHOD(WaitEvents) {
  HandleScope scope;
  glfwWaitEvents();
  return scope.Close(Undefined());
}

JS_METHOD(GetKey) {
  HandleScope scope;
  return scope.Close(JS_INT(glfwGetKey(args[0]->Int32Value())));
}

JS_METHOD(GetMouseButton) {
  HandleScope scope;
  return scope.Close(JS_INT(glfwGetMouseButton(args[0]->Int32Value())));
}

JS_METHOD(GetMousePos) {
  HandleScope scope;
  int x,y;
  glfwGetMousePos(&x, &y);
  Local<Array> arr=Array::New(2);
  arr->Set(JS_STR("x"),JS_INT(x));
  arr->Set(JS_STR("y"),JS_INT(y));
  return scope.Close(arr);
}

JS_METHOD(SetMousePos) {
  HandleScope scope;
  glfwSetMousePos(args[0]->Int32Value(),args[1]->Int32Value());
  return scope.Close(Undefined());
}

JS_METHOD(GetMouseWheel) {
  HandleScope scope;
  return scope.Close(JS_INT(glfwGetMouseWheel()));
}

JS_METHOD(SetMouseWheel) {
  HandleScope scope;
  glfwSetMouseWheel(args[0]->Int32Value());
  return scope.Close(Undefined());
}

/* Time */
JS_METHOD(GetTime) {
  HandleScope scope;
  return scope.Close(JS_NUM(glfwGetTime()));
}

JS_METHOD(SetTime) {
  HandleScope scope;
  glfwSetTime(args[0]->NumberValue());
  return scope.Close(Undefined());
}

JS_METHOD(Sleep) {
  HandleScope scope;
  glfwSleep(args[0]->NumberValue());
  return scope.Close(Undefined());
}

/* Extension support */
JS_METHOD(ExtensionSupported) {
  HandleScope scope;
  String::AsciiValue str(args[0]->ToString());
  return scope.Close(JS_BOOL(glfwExtensionSupported(*str)==1));
}

//GLFWAPI void* GLFWAPIENTRY glfwGetProcAddress( const char *procname );
JS_METHOD(GetGLVersion) {
  HandleScope scope;
  int major, minor, rev;
  glfwGetGLVersion(&major, &minor, &rev);
  Local<Array> arr=Array::New(3);
  arr->Set(JS_STR("major"),JS_INT(major));
  arr->Set(JS_STR("minor"),JS_INT(minor));
  arr->Set(JS_STR("rev"),JS_INT(rev));
  return scope.Close(arr);
}

/* Enable/disable functions */
JS_METHOD(Enable) {
  HandleScope scope;
  glfwEnable(args[0]->Int32Value());
  return scope.Close(Undefined());
}

JS_METHOD(Disable) {
  HandleScope scope;
  glfwDisable(args[0]->Int32Value());
  return scope.Close(Undefined());
}

// make sure we close everything when we exit
void AtExit() {
  glfwTerminate();
}

} // namespace glfw

///////////////////////////////////////////////////////////////////////////////
//
// bindings
//
///////////////////////////////////////////////////////////////////////////////
#define JS_GLFW_CONSTANT(name) target->Set(JS_STR( #name ), JS_INT(GLFW_ ## name))
#define JS_GLFW_SET_METHOD(name) NODE_SET_METHOD(target, #name , glfw::name);

extern "C" {
void init(Handle<Object> target) {
  atexit(glfw::AtExit);

  HandleScope scope;

  /* GLFW initialization, termination and version querying */
  JS_GLFW_SET_METHOD(Init);
  JS_GLFW_SET_METHOD(Terminate);
  JS_GLFW_SET_METHOD(GetVersion);

  /* Window handling */
  JS_GLFW_SET_METHOD(OpenWindow);
  JS_GLFW_SET_METHOD(OpenWindowHint);
  JS_GLFW_SET_METHOD(CloseWindow);
  JS_GLFW_SET_METHOD(SetWindowTitle);
  JS_GLFW_SET_METHOD(GetWindowSize);
  JS_GLFW_SET_METHOD(SetWindowSize);
  JS_GLFW_SET_METHOD(SetWindowPos);
  JS_GLFW_SET_METHOD(IconifyWindow);
  JS_GLFW_SET_METHOD(RestoreWindow);
  JS_GLFW_SET_METHOD(SwapBuffers);
  JS_GLFW_SET_METHOD(SwapInterval);
  JS_GLFW_SET_METHOD(GetWindowParam);

  /* Input handling */
  JS_GLFW_SET_METHOD(PollEvents);
  JS_GLFW_SET_METHOD(WaitEvents);
  JS_GLFW_SET_METHOD(GetKey);
  JS_GLFW_SET_METHOD(GetMouseButton);
  JS_GLFW_SET_METHOD(GetMousePos);
  JS_GLFW_SET_METHOD(SetMousePos);
  JS_GLFW_SET_METHOD(GetMouseWheel);
  JS_GLFW_SET_METHOD(SetMouseWheel);

  /* Time */
  JS_GLFW_SET_METHOD(GetTime);
  JS_GLFW_SET_METHOD(SetTime);
  JS_GLFW_SET_METHOD(Sleep);

  /* Extension support */
  JS_GLFW_SET_METHOD(ExtensionSupported);
  //JS_GLFW_SET_METHOD(GetProcAddress( const char *procname );
  JS_GLFW_SET_METHOD(GetGLVersion);

  /* Enable/disable functions */
  JS_GLFW_SET_METHOD(Enable);
  JS_GLFW_SET_METHOD(Disable);

  /*************************************************************************
   * GLFW version
   *************************************************************************/

  JS_GLFW_CONSTANT(VERSION_MAJOR);
  JS_GLFW_CONSTANT(VERSION_MINOR);
  JS_GLFW_CONSTANT(VERSION_REVISION);


  /*************************************************************************
   * Input handling definitions
   *************************************************************************/

  /* Key and button state/action definitions */
  JS_GLFW_CONSTANT(RELEASE);
  JS_GLFW_CONSTANT(PRESS);

  /* Keyboard key definitions: 8-bit ISO-8859-1 (Latin 1) encoding is used
   * for printable keys (such as A-Z, 0-9 etc), and values above 256
   * represent special (non-printable) keys (e.g. F1, Page Up etc).
   */
  JS_GLFW_CONSTANT(KEY_UNKNOWN);
  JS_GLFW_CONSTANT(KEY_SPACE);
  JS_GLFW_CONSTANT(KEY_SPECIAL);
  JS_GLFW_CONSTANT(KEY_ESC);
  JS_GLFW_CONSTANT(KEY_F1);
  JS_GLFW_CONSTANT(KEY_F2);
  JS_GLFW_CONSTANT(KEY_F3);
  JS_GLFW_CONSTANT(KEY_F4);
  JS_GLFW_CONSTANT(KEY_F5);
  JS_GLFW_CONSTANT(KEY_F6);
  JS_GLFW_CONSTANT(KEY_F7);
  JS_GLFW_CONSTANT(KEY_F8);
  JS_GLFW_CONSTANT(KEY_F9);
  JS_GLFW_CONSTANT(KEY_F10);
  JS_GLFW_CONSTANT(KEY_F11);
  JS_GLFW_CONSTANT(KEY_F12);
  JS_GLFW_CONSTANT(KEY_F13);
  JS_GLFW_CONSTANT(KEY_F14);
  JS_GLFW_CONSTANT(KEY_F15);
  JS_GLFW_CONSTANT(KEY_F16);
  JS_GLFW_CONSTANT(KEY_F17);
  JS_GLFW_CONSTANT(KEY_F18);
  JS_GLFW_CONSTANT(KEY_F19);
  JS_GLFW_CONSTANT(KEY_F20);
  JS_GLFW_CONSTANT(KEY_F21);
  JS_GLFW_CONSTANT(KEY_F22);
  JS_GLFW_CONSTANT(KEY_F23);
  JS_GLFW_CONSTANT(KEY_F24);
  JS_GLFW_CONSTANT(KEY_F25);
  JS_GLFW_CONSTANT(KEY_UP);
  JS_GLFW_CONSTANT(KEY_DOWN);
  JS_GLFW_CONSTANT(KEY_LEFT);
  JS_GLFW_CONSTANT(KEY_RIGHT);
  JS_GLFW_CONSTANT(KEY_LSHIFT);
  JS_GLFW_CONSTANT(KEY_RSHIFT);
  JS_GLFW_CONSTANT(KEY_LCTRL);
  JS_GLFW_CONSTANT(KEY_RCTRL);
  JS_GLFW_CONSTANT(KEY_LALT);
  JS_GLFW_CONSTANT(KEY_RALT);
  JS_GLFW_CONSTANT(KEY_TAB);
  JS_GLFW_CONSTANT(KEY_ENTER);
  JS_GLFW_CONSTANT(KEY_BACKSPACE);
  JS_GLFW_CONSTANT(KEY_INSERT);
  JS_GLFW_CONSTANT(KEY_DEL);
  JS_GLFW_CONSTANT(KEY_PAGEUP);
  JS_GLFW_CONSTANT(KEY_PAGEDOWN);
  JS_GLFW_CONSTANT(KEY_HOME);
  JS_GLFW_CONSTANT(KEY_END);
  JS_GLFW_CONSTANT(KEY_KP_0);
  JS_GLFW_CONSTANT(KEY_KP_1);
  JS_GLFW_CONSTANT(KEY_KP_2);
  JS_GLFW_CONSTANT(KEY_KP_3);
  JS_GLFW_CONSTANT(KEY_KP_4);
  JS_GLFW_CONSTANT(KEY_KP_5);
  JS_GLFW_CONSTANT(KEY_KP_6);
  JS_GLFW_CONSTANT(KEY_KP_7);
  JS_GLFW_CONSTANT(KEY_KP_8);
  JS_GLFW_CONSTANT(KEY_KP_9);
  JS_GLFW_CONSTANT(KEY_KP_DIVIDE);
  JS_GLFW_CONSTANT(KEY_KP_MULTIPLY);
  JS_GLFW_CONSTANT(KEY_KP_SUBTRACT);
  JS_GLFW_CONSTANT(KEY_KP_ADD);
  JS_GLFW_CONSTANT(KEY_KP_DECIMAL);
  JS_GLFW_CONSTANT(KEY_KP_EQUAL);
  JS_GLFW_CONSTANT(KEY_KP_ENTER);
  JS_GLFW_CONSTANT(KEY_KP_NUM_LOCK);
  JS_GLFW_CONSTANT(KEY_CAPS_LOCK);
  JS_GLFW_CONSTANT(KEY_SCROLL_LOCK);
  JS_GLFW_CONSTANT(KEY_PAUSE);
  JS_GLFW_CONSTANT(KEY_LSUPER);
  JS_GLFW_CONSTANT(KEY_RSUPER);
  JS_GLFW_CONSTANT(KEY_MENU);
  JS_GLFW_CONSTANT(KEY_LAST);

  /* Mouse button definitions */
  JS_GLFW_CONSTANT(MOUSE_BUTTON_1);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_2);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_3);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_4);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_5);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_6);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_7);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_8);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_LAST);

  /* Mouse button aliases */
  JS_GLFW_CONSTANT(MOUSE_BUTTON_LEFT);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_RIGHT);
  JS_GLFW_CONSTANT(MOUSE_BUTTON_MIDDLE);


  /* Joystick identifiers */
  JS_GLFW_CONSTANT(JOYSTICK_1);
  JS_GLFW_CONSTANT(JOYSTICK_2);
  JS_GLFW_CONSTANT(JOYSTICK_3);
  JS_GLFW_CONSTANT(JOYSTICK_4);
  JS_GLFW_CONSTANT(JOYSTICK_5);
  JS_GLFW_CONSTANT(JOYSTICK_6);
  JS_GLFW_CONSTANT(JOYSTICK_7);
  JS_GLFW_CONSTANT(JOYSTICK_8);
  JS_GLFW_CONSTANT(JOYSTICK_9);
  JS_GLFW_CONSTANT(JOYSTICK_10);
  JS_GLFW_CONSTANT(JOYSTICK_11);
  JS_GLFW_CONSTANT(JOYSTICK_12);
  JS_GLFW_CONSTANT(JOYSTICK_13);
  JS_GLFW_CONSTANT(JOYSTICK_14);
  JS_GLFW_CONSTANT(JOYSTICK_15);
  JS_GLFW_CONSTANT(JOYSTICK_16);
  JS_GLFW_CONSTANT(JOYSTICK_LAST);


  /*************************************************************************
   * Other definitions
   *************************************************************************/

  /* glfwOpenWindow modes */
  JS_GLFW_CONSTANT(WINDOW);
  JS_GLFW_CONSTANT(FULLSCREEN);

  /* glfwGetWindowParam tokens */
  JS_GLFW_CONSTANT(OPENED);
  JS_GLFW_CONSTANT(ACTIVE);
  JS_GLFW_CONSTANT(ICONIFIED);
  JS_GLFW_CONSTANT(ACCELERATED);
  JS_GLFW_CONSTANT(RED_BITS);
  JS_GLFW_CONSTANT(GREEN_BITS);
  JS_GLFW_CONSTANT(BLUE_BITS);
  JS_GLFW_CONSTANT(ALPHA_BITS);
  JS_GLFW_CONSTANT(DEPTH_BITS);
  JS_GLFW_CONSTANT(STENCIL_BITS);

  /* The following constants are used for both glfwGetWindowParam
   * and glfwOpenWindowHint
   */
  JS_GLFW_CONSTANT(REFRESH_RATE);
  JS_GLFW_CONSTANT(ACCUM_RED_BITS);
  JS_GLFW_CONSTANT(ACCUM_GREEN_BITS);
  JS_GLFW_CONSTANT(ACCUM_BLUE_BITS);
  JS_GLFW_CONSTANT(ACCUM_ALPHA_BITS);
  JS_GLFW_CONSTANT(AUX_BUFFERS);
  JS_GLFW_CONSTANT(STEREO);
  JS_GLFW_CONSTANT(WINDOW_NO_RESIZE);
  JS_GLFW_CONSTANT(FSAA_SAMPLES);
  JS_GLFW_CONSTANT(OPENGL_VERSION_MAJOR);
  JS_GLFW_CONSTANT(OPENGL_VERSION_MINOR);
  JS_GLFW_CONSTANT(OPENGL_FORWARD_COMPAT);
  JS_GLFW_CONSTANT(OPENGL_DEBUG_CONTEXT);
  JS_GLFW_CONSTANT(OPENGL_PROFILE);

  /* GLFW_OPENGL_PROFILE tokens */
  JS_GLFW_CONSTANT(OPENGL_CORE_PROFILE);
  JS_GLFW_CONSTANT(OPENGL_COMPAT_PROFILE);

  /* glfwEnable/glfwDisable tokens */
  JS_GLFW_CONSTANT(MOUSE_CURSOR);
  JS_GLFW_CONSTANT(STICKY_KEYS);
  JS_GLFW_CONSTANT(STICKY_MOUSE_BUTTONS);
  JS_GLFW_CONSTANT(SYSTEM_KEYS);
  JS_GLFW_CONSTANT(KEY_REPEAT);
  JS_GLFW_CONSTANT(AUTO_POLL_EVENTS);

  /* glfwWaitThread wait modes */
  JS_GLFW_CONSTANT(WAIT);
  JS_GLFW_CONSTANT(NOWAIT);

  /* glfwGetJoystickParam tokens */
  JS_GLFW_CONSTANT(PRESENT);
  JS_GLFW_CONSTANT(AXES);
  JS_GLFW_CONSTANT(BUTTONS);

  /* glfwReadImage/glfwLoadTexture2D flags */
  JS_GLFW_CONSTANT(NO_RESCALE_BIT); /* Only for glfwReadImage */
  JS_GLFW_CONSTANT(ORIGIN_UL_BIT);
  JS_GLFW_CONSTANT(BUILD_MIPMAPS_BIT); /* Only for glfwLoadTexture2D */
  JS_GLFW_CONSTANT(ALPHA_MAP_BIT);

  /* Time spans longer than this (seconds) are considered to be infinity */
  JS_GLFW_CONSTANT(INFINITY);
}

NODE_MODULE(glfw, init)
}

