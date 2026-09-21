#pragma once

class Tray
{
private:
    const float SCALE = 30.0f;
    b2Body* Tray_Body;
    b2Fixture* Tray_Fixture;
    int mouseX, mouseY;
public:
    Tray(b2World* world);
    ~Tray();
    void TrayMovement();
    b2Fixture* GetFixture(){return Tray_Fixture;}
    b2Body* GetBody(){return Tray_Body;}
};