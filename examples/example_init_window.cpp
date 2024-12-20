#include "umbra.h"

int main()
{
  (void)U_InitContext();
  (void)U_InitWindow("hello, world", 800, 600);
  
  while(!U_ShouldCloseWindow())
  {
    // your code here
  }

  U_CloseWindow();

  return 0;
}