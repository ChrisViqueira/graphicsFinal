#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#include "vec_mat.h"
#include <stack>
#include <vector>

const int HEAD = 0;
const int RIGHT_THIGH = 1;
const int RIGHT_CALF = 2;
const int LEFT_THIGH = 3;
const int LEFT_CALF = 4;
const int RIGHT_BICEP = 5;
const int RIGHT_FOREARM = 6;
const int LEFT_BICEP = 7;
const int LEFT_FOREARM = 8;
const int TORSO = 9;

class cube
{
private:
    GLfloat red = 1;
    GLfloat green = 1;
    GLfloat blue = 1;
    GLfloat bred = 0;
    GLfloat bgreen = 0;
    GLfloat bblue = 0;

    GLuint vboptr;
    GLuint bufptr;
    GLuint vboptr2;
    GLuint bufptr2;

    GLint vPosition = 0;
    GLint vColor = 1;
    GLint vNormal = 2;

    GLboolean reload = GL_FALSE;
    GLboolean drawFaces = GL_TRUE;
    GLboolean drawBorder = GL_TRUE;
    GLboolean isColorCube = GL_TRUE;

    ///////////////////////added in variables/////////////////////////

    mat4 robotOriginal[10]; //original unchanging positions of the robot
    mat4 robotTrans[10];    //the matrices that will be transformed to animation robot
    struct transformations{ //to hold and access previous transformations
        vec3 Trans;
        vec3 Scale;
        vec3 Rot;
        GLfloat thetaX = 0.0; //x transformations
        GLfloat phiY = 0.0;  //y transformations
        GLfloat psyZ = 0.0;  //z transformations
    };

    transformations transStore[10];

    //////////////////////////////////////////////////////////////////

public:
    cube();
    cube(GLfloat r, GLfloat g, GLfloat b);
    ~cube();

    void setDrawBorder(GLboolean d);
    void setDrawFaces(GLboolean d);
    void setColorCube(GLboolean d);

    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void setBorderColor(GLfloat r, GLfloat g, GLfloat b);

    void load(GLint v, GLint c, GLint n);
    void reloadData();
    void draw();

    /////////////////////added in functions//////////////////////////

    void createRobot(); //create robot through a stack of transformations

    mat4 getRobotTrans(int counter);
    vec3 getTranslation(int counter);
    GLfloat getTorsoY();
    GLfloat getCollisionFeet();
    vec3 getCollisionBody();
    void setMiddle();

    void updateRobotTransformations(int up, int bodyPart, GLfloat Xangle = 0.0, GLfloat Yangle = 0.0, GLfloat Zangle = 0.0, double walkSpeed = 0, double jump = 0);
    void updateRobotTransformations(int up, int bodyPart, vec3 rotations = vec3(0, 0, 0), double walkSpeed = 0);

    int set_to_standing();

    void interpretive_dance(double speed, double Yoffset);
    void gentleman_bow(); //later not added in yet

    void walking_animation(double speed, double walkSpeed = .1, int run = 0);
    void running_animation(double speed, double walkSpeed = .3);
    void levitate(int on);

    ///////////////////////////////////////////////////////////////////

};

#endif // CUBE_H_INCLUDED
