
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <vector>

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279112884L
#endif

using namespace std;

int WINDOW_WIDTH = 720;
int WINDOW_HEIGHT = 480;

float lower = -3.0f, upper = 3.0f;
float tLower = 2.0f, tUpper = 8.0f;
float bLower = -8.0f, bUpper = -2.0f;

struct Point {
	float x, y;
	float inc; // Speed for each point

	Point(float _x, float _y, float _inc) : x(_x), y(_y), inc(_inc) {}
};

vector<Point> flagControlPoints;

void drawBezierCurve(const vector<Point>& controlPoints) {

	glBegin(GL_LINE_STRIP);
	for (float t = 0; t <= 1; t += 0.01) {
		float x = pow(1 - t, 3) * controlPoints[0].x +
			3 * pow(1 - t, 2) * t * controlPoints[1].x +
			3 * (1 - t) * pow(t, 2) * controlPoints[2].x +
			pow(t, 3) * controlPoints[3].x;

		float y = pow(1 - t, 3) * controlPoints[0].y +
			3 * pow(1 - t, 2) * t * controlPoints[1].y +
			3 * (1 - t) * pow(t, 2) * controlPoints[2].y +
			pow(t, 3) * controlPoints[3].y;

		glVertex2f(x, y);
	}
	glEnd();
}

// Function to animate the control points to simulate waving in the wind
void animateControlPoints(int value) {

	for (auto& point : flagControlPoints) {
		// Oscillate along the y-axis within a certain range
		if (point.y > upper || point.y < lower) {
			point.inc *= -1.0f;
		}
		point.y += point.inc;
	}

	glutPostRedisplay();

	// Set different intervals for each point
	glutTimerFunc(1000 / 30, animateControlPoints, 0);  // 60 frames per second
}


void axes() {
	// x axis
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); // red
	glVertex2f(-10.0f, 0.0f);
	glVertex2f(10.0f, 0.0f);
	glEnd();

	// y axis
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f); // blue
	glVertex2f(0.0f, 10.0f);
	glVertex2f(0.0f, -10.0f);
	glEnd();
}

void drawCartesianCoordinates() {
	// Set the line color
	glColor3f(0.3f, 0.3f, 0.3f);

	// Display tick marks on x-axis
	for (int i = -10; i <= 10; ++i) {
		glBegin(GL_LINES);
		glVertex2f(i, -0.2);
		glVertex2f(i, 0.2);
		glEnd();
	}

	// Display tick marks on y-axis
	for (int i = -10; i <= 10; ++i) {
		glBegin(GL_LINES);
		glVertex2f(-0.2, i);
		glVertex2f(0.2, i);
		glEnd();
	}

	// Display the axis labels
	glColor3f(0.5f, 0.5f, 0.5f);
	glRasterPos2f(10.2, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'X');
	glRasterPos2f(0.0, 10.2);
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'Y');
}

void drawFlag(const vector<Point>& points) {

	for (float i = -6.0f; i < 6.0f; i += 0.025f) {

		if (i < -2.0f) {
			glColor3f(1.0f, 0.0f, 0.0f);
		} else if (i < 2.0f) {
			glColor3f(1.0f, 1.0f, 0.0f);
		} else {
			glColor3f(0.0f, 0.0f, 1.0f);
		}

		glPushMatrix();
		glTranslatef(0.0f, i, 0.0f);
		drawBezierCurve(points);
		glPopMatrix();
	}

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	//axes();
	//drawCartesianCoordinates();

	drawFlag(flagControlPoints);

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h) {
		gluOrtho2D(-11, 11, -11 / aspect_ratio, 11 / aspect_ratio);
	}
	else {
		gluOrtho2D(-11 * aspect_ratio, 11 * aspect_ratio, -11, 11);
	}
}

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(110, 300);
	glutCreateWindow("Flag Animation");

	flagControlPoints.push_back({ -8.0, 0.0, 0.0 });
	flagControlPoints.push_back({ -5.0, 0.0, 0.25 });
	flagControlPoints.push_back({ 5.0, 0.0, -0.25 });
	flagControlPoints.push_back({ 8.0, 0.0, 0.0 });

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutTimerFunc(1000 / 30, animateControlPoints, 0);

	init();
	glutMainLoop();

	return 0;
}