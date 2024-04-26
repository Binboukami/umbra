#pragma once

#include <stdbool.h>
#include "gl_include.hpp"
#include <iostream>

class GLFWWindowException : public std::exception
{
  private:
    char * message;

  public:
    GLFWWindowException(char * msg) : message(msg) {};

    char * what ()
    {
      return message;
    }
};

namespace UECore
{
  class Window
  {
    private:
      GLFWwindow* _handler = NULL;

      bool _exit = false;

      const char* _title;
      int _width = 0;
      int _height = 0;

    protected:
      GLFWwindow*
      InitWindow();

    public:
      Window(const char* title, int width, int height)
      {
        this->_title = title;
        this->_width = width; this->_height = height;
        
        this->_handler = this->InitWindow();

        if(!this->_handler)
        {
          throw GLFWWindowException((char*)"An error occurred while initializing window handler");
        }

        glfwMakeContextCurrent(this->_handler);

        gladLoadGL();
      }

      ~Window()
      {
        this->CloseWindow();
      };

      GLFWwindow*
      GetHandler()
      {
        return this->_handler;
      }

      bool
      ShouldClose(void);

      void
      PollEvents(void);

      void
      CloseWindow(void);
  };
}