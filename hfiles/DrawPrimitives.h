
#define GLFW_INCLUDE_GLU
#include <glfw/glfw3.h>
//#include <GL/gl.h>

#include <math.h>


/* PI */
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


void drawSphere(double r, int lats, int longs) {
	int i, j;
	for(i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r * sin(lat0);
		double zr0 = r *  cos(lat0);

		double lat1 = M_PI * (-0.5 + (double) i / lats);
		double z1  = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f((GLfloat)(x * zr0), (GLfloat)(y * zr0), (GLfloat)z0);
			glVertex3f((GLfloat)(x * zr0), (GLfloat)(y * zr0), (GLfloat)z0);
			glNormal3f((GLfloat)(x * zr1), (GLfloat)(y * zr1), (GLfloat)z1);
			glVertex3f((GLfloat)(x * zr1), (GLfloat)(y * zr1), (GLfloat)z1);
		}
		glEnd();
	}
}

void solidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
	glBegin(GL_LINE_LOOP);
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);

	gluCylinder(quadric, base, 0, height, slices, stacks);

	gluDeleteQuadric(quadric);
	glEnd();
}

void drawCube(float length)//
{
	//glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	//glColor4f(0.0f, 1.0f, 1.0f, 0.0f);

	glBegin(GL_POLYGON); //前表面  
	glVertex3f(length, length, length);
	glVertex3f(length, -length, length);
	glVertex3f(-length, -length, length); 
	glVertex3f(-length, length, length);
	glEnd();

	glBegin(GL_POLYGON); //后表面   
	glVertex3f(length, length, -length);
	glVertex3f(length, -length, -length);
	glVertex3f(-length, -length, -length);
	glVertex3f(-length, length, -length);
	glEnd();

	glBegin(GL_POLYGON); //上表面  
	glVertex3f(length, length, -length); 
	glVertex3f(length, length, length);  
	glVertex3f(-length, length, length); 
	glVertex3f(-length, length, -length);
	glEnd();

	glBegin(GL_POLYGON); //下表面    
	glVertex3f(length, -length, -length);
	glVertex3f(length, -length, length);
	glVertex3f(-length, -length, length);
	glVertex3f(-length, -length, -length);
	glEnd();

	glBegin(GL_POLYGON); //左表面    
	glVertex3f(length, length, length);
	glVertex3f(length, length, -length); 
	glVertex3f(length, -length, -length);  
	glVertex3f(length, -length, length);
	glEnd();

	glBegin(GL_POLYGON); //右表面  
	glVertex3f(-length, length, length);
	glVertex3f(-length, length, -length); 
	glVertex3f(-length, -length, -length); 
	glVertex3f(-length, -length, length);
	glEnd();

}

void rectang(float a1, float b1, float c1)
{
	float b = a1;
	float c = b1;
	float a = c1;

	//a高，b长，c宽
	//x上,y右,z屏幕朝外
	//glRotatef(90, 0, 1, 0);

//	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);//白色
	glBegin(GL_POLYGON); //前表面    
	glVertex3f(0, b, 0);
	glVertex3f(a, b, 0);
	glVertex3f(a, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	
//	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);//青色
	glBegin(GL_POLYGON); //上表面  
	glVertex3f(a, 0, 0);
	glVertex3f(a, b, 0);
	glVertex3f(a, b, c);
	glVertex3f(a, 0, c);
	glEnd();
//	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);//黑色


	glBegin(GL_POLYGON); //后表面
	glVertex3f(0,0,c);
	glVertex3f(a,0,c);
	glVertex3f(a,b,c);
	glVertex3f(0, b, c);
	glEnd();
//	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);//黑色

//	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);//黄色
	glBegin(GL_POLYGON); //右表面
	glVertex3f(0, b, c);
	glVertex3f(a, b, c);
	glVertex3f(a, b,0);
	glVertex3f(0, b, 0); 
	glEnd();
//	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);//黑色

	glBegin(GL_POLYGON); //下表面   
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, c);
	glVertex3f(0, b, c);
	glVertex3f(0, b, 0);
	glEnd();
//	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);//黑色

	glBegin(GL_POLYGON);  //左表面
	glVertex3f(0, 0, 0);
	glVertex3f(a, 0, 0);
	glVertex3f(a, 0, c);
	glVertex3f(0, 0, c);
	glEnd();

}

void circle(GLfloat xc, GLfloat yc, GLfloat raio, bool fill)
{
	const GLfloat c = 3.1415926f / 180.0f;
	GLint i;

	glBegin(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP);

	for (i = 0; i <= 360; i += 2)
	{
		float a = i * c;
		glVertex2f(xc + sin(a) * raio, yc + cos(a) * raio);
	}

	glEnd();
}

void eye()
{
	//glRotatef(45, 0, 1, 0);
	drawSphere(0.5, 10, 10);

	glTranslatef(0.0f, 0.0f, -0.1f);//x,y,z
	glColor3f(1.0, 1.0, 1.0);
	drawSphere(0.45, 10, 10);

	glTranslatef(0.0f, 0.0f, -0.11f);//x,y,z
	glColor3f(0.0, 0.0, 0.0);
	drawSphere(0.35, 10, 10);
}

void wingdragon()
{
	glPushMatrix();
	//translateF---x上,y右,z屏幕朝内

	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);//青色
	float headsize = 1;
	float wingsize[4];
	wingsize[0] = 3*headsize;
	wingsize[1] = 0.1*headsize;
	wingsize[2] = wingsize[1];

	float bodysize[3];
	bodysize[0] = headsize/2;
	bodysize[1] = headsize/2;
	bodysize[2] = -headsize*1.5;
	
	rectang(headsize, headsize, headsize);//head
	glTranslatef(0, headsize/4, headsize/4);//body
	rectang(bodysize[0], bodysize[1], bodysize[2]);//body
	glTranslatef(0, headsize / 8, headsize / 8);

	//wings
	rectang(wingsize[0], wingsize[1], -wingsize[2]);
	rectang(-wingsize[0], wingsize[1], -wingsize[2]);
	glTranslatef(0, 0, bodysize[2] / wingsize[3]);
	
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(bodysize[2], 0);
	glVertex2f(0, -wingsize[0]);
	glEnd();

	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(bodysize[2], 0);
	glVertex2f(0, wingsize[0]);
	glEnd();

	glPopMatrix();

	/////////////////////////////eyes
	glPushMatrix();

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.75, 0.25, -0.01);
	glRotatef(45, 0, 0, 1);
	drawCube(0.1);

	glPopMatrix();
	glPushMatrix();

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.75, 0.75, -0.01);
	glRotatef(45, 0, 0, 1);
	drawCube(0.1);

	glPopMatrix();
	/////////////////////////////eyes

	glPushMatrix();
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);//青色
	glTranslatef(0, headsize / 4, -headsize *0.3);//body
	rectang(headsize/2, headsize / 2, headsize *0.3);
	glPopMatrix();

	}

void giya()
{
	//translateF---x上,y右,z屏幕朝内
	glPushMatrix();
	//------------body
	glColor3f(1.0, 0.0, 1.0);
	rectang(1, 0.6, 0.6);
	//------------body
	//------------neck
	glTranslatef(0.0f, 0.0f, 0.2f);
	glRotatef(-120, 0, 0, 1);
	rectang(0.8, 0.2, -0.2);
	//------------neck

	glPopMatrix();

	glPushMatrix();
	//-------------------------head
	glTranslatef(0.8f*sin(60* M_PI /180)-0.2, -0.5f, 0.2f);
	glRotatef(-20, 0, 0, 1);
	rectang(0.5, 0.3, 0.3);
	
	//-----------------------------head
	glPopMatrix();

	glPushMatrix();
	//------------------man
	float high = 0.5;
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.6, 0.3, 0);
	rectang(0.3, 0.6, high);
	glTranslatef(high, 0.1, 0.2);
	rectang(0.1, 0.2, 0.2);

	//------------------------------------spir

	glTranslatef(-high/3, -0.7, -0.4);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	//glColor3f(1.0, 0.0, 0.0);//红色
	glColor3f(1.0f, 0.0f, 0.0f);
	GLUquadricObj* cylinder;
	cylinder = gluNewQuadric();

	gluCylinder(cylinder, 0, 0.08f, 0.8f, 30, 30);

	//------------------man
	glPopMatrix();

}

void trangle(float radios) 
{
	glBegin(GL_TRIANGLES);
	glVertex3f(-radios * cos(30 * M_PI / 180), -radios * sin(30.0f * M_PI / 180), 0.0);
	glVertex3f(radios * cos(30.0f * M_PI / 180), -radios * sin(30.0f * M_PI / 180),0.0);
	glVertex3f(0, radios,0.0);
	glEnd();
	
	glRotatef(180, 0, 0, 1);

	glBegin(GL_TRIANGLES);
	glVertex3f(-radios * cos(30 * M_PI / 180), -radios * sin(30.0f * M_PI / 180), 0.0);
	glVertex3f(radios * cos(30.0f * M_PI / 180), -radios * sin(30.0f * M_PI / 180), 0.0);
	glVertex3f(0, radios, 0.0);
	glEnd();
}

void magician()
{

	//------------------------------stick
	glPushMatrix();
	glRotatef(10, 0, 0, 1);
	glTranslatef(0.05f, 0.5f, 0.0f);
	glRotatef(90, 0, 1, 0);
	GLUquadricObj* cylinder;
	cylinder = gluNewQuadric();
	gluCylinder(cylinder, 0.03f, 0.03f, 0.7f, 30, 30);
	glPopMatrix();
	//star
	glPushMatrix();

	glRotatef(10, 0, 0, 1);
	glTranslatef(0.75f, 0.5f, 0.0f);
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	drawSphere(0.1, 10, 10);

	glRotatef(90, 0, 1, 0);
	gluCylinder(cylinder, 0.06f, 0, 0.2f, 30, 30);
	glRotatef(90, 1, 0, 0);
	gluCylinder(cylinder, 0.06f, 0, 0.2f, 30, 30);
	glRotatef(90, 0, 0, 1);
	gluCylinder(cylinder, 0.06f, 0, 0.2f, 30, 30);
	glRotatef(180, -1, 0, 0);
	gluCylinder(cylinder, 0.06f, 0, 0.2f, 30, 30);
	glRotatef(180, 0, -1, 0);
	gluCylinder(cylinder, 0.06f, 0, 0.2f, 30, 30);
	glRotatef(180, 0, 0, -1);
	gluCylinder(cylinder, 0.06f, 0, 0.2f, 30, 30);
	glPopMatrix();
	//star

	//-----magic bg
	glPushMatrix();
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.75f, 0.0f, 0.5f);
	gluCylinder(cylinder, 0.85f, 0.85f, 0.05f, 30, 30);
	trangle(0.75);
	glPopMatrix();
	//-----magic bg


	//------------------------------stick

	//------------------------------body
	glColor4f(1.0, 1.0, 1.0, 1.0);
	drawSphere(0.3, 10, 10);
	glTranslatef(0.4f, 0.0f, 0.0f);
	drawSphere(0.3, 10, 10);
	glTranslatef(0.5f, 0.0f, 0.0f);
	drawSphere(0.4, 10, 10);
	//------------------------------body
	//------------------------------hat
	glTranslatef(0.2f, 0.0f, 0.0f);

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glColor3f(0.0, 0.0, 1.0);//蓝色

	gluCylinder(cylinder,0.7f,0.3f, 0.2f, 30, 30);
	glPopMatrix();

	glTranslatef(0.2f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);
	gluCylinder(cylinder, 0.3f, 0, 0.5f, 30, 30);
	//------------------------------hat

} 

void forbid() //黑暗大法师
{
	//translateF---x上,y右,z屏幕朝内
	glColor3f(1, 1, 0);
	glPushMatrix();
	drawSphere(0.3, 10, 10);
	glTranslatef(0.5f, 0.0f, 0.0f);
	//xiong
	glScalef(1.0f, 2.0f, 1.0f);
	drawSphere(0.4, 10, 10);
	glPopMatrix();
	//xiong

	glTranslatef(1.0f, 0, 0);
	drawSphere(0.4, 10, 10);
	glTranslatef(0.1f, -0.1f, -0.3f);
	glColor3f(0.0f, 0.0f, 0.0f);
	rectang(0.2f, 0.2f, 0.1f);

	//jian 
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(-0.2f, -0.7f, 0.2f);
	glRotatef(60, 0, 0, 1);
	rectang(0.3f, 0.3f, 0.3f);
	glTranslatef(3.0f, 1.5f, 1.5f);
	//shou
	glPopMatrix();
	//

	//jian
	glPushMatrix();
	glTranslatef(-0.2f, 0.7f, 0.2f);
	glRotatef(120, 0, 0, 1);
	rectang(0.3f, 0.3f, 0.3f);
	glTranslatef(3.0f, -1.5f, 1.5f);
	//shou
	glPopMatrix();
	//

	glPushMatrix();
	glTranslatef(-0.8, 0.8, 0.4);
	glRotatef(160, 0, 0, 1);
	glScalef(3.0f, 1.0f, 1.0f);
	drawSphere(0.2, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, -0.8, 0.4);
	glRotatef(30, 0, 0, 1);
	glScalef(3.0f, 1.0f, 1.0f);
	drawSphere(0.2, 10, 10);
	glPopMatrix();
}

void shaji()//沙基
{
	//------------------------------body
	glColor3f(1.0, 0.0, 1.0);//品红色
	drawSphere(0.3, 10, 10);
	glTranslatef(0.4f, 0.0f, 0.0f);
	drawSphere(0.3, 10, 10);
	glTranslatef(0.5f, 0.0f, 0.0f);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	drawSphere(0.4, 10, 10);
	//------------------------------body

	//------------------------------hat
	glTranslatef(0.2f, 0.0f, 0.0f);

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glColor3f(1.0, 0.0, 1.0);//品红色

	GLUquadricObj* cylinder;
	cylinder = gluNewQuadric();
	gluCylinder(cylinder, 0.7f, 0.3f, 0.2f, 30, 30);
	glPopMatrix();

	glTranslatef(0.2f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);
	gluCylinder(cylinder, 0.3f, 0, 0.5f, 30, 30);
	//------------------------------hat


}

	
void drawSnowman( bool female = 0)
{
	glScalef(0.03f, 0.03f, 0.03f);
	// draw 3 white spheres
	glColor4f( 1.0, 1.0, 1.0, 1.0 );
	drawSphere( 0.8, 10, 10 );
	glTranslatef( 0.0f, 0.8f, 0.0f );
	drawSphere( 0.6, 10, 10 );
	if(female)
	{
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(-0.2f, 0.05f, 0.3f);
		drawSphere( 0.32, 10, 10 );
		glTranslatef(0.4f, 0, 0);
		drawSphere( 0.32, 10, 10 );
		glPopMatrix();
	}
	glTranslatef( 0.0f, 0.6f, 0.0f );
	drawSphere( 0.4, 10, 10 );

	// draw the eyes
	glPushMatrix();
	drawCube(0.5);
	glPopMatrix();
}
