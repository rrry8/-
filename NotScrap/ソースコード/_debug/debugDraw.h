#pragma once
#include <LinearMath/btIDebugDraw.h>
class debugDraw :
    public btIDebugDraw
{
public:
    debugDraw();
    void    drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar   distance, int lifeTime, const btVector3& color);
    void    reportErrorWarning(const char* warningString);
    void    draw3dText(const btVector3& location, const char* textString);
    void    setDebugMode(int debugMode);
    int     getDebugMode() const;
    void drawAabb(const btVector3& from, const btVector3& to, const btVector3& color);

    void  drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    //void drawRectangle(VECTOR p0, VECTOR p1, VECTOR p2, VECTOR p3, unsigned int color);
private:
    int debug_mode;
};

