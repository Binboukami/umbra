#include <iostream>
#include "umbra.h"

int main() {
	const U_InitContextResult result = U_InitContext(); // Ignore result

	if (result.uc_ == nullptr) {
		std::cerr << "Error while initializing UmbraLib; ERROR: " << result.error_ << std::endl;
		exit(EXIT_FAILURE);
	}

	(void) U_InitWindow("hello, world", 800, 600);

	constexpr UCamera camera = {0.0f, 0.0f, 0.0f, UProjectionType::U_ORTHO, 90.0f};
	const TextureID tex = LoadTexture("../../../examples/textures/sample_texture.jpg");

	const UCoreContextRef ucore = result.uc_;

	while (!U_ShouldCloseWindow()) {
		U_BeginDrawing(camera);

		U_DrawTextureQuad(&ucore->renderer, {0.0f, 0.0f, 0.0f}, 100.0f, tex);

		U_EndDrawing();
	}

	U_CloseWindow();
	U_DestroyContext();

	return 0;
}
