#include "object.h"
#include "dimen.h"
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "color.h"
#include "strings.h"

// Default constructor
Object::Object() {}

Object::Object(GLfloat x, GLfloat y)
{
	this->x = x;
	this->y = y;
}

// Destructor
Object::~Object() {}

void Object::drawPoint(GLfloat x, GLfloat y,
					   GLfloat size)
{
	glPushMatrix();
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glPopMatrix();
}

void Object::drawLine(GLfloat x1, GLfloat y1,
					  GLfloat x2, GLfloat y2,
					  GLfloat thickness)
{
	glPushMatrix();
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
	glPopMatrix();
}

void Object::drawTriangle(GLfloat x1, GLfloat y1,
						  GLfloat x2, GLfloat y2,
						  GLfloat x3, GLfloat y3)
{
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glEnd();
	glPopMatrix();
}

void Object::drawQuad(GLfloat x1, GLfloat y1,
					  GLfloat x2, GLfloat y2,
					  GLfloat x3, GLfloat y3,
					  GLfloat x4, GLfloat y4)
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glVertex2i(x4, y4);
	glEnd();
	glPopMatrix();
}

// (x,y) is the bottom left point
void Object::drawBackgroundRect(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	for (this->x = x; this->x < x + width; this->x++)
	{
		for (this->y = y; this->y < y + height; this->y++)
		{
			glColor3f(0.671 - ((this->y - y) * 1.0f / height), 0.753, 0.894); // top light blue
			this->drawPoint(this->x, this->y, 1.0);
		}
	}
}

void Object::drawRect(GLfloat width, GLfloat height)
{
	GLfloat currentX, currentY;

	GLfloat startX, startY, endX, endY;
	startX = this->x - (width / 2);
	startY = this->y - (height / 2);
	endX = this->x + (width / 2);
	endY = this->y + (height / 2);

	currentX = startX;
	currentY = startY;

	// Draw the rectangle
	this->drawQuad(startX, startY, endX, startY, endX, endY, startX, endY);
}

void Object::drawRegularPolygon(GLfloat cx, GLfloat cy, GLfloat radius, GLint sides, GLfloat orientation, GLfloat width, GLfloat height)
{
	glPushMatrix(); // Save the current matrix

	glBegin(GL_POLYGON); // Start drawing a polygon (multi-sided shape)

	// Loop through the number of sides to calculate each vertex
	for (int i = 0; i < sides; ++i)
	{
		// Calculate the angle for each vertex
		GLfloat angle = 2.0f * M_PI * i / sides + orientation;

		// Calculate the x and y position of the vertex, applying radius and scaling
		GLfloat x = cx + radius * cos(angle) * width;
		GLfloat y = cy + radius * sin(angle) * height;

		// Set the vertex at this calculated point
		glVertex2f(x, y);
	}

	glEnd();	   // End the polygon drawing
	glPopMatrix(); // Restore the previous matrix
}

// 2D Circle & Torus
void Object::drawCircle(GLfloat radius, GLfloat startDegree, GLfloat endDegree)
{
	endDegree += startDegree > endDegree ? 360 : 0;
	GLfloat angle = (endDegree - startDegree) * M_PI / 180;	   // get the angle in radian
	int triangleAmount = (int)((endDegree - startDegree) / 3); // Calculate the triangle amount base on the angle
	GLfloat cx = this->x;
	GLfloat cy = this->y;

	// Pre-rotate the circle to counter the drawing rotation
	this->rotate(-90 + startDegree, cx, cy);

	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(cx, cy); // center of circle
	for (int i = 0; i <= triangleAmount; i++)
	{
		GLfloat currentAngle = i * angle / triangleAmount;

		glVertex2f(
			cx + (radius * cos(currentAngle)),
			cy - (radius * sin(currentAngle)));
	}

	this->glEndReset();
}

void Object::drawTorus(GLfloat radius, GLfloat thickness, GLfloat startDegree, GLfloat endDegree)
{
	endDegree += startDegree > endDegree ? 360 : 0;
	GLfloat angle = (endDegree - startDegree) * M_PI / 180;	 // get the angle in radian
	int triangleAmount = (int)(endDegree - startDegree) * 4; // Calculate the triangle amount base on the angle
	GLfloat cx = this->x;
	GLfloat cy = this->y;
	GLfloat currentX, currentY, currentAngle;
	GLfloat radius_outer = radius + (thickness / 2);
	GLfloat radius_inner = radius - (thickness / 2);

	// Pre-rotate the circle to counter the drawing rotation
	this->rotate(-90 + startDegree, cx, cy);

	glPushMatrix();
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= triangleAmount; i++)
	{
		currentAngle = i * angle / triangleAmount;
		glVertex2f(
			cx + (radius_outer * cos(currentAngle)),
			cy - (radius_outer * sin(currentAngle)));
		glVertex2f(
			cx + (radius_inner * cos(currentAngle)),
			cy - (radius_inner * sin(currentAngle)));
	}
	this->glEndReset();
}

// Rounded rectangle
void Object::drawRoundedRectFill(GLfloat width, GLfloat height, GLfloat radius)
{
	GLfloat originalX = this->x;
	GLfloat originalY = this->y;
	GLfloat startX1, startX2, endX1, endX2;
	GLfloat startY1, startY2, endY1, endY2;
	startX1 = this->x - (width / 2);
	startX2 = this->x - (width / 2) + radius;
	endX1 = this->x + (width / 2) - radius;
	endX2 = this->x + (width / 2);
	startY1 = this->y - (height / 2);
	startY2 = this->y - (height / 2) + radius;
	endY1 = this->y + (height / 2) - radius;
	endY2 = this->y + (height / 2);

	GLfloat diameter = radius * 2;

	// Draw the rectangles
	// Center rectangle
	this->drawQuad(startX2, startY2, endX1, startY2, endX1, endY1, startX2, endY1);
	// Start rectangle
	this->drawQuad(startX1, startY2, startX2, startY2, startX2, endY1, startX1, endY1);
	// End rectangle
	this->drawQuad(endX1, startY2, endX2, startY2, endX2, endY1, endX1, endY1);
	// Top rectangle
	this->drawQuad(startX2, endY1, endX1, endY1, endX1, endY2, startX2, endY2);
	// Bottom rectangle
	this->drawQuad(startX2, startY1, endX1, startY1, endX1, startY2, startX2, startY2);

	// Draw the corners
	// Top left corner
	this->x = startX2;
	this->y = endY1;
	this->drawCircle(radius, 270, 360);

	// Top right corner
	this->x = endX1;
	this->y = endY1;
	this->drawCircle(radius, 0, 90);

	// Bottom right corner
	this->x = endX1;
	this->y = startY2;
	this->drawCircle(radius, 90, 180);

	// Bottom left corner
	this->x = startX2;
	this->y = startY2;
	this->drawCircle(radius, 180, 270);

	// Reset the position
	this->x = originalX;
	this->y = originalY;
}

void Object::drawRoundedRectLine(GLfloat width, GLfloat height, GLfloat radius, GLfloat thickness)
{
	GLfloat originalX = this->x;
	GLfloat originalY = this->y;
	GLfloat startX1, startX2, startX3, endX1, endX2, endX3;
	GLfloat startY1, startY2, startY3, endY1, endY2, endY3;
	startX1 = this->x - (width / 2) - (thickness / 2);
	startX2 = this->x - (width / 2) + (thickness / 2);
	startX3 = this->x - (width / 2) + radius;
	endX1 = this->x + (width / 2) - radius;
	endX2 = this->x + (width / 2) - (thickness / 2);
	endX3 = this->x + (width / 2) + (thickness / 2);
	startY1 = this->y - (height / 2) - (thickness / 2);
	startY2 = this->y - (height / 2) + (thickness / 2);
	startY3 = this->y - (height / 2) + radius;
	endY1 = this->y + (height / 2) - radius;
	endY2 = this->y + (height / 2) - (thickness / 2);
	endY3 = this->y + (height / 2) + (thickness / 2);

	GLfloat diameter = radius * 2;

	// Draw the rectangles
	// Start rectangle
	this->drawQuad(startX1, startY3, startX2, startY3, startX2, endY1, startX1, endY1);
	// End rectangle
	this->drawQuad(endX2, startY3, endX3, startY3, endX3, endY1, endX2, endY1);
	// Top rectangle
	this->drawQuad(startX3, endY2, endX1, endY2, endX1, endY3, startX3, endY3);
	// Bottom rectangle
	this->drawQuad(startX3, startY1, endX1, startY1, endX1, startY2, startX3, startY2);

	// Draw the corners
	// Top left corner
	this->x = startX3;
	this->y = endY1;
	this->drawTorus(radius, thickness, 270, 360);

	// Top right corner
	this->x = endX1;
	this->y = endY1;
	this->drawTorus(radius, thickness, 0, 90);

	// Bottom right corner
	this->x = endX1;
	this->y = startY3;
	this->drawTorus(radius, thickness, 90, 180);

	// Bottom left corner
	this->x = startX3;
	this->y = startY3;
	this->drawTorus(radius, thickness, 180, 270);

	// Reset the position
	this->x = originalX;
	this->y = originalY;
}

void Object::drawWatchBody(GLfloat radius, GLfloat thickness, GLfloat width, GLfloat height)
{
	GLfloat originalX = this->x;
	GLfloat originalY = this->y;
	GLfloat radiusOuterTorus = radius + (thickness / 2);
	GLfloat radiusInnerTorus = radius - thickness - 5;
	GLfloat thicknessInnerTorus = (thickness / 2);
	GLfloat widthBtn = radius / 4;
	GLfloat heightBtn = radius / 5;

	this->x = originalX + radius;
	glColor4f(COLOR_PINK);
	this->drawRect(widthBtn, heightBtn);

	this->x = originalX + radius;
	glColor4f(COLOR_WHITE);
	this->drawRect(widthBtn - 9, heightBtn - 6);

	this->x = originalX;
	this->y = originalY;
	glColor4f(COLOR_WHITE);
	this->drawCircle(radius, 0, 360);

	glColor4f(COLOR_PINK);
	this->drawTorus(radiusOuterTorus, thickness, 0, 360);

	glColor4f(COLOR_BLACK);
	this->drawTorus(radiusInnerTorus, thicknessInnerTorus, 0, 360);
}

void Object::drawWatchStrap(GLfloat width, GLfloat height)
{
	GLfloat originalX = this->x;
	GLfloat originalY = this->y;
	GLfloat radius = WATCH_BODY_RADIUS;
	GLfloat SmallbottomY = originalY - radius;
	GLfloat BigbottomY = originalY - radius - 115;
	GLfloat SmalltopY = originalY + radius;
	GLfloat BigtopY = originalY + radius + 115;

	// Bottom WatchStrap
	this->y = BigbottomY;
	glColor4f(COLOR_BLACK);
	this->drawRect(width, height);

	this->y = BigbottomY;
	glColor4f(COLOR_PINK);
	this->drawRect(width - 5, height - 4);

	this->y = SmallbottomY;
	glColor4f(COLOR_BLACK);
	this->drawRect(width / 2, height / 3);

	this->y = SmallbottomY;
	glColor4f(COLOR_PINK);
	this->drawRect((width / 2) - 5, (height / 3) - 4);

	// Top WatchStrap
	this->y = BigtopY;
	glColor4f(COLOR_BLACK);
	this->drawRect(width, height);

	this->y = BigtopY;
	glColor4f(COLOR_PINK);
	this->drawRect(width - 5, height - 4);

	this->y = SmalltopY;
	glColor4f(COLOR_BLACK);
	this->drawRect(width / 2, height / 3);

	this->y = SmalltopY;
	glColor4f(COLOR_PINK);
	this->drawRect((width / 2) - 5, (height / 3) - 4);
}

// Heart shape
void Object::drawHeart(GLfloat size)
{
	GLfloat x1, x2;
	GLfloat y1, y2;
	GLfloat originalX = this->x;
	GLfloat originalY = this->y;
	GLfloat radius = size / 2;
	GLfloat offset = size / 4;
	x1 = this->x - radius;
	y1 = this->y;
	x2 = this->x;
	y2 = this->y + radius;

	this->rotate(45, this->x, this->y);
	this->drawRect(size, size);

	this->x = x1;
	this->y = y1;
	this->drawCircle(radius, 180, 360);

	this->rotate(45, originalX, originalY);
	this->x = x2;
	this->y = y2;
	this->drawCircle(radius, 270, 90);
}

void Object::drawLighting(GLfloat width, GLfloat height)
{
	GLfloat originalX = this->x;
	GLfloat originalY = this->y;

	// Top Lighting
	GLfloat bottomLeftx1 = originalX - (width * 0.2);
	GLfloat bottomLefty1 = originalY;
	GLfloat bottomRightx1 = originalX + (width * 0.5);
	GLfloat bottomRighty1 = originalY;
	GLfloat topRightx1 = originalX + (width * 1.5);
	GLfloat topRighty1 = originalY + height;
	GLfloat topLeftx1 = originalX + (width * 0.5);
	GLfloat topLefty1 = originalY + height;

	// Bottom Lighting
	GLfloat leftx1 = originalX - (width * 0.15);
	GLfloat lefty2 = originalY - height;
	GLfloat rightx1 = originalX + (width * 1.5);
	GLfloat righty2 = originalY + (height * 0.3);
	GLfloat topx1 = originalX + (width * 0.4);
	GLfloat topy2 = originalY + (height * 0.3);

	this->drawQuad(bottomLeftx1, bottomLefty1,
				   bottomRightx1, bottomRighty1,
				   topRightx1, topRighty1,
				   topLeftx1, topLefty1);

	this->drawTriangle(leftx1, lefty2, rightx1, righty2, topx1, topy2);
}

void Object::drawHeartContainer(GLfloat x, GLfloat y, GLfloat radius, GLfloat thickness, GLfloat size)
{
	GLfloat x1 = x - 23;
	GLfloat y1 = y - 5;
	GLfloat y2 = y - 42;

	this->x = x;
	this->y = y;
	glColor4f(COLOR_YELLOW);
	this->drawTorus(radius, thickness, 210, 150);

	this->x = x1;
	this->y = y1;
	glColor4f(COLOR_BLACK);
	this->scale(0.14);
	this->drawText("075", 2);

	this->x = x;
	this->y = y2;
	glColor4f(COLOR_RED);
	this->drawHeart(size);
}

void Object::drawBatteryContainer(GLfloat x, GLfloat y, GLfloat radius, GLfloat thickness, GLfloat size)
{
	GLfloat x1 = x - 30;
	GLfloat x2 = x - 10;
	GLfloat y1 = y - 5;
	GLfloat y2 = y - 42;

	this->x = x;
	this->y = y;
	glColor4f(COLOR_YELLOW);
	this->drawTorus(radius, thickness, 210, 150);

	this->x = x1;
	this->y = y1;
	glColor4f(COLOR_BLACK);
	this->scale(0.14);
	this->drawText("100%", 2);

	this->x = x2;
	this->y = y2;
	glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
	this->drawLighting(18, 15);
}

void Object::drawTautBelt(GLfloat circleRadius1, GLfloat circleRadius2, GLfloat h, GLfloat orientation)
{
	GLfloat originalX, originalY;
	originalX = this->x;
	originalY = this->y;

	GLfloat dummyAngle = orientation;
	while (dummyAngle >= 90)
		dummyAngle -= 90;
	GLfloat angle_a = (90 - dummyAngle);

	// angle_a = angle_a * M_PI / 180.0;
	angle_a = angle_a * M_PI / 180.0;

	GLfloat x1, x2, x3, x4;
	GLfloat y1, y2, y3, y4;
	x1 = originalX - circleRadius1;
	y1 = originalY;
	x2 = originalX - circleRadius2;
	y2 = originalY + h;
	x3 = originalX + circleRadius2;
	y3 = originalY + h;
	x4 = originalX + circleRadius1;
	y4 = originalY;

	// Draw the body
	this->rotate(orientation, this->x, this->y);
	this->drawQuad(x1, y1,
				   x2, y2,
				   x3, y3,
				   x4, y4);

	// Draw the bottom circle
	this->drawCircle(circleRadius1, 90 + orientation, 270 + orientation);

	// Draw the top circle
	this->rotate(orientation, this->x, this->y);
	this->y = originalY + h;
	this->drawCircle(circleRadius2, 270, 90);

	// Reset the position
	this->x = originalX;
	this->y = originalY;
}

void Object::drawFlower(GLfloat length)
{
	GLfloat originalX = this->x;
	GLfloat originalY = this->y;
	GLfloat length2 = length * 0.7;
	GLfloat radius = length * 0.22;
	GLfloat outerCR1 = length * 0.15;
	GLfloat innerCR1 = length * 0.5;
	GLfloat outerCR2 = length * 0.08;
	GLfloat innerCR2 = length * 0.3;

	// outer flower
	glColor4f(COLOR_PINK_1);
	this->drawTautBelt(outerCR1, innerCR1, length, 0);

	glColor4f(COLOR_PINK_1);
	this->drawTautBelt(outerCR1, innerCR1, length, 72);

	glColor4f(COLOR_PINK_1);
	this->drawTautBelt(outerCR1, innerCR1, length, 144);

	glColor4f(COLOR_PINK_1);
	this->drawTautBelt(outerCR1, innerCR1, length, 216);

	glColor4f(COLOR_PINK_1);
	this->drawTautBelt(outerCR1, innerCR1, length, 288);

	// inner flower
	glColor4f(COLOR_DARK_PINK_1);
	this->drawTautBelt(outerCR2, innerCR2, length2, 0);

	glColor4f(COLOR_DARK_PINK_1);
	this->drawTautBelt(outerCR2, innerCR2, length2, 72);

	glColor4f(COLOR_DARK_PINK_1);
	this->drawTautBelt(outerCR2, innerCR2, length2, 144);

	glColor4f(COLOR_DARK_PINK_1);
	this->drawTautBelt(outerCR2, innerCR2, length2, 216);

	glColor4f(COLOR_DARK_PINK_1);
	this->drawTautBelt(outerCR2, innerCR2, length2, 288);

	// center point
	glColor4f(COLOR_YELLOW_1);
	this->drawCircle(radius, 0, 360);
}

void Object::drawFilter(GLfloat width)
{
	GLfloat alpha = 0.000278;

	for(int r = width; r < 650; r++){
		glColor4f(0.0f, 0.0f, 0.0f, r*alpha);	// transparency from 0.03f increase until 0.7f
		this->drawTorus(r, 1, 0, 360);
		alpha += 0.000008;
		if (alpha > 0.001079f) alpha = 0.001079f;	// maximum 0.7f transparency
	}

}

void Object::drawBackgroundLine(GLfloat bx, GLfloat by, GLfloat thickness)
{

	for (int x = 0; x <= bx; x += 10)
	{
		for (int y = 0; y <= by; y += 10)
		{

			glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
			this->drawLine(x, 0, x, by, thickness); // vertical line
			this->drawLine(0, y, bx, y, thickness); // horizontal line
		}
	}
}

// 2D transformation
void Object::translate(GLfloat tX, GLfloat tY)
{
	this->x = this->x + tX;
	this->y = this->y + tY;
}

void Object::rotate(GLfloat t, GLfloat pX, GLfloat pY)
{
	glTranslated(pX, pY, 0);
	glRotatef(-t, 0.0f, 0.0f, 1.0f);
	glTranslated(-pX, -pY, 0);
}

void Object::scale(GLfloat sX, GLfloat sY)
{
	glTranslated(this->x, this->y, 0);
	glScalef(sX, sY, 1);
	glTranslated(-this->x, -this->y, 0);
}

void Object::scale(GLfloat scale)
{
	this->scale(scale, scale);
}

void Object::drawText(char *string, GLfloat size)
{
	char *p;
	GLfloat x = this->x;
	GLfloat y = this->y;
	glPushMatrix();
	glLineWidth(size);

	glTranslatef(x, y, 0);
	for (p = string; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);

	this->glEndReset();
}

void Object::glEndReset()
{
	glEnd();
	glPopMatrix();
	glLoadIdentity();								 // Reset the matrix.
	gluOrtho2D(0, WINDOWS_WIDTH, 0, WINDOWS_HEIGHT); // Set canvas to windows width and height.
}