#pragma once
class Box2D_Init
{
    private:
    b2Vec2 gravity;
    b2World world;
    float timeStep = 1.0f / 60.0f;
    int velocityIterations = 6;
    int positionIterations = 2;
    public:
    Box2D_Init();
    ~Box2D_Init();
    b2World& GetWorld(){return world;}
    b2Vec2 GetGravity(){return gravity;}
    float GetTimeStep(){return timeStep;}
    int GetVelocityIterations(){return velocityIterations;}
    int GetPositionIterations(){return positionIterations;}
};
