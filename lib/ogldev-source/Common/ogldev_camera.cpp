/*

	Copyright 2010 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ogldev_camera.h"

const static float STEP_SCALE = 0.1f;
const static float EDGE_STEP = 0.5f;
const static int MARGIN = 10;

OVGCamera::OVGCamera(int WindowWidth, int WindowHeight)
{
    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos          = Vector3f(0.0f, 0.0f, 0.0f);
    m_target       = Vector3f(0.0f, 0.0f, 1.0f);
    m_target.Normalize();
    m_up           = Vector3f(0.0f, 1.0f, 0.0f);

    Init();
}


OVGCamera::OVGCamera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
{
    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = Pos;

    m_target = Target;
    m_target.Normalize();

    m_up = Up;
    m_up.Normalize();

    Init();
}


void OVGCamera::Init()
{
    Vector3f HTarget(m_target.x, 0.0, m_target.z);
    HTarget.Normalize();
    
    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
			m_AngleH = 2 * M_PI - asin(HTarget.z); // 360.0f - ToDegree(asin(HTarget.z));
        }
        else
        {
			m_AngleH = M_PI + asin(HTarget.z); //180.0f + ToDegree(asin(HTarget.z));
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
			m_AngleH = asin(-HTarget.z);//ToDegree(asin(-HTarget.z));
        }
        else
        {
			m_AngleH = M_PI/2.0f + asin(-HTarget.z);//90.0f + ToDegree(asin(-HTarget.z));
        }
    }
    
	m_AngleV = -asin(m_target.y);// -ToDegree(asin(m_target.y));

    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge  = false;
    m_OnRightEdge = false;
    m_mousePos.x  = m_windowWidth / 2;
    m_mousePos.y  = m_windowHeight / 2;
	OnMouseTurn(0.0f, 0.0f);
   // glutWarpPointer(m_mousePos.x, m_mousePos.y);
}


bool OVGCamera::OnKeyboard(OGLDEV_KEY Key)
{
    bool Ret = false;

    switch (Key) {

    case OGLDEV_KEY_UP:
        {
            m_pos += (m_target * STEP_SCALE);
            Ret = true;
        }
        break;

    case OGLDEV_KEY_DOWN:
        {
            m_pos -= (m_target * STEP_SCALE);
            Ret = true;
        }
        break;

    case OGLDEV_KEY_LEFT:
        {
            Vector3f Left = m_target.Cross(m_up);
            Left.Normalize();
            Left *= STEP_SCALE;
            m_pos += Left;
            Ret = true;
        }
        break;

    case OGLDEV_KEY_RIGHT:
        {
            Vector3f Right = m_up.Cross(m_target);
            Right.Normalize();
            Right *= STEP_SCALE;
            m_pos += Right;
            Ret = true;
        }
        break;
        
    case OGLDEV_KEY_PAGE_UP:
        m_pos.y += STEP_SCALE;
        break;
    
    case OGLDEV_KEY_PAGE_DOWN:
        m_pos.y -= STEP_SCALE;
        break;
    }

    return Ret;
}


void OVGCamera::OnMouseMove(float x, float y)
{
  //  const int DeltaX = x - m_mousePos.x;
  //  const int DeltaY = y - m_mousePos.y;

	Vector3f up(0.f, m_AngleV < M_PI ? 1.f : -1.f, 0.f);
	Vector3f fwd = (m_target - m_pos).Normalize();
	Vector3f side = (fwd.Cross(up)).Normalize();
	m_up = (side.Cross(fwd)).Normalize();
	
	m_target.x += up.x * y * 2;
	m_target.y += up.y * y * 2;
	m_target.z += up.z  * y * 2;
	m_target.x -= side.x  * x * 2;
	m_target.y -= side.y  * x * 2;
	m_target.z -= side.z * x  * 2;
	
	//m_mousePos.x = x;
//    m_mousePos.y = y;

    //m_AngleH += (float)DeltaX / 20.0f;
   // m_AngleV += (float)DeltaY / 20.0f;
/*
    if (DeltaX == 0) {
        if (x <= MARGIN) {
        //    m_AngleH -= 1.0f;
            m_OnLeftEdge = true;
        }
        else if (x >= (m_windowWidth - MARGIN)) {
        //    m_AngleH += 1.0f;
            m_OnRightEdge = true;
        }
    }
    else {
        m_OnLeftEdge = false;
        m_OnRightEdge = false;
    }

    if (DeltaY == 0) {
        if (y <= MARGIN) {
            m_OnUpperEdge = true;
        }
        else if (y >= (m_windowHeight - MARGIN)) {
            m_OnLowerEdge = true;
        }
    }
    else {
        m_OnUpperEdge = false;
        m_OnLowerEdge = false;
    }
	*/
    Update();
}

void OVGCamera::OnMouseTurn(float th, float ph)
{
	m_AngleH += 1.f * th;
	m_AngleV -= 1.f * ph;
	if (m_AngleV >= (2 * M_PI) - 0.1)
		m_AngleV = 0.00001f;
	else
		if (m_AngleV <= 0)
			m_AngleV = 2.f * M_PI - 0.1f;
	Update();
}

void OVGCamera::OnRender()
{
	bool ShouldUpdate = false;

	if (m_OnLeftEdge) {
		m_AngleH -= EDGE_STEP;
		ShouldUpdate = true;
	}
	else if (m_OnRightEdge) {
		m_AngleH += EDGE_STEP;
		ShouldUpdate = true;
	}

    if (m_OnUpperEdge) {
        if (m_AngleV > -90.0f) {
            m_AngleV -= EDGE_STEP;
            ShouldUpdate = true;
        }
    }
    else if (m_OnLowerEdge) {
        if (m_AngleV < 90.0f) {
           m_AngleV += EDGE_STEP;
           ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        Update();
    }
}

void OVGCamera::Update()
{
    const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f View(1.0f, 0.0f, 0.0f);
    View.Rotate(m_AngleH, Vaxis);
    View.Normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f Haxis = Vaxis.Cross(View);
    Haxis.Normalize();
    View.Rotate(m_AngleV, Haxis);
       
    m_target = View;
    m_target.Normalize();

    m_up = m_target.Cross(Haxis);
    m_up.Normalize();
/*
	m_pos.x = cos(m_AngleH) * sin(m_AngleV) + m_target.x;
	m_pos.y = cos(m_AngleV)  + m_target.y;
	m_pos.z = sin(m_AngleH) * sin(m_AngleV)  + m_target.z;
	m_up = Vector3f(0.f, m_AngleV < M_PI ? 1.f : -1.f, 0.f);*/

}
