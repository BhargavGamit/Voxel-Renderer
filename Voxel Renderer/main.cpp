//Voxel Renderer written by Bhargav.K 
//Email : bhargav.gamit@gmail.com
//Takes 2D image sequence as input and displays in a 3D space
//Reference : OpenGL Programming Guide 

#include <iostream>
#include <GL/glew.h>        // Header File For The GLEW Library
#include <GL/glut.h>        // Header File For The GLUT Library
#include "Transform.h"      // Custom Header file
#include <string>
#include <sstream>
#include <FreeImage.h>		// Header File For The FreeImage Library
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <SOIL.h>			// Header File For The SOIL Library
using namespace std;

int arrowForce; // The amount of rotation for each arrow press
vec3 camera; // The (regularly updated) vector coordinates of the camera location 
vec3 up;  // The (regularly updated) vector coordinates of the up location 
const vec3 camerainit(0.0, 0.0, 4.0); // Initial camera position, also for resets (Change if needed)
const vec3 upinit(0.0, 1.0, 0.0); // Initial up position, also for resets
const int arrowForceInit = 5; //Initial step amount for camera movement, also for resets (Change if needed)

int w = 500, h = 500; // width and height 
int noofimages;
//array used to store textures
GLuint texture[1000];

//Function used to load texture from file to OpenGL format  (No need to alter this)
//Input .png filename
//Returns Texture loadable in OpenGL format
GLuint makeTexture(char *filename) {
	GLuint tex_2D;
	tex_2D = SOIL_load_OGL_single_cubemap(
		filename,
		SOIL_DDS_CUBEMAP_FACE_ORDER,
		SOIL_LOAD_RGBA,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		| SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_DDS_LOAD_DIRECT
		);
	if (tex_2D <= 0)
	{
		tex_2D = SOIL_load_OGL_HDR_texture(
			filename,
			SOIL_HDR_RGBE,
			0,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_POWER_OF_TWO
			| SOIL_FLAG_MIPMAPS
			);
		if (tex_2D < 1)
		{
			tex_2D = SOIL_load_OGL_texture(
				filename,
				SOIL_LOAD_RGBA,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_POWER_OF_TWO
				| SOIL_FLAG_MIPMAPS
				| SOIL_FLAG_MULTIPLY_ALPHA
				| SOIL_FLAG_DDS_LOAD_DIRECT
				);
		}

	}
	return tex_2D;
}


//Funtion used to save screenshot  (No need to alter this)
//Input filename
//Output screenshot of image in file No alpha in output file
void saveScreenshot(const char* fname) {
	int pix = w * h;
	BYTE *pixels = new BYTE[3 * pix];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w,h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	std::cout << "Saving screenshot: screenshot.png\n";
	FreeImage_Save(FIF_PNG, img, fname, 0);
	delete pixels;
}

//Self Explainatory
void printHelp() {
	std::cout << "\npress 'h' to print this message again.\n"
		<< "press '+' or '-' to change the amount of rotation that\n"
		<< "occurs with each arrow press.\n"
		<< "press 'o' to save screenshot.\n"
		<< "press 'r' to reset the transformation (camera and up).\n"
		<< "press ESC to quit.\n";

}

//Takes input from keyboard and performs appropriate action
//Input key
//You can add more functionality by adding case 
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '+':
		arrowForce++;
		std::cout << "amount set to " << arrowForce << "\n";
		break;
	case '-':
		arrowForce--;
		std::cout << "amount set to " << arrowForce << "\n";
		break;
	case 'o':saveScreenshot("sc.png");
		break;
	case 'h':
		printHelp();
		break;
	case 27:  // Escape to quit
		exit(0);
		break;
	case 'r': // Reset camera and up vectors 
		camera = camerainit;
		up = upinit;
		arrowForce = arrowForceInit;
		std::cout << "camera and up vectors reset, amount set to " << arrowForceInit << "\n";
		break;
	}
	glutPostRedisplay();
}

//  You will need to enter code for the arrow keys 
//  When an arrow key is pressed, it will call your transform functions
void specialKey(int key, int x, int y) {
	switch (key) {
	case 100: //left
		Transform::left(arrowForce, camera, up);
		break;
	case 101: //up
		Transform::up(arrowForce, camera, up);
		break;
	case 102: //right
		Transform::left(-arrowForce, camera, up);
		break;
	case 103: //down
		Transform::up(-arrowForce, camera, up);
		break;
	}
	glutPostRedisplay();
}

// Uses the Projection matrices to set perspective You can do using glm 
void reshape(int width, int height) {
	w = width;
	h = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, w / (float)h, 0.1, 99);
	glViewport(0, 0, w, h);
}


//Intializes Displaying Window
void init() {

	// Set up initial position for camera,up and amount
	camera = camerainit;
	up = upinit;
	arrowForce = arrowForceInit;

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.9); //Allows only alpha value > 0.9 to be displayed on the screen
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
}

//Displays the output on the window
//Enables to draw in 3D
void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	mat4 mv;
	const vec3 center(0.0, 0.0, 0.0);

	mv = glm::lookAt(camera, center, up);

	glLoadMatrixf(&mv[0][0]);
	
	float offset = 0.01f;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	float negative_z_border = -offset*(noofimages + 1)*0.5;
	float positive_z_border = ((noofimages + 1)*offset)+negative_z_border;
	for (float image_iterator = negative_z_border; image_iterator < positive_z_border; image_iterator = image_iterator + offset)
	{

		glBindTexture(GL_TEXTURE_2D, texture[int((image_iterator / offset) + (noofimages+1 / 2) )+1 ] );
		
		//Get image width and height
		int width, height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
		
		//Image borders
		float negative_x_border, positive_x_border, negative_y_border, positive_y_border;
		negative_x_border = -offset*width*0.5;
		positive_x_border = (width*offset) + (negative_x_border);
		negative_y_border = -offset*height*0.5;
		positive_y_border = (height*offset) + (negative_y_border);
		float z = image_iterator;
				
		for (float texture_width_iterator = negative_x_border; texture_width_iterator < positive_x_border; texture_width_iterator = texture_width_iterator + offset)
		{
			glBegin(GL_QUADS);
			for (float texture_height_iterator = negative_y_border; texture_height_iterator < positive_y_border; texture_height_iterator = texture_height_iterator + offset)
			{
				GLfloat image_width_iterator = ((texture_width_iterator / offset) + (width / 2) + 1) / float(width);
				GLfloat image_height_interator = ((texture_height_iterator / offset) + (height / 2) + 1) / float(height);
				
				//Voxel renderer written by Bhargav.K 
				//Email : bhargav.gamit@gmail.com
				//This is simple voxel rendering loop which converts 2D textures into 3D cubes and plots it in 3D space
				//front face
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator, z);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator, z);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator + offset, z);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator + offset, z);
				//back face
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator + offset, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator + offset, z - offset);
				//top face
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator, z);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator, z);

				//bottom face
				glNormal3f(0.0, -1.0, 0.0);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator + offset, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator + offset, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator + offset, z);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator + offset, z);
				//left face
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator + offset, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator, z);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator, texture_height_iterator + offset, z);
				//right face
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator + offset, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator, z - offset);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator, z);
				glTexCoord2f(image_width_iterator, image_height_interator);
				glVertex3f(texture_width_iterator + offset, texture_height_iterator + offset, z);
			}
			glEnd();
		}
	}
	glutSwapBuffers();
	glFlush();
}

	
//Use to load textures to texture array{opengl format}
void loadTexture(char *start, char *end)
{
	string startfile(start);
	string endfile(end);
	//Find number of images to be loaded
	int inl = startfile.length();
	int no_of_images = ((endfile[inl - 7] * 100) + (endfile[inl - 6] * 10) + (endfile[inl - 5] * 1)) - ((startfile[inl - 7] * 100) + (startfile[inl - 6] * 10) + (startfile[inl - 5] * 1));
	noofimages = no_of_images;
	no_of_images++;
	//Load the number of images
	for (int iterator = 1; iterator <= no_of_images; iterator++)
	{
		string name(start);
		int namesupport = 5;
		int x = iterator;
		while (x > 0)
		{
			name[inl - namesupport] = (x % 10) + '0';
			x = x / 10;
			namesupport++;

		}
		char * writable = new char[name.size() + 1];
		std::copy(name.begin(), name.end(), writable);
		writable[name.size()] = '\0';
		texture[iterator] = makeTexture(writable);
		delete[] writable;
	}	
}	
	



int main(int argc, char* argv[]) {

	//Initialize GLUT
	FreeImage_Initialise();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Voxel Renderer");
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Error: " << glewGetString(err) << std::endl;
	}

	//load texture call
	loadTexture("data001.png","data256.png");
	

	init();
	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutReshapeWindow(w, h);

	printHelp();
	glutMainLoop();
	FreeImage_DeInitialise();

	return 0;
}