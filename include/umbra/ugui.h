#pragma once

#include "urenderer.h"
#include "umath.h"
#include "umbragl.h"
#include "umbra.h"

typedef struct UGuiContext {
  URenderer renderer;

} UGuiContext;

extern UGuiContext UGUI;

#ifdef __cplusplus
extern "C" {
#endif

UMBRA_API void InitGUI();
UMBRA_API void BeginGUI(ui32 width, ui32 height);
UMBRA_API void EndGUI();

// Layout
// TODO: Layout can be set with a combination of calls to 
// 	U_SetViewport() and U_Scissor();

// Widgets
UMBRA_API void DoButton(const UVec3 position, const f32 size, const UVec3 color);

#ifdef __cplusplus
}
#endif