#include "pch.h"
#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h>
#include <GL/freeglut.h>

#include <iostream>
#include <iomanip>
#include <opencv2/core.hpp>    // include OpenCV core headers
#include <opencv2/imgproc.hpp> // include image processing headers
#include <opencv2/highgui.hpp> // include GUI-related headers


#include "PoseEstimation.h"
#include "MarkerTracker.h"
#include "DrawPrimitives.h"

//New added


//#include <glut.h>
#include <math.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4996)
//End

const GLfloat tam_x = 2.6f;
const GLfloat tam_y = 2.6f;

const GLint sy = 2;
const GLint my = 2;
const GLint hy = 2;

int hour;
int minute;
int second;
// Added in Exercise 9 - Start *****************************************************************

struct Position { double x,y,z; };

bool debugmode = false;
bool balldebug = false;

float resultTransposedMatrix[16];
float snowmanLookVector[4];
int towards = 0x005A;
int towardsList[2] = {0x005a, 0x0272};
int towardscounter = 0;
Position ballpos;
int ballspeed = 100;



// Added in Exercise 9 - End *****************************************************************

//camera settings
const int camera_width  = 640;
const int camera_height = 480;
const int virtual_camera_angle = 30;
unsigned char bkgnd[camera_width*camera_height*3];


void circle(GLfloat xc, GLfloat yc, GLfloat raio, bool fill)
{
	const GLfloat c = 3.1415926f / 180.0f;
	GLint i;

	glBegin(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP);

	for (i = 0; i <= 360; i += 2)
	{
		float a = i * c;
		glVertex2f(xc + sin(a) * raio, yc + cos(a) * raio);
		//	glVertex2f(100 + sin(a) * 100, yc + cos(a) * 100);
	}

	glEnd();
}

void cclock()
{
	//��ȡʱ��
	time_t now = time(0);
	struct tm *datahora = localtime(&now);
//////////////////////////////////////////////////////////
	glLoadIdentity();
	glLoadMatrixf(resultTransposedMatrix);
	glScalef(0.03f, 0.03f, 0.03f);
	glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
	glTranslatef(0.3f, 0.0f, 0.0f);
	//drawSnowman();

	// Added in Exercise 9 - Start *****************************************************************
//	rotateToMarker(resultMatrix_005A, resultMatrix_0272, 0x005a);
	glRotatef(45, 0, 0, 1);
	glRotatef(180, 0, 1, 0);


//////////////////////////////////////////////
	glScalef(0.3f, 0.3f, 0.3f);
	glBegin(GL_LINE_LOOP);
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, 2.6f, 2.6f, 0.4f, 50, 50);//��Բ��
	gluDeleteQuadric(quadratic);
	glEnd();

	// Paint background with specified color
	//	glClear(GL_COLOR_BUFFER_BIT);

	// Draw a circle	
	glColor3f(1.0f, 1.0f, 0.0f); // Color of Clock - Yellow
	circle(0, 0, tam_x, true);

	glColor3f(0.0f, 0.0f, 0.0f); // Black
	circle(0, 0, tam_x, false);

	const GLfloat c = 3.1415926f / 180.0f;

	float anguloS = second * 6;

	glRotatef(anguloS, 0.0f, 0.0f, 1.0f);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); // Red-s
	glVertex2f(0, 0);
	glVertex2f(sy* sin(6 * c*datahora->tm_sec), sy* cos(6 * c*datahora->tm_sec));
	glEnd();

	//	glLoadIdentity();

	float anguloM = minute * 6;

	glRotatef(anguloM, 0.0f, 0.0f, 1.0f);

	glLineWidth(50);
	glBegin(GL_LINES);  //����
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0, 0);
	glVertex2f(my* sin(6 * c*datahora->tm_min), my* cos(6 * c*datahora->tm_min));
	glEnd();

	//	glLoadIdentity();

	float anguloH = (hour + minute / 60.0) * 30;

	glRotatef(anguloH, 0.0f, 0.0f, 1.0f);

	glBegin(GL_LINES);   //ʱ��
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0, 0);
	glVertex2f(hy* sin(30 * c*datahora->tm_hour), hy* cos(30 * c*datahora->tm_hour));
	glEnd();

	//	glLoadIdentity();

	glFlush();
}


void redimensiona(GLsizei largura, GLsizei altura)
{
	glViewport(0, 0, largura, altura);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (largura <= altura)
		gluOrtho2D(-tam_x, tam_x, -tam_y * altura / largura, tam_y * altura / largura);
	else
		gluOrtho2D(-tam_x * largura / altura, tam_x * largura / altura, -tam_y, tam_y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void InitializeVideoStream( cv::VideoCapture &camera ) {
	if( camera.isOpened() )
		camera.release();

	camera.open(0); // open the default camera
	if ( camera.isOpened()==false ) {
		std::cout << "No webcam found, using a video file" << std::endl;
		camera.open("MarkerMovie.mpg");
		if ( camera.isOpened()==false ) {
			std::cout << "No video file found. Exiting."      << std::endl;
			exit(0);
		}
	}
}

// Added in Exercise 9 - Start *****************************************************************
void multMatrix(float mat[16], float vec[4])
{
	for(int i=0; i<4; i++)
	{
		snowmanLookVector[i] = 0;
		for(int j=0; j<4; j++)
			  snowmanLookVector[i] += mat[4*i + j] * vec[j];
	}
}

void moveBall(float mat[16])
{
	float vector[3];
	vector[0] = mat[3]  - (float)ballpos.x;
	vector[1] = mat[7]  - (float)ballpos.y;
	vector[2] = mat[11] - (float)ballpos.z;

	float length = sqrt( vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2] );
	if(balldebug) std::cout << length << std::endl;
	if( length < 0.01) 
	{ 
		towards = towardsList[(towardscounter++)%2]; 
		if(balldebug) std::cout << "target changed to marker " << towards << std::endl; 
		ballspeed = 60 + 80 * rand()/RAND_MAX;
		return; 
	}
	ballpos.x += vector[0] / (ballspeed * length);
	ballpos.y += vector[1] / (ballspeed * length);
	ballpos.z += vector[2] / (ballspeed * length);

}

void rotateToMarker(float thisMarker[16], float lookAtMarker[16], int markernumber)
{
	float vector[3];
	vector[0] = lookAtMarker[3] - thisMarker[3];
	vector[1] = lookAtMarker[7] - thisMarker[7];
	vector[2] = lookAtMarker[11] - thisMarker[11];

	if(towards == markernumber) moveBall(lookAtMarker);
	
	//normalize vector
	float help = sqrt( vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2] );
	vector[0] /= help;
	vector[1] /= help;
	vector[2] /= help;

	if(debugmode) std::cout << "Vector: " << vector[0] << ", " << vector[1] << ", " << vector[2] << std::endl;

	float defaultLook[4] = {1,0,0,0};
	multMatrix(thisMarker, defaultLook);

	//normalize snowmanLookVector
	help = sqrt( snowmanLookVector[0]*snowmanLookVector[0] + snowmanLookVector[1]*snowmanLookVector[1] + snowmanLookVector[2]*snowmanLookVector[2] );
	snowmanLookVector[0] /= help;
	snowmanLookVector[1] /= help;
	snowmanLookVector[2] /= help;

	if(debugmode) std::cout << "SnowmanLookVector: " << snowmanLookVector[0] << ", " << snowmanLookVector[1] << ", " << snowmanLookVector[2] << std::endl;

	float angle = ((float) (180.0 / M_PI)) * acos( vector[0] * snowmanLookVector[0] + vector[1] * snowmanLookVector[1] + vector[2] * snowmanLookVector[2] );
	if((vector[0] * snowmanLookVector[1] - vector[1] * snowmanLookVector[0]) < 0 ) angle *= -1;
	
	if(debugmode) std::cout << "Angle: " << angle << std::endl;
	
	glRotatef(angle, 0, 0, 1);
}
// Added in Exercise 9 - End *****************************************************************

/* program & OpenGL initialization */
void initGL(int argc, char *argv[])
{
// initialize the GL library
// Added in Exercise 8 - End *****************************************************************
    // pixel storage/packing stuff
	glPixelStorei(GL_PACK_ALIGNMENT, 1);// for glReadPixels
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // for glTexImage2D
	glPixelZoom(1.0, -1.0);
// Added in Exercise 8 - End *****************************************************************
    // enable and set colors
    glEnable( GL_COLOR_MATERIAL );
    glClearColor( 0, 0, 0, 1.0 );
	
	// enable and set depth parameters
	glEnable( GL_DEPTH_TEST );
	glClearDepth( 1.0 );

	// light parameters
	GLfloat light_pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat light_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_dif[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	// enable lighting
	glLightfv( GL_LIGHT0, GL_POSITION, light_pos );
	glLightfv( GL_LIGHT0, GL_AMBIENT,  light_amb );
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  light_dif );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
}

void display(GLFWwindow* window, const cv::Mat& img_bgr, std::vector<Marker>& markers)
{
	int button=1;
	const auto camera_image_size = sizeof(unsigned char) * img_bgr.rows * img_bgr.cols * 3;
	auto background_buffer_size = sizeof(bkgnd);
	memcpy(bkgnd, img_bgr.data, background_buffer_size);

	int width0, height0;
	glfwGetFramebufferSize(window, &width0, &height0);
	//	reshape(window, width, height);

		// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// draw background image
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, camera_width, 0.0, camera_height);

	glRasterPos2i(0, camera_height - 1);
	glDrawPixels(camera_width, camera_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, bkgnd);

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);

	//	return;

		// move to marker-position
	glMatrixMode(GL_MODELVIEW);

	// Added in Exercise 9 - Start *****************************************************************
	float resultMatrix_005A[16];
	float resultMatrix_0272[16];
	for (int i = 0; i < markers.size(); i++)
	{
		for (int j = 0; j < 16; j++)
			resultMatrix_005A[j] = markers[i].resultMatrix[j];
		const int code= markers[i].code;
		if (code == 0x005a)
		{
			//for (int j = 0; j < 16; j++)
			//	resultMatrix_005A[j] = markers[i].resultMatrix[j];
			button = 0;
		}
		else if (code == 0x0272)
			button = 1;
	}
	for (int x = 0; x < 4; ++x)
		for (int y = 0; y < 4; ++y)
			resultTransposedMatrix[x * 4 + y] = resultMatrix_005A[y * 4 + x];
	if (button)
	{
/*		glLoadIdentity();
		glLoadMatrixf(resultTransposedMatrix);
		glScalef(0.3f, 0.3f, 0.3f);
	//	glBegin(GL_LINE_LOOP);
		GLUquadricObj* cylinder2;
		cylinder2 = gluNewQuadric();
		gluCylinder(cylinder2, 2.6f, 2.6f, 0.4f, 50, 50);//��Բ��
		gluDeleteQuadric(cylinder2);
		glEnd();
*/
		glLoadIdentity();
		glLoadMatrixf(resultTransposedMatrix);
		glColor3f(1.0f, 0.0f, 0.0f);
		GLUquadricObj* cylinder2;
		cylinder2 = gluNewQuadric();
		gluCylinder(cylinder2,0.005f ,0.005f, 0.01f, 30, 30);
		circle(0, 0, 0.007f, true);

	}
	else {
	/*	glLoadIdentity();
		glLoadMatrixf(resultTransposedMatrix);
		glScalef(0.03f, 0.03f, 0.03f);
		glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
		glTranslatef(0.3f, 0.0f, 0.0f);
		//drawSnowman();

		// Added in Exercise 9 - Start *****************************************************************
	//	rotateToMarker(resultMatrix_005A, resultMatrix_0272, 0x005a);
		glRotatef(45, 0, 0, 1);
		glRotatef(180, 0, 1, 0);*/
		cclock();
	}
		/*else if(code == 0x0272){
			for(int j=0; j<16; j++)
				resultMatrix_0272[j] = markers[i].resultMatrix[j];
		}*/


		/*else if(code == 0x0272){
			for(int j=0; j<16; j++)
				resultMatrix_0272[j] = markers[i].resultMatrix[j];
		}*


	for (int x = 0; x < 4; ++x)
		for (int y = 0; y < 4; ++y)
			resultTransposedMatrix[x * 4 + y] = resultMatrix_005A[y * 4 + x];

	// Added in Exercise 9 - End *****************************************************************

	/*
	//drawBall
	glLoadIdentity();
	glTranslatef((float) ballpos.x, (float) ballpos.y + 0.024f, (float) ballpos.z);
	glColor4f(1,0,0,1);
	drawSphere(0.005, 10, 10);
// Added in Exercise 9 - End *****************************************************************

	
	//drawBall
	for (int x=0; x<4; ++x)
		for (int y=0; y<4; ++y)
			resultTransposedMatrix[x*4+y] = resultMatrix_0272[y*4+x];
	glLoadIdentity();
	glLoadMatrixf( resultTransposedMatrix );
	drawSphere(0.005, 10, 10);

	int key = cv::waitKey (10);
	if (key == 27) exit(0);
	// Added in Exercise 9 - Start *****************************************************************
	else if (key == 100) debugmode = !debugmode;
	else if (key == 98) balldebug = !balldebug;
	*/
	// Added in Exercise 9 - End *****************************************************************
	
}

void reshape( GLFWwindow* window, int width, int height ) {

	// set a whole-window viewport
	glViewport( 0, 0, (GLsizei)width, (GLsizei)height );

	// create a perspective projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 30, ((GLfloat)width/(GLfloat)height), 0.01, 100 );

	// invalidate display
	//glutPostRedisplay();
}


int main(int argc, char* argv[]) {
	
	cv::VideoCapture cap;

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	// initialize the window system
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(camera_width, camera_height, "Final", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	// Set callback functions for GLFW
	glfwSetFramebufferSizeCallback(window, reshape);

	glfwMakeContextCurrent(window);
	glfwSwapInterval( 1 );
	
	int window_width, window_height;
	glfwGetFramebufferSize(window, &window_width, &window_height);
	reshape(window, window_width, window_height);

	glViewport(0, 0, window_width, window_height);

	// initialize the GL library
	initGL(argc, argv);

    // setup OpenCV
	cv::Mat img_bgr;
	InitializeVideoStream(cap);
	const double kMarkerSize = 0.03;// 0.048; // [m]
	MarkerTracker markerTracker(kMarkerSize);
	
	std::vector<Marker> markers;
//	float resultMatrix[16];
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		markers.resize(0);
		/* Capture here */
		cap >> img_bgr;
		
		if(img_bgr.empty()){
			std::cout << "Could not query frame. Trying to reinitialize." << std::endl;
			InitializeVideoStream(cap);
			cv::waitKey(1000); /// Wait for one sec.
			continue;
		}

		/* Track a marker */
		markerTracker.findMarker( img_bgr, markers);///resultMatrix);

//		cv::imshow("img_bgr", img_bgr);
//		cv::waitKey(10); /// Wait for one sec.

		/* Render here */
		display(window, img_bgr, markers);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

  
    return 0;

}


