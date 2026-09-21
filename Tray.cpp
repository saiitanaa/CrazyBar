#include <box2d/box2d.h>
#include <SDL2/SDL.h>

#include "Tray.h"

Tray::Tray(b2World* world)

{
    // TrayBody
    b2PolygonShape TrayBodyShape;
    TrayBodyShape.SetAsBox((200.0f / 2.0f) / SCALE, (10.0f / 2.0f) / SCALE);
    const float TrayAngle = 0.0f;

    b2FixtureDef TrayBodyFixtureDef;
    TrayBodyFixtureDef.shape = &TrayBodyShape;
    TrayBodyFixtureDef.density = 1.0f;
    TrayBodyFixtureDef.friction = 1.0f; // (0 = glass, 1 = sandpaper)

    b2BodyDef TrayBodyDef;
    TrayBodyDef.type = b2_kinematicBody; // (b2_staticBody, b2_dynamicBody, ou b2_kinematicBody)
    Tray_Body = world->CreateBody(&TrayBodyDef);

    Tray_Body->CreateFixture(&TrayBodyFixtureDef);
    Tray_Fixture = Tray_Body->GetFixtureList();

    // ColliderListener.Tray = TrayBody;
}

Tray::~Tray()
{

}

void Tray::TrayMovement()
{
    SDL_GetMouseState(&mouseX, &mouseY);
    b2Vec2 targetPosition(mouseX / SCALE, mouseY / SCALE);
    b2Vec2 currentPosition = Tray_Body->GetPosition();
    b2Vec2 velocity = targetPosition - currentPosition;
    velocity *= 60.0f;

    Tray_Body->SetLinearVelocity(velocity);
}





