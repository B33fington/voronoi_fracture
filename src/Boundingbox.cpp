#include "Boundingbox.h"

Boundingbox::Boundingbox(std::vector<Vector3<float> > uniqueVerts) {

    calculateBoundingbox(uniqueVerts);

}


Boundingbox::~Boundingbox() {
    mVerts.clear();
    //mVerts.shrink_to_fit();
}


void Boundingbox::initialize() {

    std::cout << "\nInitializing Boundingbox ...\n";

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    shaderProgram = LoadShaders( "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader" );

    // Set names for our uniforms, same as in shaders
    MVPLoc = glGetUniformLocation(shaderProgram, "MVP");

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(Vector3<float>), &mVerts[0], GL_STATIC_DRAW);

     // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                          // attribute 0. I.e. layout 0 in shader
        3,                          // size
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalized?
        0,                          // stride
        reinterpret_cast<void*>(0)  // array buffer offset
    );

     std::cout << "\nBoundingbox initialized!\n" << std::endl;
}


void Boundingbox::render(Matrix4x4<float> MVP) {

    //borde inte sättas här, de borde tas in i render function med en kosntant typ RENDER_WITH_WIREFRAME
    drawWireframe = true;

    glDisable( GL_CULL_FACE );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

     // Use shader
    glUseProgram(shaderProgram);

    // Pass values of our matrices and materials to the GPU via uniforms
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &MVP(0, 0));

    // Rebind the buffer data, vertices are now updated
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(Vector3<float>), &mVerts[0], GL_STATIC_DRAW);

    // Draw geometry

    
    if(drawWireframe)   //Draw Wireframe
        glDrawArrays(GL_LINES, 0, mVerts.size()); // 3 indices starting at 0 -> 1 triangle
    else                //draw triangles
        glDrawArrays(GL_TRIANGLES, 0, mVerts.size());

    
    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);

    glDisable( GL_BLEND );
    glEnable( GL_CULL_FACE );
}


void Boundingbox::calculateBoundingbox(std::vector<Vector3<float> > uniqueVerts) {

    std::map<std::string, unsigned int> boundingValues;

    for(unsigned int i = 0; i < uniqueVerts.size(); i++) {

        if(uniqueVerts.at(i)[0] < uniqueVerts.at(boundingValues["xMin"])[0]){
            boundingValues["xMin"] = i;
        }
        if(uniqueVerts.at(i)[0] > uniqueVerts.at(boundingValues["xMax"])[0]){
            boundingValues["xMax"] = i;
        }
        
        if(uniqueVerts.at(i)[1] < uniqueVerts.at(boundingValues["yMin"])[1]){
            boundingValues["yMin"] = i;
        }
        if(uniqueVerts.at(i)[1] > uniqueVerts.at(boundingValues["yMax"])[1]){
            boundingValues["yMax"] = i;
        }
        
        if(uniqueVerts.at(i)[2] < uniqueVerts.at(boundingValues["zMin"])[2]){
            boundingValues["zMin"] = i;
        }
        if(uniqueVerts.at(i)[2] > uniqueVerts.at(boundingValues["zMax"])[2]){
            boundingValues["zMax"] = i;
        }
    }

    // xMin Plane
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));

    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));

    //xMax Plane
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));

    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));

    //yMin Plane
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));

    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));

     //yMax Plane
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));

    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));

     //zMin Plane
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));

    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMin"])[2]));

     //zMax Plane
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));

    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMin"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMax"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));
    mVerts.push_back(Vector3<float>(uniqueVerts.at(boundingValues["xMin"])[0], uniqueVerts.at(boundingValues["yMax"])[1], uniqueVerts.at(boundingValues["zMax"])[2]));

}