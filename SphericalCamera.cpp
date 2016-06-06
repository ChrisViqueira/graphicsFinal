#include "SphericalCamera.h"

SphericalCamera::SphericalCamera()
{
    r = 1;
    theta = 0;
    psy = 0;
}

void SphericalCamera::setPosition(float R, float Theta, float Psy)
{
    r = R;
    theta = Theta;
    psy = Psy;
}

mat4 SphericalCamera::lookAt(vec3 robot)
{
    return LookAt(r*cos(psy*deg)*cos(theta*deg) + robot.x, r*sin(psy*deg) + robot.y, r*cos(psy*deg)*sin(theta*deg) + robot.z,
                  robot.x, robot.y, robot.z,
                  0.0,1.0,0.0);
}

void SphericalCamera::addR(float num)
{
    r += num;
    if (r < 0.000001f) r = 0.000001f;
}

void SphericalCamera::addTheta(float num)
{
    theta += num;
    if (theta > 360) theta -= 360;
    if (theta < 0) theta += 360;
}

void SphericalCamera::addPsy(float num)
{
    psy += num;
    if (psy > 90) psy = 90;
    if (psy < -90) psy = -90;
}

void SphericalCamera::setR(float num)
{
    r = num;
    if (r < 0.000001f) r = 0.000001f;
}

void SphericalCamera::setTheta(float num)
{
    theta = num;
    while (theta > 360) theta -= 360;
    while (theta < 0) theta += 360;
}

void SphericalCamera::setPsy(float num)
{
    psy = num;
    if (psy > 90) psy = 90;
    if (psy < -90) psy = -90;
}

float SphericalCamera::getR()
{
    return r;
}

float SphericalCamera::getTheta()
{
    return theta;
}

float SphericalCamera::getPsy()
{
    return psy;
}

vec3 SphericalCamera::getPosition(vec3 robot)
{
    vec3 retpos;
    retpos.x = r*cos(psy*deg)*cos(theta*deg) + robot.x;
    retpos.y = r*sin(psy*deg) + robot.y;
    retpos.z = r*cos(psy*deg)*sin(theta*deg) + robot.z;

    return retpos;
}
