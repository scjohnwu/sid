#include "utility/window.h"

int main(int arch, const char** argv) {
   sid::OpenGLSupport::LoadCore();

   sid::Window window(640, 480, "Shooting in the dark");

   if(!window.IsValid()) {
      sid::OpenGLSupport::TerminateCore();
      return -1;
   }

   if(!sid::OpenGLSupport::LoadExtensions()){
      sid::OpenGLSupport::TerminateCore();
      return -1;
   }

   sid::OpenGLSupport::EnableVSync();

   while(window.IsNotClosing()) {
      glClearColor(1.0,0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT);
      glClear(GL_DEPTH_BUFFER_BIT);

      window.SwapBuffers();

      window.PollEvents();
   }

   sid::OpenGLSupport::TerminateCore();

   return 0;
}