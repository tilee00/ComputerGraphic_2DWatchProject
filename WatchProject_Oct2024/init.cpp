// Include rendering functions
#include "render.h"

void renderMaster(){

    //All the render function
    renderBackground();     // Background
    renderFlower();         // Flower
    renderWatchBody();      // Watch Body
    renderDate();           // Date 
    renderTime();           // Time
    renderHeart();          // Heart
    renderBattery();        // Battery
    renderStep();           // Step
    renderFilter();         // Shadow

    glFlush();  // Clear all GL executions.
    glFinish(); // Block until all GL executions are completed.
}

void init()
{
    // Initialize the windows
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);                   // Set the display mode to single buffer and RGBA.
    glutInitWindowSize(WINDOWS_WIDTH, WINDOWS_HEIGHT);              // Set the windows size.
    glutInitWindowPosition(0, 0);     // Set the windows position.
    glutCreateWindow("Watch v1.0 by @tilee");                       // Set the windows title.                                              

    // Initialize the rendering context
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);              // Set the background color to white, any area not rendered will be white.
    glMatrixMode(GL_PROJECTION);                       // Set the matrix mode to projection.
    glEnable(GL_LINE_SMOOTH);                          // Enables line anti-aliasing.
    glEnable(GL_BLEND);                                // Enable for proper transparency render.
    glDisable(GL_DEPTH_TEST);                          // Disable depth testing for overlap object. Use for transparency object involved.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable transparency.
    glEnd();
	glPopMatrix();
	glLoadIdentity();								   // Reset the matrix.
	gluOrtho2D(0, WINDOWS_WIDTH, 0, WINDOWS_HEIGHT);   // Set canvas to windows width and height.                          
    glClear(GL_COLOR_BUFFER_BIT);                      // Load frame buffer.
}