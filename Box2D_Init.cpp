#include <box2d/box2d.h>

#include "Box2D_Init.h"

Box2D_Init::Box2D_Init() :gravity(0.0f, 15.0f),world(gravity)
{

}

Box2D_Init::~Box2D_Init()
{

}