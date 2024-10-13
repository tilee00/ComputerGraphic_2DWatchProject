#include "init.cpp"

int main()
{
	// Initialize the windows
	init();

	// Render the scene
	glutDisplayFunc(renderMaster);

	// Start the main loop
	glutMainLoop();

	// Exit the program
	return 0;
}
