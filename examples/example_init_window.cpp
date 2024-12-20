#include <iostream>
#include "umbra.h"

int main() {
	const U_InitContextResult result = U_InitContext(); // Ignore result

	if (result.uc_ == nullptr) {
		std::cerr << "Error while initializing UmbraLib; ERROR: " << result.error_ << std::endl;
		exit(EXIT_FAILURE);
	}

	(void) U_InitWindow("hello, world", 800, 600);

	while (!U_ShouldCloseWindow()) {
		// your code here
	}

	U_CloseWindow();

	return 0;
}
