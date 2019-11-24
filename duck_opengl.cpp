#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include "SOIL2/SOIL2.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define checkImageWidth 64
#define checkImageHeight 64

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n"
"ourColor = aColor;\n"
"TexCoord = aTexCoord;\n"
"}";

const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"FragColor = texture(ourTexture, TexCoord);\n"
"}";

GLuint shaderProgram;

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
int width, height,colours;

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods );
void DrawCube( GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLengthX, GLfloat edgeLengthY, GLfloat edgeLengthZ );

// INITIAL ROTATION FACTOR

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat rotationZ = 0.0f;

// INITAL SCALING FACTOR

GLfloat scalingX = 1.0f;
GLfloat scalingY = 1.0f;

// INITIAL TRANSLATION FACTOR

GLfloat translationX = 0.0f;
GLfloat translationY = 0.0f;

// MAKE IMAGE TO TEXTURE BODY
void makeCheckImage(void)
{
    int i, j, c;
    char keyword[2];
    std::string comment;
    
    //Reading texture file
    std::ifstream inFile;
    
    inFile.open("/Users/anamarcelav/Documents/ITESM/Gráficas/Window_OpenGL/Raul.ppm");
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    
    inFile >> keyword;
    std::cout << keyword << " ";
    inFile >> comment;
    std::cout << comment << " ";
    inFile >> comment;
    std::cout << comment << " ";
    inFile >> comment;
    std::cout << comment << " ";
    inFile >> comment;
    std::cout << comment << " ";
    
    inFile >> width;
    std::cout << width;
    inFile >> height;
    std::cout << height << "\n";
    inFile >> colours;
    std::cout << colours;
    std::cout << std::endl;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            inFile >> c;
            checkImage[height-i][j][0] = (GLubyte) c;
            //cout << c << " ";
            inFile >> c;
            checkImage[height-i][j][1] = (GLubyte) c;
            //cout << c << " ";
            inFile >> c;
            checkImage[height-i][j][2] = (GLubyte) c;
            //cout << c << " ";
            checkImage[height-i][j][3] = (GLubyte) 255;
            //cout << "255n";
        }
        //cout << endl;
    }
    inFile.close();
}

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
    
    // --------- shaders-----------
    //    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //CREATE SHADER RETURNS 0 AND CREATES A BAD ACCESS.
    //    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    //    glCompileShader(vertexShader);
    //
    //    GLint success;
    //    GLchar infoLog[512];
    //
    //    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //
    //    if (!success) {
    //        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //        std::cout << "error in vertex shaderrrr \n" << infoLog << std::endl;
    //    }
    //
    //    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    //    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    //    glCompileShader(fragmentShader);
    //
    //    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //    if (!success) {
    //        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //        std::cout << "error in fragment shaderrrr \n" << infoLog << std::endl;
    //    }
    //    shaderProgram = glCreateProgram( );
    //    glAttachShader(shaderProgram, vertexShader);
    //    glAttachShader(shaderProgram, fragmentShader);
    //    glLinkProgram(shaderProgram);
    //
    //    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //    if (!success) {
    //        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //        std::cout << "error in shader program linking \n" << infoLog << std::endl;
    //    }
    //
    //    glDeleteShader(vertexShader);
    //    glDeleteShader(fragmentShader);
    
    // -------- TEXTURE ------------
    //    GLuint texture;
    //    glGenTextures(1, &texture);
    //    glBindTexture(GL_TEXTURE_2D, texture);
    
    // ---- SOIL para convertir la imagen jpg
    
    //    unsigned char* image = SOIL_load_image("brown-texture.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
    //                 height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
    //                 image);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    makeCheckImage();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                 checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 checkImage);
    
    
    
    // Loop until the user closes the window
    while ( !glfwWindowShouldClose( window ) )
    {
        glClear( GL_COLOR_BUFFER_BIT );
        glClear( GL_DEPTH_BUFFER_BIT );
        
        
        // Render OpenGL
        glPushMatrix( );
        glTranslatef( halfScreenWidth, halfScreenHeight, -500 );
        glRotatef( rotationX, 1, 0, 0 );
        glRotatef( rotationY, 0, 1, 0 );
        glRotatef( rotationZ, 0, 0, 1 );
        glTranslatef( -halfScreenWidth, -halfScreenHeight, 500 );
        glEnable(GL_TEXTURE_2D);
        
        glColor3f(0.41961,0.2784,0.2);
        //TORSO
        DrawCube( halfScreenWidth, halfScreenHeight, -500, 150, 100, 100 );
        glDisable(GL_TEXTURE_2D);
        
        glColor3f(1,0.709804,0.274501);
        //PICO
        DrawCube( halfScreenWidth+85, halfScreenHeight+80, -500, 40, 15, 50 );
        glColor3f(0.41961,0.2784,0.2);
        //ALAS
        DrawCube( halfScreenWidth+10, halfScreenHeight, -443, 100, 50, 14 );
        DrawCube( halfScreenWidth+10, halfScreenHeight, -557, 100, 50, 14 );
        glColor3f(1,0.4902,0.12157);
        //PATAS
        DrawCube( halfScreenWidth+10, halfScreenHeight-60, -480, 10, 20, 10 );
        DrawCube( halfScreenWidth+15, halfScreenHeight-75, -475, 40, 10, 40 );
        DrawCube( halfScreenWidth+10, halfScreenHeight-60, -520, 10, 20, 10 );
        DrawCube( halfScreenWidth+15, halfScreenHeight-75, -525, 40, 10, 40 );
        glColor3f(0.1765,0.494,0.30588);
        //CABEZA
        DrawCube( halfScreenWidth+15, halfScreenHeight+94, -500, 100, 60, 80 );
        glColor3f(1,1,1);
        DrawCube( halfScreenWidth+15, halfScreenHeight+57, -500, 100, 14, 80 );
        glColor3f(1,1,1);
        //OJOS
        DrawCube( halfScreenWidth+35, halfScreenHeight+94, -542, 30, 30, 4 );
        DrawCube( halfScreenWidth+35, halfScreenHeight+94, -458, 30, 30, 4 );
        glColor3f(0,0,0);
        DrawCube( halfScreenWidth+35, halfScreenHeight+94, -543, 10, 10, 4 );
        DrawCube( halfScreenWidth+35, halfScreenHeight+94, -457, 10, 10, 4 );
        
        
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
GLfloat rz = 0.000f;

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
                        sy *= ds;
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
                        sy *= 1/ds;
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
                        sx *= ds;
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
                        sx *= 1/ds;
                        break;
                }
                break;
                
            case GLFW_KEY_PERIOD:
                switch ( transform ) {
                    case 1:
                        rz += dr;
                        break;
                    case 2:
                        tz += dt;
                        break;
                    case 3:
                        sz *= 1/ds;
                        break;
                }
                break;
            case GLFW_KEY_SLASH:
                switch ( transform ) {
                    case 1:
                        rz -= dr;
                        break;
                    case 2:
                        tz -= dt;
                        break;
                    case 3:
                        sz *= ds;
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
        
        // bottom face
        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ  // bottom left
    };
    
    // APPLIES TRANSFORMATION OF ROTATION
    
    rotationX = rx;
    rotationY = ry;
    rotationZ = rz;
    
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
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, vertices );
    glDrawArrays( GL_QUADS, 0, 24 );
    glDisableClientState( GL_VERTEX_ARRAY );
}

// FUNCTION TO DRAW BODY WITH TEXTURE

//void DrawCubeWithTexture( GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLengthX, GLfloat edgeLengthY, GLfloat edgeLengthZ)
//{
//    GLfloat halfSideLengthX = edgeLengthX * 0.5f;
//    GLfloat halfSideLengthY = edgeLengthY * 0.5f;
//    GLfloat halfSideLengthZ = edgeLengthZ * 0.5f;
//
//    GLfloat vertices[] =
//    {
//        // front face
//        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 1.0f, // top left
//        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 1.0f,  // top right
//        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 0.0f,  // bottom right
//        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 0.0f, // bottom left
//
//        // back face
//        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 1.0f, // top left
//        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 1.0f, // top right
//        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 0.0f, // bottom right
//        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 0.0f, // bottom left
//
//        // left face
//        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 1.0f, // top left
//        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 1.0f, // top right
//        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 0.0f, // bottom right
//        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 0.0f, // bottom left
//
//        // right face
//        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 1.0f, // top left
//        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 1.0f, // top right
//        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 0.0f, // bottom right
//        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 0.0f, // bottom left
//
//        // top face
//        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 1.0f, // top left
//        centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 1.0f, // top right
//        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 0.0f, // bottom right
//        centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 0.0f, // bottom left
//
//        // bottom face
//        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 1.0f, // top left
//        centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 1.0f, // top right
//        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 1.0f, 0.0f, // bottom right
//        centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ,       /*colors*/  1.0f, 1.0f, 1.0f,     /*texture*/ 0.0f, 0.0f  // bottom left
//    };
//
//    GLuint indices[] = {
//        0, 1, 3,
//        1, 2, 3
//    };
//
//    GLuint VBO, VAO, EBO;
//    glGenVertexArrays(1,&VAO);
//    glGenBuffers(1,&VBO);
//    glGenBuffers(1,&EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//    // texture coord attribute
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);
//
//
//
//
//    GLuint texture;
//    int width, height;
//
//    glGenTextures( 1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    unsigned char* img = SOIL_load_image("brown-texture.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    if (img)
//    {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
//                     height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//                     img);
//        glGenerateMipmap(GL_TEXTURE_2D);
//        SOIL_free_image_data(img);
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }
//    else
//    {
//        std::cout << "Failed to load texture" << std::endl;
//    }
//
//    glUseProgram(shaderProgram);
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0 );
//
//
//    glBindVertexArray(VAO);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glBindVertexArray(0);
//
//    glDeleteVertexArrays(1,&VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//
//
//    // APPLIES TRANSFORMATION OF ROTATION
//
//    rotationX = rx;
//    rotationY = ry;
//    rotationZ = rz;
//
//    // APPLIES TRANSFORMATIONS OF SCALING AND TRANSLATION
//
//    int counter = 0;
//    for (int i=0; i<72; i++) {
//        if (counter == 0) {
//            vertices[i] *= sx;
//            vertices[i] += tx;
//        }
//        else if (counter == 1) {
//            vertices[i] *= sy;
//            vertices[i] += ty;
//        }
//        if (counter == 2) {
//            vertices[i] *= sz;
//            vertices[i] += tz;
//        }
//        counter++;
//        counter %= 3;
//    }
//
//    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
//    glEnableClientState( GL_VERTEX_ARRAY );
//    glVertexPointer( 3, GL_FLOAT, 0, vertices );
//    glDrawArrays( GL_QUADS, 0, 24 );
//    glDisableClientState( GL_VERTEX_ARRAY );
//}

