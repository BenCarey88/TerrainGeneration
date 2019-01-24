#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <vector>
#include <ngl/Vec3.h>

class Camera
{    
public:
    Camera() = default;

    ngl::Vec3 m_from = {0,-50,50};
    ngl::Vec3 m_to = {0,0,0};
    ngl::Vec3 m_up = {0,0,1};

    ngl::Vec3 m_right = (m_to-m_from).cross(m_up);
    ngl::Vec3 m_trueUp = m_right.cross(m_to-m_from);

    void update();

    void initialise();

protected:

};


#endif //CAMERA_H_
