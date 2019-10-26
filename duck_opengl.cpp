#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods );
void DrawCube( GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLengthX, GLfloat edgeLengthY, GLfloat edgeLengthZ );

// INITIAL ROTATION FACTOR

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;

// INITAL SCALING FACTOR

GLfloat scalingX = 1.0f;
GLfloat scalingY = 1.0f;

// INITIAL TRANSLATION FACTOR

GLfloat translationX = 0.0f;
GLfloat translationY = 0.0f;

int main( void )
{
    GLFWwindow *window;

    // Initialize the library
    if ( !glfwInit( ) )
    {
        return -1;
    }

    // Create a window
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Duck GL", NULL, NULL );

    glfwSetKeyCallback( window, keyCallback );
    glfwSetInputMode( window, GLFW_STICKY_KEYS, 1 );


    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );

    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent( window );

    glViewport( 0.0f, 0.0f, screenWidth, screenHeight ); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode( GL_PROJECTION ); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity( ); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1000 ); // essentially set coordinate system
    glMatrixMode( GL_MODELVIEW ); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity( ); // same as above comment

    GLfloat halfScreenWidth = SCREEN_WIDTH / 2;
    GLfloat halfScreenHeight = SCREEN_HEIGHT / 2;


    // Loop until the user closes the window
    while ( !glfwWindowShouldClose( window ) )
    {
        glClear( GL_COLOR_BUFFER_BIT );

        // Render OpenGL

        glPushMatrix( );
        glTranslatef( halfScreenWidth, halfScreenHeight, -500 );
        glRotatef( rotationX, 1, 0, 0 );
        glRotatef( rotationY, 0, 1, 0 );
        glTranslatef( -halfScreenWidth, -halfScreenHeight, 500 );

        //TORSO
        DrawCube( halfScreenWidth, halfScreenHeight, -500, 150, 100, 100 );

        //PICO
        DrawCube( halfScreenWidth+85, halfScreenHeight+80, -500, 40, 15, 50 );

        //ALAS
        DrawCube( halfScreenWidth+10, halfScreenHeight, -443, 100, 50, 14 );
        DrawCube( halfScreenWidth+10, halfScreenHeight, -557, 100, 50, 14 );

        //PATAS
        DrawCube( halfScreenWidth+10, halfScreenHeight-60, -480, 10, 20, 10 );
        DrawCube( halfScreenWidth+15, halfScreenHeight-75, -475, 40, 10, 40 );

        DrawCube( halfScreenWidth+10, halfScreenHeight-60, -520, 10, 20, 10 );
        DrawCube( halfScreenWidth+15, halfScreenHeight-75, -525, 40, 10, 40 );

        //CABEZA
        DrawCube( halfScreenWidth+15, halfScreenHeight+94, -500, 100, 60, 80 );
        DrawCube( halfScreenWidth+15, halfScreenHeight+57, -500, 100, 14, 80 );

        //OJOS
        DrawCube( halfScreenWidth+35, halfScreenHeight+94, -542, 30, 30, 4 );
        DrawCube( halfScreenWidth+35, halfScreenHeight+94, -458, 30, 30, 4 );
        DrawCube( halfScreenWidth+35, halfScreenHeight+94, -542, 10, 10, 4 );
        DrawCube( halfScreenWidth+35, halfScreenHeight+94, -458, 10, 10, 4 );


        glPopMatrix();


        // Swap front and back buffers
        glfwSwapBuffers( window );

        // Poll for and process events
        glfwPollEvents( );
    }

    glfwTerminate( );

    return 0;
}


// TRANSFORMATIONS

int transform = 1;  //type of transform

// 1. R = ROTATION

GLfloat rx = 0.000f;
GLfloat ry = 0.000f;

// 2. T = TRANSLATION

GLfloat tx = 0.0f;
GLfloat ty = 0.0f;
GLfloat tz = 0.0f;

// 3. S = SCALE

GLfloat sx = 1.0f;
GLfloat sy = 1.0f;
GLfloat sz = 1.0f;



// FUNCTION TO APPLY TRANSFORMATIONS WITH KEYS

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods )
{

    const GLfloat dr = 10;
    const GLfloat dt = 10;
    const GLfloat ds = 1.1;

    if ( action == GLFW_PRESS || action == GLFW_REPEAT )
    {
        switch ( key )
        {
            case GLFW_KEY_R:
                transform = 1;
                std::cout << "Rotation mode" << std::endl;
                break;
            case GLFW_KEY_T:
                transform = 2;
                std::cout << "Translation mode" << std::endl;
                break;
            case GLFW_KEY_S:
                transform = 3;
                std::cout << "Scaling mode" << std::endl;
                break;

            case GLFW_KEY_UP:
                switch ( transform ) {
                    case 1:
                        rx -= dr;
                        break;
                    case 2:
                        ty += dt;
                        break;
                    case 3:
                        sx *= 1/ds;
                        break;
                }
                break;

            case GLFW_KEY_DOWN:
                switch ( transform ) {
                    case 1:
                        rx += dr;
                        break;
                    case 2:
                        ty -= dt;
                        break;
                    case 3:
                        sx *= ds;
                        break;
                }
                break;

            case GLFW_KEY_RIGHT:
                switch ( transform ) {
                    case 1:
                        ry += dr;
                        break;
                    case 2:
                        tx += dt;
                        break;
                    case 3:
                        sy *= ds;
                        break;
                }
                break;

            case GLFW_KEY_LEFT:
                switch ( transform ) {
                    case 1:
                        ry -= dr;
                        break;
                    case 2:
                        tx -= dt;
                        break;
                    case 3:
                        sy *= 1/ds;
                        break;
                }
                break;
        }
    }
}

// FUNCTION TO DRAW THE CUBES
// -- Takes legth on each axis and the coordinates for the center position

void DrawCube( GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLengthX, GLfloat edgeLengthY, GLfloat edgeLengthZ)
{
    GLfloat halfSideLengthX = edgeLengthX * 0.5f;
    GLfloat halfSideLengthY = edgeLengthY * 0.5f;
    GLfloat halfSideLengthZ = edgeLengthZ * 0.5f;

    GLfloat vertices[] =
    {
        // front face
        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top right
        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom right
        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom left

        // back face
        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top left
        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom left

        // left face
        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom left

        // right face
        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom left

        // top face
        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom left

        // top face
        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ  // bottom left
    };

    // APPLIES TRANSFORMATION OF ROTATION

    rotationX = rx;
    rotationY = ry;

    // APPLIES TRANSFORMATIONS OF SCALING AND TRANSLATION

    int counter = 0;
    for (int i=0; i<72; i++) {
      if (counter == 0) {
        vertices[i] *= sx;
        vertices[i] += tx;
      }
      else if (counter == 1) {
        vertices[i] *= sy;
        vertices[i] += ty;
      }
      if (counter == 2) {
        vertices[i] *= sz;
        vertices[i] += tz;
      }
      counter++;
      counter %= 3;
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, vertices );

    glDrawArrays( GL_QUADS, 0, 24 );

    glDisableClientState( GL_VERTEX_ARRAY );
}
