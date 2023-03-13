#include "debugDraw.h"
#include <DxLib.h>
debugDraw::debugDraw()
{
}

void debugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void debugDraw::reportErrorWarning(const char* warningString)
{
}

void debugDraw::draw3dText(const btVector3& location, const char* textString)
{
}

void debugDraw::setDebugMode(int debugMode)
{
	debug_mode = debugMode;
}

int debugDraw::getDebugMode() const
{
	return debug_mode;
}

void debugDraw::drawAabb(const btVector3& from, const btVector3& to, const btVector3& color)
{
}

void debugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	//DrawLine3D(VGet(from.getX(), from.getY(), from.getZ()), VGet(to.getX(), to.getY(), to.getZ()), GetColor( color.getX(), color.getY(), color.getZ()));
	DrawLine3D(VGet(from.getX(), from.getY(), from.getZ()), VGet(to.getX(), to.getY(), to.getZ()), 0xff0000);
    //float x = from.getX();
    //float y = from.getY();
    //float z = from.getZ();
    //
    //float s = width / 2.0f;
    //
    //    // óßï˚ëÃÇÃ8í∏ì_ÇéÊìæÇ∑ÇÈÅB
    //    VECTOR v_XYZ = VGet(x + s, y + s, z + s);
   
    //    VECTOR v_XYz = VGet(x + s, y + s, z - s);
   
    //    VECTOR v_XyZ = VGet(x + s, y - s, z + s);
   
    //    VECTOR v_xYZ = VGet(x - s, y + s, z + s);
    //
    //    VECTOR v_Xyz = VGet(x + s, y - s, z - s);
    //
    //    VECTOR v_xYz = VGet(x - s, y + s, z - s);
    //
    //    VECTOR v_xyZ = VGet(x - s, y - s, z + s);
    //
    //    VECTOR v_xyz = VGet(x - s, y - s, z - s);
    //
    //    // óßï˚ëÃÇÃñ Çï`âÊÇ∑ÇÈÅB
    //    
    //    drawRectangle(v_XYZ, v_XYz, v_Xyz, v_XyZ, color);
    //
    //    drawRectangle(v_XYZ, v_xYZ, v_xYz, v_XYz, color);
    //
    //    drawRectangle(v_xyz, v_Xyz, v_XYz, v_xYz, color);
    //
    //    drawRectangle(v_Xyz, v_XyZ, v_xyZ, v_xyz, color);
    //
    //    drawRectangle(v_XYZ, v_xYZ, v_xyZ, v_XyZ, color);
    //
    //    drawRectangle(v_xyz, v_xyZ, v_xYZ, v_xYz, color);

}

void drawRectangle(VECTOR p0, VECTOR p1, VECTOR p2, VECTOR p3, unsigned int color) {
    
        DrawTriangle3D(p0, p1, p2, color, true);
   
        DrawTriangle3D(p2, p3, p0, color, true);
   
}
