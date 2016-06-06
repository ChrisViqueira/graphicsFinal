#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include "LoadShaders.h"
#include "vec_mat.h"
#include "cube.h"
#include "Models.h"
#include "SphericalCamera.h"
#include "MaterialLight.h"
#include "Image_File_IO.h"

#define PI 3.14159265358979323846
#define PI_DIV_180 0.017453292519943296
#define deg PI_DIV_180

#define BUFFER_OFFSET(x) ((const void*) (x))

GLint vPosition = 0;
GLint vColor = 1;
GLint vNormal = 2;
GLint vTex = 3;

clock_t timer;
clock_t startTime;
clock_t lastTime;

GLenum  mode = GL_FILL;
//for textures
GLuint program1;
GLuint program2;
GLuint CMprogram;
GLuint texID[9];
GLuint texSkyID;

GLboolean backcolorblack = GL_TRUE;

/*  Set mouse tracking.  */
int mouseDown = 0;
int startX = 0;
int startY = 0;

GLint winWidth;
GLint winHeight;

mat4 projection;
mat4 view;
vec4 GlobalAmbient;

SphericalCamera sphcamera;
GLint CameraNumber = 1;

cube cubeObj;

Light Lts[10];
mat4 modelMatrices[11];
Material Mats[10];
Material robotMat;
Models obj[11];
Models lightobj;

Models skySphere;

GLint tex1_uniform_loc1;
GLint tex1_uniform_loc2;
GLint tex2_uniform_loc2;

GLubyte texture2[64][64][3];
GLint texture2Width;
GLint texture2Height;

GLuint cubeMapId;

int numObjects = 1;
int numLights = 1;
int robot_action = 0;
GLfloat randAng[2];
int standing = 1;
int onGround = 0;

////used for basic collision
vec3 wallCollide[10];
vec3 wallSize[10];
int wallNum;
int horizonalWall;
double bounce = 0;


int ePressed = 0;
int dPressed = 0;
int sPressed = 0;
int fPressed = 0;
int zPressed = 0;
int levitate = 0;

//gravity
GLfloat dt = 0;
GLfloat grav = 0;
GLfloat vel = 0;
GLfloat pos = 0;

//wall bounces
GLfloat bouncefact = -.02;

void printOpenGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        std::cout << "OpenGL Error: " << errString << std::endl;
    }
}

void LoadLights(GLuint program, Light Lt[], int num)
{
    using namespace std;
    string arrayname = "Lt";  // array name in the shader.

    for (int i = 0; i < num; i++)
    {
        char n[10];
        itoa(i, n, 10);
        string arraypos = arrayname;
        arraypos += "[";
        arraypos += n;
        arraypos += "]";

        string locID = arraypos;
        locID.append(".on");
        glUniform1i(glGetUniformLocation(program, locID.c_str()), Lt[i].on);

        locID = arraypos;
        locID.append(".position");
        glUniform4fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].position);

        locID = arraypos;
        locID.append(".ambient");
        glUniform4fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].ambient);

        locID = arraypos;
        locID.append(".diffuse");
        glUniform4fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].diffuse);

        locID = arraypos;
        locID.append(".specular");
        glUniform4fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].specular);

        locID = arraypos;
        locID.append(".spotDirection");
        glUniform3fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].spotDirection);

        locID = arraypos;
        locID.append(".attenuation");
        glUniform3fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].attenuation);

        locID = arraypos;
        locID.append(".spotCutoff");
        glUniform1f(glGetUniformLocation(program, locID.c_str()), Lt[i].spotCutoff);

        locID = arraypos;
        locID.append(".spotExponent");
        glUniform1f(glGetUniformLocation(program, locID.c_str()), Lt[i].spotExponent);
    }
}

void LoadLight(GLuint program, Light Lt)
{
    glUniform1i(glGetUniformLocation(program, "Lt.on"), Lt.on);
    glUniform4fv(glGetUniformLocation(program, "Lt.position"), 1, Lt.position);
    glUniform4fv(glGetUniformLocation(program, "Lt.ambient"), 1, Lt.ambient);
    glUniform4fv(glGetUniformLocation(program, "Lt.diffuse"), 1, Lt.diffuse);
    glUniform4fv(glGetUniformLocation(program, "Lt.specular"), 1, Lt.specular);
    glUniform3fv(glGetUniformLocation(program, "Lt.spotDirection"), 1, Lt.spotDirection);
    glUniform3fv(glGetUniformLocation(program, "Lt.attenuation"), 1, Lt.attenuation);
    glUniform1f(glGetUniformLocation(program, "Lt.spotCutoff"), Lt.spotCutoff);
    glUniform1f(glGetUniformLocation(program, "Lt.spotExponent"), Lt.spotExponent);
}

void TurnLightOn(GLuint program, Light Lt)
{
    Lt.on = true;
    glUniform1i(glGetUniformLocation(program, "Lt.on"), Lt.on);
}

void TurnLightOff(GLuint program, Light Lt)
{
    Lt.on = false;
    glUniform1i(glGetUniformLocation(program, "Lt.on"), Lt.on);
}

void TurnLightsOn(GLuint program, Light Lt[], int num)
{
    using namespace std;
    string arrayname = "Lt";  // array name in the shader.

    for (int i = 0; i < num; i++)
    {
        char n[10];
        itoa(i, n, 10);
        string arraypos = arrayname;
        arraypos += "[";
        arraypos += n;
        arraypos += "]";

        Lt[i].on = true;
        glUniform1i(glGetUniformLocation(program, arraypos.append(".on").c_str()), Lt[i].on);
    }
}

void TurnLightsOff(GLuint program, Light Lt[], int num)
{
    using namespace std;
    string arrayname = "Lt";  // array name in the shader.

    for (int i = 0; i < num; i++)
    {
        char n[10];
        itoa(i, n, 10);
        string arraypos = arrayname;
        arraypos += "[";
        arraypos += n;
        arraypos += "]";

        Lt[i].on = false;
        glUniform1i(glGetUniformLocation(program, arraypos.append(".on").c_str()), Lt[i].on);
    }
}

void TurnOnALight(GLuint program, Light Lt[], int num)
{
    using namespace std;
    string arrayname = "Lt";  // array name in the shader.

    char n[10];
    itoa(num, n, 10);
    string arraypos = arrayname;
    arraypos += "[";
    arraypos += n;
    arraypos += "]";

    Lt[num].on = true;
    glUniform1i(glGetUniformLocation(program, arraypos.append(".on").c_str()), Lt[num].on);
}

void TurnOffALight(GLuint program, Light Lt[], int num)
{
    using namespace std;
    string arrayname = "Lt";  // array name in the shader.

    char n[10];
    itoa(num, n, 10);
    string arraypos = arrayname;
    arraypos += "[";
    arraypos += n;
    arraypos += "]";

    Lt[num].on = false;
    glUniform1i(glGetUniformLocation(program, arraypos.append(".on").c_str()), Lt[num].on);
}

void LoadMaterial(GLuint program, Material Mat)
{
    glUniform4fv(glGetUniformLocation(program, "Mat.ambient"), 1, Mat.ambient);
    glUniform4fv(glGetUniformLocation(program, "Mat.diffuse"), 1, Mat.diffuse);
    glUniform4fv(glGetUniformLocation(program, "Mat.specular"), 1, Mat.specular);
    glUniform4fv(glGetUniformLocation(program, "Mat.emission"), 1, Mat.emission);
    glUniform1f(glGetUniformLocation(program, "Mat.shininess"), Mat.shininess);
}

GLuint loadTextureBMP(const char* filename)
{
    GLint imgwidth;
    GLint imgheight;
    GLuint texID;

    GLbyte* image1 = LoadBMP(filename, &imgwidth, &imgheight);
    if (!image1) return 0;

    glGenTextures(1, &texID);
    if (texID == 0) return 0;

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgwidth, imgheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    free(image1);
    return texID;
}

GLuint load_texture_cube_map()
{
    GLbyte* texture = NULL;
    GLint textureWidth;
    GLint textureHeight;

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    GLuint texId;
    glActiveTexture(GL_TEXTURE0+10);

    /* Generate a new cube map texture and bind to it */
    glGenTextures (1, &texId);
    glBindTexture (GL_TEXTURE_CUBE_MAP, texId);

    /* Setup some parameters for texture filters and mipmapping */
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    texture = LoadBMP("sky_posx.bmp", &textureWidth, &textureHeight);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    free(texture);
    texture = LoadBMP("sky_negx.bmp", &textureWidth, &textureHeight);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    free(texture);
    texture = LoadBMP("sky_posy.bmp", &textureWidth, &textureHeight);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    free(texture);
    texture = LoadBMP("sky_negy.bmp", &textureWidth, &textureHeight);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    free(texture);
    texture = LoadBMP("sky_posz.bmp", &textureWidth, &textureHeight);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    free(texture);
    texture = LoadBMP("sky_negz.bmp", &textureWidth, &textureHeight);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    free(texture);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    return texId;
}

//Initialize the robot in its default position
//also load the cube data to the buffer
void createRobot(){

    cubeObj.createRobot();
    cubeObj.reloadData();

    robotMat = redPlastic;
}

//Creates the world layout
void createWorld(){
    numLights = 1;
    numObjects = 5;

    //Create the walls and load the data
    obj[0].createTessellatedWallOBJ(2, 2, 1, 1);
    obj[0].reloadData();

    /////////////////////////////// CREATING THE ENVIRONMENT //////////////////////////////////

    //Create the sky sphere and load the data
    skySphere.createSphereOBJ(100, 100, 100);
    skySphere.load(vPosition, vColor, vNormal, vTex);

    ////////////////////////////// GRASSY GROUND///////////////////////////////////////
    wallCollide[0] = vec3(0, -7, 0);
    wallSize[0] = vec3(100, 100, 100);
    modelMatrices[0] =  Translate(wallCollide[0])*Rotate(-90, 1, 0, 0)*Scale(wallSize[0]);

    ////////////////////////////// HOUSE /////////////////////////////////////////////////////

    ///////////////////////////////////// [1] FLOOR ///////////////////////////////////
    wallCollide[1] = vec3(25, -6.5, 25);
    wallSize[1] = vec3(30, 30, 30);
    modelMatrices[1] =  Translate(wallCollide[1])*Rotate(-90, 1, 0, 0)*Scale(wallSize[1]);

    //////////////////////////////////// [2] CEILING ////////////////////////////////////
    wallCollide[2] = vec3(25, 10, 25);
    wallSize[2] = vec3(30, 30, 30);
    modelMatrices[2] =  Translate(wallCollide[2])*Rotate(-90, 1, 0, 0)*Scale(wallSize[2]);

    /////////////////////////////////// [3] WALL pos z ////////////////////////////////////////
    wallCollide[3] = vec3(wallCollide[1].x, 0, wallCollide[1].z + wallSize[1].z);
    wallSize[3] = vec3(30, 10, 30);
    modelMatrices[3] =  Translate(wallCollide[3])*Scale(wallSize[3]);

    /////////////////////////////////// [4] WALL neg z ////////////////////////////////////////
    wallCollide[4] = vec3(wallCollide[1].x, 0, wallCollide[1].z - wallSize[1].z);
    wallSize[4] = vec3(30, 10, 30);
    modelMatrices[4] =  Translate(wallCollide[4])*Scale(wallSize[4]);

    ////////////////////////////////// LIGHTS //////////////////////////////////////////
    for (int i = 0; i < numLights; i++)
        Lts[i] = defaultLight;

    Lts[0].position = vec4(-31, 110, 39, 1);

    float fact = 2;
    for (int i = 0; i < numLights; i++)
    {
        Lts[i].diffuse = fact*Lts[i].diffuse;
        Lts[i].specular = fact*Lts[i].specular;
    }

    LoadLights(program1 ,Lts, numLights);
    glUniform1i(glGetUniformLocation(program1, "numLights"), numLights);

}

// Very Simple gravity
// Only works for the Y direction, no forward velocity
// Gives the robot a "floaty" feel
void gravity(double now, double prev){
    dt = (GLfloat)now - (GLfloat)prev; //Calculates the time passed since last frame updated
    grav = -.49; //gravity constant
    vel += grav * dt; //Calculate velocity
    pos += vel * dt; //New position of the robot based on velocity and time passed
    cubeObj.updateRobotTransformations(1, TORSO, 0, 0, 0, 0, pos);
}

// Calculates the Y bounds of the house and ground
int WallBoundsY(){
    GLfloat posx, negx;
    GLfloat y;
    GLfloat posz, negz;
    int retVal = 0;

    for(int objNum = 2; objNum >= 0; objNum--){
        // Gets the value of the outside edges of the house and ground
        posx = wallCollide[objNum].x + wallSize[objNum].x;
        negx = wallCollide[objNum].x - wallSize[objNum].x;
        y = wallCollide[objNum].y - wallCollide[0].y; //ground is lowest point
        posz = wallCollide[objNum].z + wallSize[objNum].z;
        negz = wallCollide[objNum].z - wallSize[objNum].z;

        //check to see if the robot is inside the house or not, and if it is on the floor or the ceiling
        if(cubeObj.getTranslation(TORSO).x < posx && cubeObj.getTranslation(TORSO).x > negx){
            if(cubeObj.getTranslation(TORSO).z < posz && cubeObj.getTranslation(TORSO).z > negz){
                if(cubeObj.getCollisionFeet() - wallCollide[0].y >= y - 1){ // - 1 is round off error / re animating too quickly for ceilings
                    retVal = objNum;
                    break;
                }
            }
        }
    }

    return retVal; //not in any bounds of any wall

}

int WallBoundsX(){
    GLfloat posx, negx;
    GLfloat y;
    GLfloat posz, negz;

    for (int i = 3; i < numObjects; i++){
        posx = wallCollide[i].x + wallSize[i].x;
        negx = wallCollide[i].x - wallSize[i].x;
        y = wallCollide[2].y; //ground is lowest point

        if(cubeObj.getTranslation(TORSO).x < posx && cubeObj.getTranslation(TORSO).x > negx){
            if(cubeObj.getTranslation(TORSO).y < y){
                if(cubeObj.getTranslation(TORSO).z - wallCollide[i].z < 1 && cubeObj.getTranslation(TORSO).z - wallCollide[i].z > -1){
                    return 1;
                }
            }
        }
    }

    return 0;

}

int sphereBounds(){
    if(cubeObj.getTranslation(TORSO).x > 100 || cubeObj.getTranslation(TORSO).x < -100){
        cubeObj.setMiddle();
    }
    if(cubeObj.getTranslation(TORSO).y > 100 || cubeObj.getTranslation(TORSO).y < -100){
        cubeObj.setMiddle();
    }
    if(cubeObj.getTranslation(TORSO).z > 100 || cubeObj.getTranslation(TORSO).z < -100){
        cubeObj.setMiddle();
    }
}

void checkDownwardsCollision(){
    double roundoff = wallCollide[wallNum].y - cubeObj.getCollisionFeet();
    cubeObj.updateRobotTransformations(1, TORSO, 0, 0, 0, 0, roundoff);

    vel = 0;
    pos = 0;
}

void checkWallCollision(double now, double prev){
    cubeObj.updateRobotTransformations(1, TORSO, 0, 0, 0, bouncefact, 0);
    bounce--;
    bouncefact *= -(bouncefact);
}

void init()
{

    for (int i = 0; i < 10; i++)
    {
        modelMatrices[i] = Identity();
        Lts[i] = defaultLight;
        Lts[i].position = vec4(2, 3, 4, 1);
    }

    obj[10].createSphereOBJ(3, 10, 10);
    obj[10].reloadData();
    modelMatrices[10] = Identity();


    for (int i = 0; i < 64; i++)
        for (int j = 0; j < 64; j++)
        {
            int c = (((i&0x8) == 0)^((j&0x8) == 0))*255;
            texture2[i][j][0] = (GLubyte)c;
            texture2[i][j][1] = (GLubyte)c;
            texture2[i][j][2] = (GLubyte)c;
        }

    texture2Width = 64;
    texture2Height = 64;

    cubeObj.load(vPosition, vColor, vNormal);

    sphcamera.setPosition(7.4, 70, 20);

    glEnable(GL_DEPTH_TEST);

    lightobj.createSphereOBJ(3, 30, 30);
    lightobj.load(vPosition, vColor, vNormal, vTex);
    lightobj.setColor(1, 1, 0);
    lightobj.setDrawBorder(GL_TRUE);

//  Load the shaders

    ShaderInfo shadersCM[] =
    {
        {GL_VERTEX_SHADER, "vertexShaderCM.vert"},
        {GL_FRAGMENT_SHADER, "TextOnlyCM.frag"},
        {GL_NONE, NULL}
    };

    CMprogram = LoadShaders(shadersCM);

    ShaderInfo shaders1[] =
    {
        {GL_VERTEX_SHADER, "vertexShader.vert"},
        {GL_FRAGMENT_SHADER, "PhongOneTexture.frag"},
        {GL_NONE, NULL}
    };

    program1 = LoadShaders(shaders1);
    glUseProgram(program1);
    tex1_uniform_loc1 = glGetUniformLocation(program1, "tex1");
    glUniform4fv(glGetUniformLocation(program1, "GlobalAmbient"), 1, GlobalAmbient);

    createRobot();
    createWorld();

    glActiveTexture(GL_TEXTURE0);
    texID[0] = loadTextureBMP("grass.bmp");
    if (texID[0] == 0) std::cout << "Texture Load Error!" << "\n";

    glActiveTexture(GL_TEXTURE0+1);
    texID[1] = loadTextureBMP("woodfloor.bmp");
    if (texID[1] == 0) std::cout << "Texture Load Error!" << "\n";

    glActiveTexture(GL_TEXTURE0+2);
    texID[2] = loadTextureBMP("Brick001.bmp");
    if (texID[2] == 0) std::cout << "Texture Load Error!" << "\n";

    cubeMapId = load_texture_cube_map();

    startTime = clock();
    lastTime = startTime;
}

void reshape(GLint width, GLint height)
{
    winWidth = width;
    winHeight = height;

    glViewport(0, 0, width, height);
    projection = Perspective(60.0f, (float)width/(float)height, 0.0001f, 100.0f);
    glutPostRedisplay();
}

void SpKeyFcn(int key, int x, int y)
{
    GLint ctrlDown = glutGetModifiers() & GLUT_ACTIVE_CTRL;
    GLint altDown = glutGetModifiers() & GLUT_ACTIVE_ALT;
    GLint shiftDown = glutGetModifiers() & GLUT_ACTIVE_SHIFT;

        if(ctrlDown){
            switch(key){
            }
        }
        else if (altDown){
                switch(key){
                }
        }
        else if(shiftDown){
                switch(key){
                }
        }

    switch (key)
    {
        if (CameraNumber == 1)
        {
            if (ctrlDown)
            {
                switch (key)
                {
                case GLUT_KEY_DOWN:
                    sphcamera.addR(-0.2);
                    break;

                case GLUT_KEY_UP:
                    sphcamera.addR(0.2);
                    break;
                }
            }
            else
            {
                switch (key)
                {
                case GLUT_KEY_UP:
                    sphcamera.addPsy(-2);
                    break;

                case GLUT_KEY_DOWN:
                    sphcamera.addPsy(2);
                    break;

                case GLUT_KEY_LEFT:
                    sphcamera.addTheta(-2);
                    break;

                case GLUT_KEY_RIGHT:
                    sphcamera.addTheta(2);
                    break;
                }
            }
        }
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key){
        case 033:  // ASCII Escape Key Code
            exit(EXIT_SUCCESS);
            break;

        case 'm':
            if (mode == GL_FILL)
                mode = GL_LINE;
            else
                mode = GL_FILL;

            glPolygonMode(GL_FRONT_AND_BACK, mode);
            break;

        case 'c':
            if (backcolorblack)
            {
                glClearColor(1, 1, 1, 1);
                backcolorblack = GL_FALSE;
            }
            else
            {
                glClearColor(0, 0, 0, 1);
                backcolorblack = GL_TRUE;
            }
            break;

        case 'r':
            robot_action = 0;
            standing = cubeObj.set_to_standing();
            break;

        case 'e':
            ePressed = 1;
            break;
        case 's':
            sPressed = 1;
            break;
        case 'f':
            fPressed = 1;
            break;
        case 'z':
            zPressed = 1;
            break;
        case ' ':
            levitate = 1;
            break;
    }

    glutPostRedisplay();
}

void specialKeyboard(unsigned char key, int x, int y){
    switch(key)
    {
        case 'e':
            ePressed = 0;
            break;
        case 's':
            sPressed = 0;
            break;
        case 'f':
            fPressed = 0;
            break;
        case 'z':
            zPressed = 0;
            break;
        case ' ':
            levitate = 0;
            break;
    }
}

void detectUpDown(int button, int state, int x, int y)
{
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
    {
        mouseDown = 1;
        startX = x;
        startY = y;
    }
    else
        mouseDown = 0;
}

void detectMove(int x, int y)
{
    GLint ctrlDown = glutGetModifiers() & GLUT_ACTIVE_CTRL;
    GLint altDown = glutGetModifiers() & GLUT_ACTIVE_ALT;
    GLint shiftDown = glutGetModifiers() & GLUT_ACTIVE_SHIFT;

    if (mouseDown == 1)
    {
        if (ctrlDown)
        {
            sphcamera.addR((startY-y)/20.0);
            startY = y;
        }
        else
        {
            sphcamera.addTheta((x-startX)/2.0);
            startX = x;

            sphcamera.addPsy((y-startY)/2.0);
            startY = y;
        }

        glutPostRedisplay();
    }
}

void idleAnimation(){

    double PrevSec = (double)(timer - startTime)/CLOCKS_PER_SEC;
    timer = clock();
    double NowSec = (double)(timer - startTime)/CLOCKS_PER_SEC;
    double elap = (double)(timer - lastTime)/CLOCKS_PER_SEC;
    wallNum = WallBoundsY();
    horizonalWall = WallBoundsX();

    if (elap > 1.0/100.0)
    {
        lastTime = timer;
        sphereBounds();
        if(standing == 0){
            standing = cubeObj.set_to_standing();
        }

        if(robot_action == 1){
        }
        else{
            if(horizonalWall){
                bounce = 15;
                bouncefact = -.9;
            }
            if (bounce != 0){
                checkWallCollision(NowSec, PrevSec);
            }
            if(zPressed){
                if (ePressed){
                    cubeObj.running_animation(10);
                }
                if(sPressed){
                    cubeObj.updateRobotTransformations(1, TORSO, 0, 3, 0);
                }
                if(fPressed){
                    cubeObj.updateRobotTransformations(0, TORSO, 0, 3, 0);
                }
            }
            else{
                if (ePressed){
                    cubeObj.walking_animation(3, .1);
                }
                if(sPressed){
                    cubeObj.updateRobotTransformations(1, TORSO, 0, 4, 0);
                }
                if(fPressed){
                    cubeObj.updateRobotTransformations(0, TORSO, 0, 4, 0);
                }
            }
            if(levitate){
                cubeObj.levitate(1);
                vel = 0;
                pos = 0;
            }
            else{
                cubeObj.levitate(0);
                if(cubeObj.getCollisionFeet() > wallCollide[wallNum].y) {
                    gravity(NowSec, PrevSec);
                }
                else{
                    checkDownwardsCollision();
                }
            }
            if(!ePressed && !sPressed && !fPressed){
                standing = cubeObj.set_to_standing();

            }
        }
    }
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat3 normal;

    if (CameraNumber == 1){
        view = sphcamera.lookAt(cubeObj.getTranslation(TORSO));
    }

    glUseProgram(CMprogram);
    glUniformMatrix4fv(glGetUniformLocation(CMprogram, "ProjModelView"),
                       1, GL_TRUE, projection*view*Rotate(90, 1, 0, 0));
    glUniformMatrix4fv(glGetUniformLocation(CMprogram, "TCRotate"),
                       1, GL_TRUE, Rotate(-90, 1, 0, 0));
    glActiveTexture(GL_TEXTURE0+10);
    glUniform1i(glGetUniformLocation(CMprogram, "tex1"), 10);
    skySphere.draw();

    glUseProgram(program1);
    TurnLightsOff(program1, Lts, 10);
    TurnLightsOn(program1, Lts, 10);

    if (CameraNumber == 1)
        glUniform3fv(glGetUniformLocation(program1, "eye"), 1, sphcamera.getPosition(cubeObj.getTranslation(TORSO)));

//////////////////////////////////////////////////////////////////////////////////////////

    for(int i = 0; i < 10; i++){
        glUniform1i(glGetUniformLocation(program1, "useTexture"), 0);
        LoadMaterial(program1, robotMat);
        glUniformMatrix4fv(glGetUniformLocation(program1, "ProjModelView"),
                               1, GL_TRUE, projection*view*cubeObj.getRobotTrans(i));
        glUniformMatrix4fv(glGetUniformLocation(program1, "ModelView"),
                           1, GL_TRUE, cubeObj.getRobotTrans(i));
        mat3 normal = Adjoint(cubeObj.getRobotTrans(i));
        glUniformMatrix3fv(glGetUniformLocation(program1, "NormalMatrix"),
                       1, GL_FALSE, normal);
        cubeObj.draw();
    }

    int j = 0;
    for(int i = 0; i < numObjects; i++){
        glUniform1i(glGetUniformLocation(program1, "useTexture"), 1);
        LoadMaterial(program1, Mats[i]);
        glUniformMatrix4fv(glGetUniformLocation(program1, "ProjModelView"),
                           1, GL_TRUE, projection*view*modelMatrices[i]);
        glUniformMatrix4fv(glGetUniformLocation(program1, "ModelView"),
                           1, GL_TRUE, modelMatrices[i]);
        normal = Adjoint(modelMatrices[i]);
        glUniformMatrix3fv(glGetUniformLocation(program1, "NormalMatrix"),
                           1, GL_FALSE, normal);
        glUniform1i(glGetUniformLocation(program1, "useTexture"), 1);

        glActiveTexture(GL_TEXTURE0+j);
        glUniform1i(tex1_uniform_loc1, j);
        glUniformMatrix2fv(glGetUniformLocation(program1, "texTrans"),
                       1, GL_TRUE, Scale(10, 10));

        obj[0].draw();
        if(j < 2)
            j++;
    }

////////////////////////////////////////////////////////////////////////////////////////////

    glUseProgram(program1);
    glUniform1i(glGetUniformLocation(program1, "useTexture"), 0);
    TurnLightsOff(program1, Lts, 10);
    for (int i = 0; i < numLights; i++)
    {
        glUniformMatrix4fv(glGetUniformLocation(program1, "ProjModelView"),
                           1, GL_TRUE, projection*view*Translate(Lts[0].position));
        lightobj.draw();
    }
    TurnLightsOn(program1, Lts, 10);

    glutSwapBuffers();
    printOpenGLErrors();
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    std::cout << "The compilation began at: " << __TIME__ << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow(argv[0]);

    if (glewInit())
    {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit(EXIT_FAILURE);
    }

    init();
    glutSetWindowTitle("MR. ROBOTO");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutSpecialFunc(SpKeyFcn);
    glutMouseFunc(detectUpDown);
    glutMotionFunc(detectMove);
    glutIdleFunc(idleAnimation);
    glutIgnoreKeyRepeat(1);
    glutKeyboardUpFunc(specialKeyboard);

    glutMainLoop();
}
