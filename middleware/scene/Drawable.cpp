#include "jmw.h"
#include "Drawable.h"


NS_JMW_BEGIN

GLfloat Drawable::m_vertices[36] = { 0, };

Drawable::Drawable(void) : m_CmdListener(nullptr), m_TouchUpListener(nullptr), m_TouchDownListener(nullptr), m_TouchMoveListener(nullptr)
, m_Img(nullptr)
{
}


Drawable::~Drawable(void)
{
}


void Drawable::OnCalc(sl32 delta)
{
	
}

void Drawable::OnDraw(sl32 delta)
{
	CalcTex();
	DrawTex();
}

bl Drawable::OnCommand(Msg* msg)
{
	if (m_CmdListener)
		return (this->*m_CmdListener)(msg);
	else
		return false;
}


bl Drawable::OnTouchUp(Point pos)
{
	if (m_TouchUpListener)
		return (this->*m_TouchUpListener)(pos);
	else
		return false;
}

bl Drawable::OnTouchDown(Point pos)
{
	if (m_TouchDownListener)
		return (this->*m_TouchDownListener)(pos);
	else
		return false;
}


bl Drawable::OnTouchMove(Point pos)
{
	if (m_TouchMoveListener)
		return (this->*m_TouchMoveListener)(pos);
	else
		return false;
}


void Drawable::CalcTex(void)
{
	if (m_Img)
	{

		GLfloat texSizeW = 1.0f / (GLfloat)m_Img->GettexSizeW();
		GLfloat texSizeH = 1.0f / (GLfloat)m_Img->GettexSizeH();
		GLfloat texX = (GLfloat)m_Img->GettexX() * texSizeW;
		GLfloat texY = (GLfloat)m_Img->GettexY() * texSizeH;
		GLfloat texS = (GLfloat)m_Img->GettexW() * texSizeW;
		GLfloat texT = (GLfloat)m_Img->GettexH() * texSizeH;

		// Z 좌표
		//m_vertices[2] = m_vertices[11] = m_vertices[20] = m_vertices[29] = 0.f;

		// 좌표 1
		m_vertices[0] = 0 - (GetW()*GetAnchorPoint().xpos);
		m_vertices[1] = GetH() - (GetH()*GetAnchorPoint().ypos);

		// 컬러 1
		m_vertices[3] = m_Color[0][0];
		m_vertices[4] = m_Color[0][1];
		m_vertices[5] = m_Color[0][2];
		m_vertices[6] = m_Color[0][3];


		// uv 1
		//m_vertices[7] = 0.f;
		//m_vertices[8] = 1.f;
		m_vertices[7] = texX;   
		m_vertices[8] = texY;  

		// 좌표 2
		m_vertices[9] = 0 - (GetW()*GetAnchorPoint().xpos);
		m_vertices[10] = 0 - (GetH()*GetAnchorPoint().ypos);

		// 컬러 2
		m_vertices[12] = m_Color[1][0];
		m_vertices[13] = m_Color[1][1];
		m_vertices[14] = m_Color[1][2];
		m_vertices[15] = m_Color[1][3];


		// uv 2
		//m_vertices[16] = 0.f;
		//m_vertices[17] = 0.f;
		m_vertices[16] = texX; 
		m_vertices[17] = texY + texT;  

		// 좌표 3
		m_vertices[18] = GetW() - (GetW()*GetAnchorPoint().xpos);
		m_vertices[19] = GetH() - (GetH()*GetAnchorPoint().ypos);

		// 컬러 3
		m_vertices[21] = m_Color[2][0];
		m_vertices[22] = m_Color[2][1];
		m_vertices[23] = m_Color[2][2];
		m_vertices[24] = m_Color[2][3];


		// uv 3
		//m_vertices[25] = 1.f;
		//m_vertices[26] = 1.f;
		m_vertices[25] = texX + texS; 
		m_vertices[26] = texY;        

		// 좌표 4
		m_vertices[27] = GetW() - (GetW()*GetAnchorPoint().xpos);
		m_vertices[28] = 0 - (GetH()*GetAnchorPoint().ypos);

		// 컬러 4
		m_vertices[30] = m_Color[3][0];
		m_vertices[31] = m_Color[3][1];
		m_vertices[32] = m_Color[3][2];
		m_vertices[33] = m_Color[3][3];

		// uv 4
		//m_vertices[34] = 1.f;
		//m_vertices[35] = 0.f;
		m_vertices[34] = texX + texS; 
		m_vertices[35] = texY + texT; 

	}
}


void Drawable::DrawTex(void)
{

	if (m_Img != nullptr && GetVisible())
	{
		if (m_Img->m_TextureID > 0) {

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();


			glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 9, m_vertices);
			if (GetEnableColor()) glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 9, &m_vertices[0] + 3);
			glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 9, &m_vertices[0] + 7);


			glShadeModel(GL_SMOOTH);


			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			if (GetEnableColor()) glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			glTranslatef((GLfloat)GetPosX(), (GLfloat)GetPosY(), 0.f);


			//: 처리할 정점의 개수는 4개
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_Img->m_TextureID);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);


			glDisableClientState(GL_VERTEX_ARRAY);
			if (GetEnableColor()) glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

	}
}



NS_JMW_END
