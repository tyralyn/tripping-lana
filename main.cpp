//////////////////////////////////////////////////////////////////////////////////
// Tyralyn Tran, 5957360														//
//////////////////////////////////////////////////////////////////////////////////

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
unsigned char* pixels; //global pixel array
int width;	//global width of picture
int height;	//global  height of picure
bool leftButtonDown;

using namespace std;

void writePPM(const char* filename); //defined later
void drawStamp(int r, int g, int b, int x, int y); //defined later

//////////////////////////////////////////////////////////////////////////////////
// Draws to the OpenGL window													//
//////////////////////////////////////////////////////////////////////////////////
void display()
{
	glClearColor(0,0,0,0); //clears all of the pixels to RGB(0,0,0)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears buffer 
  	glPixelZoom(1,-1); //vertically flips canvas/viewing area 
  	glRasterPos2f(-1,1); //translates viewing area to compensate for vertical rotation
  	glDrawPixels(height, width, GL_RGB, GL_UNSIGNED_BYTE, pixels); //draws pixels into corrected frame
  glutSwapBuffers(); //shifts buffer to front
}

//////////////////////////////////////////////////////////////////////////////////
// Handles keyboard events														//
// takes keyboard key and mouse positions as input								//
//////////////////////////////////////////////////////////////////////////////////
void keyboard(unsigned char k, int x, int y)
{
  switch (k)
  {
	case 'w': //'w' writes current image to output.ppm
	  writePPM("output.ppm");
	  break;
    case 27: //escape key, q exit the program
    case 'q':
      exit(0);
      break;
  }
  glutPostRedisplay(); //redisplays window
}

//////////////////////////////////////////////////////////////////////////////////
// Called occasionally to see if anything's happening						    //
//////////////////////////////////////////////////////////////////////////////////
void idle()
{
  glutPostRedisplay(); //redisplays window
}

///////////////////////////////////////////////////////////////////////////////////
// Called if a mouse button is pressed											 //
// makes a different color cross upon holding down and lifting left mouse button //	
// takes button, state, and mouse locations as input							 //			
///////////////////////////////////////////////////////////////////////////////////
void mouseButton(int button, int state, int x, int y)
{
		
	if (button == GLUT_LEFT_BUTTON && state==GLUT_DOWN) {//if pressing left button, draw yellow stamp
		drawStamp(255,255,0,x,y); 
		leftButtonDown=true; //set leftButtonDown to indicate button is down
	}
	else if (button==GLUT_LEFT_BUTTON && state==GLUT_UP) { //if lifting left button, draw yellow stamp
		drawStamp(255,255,0,x,y);
		leftButtonDown=false; //reset leftButtonDown to false to indicate that button is no longer being pressed
	}
}

///////////////////////////////////////////////////////////////////////////////////
// takes rgb color integers, as well as mouse coordinates						 //
// change pixel color values to simulate drawing								 //
///////////////////////////////////////////////////////////////////////////////////
void changePixelColor(int r, int g, int b, int x, int y) {
	int k=3*(width*(y-1)+x); //get location in pixel array corresponding to (x,y)
	if (x<width && x>0 && y>0 && y<height) { //if x and y are within frame, change pixel's RGB value to parameter values
		pixels[k]=r; 
		pixels[k+1]=g;
		pixels[k+2]=b;
	}
}

///////////////////////////////////////////////////////////////////////////////////
// takes rgb color integers, as well as coordinates								 //
// makes a five-pixel "cross" stamp												 //
///////////////////////////////////////////////////////////////////////////////////
void drawStamp(int r, int g, int b, int x, int y) {
	changePixelColor(r,g,b,x,y); //change center pixel color
	if (y>=1) //if it will be within the window, change the pixel below
		changePixelColor(r,g,b,x,y-1);
	if (y<height) //if it will be within the window, change the pixel above
		changePixelColor(r,g,b,x,y+1);
	if (x>=1) //if it will be within the window, change the pixel to the left
		changePixelColor(r,g,b,x-1,y);
	if (x<width)  //if it will be within the window, change the pixel to the right
		changePixelColor(r,g,b,x+1,y);
}

//////////////////////////////////////////////////////////////////////////////////
// Called if the mouse moves													//
// takes mouse location as input												//
//////////////////////////////////////////////////////////////////////////////////

void mouseMotion(int x, int y)
{	if(leftButtonDown)	//if left button is being pressed
		drawStamp(70,70,255,x,y); //draw stamp in blue
	
}

//////////////////////////////////////////////////////////////////////////////////
// Reads in a raw PPM file of the "P6" style.									//
// takes filename as input														//
// puts information in global pixel array										//
//////////////////////////////////////////////////////////////////////////////////
void readPPM(const char* filename)
{
  // tries to open the file
  FILE* file;
  file = fopen(filename, "rb");
  if (file == NULL) //sent out "couldn't open file!" if file can't open
  {
    cout << " Couldn't open file " << filename << "! " << endl;
    exit(1); //exits
  }

  // reads in the image dimensions
  fscanf(file, "P6\n%d %d\n255\n", &width, &height);
  int totalPixels = width * height; //calculates total number of pixels in image

  pixels = new unsigned char[3 * totalPixels]; // allocates three times as many pixels since there are R,G, and B channels
  fread(pixels, 1, 3 * totalPixels, file);//reads pixels from file and into pixel array
  fclose(file); //closes file
  
  // outputs some success information
  cout << " Successfully read in " << filename << " with dimensions: " 
       << width << " " << height << endl;
}

//////////////////////////////////////////////////////////////////////////////////
// writes to a raw PPM file of the "P6" style.									//
// takes filename as input														//
// takes info from global pixel array											//
//////////////////////////////////////////////////////////////////////////////////
void writePPM(const char* filename)
{
  // tries to open the file
  FILE* file; //new file pointer
  file = fopen(filename, "wb"); //opens writable file of name filename
  
  fprintf(file, "P6\n%d %d\n255\n", width, height); //prints file information to new file
  int totalPixels = width * height;	 //total number of pixels in window/file	

  // allocate three times as many pixels since there are R,G, and B channels
  //pixels = new unsigned char[3 * totalPixels];
  fwrite(pixels, 1, 3 * totalPixels, file); //writes pixels to file
  fclose(file); //closes file
  
  // output some success information
  cout << " Successfully wrote to " << filename << " with dimensions: " 
       << width << " " << height << endl;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  readPPM("input.ppm"); //reads ppm file od name "input.ppm"

  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //initializes display mode
  glutInitWindowSize(width,height);	//initializes window size to global variables height and width
  glutInitWindowPosition(100, 100); //intializes window position of (100,100)
  glutCreateWindow("CMPSC 180, Homework 0"); //creates window of title parameter
  
  glutDisplayFunc(display); //calls back to display function (displays pixels)
  glutMotionFunc(mouseMotion); //calls back to mouseMotion (draws blue stamps when left mouse button is held)
  glutMouseFunc(mouseButton); //calls back to mouseButton (draws yellow stamps when mouse is clicked and unclicked
  glutKeyboardFunc(keyboard); //calls back to keyboard (closes on esc and q, writes on w
  glutIdleFunc(idle); //calls to idle to periodically redisplay

  writePPM("output.ppm"); //writes window to output.ppl
  glutMainLoop(); //main loop

  return 0;
}
