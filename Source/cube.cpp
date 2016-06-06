#include <GL/glew.h>
#include "cube.h"
#include <stack>
#include "math.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

//  Constructors
cube::cube()
{
    red = 1;
    green = 1;
    blue = 1;
}

cube::cube(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;
}

//  Destructor
cube::~cube()
{
}

//  Accessor Functions

void cube::setDrawBorder(GLboolean d)
{
    drawBorder = d;
}

void cube::setDrawFaces(GLboolean d)
{
    drawFaces = d;
}

void cube::setColorCube(GLboolean d)
{
    isColorCube = d;
    reload = GL_TRUE;
    reloadData();
}

void cube::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;
    reloadData();
}

void cube::setBorderColor(GLfloat r, GLfloat g, GLfloat b)
{
    bred = r;
    bgreen = g;
    bblue = b;
    reloadData();
}

void cube::reloadData()
{
    reload = GL_TRUE;
    load(vPosition, vColor, vNormal);
}

void cube::load(GLint v, GLint c, GLint n)
{
    vPosition = v;
    vColor = c;
    vNormal = n;

    GLushort indices[] = {0, 1, 2,
                          2, 3, 0,
                          6, 5, 4,
                          4, 7, 6,
                          10, 9, 8,
                          8, 11, 10,
                          12, 13, 14,
                          14, 15, 12,
                          16, 17, 18,
                          18, 19, 16,
                          22, 21, 20,
                          20, 23, 22
                         };

    GLushort border_indices[] = {0, 1, 1, 2, 2, 3, 3, 0,
                                 4, 5, 5, 6, 6, 7, 7, 4,
                                 8, 9, 9, 10, 10, 11, 11, 8,
                                 12, 13, 13, 14, 14, 15, 15, 12,
                                 16, 17, 17, 18, 18, 19, 19, 16,
                                 20, 21, 21, 22, 22, 23, 23, 20
                                };

    GLfloat points[] = {-0.5, 0.5, 0.5, 1,
                        -0.5, -0.5, 0.5, 1,
                        0.5, -0.5, 0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, 0.5, -0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,

                        -0.5, 0.5, 0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, -0.5, 0.5, 1,

                        0.5, -0.5, 0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        -0.5, 0.5, 0.5, 1,
                       };

    GLfloat normals[] = {0, 0, 1,
                         0, 0, 1,
                         0, 0, 1,
                         0, 0, 1,

                         0, 0, -1,
                         0, 0, -1,
                         0, 0, -1,
                         0, 0, -1,

                         0, 1, 0,
                         0, 1, 0,
                         0, 1, 0,
                         0, 1, 0,

                         0, -1, 0,
                         0, -1, 0,
                         0, -1, 0,
                         0, -1, 0,

                         1, 0, 0,
                         1, 0, 0,
                         1, 0, 0,
                         1, 0, 0,

                         -1, 0, 0,
                         -1, 0, 0,
                         -1, 0, 0,
                         -1, 0, 0,
                        };


    GLfloat colors[4*24];
    for (int i = 0; i < 24; i++)
    {
        colors[4*i] = red;
        colors[4*i+1] = green;
        colors[4*i+2] = blue;
        colors[4*i+3] = 1;
    }

    GLfloat color_cube_colors[] = {1, 0, 0, 1,
                                   1, 0, 0, 1,
                                   1, 0, 0, 1,
                                   1, 0, 0, 1,
                                   0, 1, 0, 1,
                                   0, 1, 0, 1,
                                   0, 1, 0, 1,
                                   0, 1, 0, 1,
                                   0, 0, 1, 1,
                                   0, 0, 1, 1,
                                   0, 0, 1, 1,
                                   0, 0, 1, 1,
                                   1, 1, 0, 1,
                                   1, 1, 0, 1,
                                   1, 1, 0, 1,
                                   1, 1, 0, 1,
                                   0, 1, 1, 1,
                                   0, 1, 1, 1,
                                   0, 1, 1, 1,
                                   0, 1, 1, 1,
                                   1, 0, 1, 1,
                                   1, 0, 1, 1,
                                   1, 0, 1, 1,
                                   1, 0, 1, 1,
                                  };

    GLfloat border_colors[4*24];
    for (int i = 0; i < 24; i++)
    {
        border_colors[4*i] = bred;
        border_colors[4*i+1] = bgreen;
        border_colors[4*i+2] = bblue;
        border_colors[4*i+3] = 1;
    }

    if (!reload)
        glGenVertexArrays(1, &vboptr);

    glBindVertexArray(vboptr);

    if (!reload)
    {
        GLuint eboptr;
        glGenBuffers(1, &eboptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    }

    if (!reload)
        glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    if (isColorCube)
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(color_cube_colors), color_cube_colors);
    else
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);

    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);

    if (!reload)
        glGenVertexArrays(1, &vboptr2);

    glBindVertexArray(vboptr2);

    if (!reload)
    {
        GLuint eboptr;
        glGenBuffers(1, &eboptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(border_indices), border_indices, GL_DYNAMIC_DRAW);
    }

    if (!reload)
        glGenBuffers(1, &bufptr2);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(border_colors) + sizeof(normals), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(border_colors), border_colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(border_colors), sizeof(normals), normals);

    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(border_colors)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);

    reload = GL_FALSE;
}

//////////////////////////////new functions//////////////////////////

mat4 cube::getRobotTrans(int counter){
    return robotTrans[counter];
}

vec3 cube::getTranslation(int counter){
    return transStore[counter].Trans;
}

GLfloat cube::getTorsoY(){
    return transStore[TORSO].phiY;
}

GLfloat cube::getCollisionFeet(){
    vec3 feet = transStore[LEFT_CALF].Trans - transStore[LEFT_CALF].Scale * .5;
    return transStore[TORSO].Trans.y + feet.y;
}

vec3 cube::getCollisionBody(){
    vec3 body = transStore[TORSO].Trans;
    body += 1;
    return body;
}

void cube::setMiddle(){
    transStore[TORSO].Trans = vec3(25, 15, 25);
}

//See cube.h for constant declarations
void cube::createRobot(){
    transformations transTemp; //see cube.h
    std::stack<transformations> robotStack;
    mat4 tempMatrix;

    ////////////////////////// TORSO ///////////////////////////////////
    transTemp.Scale = vec3(2.0, 2.0, 2.0);
    transStore[TORSO].Rot = transTemp.Rot =
     vec3(0, 0, 0);
    transStore[TORSO].Trans = transTemp.Trans = vec3(0, 0, 0);

    tempMatrix = Scale(transTemp.Scale);
    robotStack.push(transTemp);

    ///////////////////////// HEAD /////////////////////////////////////
    transTemp = robotStack.top();
    transStore[HEAD].Scale = transTemp.Scale = vec3(transTemp.Scale.x * .5, transTemp.Scale.y * .5, transTemp.Scale.z * .5);
    transStore[HEAD].Trans = transTemp.Trans = vec3(transTemp.Trans.x, transTemp.Trans.y + 1.5, transTemp.Trans.z);
    transStore[HEAD].Rot = transTemp.Rot = vec3(0, 0, 0);

    tempMatrix = Translate(transTemp.Trans) * Scale(transTemp.Scale);
    robotOriginal[HEAD] = tempMatrix;

    ///////////////////////// RIGHT THIGH ///////////////////////////////
    transTemp = robotStack.top();

    transStore[RIGHT_THIGH].Scale = transTemp.Scale = vec3(transTemp.Scale.x * .35, transTemp.Scale.y * .7, transTemp.Scale.z * .35);
    transStore[RIGHT_THIGH].Trans = transTemp.Trans = vec3(transTemp.Trans.x + .6, transTemp.Trans.y - 1.8, transTemp.Trans.z);
    transStore[RIGHT_THIGH].Rot = transTemp.Rot = vec3(0, 0, 0);

    tempMatrix = Translate(transTemp.Trans) * Scale(transTemp.Scale);
    robotOriginal[RIGHT_THIGH] = tempMatrix;
    robotStack.push(transTemp);

    //////////////////////// RIGHT CALF //////////////////////////////////
    transTemp = robotStack.top();
    robotStack.pop(); //back to torso

    transStore[RIGHT_CALF].Scale = transTemp.Scale;
    transStore[RIGHT_CALF].Trans = transTemp.Trans = vec3(transTemp.Trans.x, transTemp.Trans.y - 1.4, transTemp.Trans.z);
    transStore[RIGHT_CALF].Rot = transTemp.Rot = vec3(0, 0, 0);

    tempMatrix = Translate(transTemp.Trans) * Scale(transTemp.Scale);
    robotOriginal[RIGHT_CALF] = tempMatrix;

    ///////////////////////// LEFT THIGH ///////////////////////////////
    transTemp = robotStack.top();

    transStore[LEFT_THIGH].Scale = transTemp.Scale = vec3(transTemp.Scale.x * .35, transTemp.Scale.y * .7, transTemp.Scale.z * .35);
    transStore[LEFT_THIGH].Trans = transTemp.Trans = vec3(transTemp.Trans.x - .6, transTemp.Trans.y - 1.8, transTemp.Trans.z);
    transStore[LEFT_THIGH].Rot = transTemp.Rot = vec3(0, 0, 0);

    tempMatrix = Translate(transTemp.Trans) * Scale(transTemp.Scale);
    robotOriginal[LEFT_THIGH] = tempMatrix;
    robotStack.push(transTemp);

    //////////////////////// LEFT CALF //////////////////////////////////
    transTemp = robotStack.top();
    robotStack.pop(); //back to torso

    transStore[LEFT_CALF].Scale = transTemp.Scale;
    transStore[LEFT_CALF].Trans = transTemp.Trans = vec3(transTemp.Trans.x, transTemp.Trans.y - 1.4, transTemp.Trans.z);
    transStore[LEFT_CALF].Rot = transTemp.Rot = vec3(0, 0, 0);

    tempMatrix = Translate(transTemp.Trans) * Scale(transTemp.Scale);
    robotOriginal[LEFT_CALF] = tempMatrix;

    ///////////////////////// RIGHT BICEP ///////////////////////////////
    transTemp = robotStack.top();

    transStore[RIGHT_BICEP].Scale = transTemp.Scale = vec3(transTemp.Scale.x * .7, transTemp.Scale.y * .25, transTemp.Scale.z * .25);
    transStore[RIGHT_BICEP].Trans = transTemp.Trans = vec3(transTemp.Trans.x + 1.8, transTemp.Trans.y + .7, transTemp.Trans.z);
    transStore[RIGHT_BICEP].Rot = transTemp.Rot = vec3(0, 0, 0);

    tempMatrix = Translate(transTemp.Trans) * Scale(transTemp.Scale);
    robotOriginal[RIGHT_BICEP] = tempMatrix;
    robotStack.push(transTemp);

    ///////////////////////// RIGHT FOREARM ///////////////////////////////
    transTemp = robotStack.top();
    robotStack.pop();

    transStore[RIGHT_FOREARM].Scale = transTemp.Scale;
    transStore[RIGHT_FOREARM].Trans = transTemp.Trans = vec3(transTemp.Trans.x + 1.4, transTemp.Trans.y, transTemp.Trans.z);
    transStore[RIGHT_FOREARM].Rot = transTemp.Rot = vec3(0, 0, 0);

    tempMatrix = Translate(transTemp.Trans) * Scale(transTemp.Scale);
    robotOriginal[RIGHT_FOREARM] = tempMatrix;

    ///////////////////////// LEFT BICEP ///////////////////////////////
    transTemp = robotStack.top();

    transStore[LEFT_BICEP].Scale = transTemp.Scale = vec3(transTemp.Scale.x * .7, transTemp.Scale.y * .25, transTemp.Scale.z * .25);
    transStore[LEFT_BICEP].Trans = transTemp.Trans = vec3(transTemp.Trans.x - 1.8, transTemp.Trans.y + .7, transTemp.Trans.z);
    transStore[LEFT_BICEP].Rot = transTemp.Rot = vec3(0, 0, 0);

    tempMatrix = Translate(transTemp.Trans) * Scale(transTemp.Scale);
    robotOriginal[LEFT_BICEP] = tempMatrix;
    robotStack.push(transTemp);

    ///////////////////////// LEFT FOREARM ///////////////////////////////
    transTemp = robotStack.top();
    robotStack.pop();

    transStore[LEFT_FOREARM].Scale = transTemp.Scale;
    transStore[LEFT_FOREARM].Trans = transTemp.Trans = vec3(transTemp.Trans.x - 1.4, transTemp.Trans.y, transTemp.Trans.z);
    transStore[LEFT_FOREARM].Rot = transTemp.Rot = vec3(0, 0, 0);

    tempMatrix = Translate(transTemp.Trans) * Scale(transTemp.Scale);
    robotOriginal[LEFT_FOREARM] = tempMatrix;

    //FINALLY CREATE TORSO AND CLEAR STACK
    transTemp = robotStack.top();
    transStore[TORSO].Scale = vec3(2.0, 2.0, 1.6);
    tempMatrix = Translate(transTemp.Trans) * Scale(vec3(2.0, 2.0, 1.6));
    robotOriginal[TORSO] = tempMatrix;
    robotStack.pop(); //stack cleared

    //initialize the robot Transformation matrix
    for (int i = 0; i < 10; i++){
        robotTrans[i] = robotOriginal[i];
    }
}

void cube::updateRobotTransformations(int up, int bodyPart, GLfloat Xangle, GLfloat Yangle, GLfloat Zangle, double walkSpeed, double jump){
    std::stack<mat4> tempStack;
    mat4 a, b, c, d, e, f, g;
    mat4 tempMatrix;

    if(up == 1){
        transStore[bodyPart].thetaX += Xangle;
        transStore[bodyPart].phiY += Yangle;
        transStore[bodyPart].psyZ += Zangle;
    }
    else if(up == 0){
        transStore[bodyPart].thetaX -= Xangle;
        transStore[bodyPart].phiY -= Yangle;
        transStore[bodyPart].psyZ -= Zangle;
    }

    transStore[TORSO].Trans.x += sin(transStore[TORSO].phiY * M_PI / 180) * walkSpeed;
    transStore[TORSO].Trans.y += jump;
    transStore[TORSO].Trans.z += cos(transStore[TORSO].phiY * M_PI / 180) * walkSpeed;

    if(transStore[bodyPart].thetaX == 720 || transStore[bodyPart].thetaX == -720)
        transStore[bodyPart].thetaX = 0;
    if(transStore[bodyPart].phiY == 720 || transStore[bodyPart].phiY == -720)
        transStore[bodyPart].phiY = 0;
    if(transStore[bodyPart].psyZ == 720 || transStore[bodyPart].psyZ == -720)
        transStore[bodyPart].psyZ = 0;

    //////////////////////////TORSO///////////////////////////////////
    a = Scale(transStore[TORSO].Scale);
    b = RotateX(transStore[TORSO].thetaX);
    c = RotateY(transStore[TORSO].phiY);
    d = RotateZ(transStore[TORSO].psyZ);
    e = Translate(transStore[TORSO].Trans);

    robotTrans[TORSO] = e * d * c * b * a;
    tempMatrix = e * d * c * b;
    tempStack.push(tempMatrix);

    /////////////////////////HEAD//////////////////////////////////////
    a = Scale(transStore[HEAD].Scale);
    b = RotateX(transStore[HEAD].thetaX);
    c = RotateY(transStore[HEAD].phiY);
    d = RotateZ(transStore[HEAD].psyZ);
    e = Translate(transStore[HEAD].Trans);

    robotTrans[HEAD] = tempStack.top() * e * d * c * b * a;

    /////////////////////////RIGHT BICEP/////////////////////////////////
    a = Scale(transStore[RIGHT_BICEP].Scale);
    b = Translate(transStore[RIGHT_BICEP].Scale.x * .5, 0, 0);
    c = RotateX(transStore[RIGHT_BICEP].thetaX);
    d = RotateY(transStore[RIGHT_BICEP].phiY);
    e = RotateZ(transStore[RIGHT_BICEP].psyZ);
    f = Translate(transStore[RIGHT_BICEP].Scale.x * (-.5), 0, 0);
    g = Translate(transStore[RIGHT_BICEP].Trans);

    robotTrans[RIGHT_BICEP] = tempStack.top() * g * f * e * d * c * b * a;
    tempMatrix = g * f * e * d * c * b;
    tempStack.push(tempMatrix);


    ////////////////////////RIGHT FOREARM//////////////////////////////////
    a = Scale(transStore[RIGHT_FOREARM].Scale);
    b = Translate(transStore[RIGHT_FOREARM].Scale.x * .5, 0, 0);
    c = RotateX(transStore[RIGHT_FOREARM].thetaX);
    d = RotateY(transStore[RIGHT_FOREARM].phiY);
    e = RotateZ(transStore[RIGHT_FOREARM].psyZ);
    f = Translate(transStore[RIGHT_FOREARM].Scale.x * .5, 0, 0);

    robotTrans[RIGHT_FOREARM] =  tempStack.top() * f * e * d * c * b * a;
    tempStack.pop();
    robotTrans[RIGHT_FOREARM] = tempStack.top() * robotTrans[RIGHT_FOREARM];


    /////////////////////////LEFT BICEP/////////////////////////////////
    a = Scale(transStore[LEFT_BICEP].Scale);
    b = Translate(transStore[LEFT_BICEP].Scale.x * -(.5), 0, 0);
    c = RotateX(transStore[LEFT_BICEP].thetaX);
    d = RotateY(transStore[LEFT_BICEP].phiY);
    e = RotateZ(transStore[LEFT_BICEP].psyZ);
    f = Translate(transStore[LEFT_BICEP].Scale.x * .5, 0, 0);
    g = Translate(transStore[LEFT_BICEP].Trans);

    robotTrans[LEFT_BICEP] = tempStack.top() * g * f * e * d * c * b * a;
    tempMatrix = g * f * e * d * c * b;
    tempStack.push(tempMatrix);


    ////////////////////////LEFT FOREARM//////////////////////////////////
    a = Scale(transStore[LEFT_FOREARM].Scale);
    b = Translate(transStore[LEFT_FOREARM].Scale.x * -(.5), 0, 0);
    c = RotateX(transStore[LEFT_FOREARM].thetaX);
    d = RotateY(transStore[LEFT_FOREARM].phiY);
    e = RotateZ(transStore[LEFT_FOREARM].psyZ);
    f = Translate(transStore[LEFT_FOREARM].Scale.x * -(.5), 0, 0);

    robotTrans[LEFT_FOREARM] =  tempStack.top() * f * e * d * c * b * a;
    tempStack.pop();
    robotTrans[LEFT_FOREARM] = tempStack.top() * robotTrans[LEFT_FOREARM];

    ////////////////////////RIGHT THIGH///////////////////////////////
    a = Scale(transStore[RIGHT_THIGH].Scale);
    b = Translate(0, transStore[RIGHT_THIGH].Scale.y * -(.5), 0);
    c = RotateX(transStore[RIGHT_THIGH].thetaX);
    d = RotateY(transStore[RIGHT_THIGH].phiY);
    e = RotateZ(transStore[RIGHT_THIGH].psyZ);
    f = Translate(0, transStore[RIGHT_THIGH].Scale.y * .5, 0);
    g = Translate(transStore[RIGHT_THIGH].Trans);

    robotTrans[RIGHT_THIGH] = tempStack.top() * g * f * e * d * c * b * a;
    tempMatrix = g * f * e * d * c * b;
    tempStack.push(tempMatrix);

    ///////////////////////RIGHT CALF//////////////////////////////////
    a = Scale(transStore[RIGHT_CALF].Scale);
    b = Translate(0, transStore[RIGHT_CALF].Scale.y * -(.5), 0);
    c = RotateX(transStore[RIGHT_CALF].thetaX);
    d = RotateY(transStore[RIGHT_CALF].phiY);
    e = RotateZ(transStore[RIGHT_CALF].psyZ);
    f = Translate(0, transStore[RIGHT_CALF].Scale.y * -(.5), 0);
    g = Translate(transStore[RIGHT_CALF].Trans);

    robotTrans[RIGHT_CALF] = tempStack.top() * f * e * d * c * b * a;
    tempStack.pop();
    robotTrans[RIGHT_CALF] = tempStack.top() * robotTrans[RIGHT_CALF];

    ////////////////////////LEFT THIGH///////////////////////////////
    a = Scale(transStore[LEFT_THIGH].Scale);
    b = Translate(0, transStore[LEFT_THIGH].Scale.y * -(.5), 0);
    c = RotateX(transStore[LEFT_THIGH].thetaX);
    d = RotateY(transStore[LEFT_THIGH].phiY);
    e = RotateZ(transStore[LEFT_THIGH].psyZ);
    f = Translate(0, transStore[LEFT_THIGH].Scale.y * .5, 0);
    g = Translate(transStore[LEFT_THIGH].Trans);

    robotTrans[LEFT_THIGH] = tempStack.top() * g * f * e * d * c * b * a;
    tempMatrix = g * f * e * d * c * b;
    tempStack.push(tempMatrix);

    ///////////////////////LEFT CALF//////////////////////////////////
    a = Scale(transStore[LEFT_CALF].Scale);
    b = Translate(0, transStore[LEFT_CALF].Scale.y * -(.5), 0);
    c = RotateX(transStore[LEFT_CALF].thetaX);
    d = RotateY(transStore[LEFT_CALF].phiY);
    e = RotateZ(transStore[LEFT_CALF].psyZ);
    f = Translate(0, transStore[LEFT_CALF].Scale.y * -(.5), 0);
    g = Translate(transStore[LEFT_CALF].Trans);

    robotTrans[LEFT_CALF] = tempStack.top() * f * e * d * c * b * a;
    tempStack.pop();
    robotTrans[LEFT_CALF] = tempStack.top() * robotTrans[LEFT_CALF];
}

void cube::updateRobotTransformations(int up, int bodyPart, vec3 rotations, double walkSpeed){
    updateRobotTransformations(up, bodyPart, rotations.x, rotations.y, rotations.z, walkSpeed);
}

//Tried to use a loop, was not working correctly so i just hard coded in each of the body parts
//I'm sorry in advanced its very repetitive
int cube::set_to_standing(){
    int done = 0;

        ////////////////////TORSO//////////////////////////////
        if (transStore[TORSO].thetaX != 0){
            if (transStore[TORSO].thetaX < 0)
                updateRobotTransformations(1, TORSO, 1, 0, 0);
            else if (transStore[TORSO].thetaX > 0)
                updateRobotTransformations(0, TORSO, 1, 0, 0);
        }
        else
            done++;

        if (transStore[TORSO].psyZ != 0){
            if (transStore[TORSO].psyZ < 0)
                updateRobotTransformations(1, TORSO, 0, 0, 1);
            else if (transStore[TORSO].psyZ > 0)
                updateRobotTransformations(0, TORSO, 0, 0, 1);
        }
        else
            done++;

        ////////////////////HEAD//////////////////////////////
        if (transStore[HEAD].thetaX != 0){
            if (transStore[HEAD].thetaX < 0)
                updateRobotTransformations(1, HEAD, 1, 0, 0);
            else if (transStore[HEAD].thetaX > 0)
                updateRobotTransformations(0, HEAD, 1, 0, 0);
        }
        else
            done++;

        if (transStore[HEAD].phiY != 0){
            if (transStore[HEAD].phiY < 0)
                updateRobotTransformations(1, HEAD, 0, 1, 0);
            else if (transStore[HEAD].phiY > 0)
                updateRobotTransformations(0, HEAD, 0, 1, 0);
        }
        else
            done++;

        if (transStore[HEAD].psyZ != 0){
            if (transStore[HEAD].psyZ < 0)
                updateRobotTransformations(1, HEAD, 0, 0, 1);
            else if (transStore[HEAD].psyZ > 0)
                updateRobotTransformations(0, HEAD, 0, 0, 1);
        }
        else
            done++;

        ////////////////////RIGHT THIGH//////////////////////////////
        if (transStore[RIGHT_THIGH].thetaX != 0){
            if (transStore[RIGHT_THIGH].thetaX < 0)
                updateRobotTransformations(1, RIGHT_THIGH, 1, 0, 0);
            else if (transStore[RIGHT_THIGH].thetaX > 0)
                updateRobotTransformations(0, RIGHT_THIGH, 1, 0, 0);
        }
        else
            done++;

        if (transStore[RIGHT_THIGH].phiY != 0){
            if (transStore[RIGHT_THIGH].phiY < 0)
                updateRobotTransformations(1, RIGHT_THIGH, 0, 1, 0);
            else if (transStore[RIGHT_THIGH].phiY > 0)
                updateRobotTransformations(0, RIGHT_THIGH, 0, 1, 0);
        }
        else
            done++;

        if (transStore[RIGHT_THIGH].psyZ != 0){
            if (transStore[RIGHT_THIGH].psyZ < 0)
                updateRobotTransformations(1, RIGHT_THIGH, 0, 0, 1);
            else if (transStore[RIGHT_THIGH].psyZ > 0)
                updateRobotTransformations(0, RIGHT_THIGH, 0, 0, 1);
        }
        else
            done++;

        ////////////////////RIGHT CALF//////////////////////////////
        if (transStore[RIGHT_CALF].thetaX != 0){
            if (transStore[RIGHT_CALF].thetaX < 0)
                updateRobotTransformations(1, RIGHT_CALF, 1, 0, 0);
            else if (transStore[RIGHT_CALF].thetaX > 0)
                updateRobotTransformations(0, RIGHT_CALF, 1, 0, 0);
        }
        else
            done++;

        if (transStore[RIGHT_CALF].phiY != 0){
            if (transStore[RIGHT_CALF].phiY < 0)
                updateRobotTransformations(1, RIGHT_CALF, 0, 1, 0);
            else if (transStore[RIGHT_CALF].phiY > 0)
                updateRobotTransformations(0, RIGHT_CALF, 0, 1, 0);
        }
        else
            done++;

        if (transStore[RIGHT_CALF].psyZ != 0){
            if (transStore[RIGHT_CALF].psyZ < 0)
                updateRobotTransformations(1, RIGHT_CALF, 0, 0, 1);
            else if (transStore[RIGHT_CALF].psyZ > 0)
                updateRobotTransformations(0, RIGHT_CALF, 0, 0, 1);
        }
        else
            done++;

        ////////////////////LEFT THIGH//////////////////////////////
        if (transStore[LEFT_THIGH].thetaX != 0){
            if (transStore[LEFT_THIGH].thetaX < 0)
                updateRobotTransformations(1, LEFT_THIGH, 1, 0, 0);
            else if (transStore[LEFT_THIGH].thetaX > 0)
                updateRobotTransformations(0, LEFT_THIGH, 1, 0, 0);
        }
        else
            done++;

        if (transStore[LEFT_THIGH].phiY != 0){
            if (transStore[LEFT_THIGH].phiY < 0)
                updateRobotTransformations(1, LEFT_THIGH, 0, 1, 0);
            else if (transStore[LEFT_THIGH].phiY > 0)
                updateRobotTransformations(0, LEFT_THIGH, 0, 1, 0);
        }
        else
            done++;

        if (transStore[LEFT_THIGH].psyZ != 0){
            if (transStore[LEFT_THIGH].psyZ < 0)
                updateRobotTransformations(1, LEFT_THIGH, 0, 0, 1);
            else if (transStore[LEFT_THIGH].psyZ > 0)
                updateRobotTransformations(0, LEFT_THIGH, 0, 0, 1);
        }
        else
            done++;

        ////////////////////LEFT CALF//////////////////////////////
        if (transStore[LEFT_CALF].thetaX != 0){
            if (transStore[LEFT_CALF].thetaX < 0)
                updateRobotTransformations(1, LEFT_CALF, 1, 0, 0);
            else if (transStore[LEFT_CALF].thetaX > 0)
                updateRobotTransformations(0, LEFT_CALF, 1, 0, 0);
        }
        else
            done++;

        if (transStore[LEFT_CALF].phiY != 0){
            if (transStore[LEFT_CALF].phiY < 0)
                updateRobotTransformations(1, LEFT_CALF, 0, 1, 0);
            else if (transStore[LEFT_CALF].phiY > 0)
                updateRobotTransformations(0, LEFT_CALF, 0, 1, 0);
        }
        else
            done++;

        if (transStore[LEFT_CALF].psyZ != 0){
            if (transStore[LEFT_CALF].psyZ < 0)
                updateRobotTransformations(1, LEFT_CALF, 0, 0, 1);
            else if (transStore[LEFT_CALF].psyZ > 0)
                updateRobotTransformations(0, LEFT_CALF, 0, 0, 1);
        }
        else
            done++;

        ////////////////////RIGHT BICEP//////////////////////////////
        if (transStore[RIGHT_BICEP].thetaX != 0){
            if (transStore[RIGHT_BICEP].thetaX < 0)
                updateRobotTransformations(1, RIGHT_BICEP, 1, 0, 0);
            else if (transStore[RIGHT_BICEP].thetaX > 0)
                updateRobotTransformations(0, RIGHT_BICEP, 1, 0, 0);
        }
        else
            done++;

        if (transStore[RIGHT_BICEP].phiY != 0){
            if (transStore[RIGHT_BICEP].phiY < 0)
                updateRobotTransformations(1, RIGHT_BICEP, 0, 1, 0);
            else if (transStore[RIGHT_BICEP].phiY > 0)
                updateRobotTransformations(0, RIGHT_BICEP, 0, 1, 0);
        }
        else
            done++;

        if (transStore[RIGHT_BICEP].psyZ != -70){
            if (transStore[RIGHT_BICEP].psyZ < -70)
                updateRobotTransformations(1, RIGHT_BICEP, 0, 0, 1);
            else if (transStore[RIGHT_BICEP].psyZ > -70)
                updateRobotTransformations(0, RIGHT_BICEP, 0, 0, 1);
        }
        else
            done++;

        ////////////////////RIGHT FOREARM//////////////////////////////
        if (transStore[RIGHT_FOREARM].thetaX != 0){
            if (transStore[RIGHT_FOREARM].thetaX < 0)
                updateRobotTransformations(1, RIGHT_FOREARM, 1, 0, 0);
            else if (transStore[RIGHT_FOREARM].thetaX > 0)
                updateRobotTransformations(0, RIGHT_FOREARM, 1, 0, 0);
        }
        else
            done++;

        if (transStore[RIGHT_FOREARM].phiY != 0){
            if (transStore[RIGHT_FOREARM].phiY < 0)
                updateRobotTransformations(1, RIGHT_FOREARM, 0, 1, 0);
            else if (transStore[RIGHT_FOREARM].phiY > 0)
                updateRobotTransformations(0, RIGHT_FOREARM, 0, 1, 0);
        }
        else
            done++;

        if (transStore[RIGHT_FOREARM].psyZ != 0){
            if (transStore[RIGHT_FOREARM].psyZ < 0)
                updateRobotTransformations(1, RIGHT_FOREARM, 0, 0, 1);
            else if (transStore[RIGHT_FOREARM].psyZ > 0)
                updateRobotTransformations(0, RIGHT_FOREARM, 0, 0, 1);
        }
        else
            done++;

        ////////////////////LEFT BICEP//////////////////////////////
        if (transStore[LEFT_BICEP].thetaX != 0){
            if (transStore[LEFT_BICEP].thetaX < 0)
                updateRobotTransformations(1, LEFT_BICEP, 1, 0, 0);
            else if (transStore[LEFT_BICEP].thetaX > 0)
                updateRobotTransformations(0, LEFT_BICEP, 1, 0, 0);
        }
        else
            done++;

        if (transStore[LEFT_BICEP].phiY != 0){
            if (transStore[LEFT_BICEP].phiY < 0)
                updateRobotTransformations(1, LEFT_BICEP, 0, 1, 0);
            else if (transStore[LEFT_BICEP].phiY > 0)
                updateRobotTransformations(0, LEFT_BICEP, 0, 1, 0);
        }
        else
            done++;

        if (transStore[LEFT_BICEP].psyZ != 70){
            if (transStore[LEFT_BICEP].psyZ < 70)
                updateRobotTransformations(1, LEFT_BICEP, 0, 0, 1);
            else if (transStore[LEFT_BICEP].psyZ > 70)
                updateRobotTransformations(0, LEFT_BICEP, 0, 0, 1);
        }
        else
            done++;

        ////////////////////LEFT FOREARM//////////////////////////////
        if (transStore[LEFT_FOREARM].thetaX != 0){
            if (transStore[LEFT_FOREARM].thetaX < 0)
                updateRobotTransformations(1, LEFT_FOREARM, 1, 0, 0);
            else if (transStore[LEFT_FOREARM].thetaX > 0)
                updateRobotTransformations(0, LEFT_FOREARM, 1, 0, 0);
        }
        else
            done++;

        if (transStore[LEFT_FOREARM].phiY != 0){
            if (transStore[LEFT_FOREARM].phiY < 0)
                updateRobotTransformations(1, LEFT_FOREARM, 0, 1, 0);
            else if (transStore[LEFT_FOREARM].phiY > 0)
                updateRobotTransformations(0, LEFT_FOREARM, 0, 1, 0);
        }
        else
            done++;

        if (transStore[LEFT_FOREARM].psyZ != 0){
            if (transStore[LEFT_FOREARM].psyZ < 0)
                updateRobotTransformations(1, LEFT_FOREARM, 0, 0, 1);
            else if (transStore[LEFT_FOREARM].psyZ > 0)
                updateRobotTransformations(0, LEFT_FOREARM, 0, 0, 1);
        }
        else
            done++;

        if(done == 29)
            return 1;
        else
            return 0;
}

void cube::walking_animation(double speed, double walkSpeed, int run){
    static int R_leg = 0;
    static int L_leg = 1;
    static int R_arm = 1;
    static int L_arm = 1;

    transStore[RIGHT_BICEP].psyZ = -70;
    transStore[LEFT_BICEP].psyZ = 70;

    /////////////Turn back to walking animation from running animation
    if(run == 0){
        transStore[RIGHT_FOREARM].phiY = 0;
        transStore[LEFT_FOREARM].phiY = 0;
    }

        ////////////////////////////RIGHT LEG////////////////////////////////
        if(R_leg == 1 && transStore[RIGHT_THIGH].thetaX >= 70){
            R_leg = 0;
        }
        else if(R_leg == 0 && transStore[RIGHT_THIGH].thetaX <= -70){
            R_leg = 1;
        }

        ////////////////////////////LEFT LEG////////////////////////////////
        if(L_leg == 1 && transStore[LEFT_THIGH].thetaX >= 70){
            L_leg = 0;
        }
        else if(L_leg == 0 && transStore[LEFT_THIGH].thetaX <= -70){
            L_leg = 1;
        }

        ////////////////////////////RIGHT ARM////////////////////////////////
        if(R_arm == 1 && transStore[RIGHT_BICEP].phiY >= 70){
            R_arm = 0;
        }
        else if(R_arm == 0 && transStore[RIGHT_BICEP].phiY <= -70){
            R_arm = 1;
        }

        ////////////////////////////LEFT ARM////////////////////////////////
        if(L_arm == 1 && transStore[LEFT_BICEP].phiY >= 70){
            L_arm = 0;
        }
        else if(L_arm == 0 && transStore[LEFT_BICEP].phiY <= -70){
            L_arm = 1;
        }

        updateRobotTransformations(R_leg, RIGHT_THIGH, speed, 0, 0);
        updateRobotTransformations(L_leg, LEFT_THIGH, speed, 0, 0);

        if(transStore[RIGHT_CALF].thetaX < 0)
                transStore[RIGHT_CALF].thetaX = 0;
        else if(transStore[RIGHT_CALF].thetaX > 90)
                transStore[RIGHT_CALF].thetaX = 90;

        if(transStore[RIGHT_CALF].thetaX >= 0 && transStore[RIGHT_CALF].thetaX <= 90){
            updateRobotTransformations(R_leg, RIGHT_CALF, speed, 0, 0);
        }

        if(transStore[LEFT_CALF].thetaX < 0)
                transStore[LEFT_CALF].thetaX = 0;
        else if(transStore[LEFT_CALF].thetaX > 90)
            transStore[LEFT_CALF].thetaX = 90;

        if(transStore[LEFT_CALF].thetaX >= 0 && transStore[LEFT_CALF].thetaX <= 90){
            updateRobotTransformations(L_leg, LEFT_CALF, speed, 0, 0);
        }

        updateRobotTransformations(R_arm, RIGHT_BICEP, speed / 2, speed, 0);
        updateRobotTransformations(L_arm, LEFT_BICEP, speed / 2, speed, 0);

        updateRobotTransformations(R_arm, TORSO, 0, speed * .1, 0, 0);
        updateRobotTransformations(1, TORSO, 0, 0, 0, walkSpeed);
}

void cube::running_animation(double speed, double walkSpeed){
    transStore[RIGHT_FOREARM].phiY = -45;
    transStore[LEFT_FOREARM].phiY = 45;
    walking_animation(speed, walkSpeed, 1);
}

void cube::levitate(int on){
    if(on)
        updateRobotTransformations(1, TORSO, 0, 0, 0, 0, .3);
    else
        updateRobotTransformations(1, TORSO, 0, 0, 0, 0, 0);
}
//////////////////////////////////////////////////////////////////////

void cube::draw()
{
    if (drawFaces)
    {
        glBindVertexArray(vboptr);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
    }

    if (drawBorder)
    {
        glBindVertexArray(vboptr2);
        glLineWidth(2);
        glDrawElements(GL_LINES, 48, GL_UNSIGNED_SHORT, NULL);
        glLineWidth(1);
    }

}
