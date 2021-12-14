#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"
#include <math.h>
#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"
bool textureMode = true;
bool lightMode = true;

//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;


	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}


	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist * cos(fi2) * cos(fi1),
			camDist * cos(fi2) * sin(fi1),
			camDist * sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}


	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);


		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale * 0.08;
		s.Show();

		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale * 1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����

GLdouble* get_Normal(double* point_C, double* point_A, double* point_B, bool reverse) {

	double vector_A[3];
	double vector_B[3];

	if (reverse) {
		for (int i = 0; i < 3; i++) {
			vector_A[i] = point_B[i] - point_C[i];
			vector_B[i] = point_A[i] - point_C[i];
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			vector_A[i] = point_A[i] - point_C[i];
			vector_B[i] = point_B[i] - point_C[i];
		}
	}

	GLdouble vector_Normal[3];

	vector_Normal[0] = vector_A[1] * vector_B[2] - vector_B[1] * vector_A[2];
	vector_Normal[1] = -vector_A[0] * vector_B[2] + vector_B[0] * vector_A[2];
	vector_Normal[2] = vector_A[0] * vector_B[1] - vector_B[0] * vector_A[1];

	double lenght = sqrt(pow(vector_Normal[0], 2) + pow(vector_Normal[1], 2) + pow(vector_Normal[2], 2));

	for (int i = 0; i < 3; i++) {
		vector_Normal[i] /= lenght;
	}

	return vector_Normal;
}


void TRIO(double A[], double B[], double C[], double K[], bool reverse = false)
{


	glBegin(GL_TRIANGLES);

	glNormal3d((get_Normal(A, B, C, reverse))[0], (get_Normal(A, B, C, reverse))[1], (get_Normal(A, B, C, reverse))[2]);
	glColor3d(K[0], K[1], K[2]);

	glVertex3dv(A);
	glVertex3dv(B);
	glVertex3dv(C);

	glEnd();
}


void TRIO_trans(double A[], double B[], double C[], double K[], bool reverse = false)
{

	glBegin(GL_TRIANGLES);

	glNormal3d((get_Normal(A, B, C, reverse))[0], (get_Normal(A, B, C, reverse))[1], (get_Normal(A, B, C, reverse))[2]);
	glColor4d(K[0], K[1], K[2], 0.5);

	glVertex3dv(A);
	glVertex3dv(B);
	glVertex3dv(C);

	glEnd();
}

void BOX(double A[], double B[], double C[], double D[], double K[], bool reverse = false)
{

	glBegin(GL_QUADS);

	glNormal3d((get_Normal(A, B, C, reverse))[0], (get_Normal(A, B, C, reverse))[1], (get_Normal(A, B, C, reverse))[2]);
	glColor3d(K[0], K[1], K[2]);

	glVertex3dv(A);
	glVertex3dv(B);
	glVertex3dv(C);
	glVertex3dv(D);

	glEnd();
}






//����������
void Bulge(double B[], double C[], double K[])
{
	//����� ����������
	double A[] = { (B[0] + C[0]) / 2, (B[1] + C[1]) / 2, B[2] };

	double T[3], T1[3];
	double r;
	int i = 0;
	double v = -90;
	double AZ[3], T1Z[3], TZ[3];
	bool flag = false;

	T1[0] = B[0];
	T1[1] = B[1];
	T1[2] = B[2];

	//������ ���������� ����� ���������� �� ������ �� �����
	r = sqrt((A[0] - B[0]) * (A[0] - B[0]) + (A[1] - B[1]) * (A[1] - B[1]));
	T[2] = A[2];
	while (v <= 90)
	{
		T[0] = A[0] + r * cos(v * 3.141592654 / 180);
		T[1] = A[1] + r * sin(v * 3.141592654 / 180);

		//������ �����
		K[0] = 0.7;
		K[1] = 0.2;
		K[2] = 5;
		TRIO(A, T1, T, K, true);

		TZ[0] = T[0];
		TZ[1] = T[1];
		TZ[2] = T[2] + 5;

		T1Z[0] = T1[0];
		T1Z[1] = T1[1];
		T1Z[2] = T1[2] + 5;

		AZ[0] = A[0];
		AZ[1] = A[1];
		AZ[2] = A[2] + 5;

		//������� �����
		K[0] = 0;
		K[1] = 0;
		K[2] = 125;
		TRIO(AZ, T1Z, TZ, K);

		//������� �����
		K[0] = 0;
		K[1] = 225;
		K[2] = 225;
		if (flag) BOX(T, T1, T1Z, TZ, K, true);

		T1[0] = T[0];
		T1[1] = T[1];
		T1[2] = T[2];
		v = v + 5;

		flag = true;
	}
}

//����������
void Pit(double X[], double B[], double C[], double K[])
{

	//��������� ����� �� ��������� ������, ��� ����� ����������
	double A[] = { B[0] - 5.6,  C[1] + 7.5 , B[2] };

	double T[3] = { NULL }, T1[3] = { NULL };
	double r;
	int i = 0;
	double t = 252.2;
	double AZ[3] = { NULL }, T1Z[3] = { NULL }, TZ[3] = { NULL };
	bool flag = false;

	T1[0] = B[0];
	T1[1] = B[1];
	T1[2] = B[2];

	AZ[0] = X[0];
	AZ[1] = X[1];
	AZ[2] = X[2] + 5;

	r = sqrt((A[0] - C[0]) * (A[0] - C[0]) + (A[1] - C[1]) * (A[1] - C[1]));
	T[2] = A[2];


	while (t <= 316)
	{
		T[0] = A[0] + r * cos(t * 3.141592654 / 180);
		T[1] = A[1] + r * sin(t * 3.141592654 / 180);

		//���������� ������ ������ �����
		//������ �����
		K[0] = 0.7;
		K[1] = 0.2;
		K[2] = 5;
		if (flag) TRIO(X, T1, T, K);


		TZ[0] = T[0];
		TZ[1] = T[1];
		TZ[2] = T[2] + 5;

		T1Z[0] = T1[0];
		T1Z[1] = T1[1];
		T1Z[2] = T1[2] + 5;

		//���������� ������ ������ �����
		//������� �����
		K[0] = 0;
		K[1] = 0;
		K[2] = 125;
		if (flag) TRIO(AZ, T1Z, TZ, K, true);
		//������� �����
		K[0] = 0;
		K[1] = 225;
		K[2] = 225;
		if (flag) BOX(T, T1, T1Z, TZ, K);

		T1[0] = T[0];
		T1[1] = T[1];
		T1[2] = T[2];

		t = t + 7.9;

		flag = true;
	}

}


//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL* ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01 * dx;
		camera.fi2 += -0.01 * dy;
	}


	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k * r.direction.X() + r.origin.X();
		y = k * r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02 * dy);
	}


}

void mouseWheelEvent(OpenGL* ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01 * delta;

}

void keyDownEvent(OpenGL* ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL* ogl, int key)
{

}



GLuint texId;

//����������� ����� ������ ��������
void initRender(OpenGL* ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);


	//������ ����������� ���������  (R G B)
	RGBTRIPLE* texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char* texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);



	//���������� �� ��� ��������
	glGenTextures(1, &texId);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//�������� ������
	free(texCharArray);
	free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH);


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}



void Render(OpenGL* ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//��������������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  

	double z1 = 0, z2 = 5;
	double X1[] = { 12, 1, z1 };
	double X2[] = { 12, 5, z1 };
	double X3[] = { 7, 4, z1 };
	double X4[] = { 8, 6, z1 };
	double X5[] = { 8, 11, z1 };
	double X6[] = { 0, 9, z1 };
	double X7[] = { 4, 5, z1 };
	double X8[] = { 4, 0, z1 };
	double X1Z[] = { 12, 1, z2 };
	double X2Z[] = { 12, 5, z2 };
	double X3Z[] = { 7, 4, z2 };
	double X4Z[] = { 8, 6, z2 };
	double X5Z[] = { 8, 11, z2 };
	double X6Z[] = { 0, 9, z2 };
	double X7Z[] = { 4, 5, z2 };
	double X8Z[] = { 4, 0, z2 };
	double K[] = { 0.7, 0.2, 5 };


	K[0] = 0.7;
	K[1] = 0.2;
	K[2] = 5;

	//������ �����
	//TRIO(X4, X5, X6, K);
	TRIO(X6, X7, X4, K, true);
	TRIO(X4, X7, X3, K, true);
	TRIO(X3, X7, X8, K, true);
	TRIO(X4, X3, X2, K, true);
	TRIO(X1, X2, X3, K, true);



	K[0] = 0;
	K[1] = 225;
	K[2] = 225;

	//������� �����
	BOX(X2, X4, X4Z, X2Z, K);
	//BOX(X4, X5, X5Z, X4Z, K);
	BOX(X4, X5, X5Z, X4Z, K);
	BOX(X6, X7, X7Z, X6Z, K);
	BOX(X7, X8, X8Z, X7Z, K);
	BOX(X8, X3, X3Z, X8Z, K);
	BOX(X3, X1, X1Z, X3Z, K);

	//����������
	K[0] = 0;
	K[1] = 1;
	K[2] = 0;
	Bulge(X2, X1, K);

	//�������
	K[0] = 0;
	K[1] = 1;
	K[2] = 0;
	Pit(X4, X5, X6, K);

	K[0] = 0;
	K[1] = 0;
	K[2] = 125;

	//������� �����
	TRIO_trans(X6Z, X7Z, X4Z, K);
	TRIO_trans(X4Z, X7Z, X3Z, K);
	TRIO_trans(X3Z, X7Z, X8Z, K);
	TRIO_trans(X4Z, X3Z, X2Z, K);
	TRIO_trans(X1Z, X2Z, X3Z, K);


	K[0] = 0;
	K[1] = 1;
	K[2] = 0;













	//������ ��������� ���������� ��������
	//double A[2] = { -4, -4 };
	//double B[2] = { 4, -4 };
	//double C[2] = { 4, 4 };
	//double D[2] = { -4, 4 };

	//glBindTexture(GL_TEXTURE_2D, texId);

	//glColor3d(0.6, 0.6, 0.6);
	//glBegin(GL_QUADS);

	//glNormal3d(0, 0, 1);
	//glTexCoord2d(0, 0);
	//glVertex2dv(A);
	//glTexCoord2d(1, 0);
	//glVertex2dv(B);
	//glTexCoord2d(1, 1);
	//glVertex2dv(C);
	//glTexCoord2d(0, 1);
	//glVertex2dv(D);

	//glEnd();
	////����� ��������� ���������� ��������


   //��������� ������ ������


	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
									//(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R=" << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;

	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

