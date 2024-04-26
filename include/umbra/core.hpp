#pragma once

#include "window.hpp"
#include "renderer.hpp"
#include "font_renderer.hpp"
#include "input.hpp"

namespace UmbraEngine
{
  class Core
  {
    public:
      Core()
      {
        //
      }

      void
      InitEngine(const char* title, int width, int height)
      {
        Window = new UECore::Window(title, width, height);
        Text = new UECore::FontRenderer();

        Input = new UECore::Input(Window);

        /** Inner systems */
        BaseRenderer = new UECore::Renderer();
      }

      UECore::Window* Window;
      UECore::Input* Input;
      UECore::FontRenderer* Text;

      private:
        UECore::Renderer* BaseRenderer;

      /** TODO */
      // add input handling
      // add basic drawing function
      // add file system support functions
  };
}