#ifndef PHYSICS_H
#define PHYSICS_H

#include <map>
#include <iomanip>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"
#include "tools/shader.hpp"
#include "utils/Utils.h"
#include "btBulletDynamicsCommon.h"
#include "Geometry.h"

class Physics {

public:

    Physics();
    
    ~Physics(); 
    
    void initExampleWorld();
    void addGeometry(Geometry *, unsigned int);
    void stepSimulation();
    void setWireFrame(bool w) { mWireframe = w; };
    btRigidBody* getRigidBodyAt(int i) { return mRigidBodies[i]; }

private:

    // Member functions

    void calculateBoundingbox(std::vector<Vector3<float> >);
    
    // Shader data

    GLuint vertexArrayID;
    GLuint vertexBuffer;
    GLuint shaderProgram;
    GLint MVPLoc; // MVP Matrix
    GLint ColorLoc;
    
    // Member variables

    Vector4<float> mColor;

    std::vector<Vector3<float> > mVerts;

    bool mWireframe = false;

    btBroadphaseInterface* mBroadphase;
    btDefaultCollisionConfiguration* mCollisionConfiguration;
    btCollisionDispatcher* mDispatcher;
    btSequentialImpulseConstraintSolver* mSolver;
    btDiscreteDynamicsWorld* mDynamicsWorld;


    std::vector<btRigidBody* > mRigidBodies;

};

#endif // PHYSICS_H