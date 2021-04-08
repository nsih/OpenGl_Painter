/* This program illustrates the use of the glut library for
interfacing with a window system */


#define _CRT_SECURE_NO_WARNINGS

#include <glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <memory.h>




#define NULL 0
#define LINE 1
#define RECTANGLE 2
#define TRIANGLE  3
#define POINTS 4
#define Text 5
#define CIRCLE 6
#define ELLIPSE 7
#define DRAW 8
#define SPRAY 9
#define ERASE 10
#define CUT 11
#define SAVE 12
#define LOAD 13
#define IMG 14


#define PI 3.141592

void mouse(int, int, int, int);
void mouse_motion(int x, int y);
void key(unsigned char, int, int);
void display(void);
void drawSquare(int, int);
void myReshape(GLsizei, GLsizei);

void myinit(void);

void screen_box(int, int, int);
void right_menu(int);
void middle_menu(int);
void color_menu(int);
void pixel_menu(int);
void fill_menu(int);
void draw_menu(int);
void bcolor_menu(int);
void line_mode_menu(int);
void line_mode(int);
void alias_menu(int);
void alias_mode(int);
int pick(int, int);


void Save();
void Load();
void Img();


/* globals */

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat size = 3.0;   /* half side length of square */
int draw_mode = 0; /* drawing mode */
int lm_idx; /* line mode */
int am_idx; /* anti_alias mode */
int rx, ry; /*raster position*/


GLfloat r = 1.0, g = 1.0, b = 1.0; /* drawing color */
GLfloat r2 = 0.8, g2 = 0.8, b2 = 0.8; /* background color */

int fill = 0; /* fill flag */

void drawSquare(int x, int y)
{

	y = wh - y;
	glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);
	glBegin(GL_POLYGON);
	glVertex2f(x + size, y + size);
	glVertex2f(x - size, y + size);
	glVertex2f(x - size, y - size);
	glVertex2f(x + size, y - size);
	glEnd();
}

void plot_b(int cx, int cy, int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(cx + x, cy + y);
	glVertex2i(cx + x, cy - y);
	glVertex2i(cx - x, cy + y);
	glVertex2i(cx - x, cy - y);
	glVertex2i(cx + y, cy + x);
	glVertex2i(cx + y, cy - x);
	glVertex2i(cx - y, cy + x);
	glVertex2i(cx - y, cy - x);
	glEnd();
	glFlush();
}

void drawCircle(int x1, int y1, int x2, int y2)
{
	int r = (int)(sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2)));

	int pk = 3 - 2 * r;
	int x = 0;
	int y = r;

	plot_b(x1, y1, x, y);
	while (x <= y)
	{
		if (pk < 0) {
			pk = pk + 4 * x + 6;
		}
		else {
			pk = pk + 4 * (x - y) + 10;
			y--;
		}
		x++;
		plot_b(x1, y1, x, y);
	}
}





/// </summary>
void Img()
{
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bmpinfo0;
	FILE* file = fopen("img.bmp", "rb");
	if (file == NULL)
		return;


	fread(&header, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpinfo0, sizeof(BITMAPINFOHEADER), 1, file); //헤더에 bmp 정보저장

	DWORD bitsize = bmpinfo0.biSizeImage; // bits의 크기 지정
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * ww * (wh - ww / 11) * 24];
	//출력할 bits(화소) GLubte(GLu byte) 할당
	fread(bits, 1, bitsize, file);


	glEnableClientState(GL_VERTEX_ARRAY);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//glDrawPixels(300, 413, GL_RGB, GL_UNSIGNED_BYTE, bits); //파란색으로 나옴?
	glDrawPixels(376, 434, GL_BGR_EXT, GL_UNSIGNED_BYTE, bits);
	glFlush();

	fclose(file);
	delete[]bits;
	/* windows.h 비트맵 클래스 헤더
	FILE* file; //불러올 파일
	BITMAPINFOHEADER bmpinfo; //파일헤더
	BITMAPFILEHEADER header; //파일에서 정보 헤더

	fread(&header, sizeof(BITMAPFILEHEADER), 1, file); //파일 정보 세이브
	fread(&bmpinfo, sizeof(BITMAPINFO), 1, file); //파일 정보 세이브


	DWORD bitsize = bmpinfo.biSizeImage;
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * bitsize * 3];
	//출력할 bits(화소) GLubte(1byte) 할당
	fread(bits, 1, bitsize, file);

	glEnableClientState(GL_VERTEX_ARRAY);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glDrawPixels(ww, wh - ww / 11, GL_BGR_EXT, GL_UNSIGNED_BYTE, bits);

	glFlush();
	fclose(file);
	//    fread(bits, 1, bitsize, file);
	*/
}



void Save()
{
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bmpinfo;
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * ww * (wh - ww / 11) * 3];
	FILE* file = fopen("save.bmp", "wb"); //쓰기

	if (file == NULL)
		return;


	glReadPixels(0, 0, ww, (wh - ww / 11), GL_BGR_EXT, GL_UNSIGNED_BYTE, bits);

	memset(&header, 0, sizeof(header));
	memset(&bmpinfo, 0, sizeof(bmpinfo));

	header.bfType = 'MB';
	header.bfSize = sizeof(header) + sizeof(bmpinfo) + ww * (wh - ww / 11) * 3;
	header.bfOffBits = sizeof(header) + sizeof(bmpinfo); //init

	bmpinfo.biSize = sizeof(bmpinfo);//
	bmpinfo.biWidth = ww;
	bmpinfo.biHeight = (wh - ww / 11);
	bmpinfo.biPlanes = 1;
	bmpinfo.biBitCount = 24;
	bmpinfo.biSizeImage = ww * (wh - ww / 11) * 3; //저장할 이미지정보

	fwrite(&header, sizeof(header), 1, file);
	fwrite(&bmpinfo, sizeof(bmpinfo), 1, file);
	fwrite(bits, sizeof(unsigned char), (wh - ww / 11) * ww * 3, file);
	fclose(file);

	delete[]bits;
}

void Load()
{
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bmpinfo;
	FILE* file;

	file = fopen("save.bmp", "rb"); //file 읽음

	if (file == NULL)
		return;


	fread(&header, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, file); //헤더에 정보 복사

	DWORD bitsize = bmpinfo.biSizeImage; // bits의 크기 지정
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * bitsize * 3];


	fread(bits, 1, bitsize, file);

	glEnableClientState(GL_VERTEX_ARRAY);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glDrawPixels(ww, wh - ww / 11, GL_BGR_EXT, GL_UNSIGNED_BYTE, bits);
	glFlush();
	fclose(file);

	delete[]bits;
}



//////////////////////////////




void SRed()
{
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bmpinfo;
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * ww * (wh - ww / 11) * 3];
	FILE* file = fopen("Rsave.bmp", "wb"); //쓰기

	if (file == NULL)
		return;


	glReadPixels(0, 0, ww, (wh - ww / 11), GL_RED, GL_UNSIGNED_BYTE, bits);

	memset(&header, 0, sizeof(header));
	memset(&bmpinfo, 0, sizeof(bmpinfo));

	header.bfType = 'MB';
	header.bfSize = sizeof(header) + sizeof(bmpinfo) + ww * (wh - ww / 11) * 3;
	header.bfOffBits = sizeof(header) + sizeof(bmpinfo); //init

	bmpinfo.biSize = sizeof(bmpinfo);//
	bmpinfo.biWidth = ww;
	bmpinfo.biHeight = (wh - ww / 11);
	bmpinfo.biPlanes = 1;
	bmpinfo.biBitCount = 24;
	bmpinfo.biSizeImage = ww * (wh - ww / 11) * 3; //저장할 이미지정보

	fwrite(&header, sizeof(header), 1, file);
	fwrite(&bmpinfo, sizeof(bmpinfo), 1, file);
	fwrite(bits, sizeof(unsigned char), (wh - ww / 11) * ww * 3, file);
	fclose(file);

	free(bits);
}
void Red()
{
	SRed();

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bmpinfo;
	FILE* file;

	file = fopen("Rsave.bmp", "rb"); //file 읽음

	if (file == NULL)
		return;


	fread(&header, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, file); //헤더에 정보 복사

	DWORD bitsize = bmpinfo.biSizeImage; // bits의 크기 지정
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * bitsize * 3];


	fread(bits, 1, bitsize, file);

	glEnableClientState(GL_VERTEX_ARRAY);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glDrawPixels(ww, wh - ww / 11, GL_RED, GL_UNSIGNED_BYTE, bits);
	//glDrawPixels(374, 438, GL_BGR_EXT, GL_UNSIGNED_BYTE, bits);
	glFlush();
	fclose(file);

	free(bits);
}

void SGreen()
{
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bmpinfo;
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * ww * (wh - ww / 11) * 3];
	FILE* file = fopen("Gsave.bmp", "wb"); //쓰기

	if (file == NULL)
		return;


	glReadPixels(0, 0, ww, (wh - ww / 11), GL_GREEN, GL_UNSIGNED_BYTE, bits);

	memset(&header, 0, sizeof(header));
	memset(&bmpinfo, 0, sizeof(bmpinfo));

	header.bfType = 'MB';
	header.bfSize = sizeof(header) + sizeof(bmpinfo) + ww * (wh - ww / 11) * 3;
	header.bfOffBits = sizeof(header) + sizeof(bmpinfo); //init

	bmpinfo.biSize = sizeof(bmpinfo);//
	bmpinfo.biWidth = ww;
	bmpinfo.biHeight = (wh - ww / 11);
	bmpinfo.biPlanes = 1;
	bmpinfo.biBitCount = 24;
	bmpinfo.biSizeImage = ww * (wh - ww / 11) * 3; //저장할 이미지정보

	fwrite(&header, sizeof(header), 1, file);
	fwrite(&bmpinfo, sizeof(bmpinfo), 1, file);
	fwrite(bits, sizeof(unsigned char), (wh - ww / 11) * ww * 3, file);
	fclose(file);

	free(bits);
}
void Green()
{
	SGreen();

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bmpinfo;
	FILE* file;

	file = fopen("Gsave.bmp", "rb"); //file 읽음

	if (file == NULL)
		return;


	fread(&header, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, file); //헤더에 정보 복사

	DWORD bitsize = bmpinfo.biSizeImage; // bits의 크기 지정
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * bitsize * 24];


	fread(bits, 1, bitsize, file);

	glEnableClientState(GL_VERTEX_ARRAY);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glDrawPixels(ww, wh - ww / 11, GL_GREEN, GL_UNSIGNED_BYTE, bits);
	//glDrawPixels(374, 438, GL_BGR_EXT, GL_UNSIGNED_BYTE, bits);
	glFlush();
	fclose(file);

	free(bits);
}

void SBlue()
{
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bmpinfo;
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * ww * (wh - ww / 11) * 3];
	FILE* file = fopen("Bsave.bmp", "wb"); //쓰기

	if (file == NULL)
		return;


	glReadPixels(0, 0, ww, (wh - ww / 11), GL_BLUE, GL_UNSIGNED_BYTE, bits);

	memset(&header, 0, sizeof(header));
	memset(&bmpinfo, 0, sizeof(bmpinfo));

	header.bfType = 'MB';
	header.bfSize = sizeof(header) + sizeof(bmpinfo) + ww * (wh - ww / 11) * 3;
	header.bfOffBits = sizeof(header) + sizeof(bmpinfo); //init

	bmpinfo.biSize = sizeof(bmpinfo);//
	bmpinfo.biWidth = ww;
	bmpinfo.biHeight = (wh - ww / 11);
	bmpinfo.biPlanes = 1;
	bmpinfo.biBitCount = 24;
	bmpinfo.biSizeImage = ww * (wh - ww / 11) * 3; //저장할 이미지정보

	fwrite(&header, sizeof(header), 1, file);
	fwrite(&bmpinfo, sizeof(bmpinfo), 1, file);
	fwrite(bits, sizeof(unsigned char), (wh - ww / 11) * ww * 3, file);
	fclose(file);

	free(bits);
}
void Blue()
{
	SBlue();

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bmpinfo;
	FILE* file;

	file = fopen("Bsave.bmp", "rb"); //file 읽음

	if (file == NULL)
		return;


	fread(&header, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, file); //헤더에 정보 복사

	DWORD bitsize = bmpinfo.biSizeImage; // bits의 크기 지정
	GLubyte* bits = new GLubyte[sizeof(GLubyte) * bitsize * 24];


	fread(bits, 1, bitsize, file);

	glEnableClientState(GL_VERTEX_ARRAY);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glDrawPixels(ww, wh - ww / 11, GL_BLUE, GL_UNSIGNED_BYTE, bits);
	//glDrawPixels(374, 438, GL_BGR_EXT, GL_UNSIGNED_BYTE, bits);
	glFlush();
	fclose(file);

	free(bits);
}





///////////////////////




///




void myReshape(GLsizei w, GLsizei h)
{

	/* adjust clipping box */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* adjust viewport and  clear */

	glViewport(0, 0, w, h);
	glClearColor(r2, g2, b2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	display();
	glFlush();

	/* set global size for use by drawing routine */

	ww = w;
	wh = h;
}

void myinit(void)
{

	glViewport(0, 0, ww, wh);


	/* Pick 2D clipping window to match size of X window
	This choice avoids having to scale object coordinates
	each time window is resized */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)ww, 0.0, (GLdouble)wh, -1.0, 1.0);

	/* set clear color to black and clear window */

	glClearColor(r2, b2, g2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}



void mouse(int btn, int state, int x, int y)
{
	static int count;
	int where;
	static int xp[2], yp[2];
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		where = pick(x, y);
		glColor3f(r, g, b);
		line_mode(lm_idx);
		alias_mode(am_idx);
		if (where != 0)
		{
			count = 0;
			draw_mode = where;
		}
		else switch (draw_mode)
		{
		case(LINE):
			if (count == 0)
			{
				count++;
				xp[0] = x;
				yp[0] = y;
			}
			else
			{
				glBegin(GL_LINES);
				glVertex2i(x, wh - y);
				glVertex2i(xp[0], wh - yp[0]);
				glEnd();
				draw_mode = 0;
				count = 0;
			}
			break;
		case(RECTANGLE):
			if (count == 0)
			{
				count++;
				xp[0] = x;
				yp[0] = y;
			}
			else
			{
				if (fill) glBegin(GL_POLYGON);
				else glBegin(GL_LINE_LOOP);
				glVertex2i(x, wh - y);
				glVertex2i(x, wh - yp[0]);
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[0], wh - y);
				glEnd();
				draw_mode = 0;
				count = 0;
			}
			break;
		case (TRIANGLE):
			switch (count)
			{
			case(0):
				count++;
				xp[0] = x;
				yp[0] = y;
				break;
			case(1):
				count++;
				xp[1] = x;
				yp[1] = y;
				break;
			case(2):
				if (fill) glBegin(GL_POLYGON);
				else glBegin(GL_LINE_LOOP);
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[1], wh - yp[1]);
				glVertex2i(x, wh - y);
				glEnd();
				draw_mode = 0;
				count = 0;
			}
			break;
		case(POINTS):
		{
			drawSquare(x, y);
			count++;
		}
		break;
		case(Text):
		{
			rx = x;
			ry = wh - y;
			glRasterPos2i(rx, ry);
			count = 0;
		}
		case(CIRCLE):
		{
			float a, b;
			float Angle;

			if (count == 0)
			{
				count++;
#ifndef OSW  //Bresenham's algorithm
				xp[0] = x;
				yp[0] = wh - y;
#else
				xp[0] = x;
				yp[0] = y;
#endif
			}
			else
			{
#ifndef OSW
				drawCircle(xp[0], yp[0], x, wh - y);

#else
				a = abs(xp[0] - x);
				b = abs(yp[0] - y);

				if (fill) glBegin(GL_POLYGON);
				else glBegin(GL_POINTS);

				for (Angle = 0.0; Angle < 360; Angle = Angle + 0.1f)
				{
					float radian = Angle * (PI / 180);
					// 각도는 60분법으로 되어있기때문에 radian값으로 변경하여 사용  1 radian = PI/180
					glVertex2f(((a / 2) * cos(radian)) + (xp[0] + x) / 2, ((b / 2) * sin(radian)) + wh - (yp[0] + y) / 2);

				}
				glEnd();
#endif
				draw_mode = 0;
				count = 0;
			}
			break;


		case(ELLIPSE):
		{
			float a, b;
			float Angle;

			if (count == 0)
			{
				count++;
				xp[0] = x;
				yp[0] = y;
			}
			else
			{
				a = abs(xp[0] - x);
				b = abs(yp[0] - y);

				if (fill) glBegin(GL_POLYGON);
				else glBegin(GL_POINTS);

				for (Angle = 0.0; Angle < 360; Angle = Angle + 0.1f)
				{
					float radian = Angle * (PI / 180);
					// 각도는 60분법으로 되어있기때문에 radian값으로 변경하여 사용  1 radian = PI/180
					glVertex2f(((a / 2) * cos(radian)) + (xp[0] + x) / 2, ((b / 2) * sin(radian)) + wh - (yp[0] + y) / 2);

				}
				glEnd();
				draw_mode = 0;
				count = 0;
			}
			break;
		}


		}
		case(CUT):
		{
			glColor3f(r2, g2, b2);
			if (count == 0)
			{
				count++;
				xp[0] = x;
				yp[0] = y;
			}
			else
			{
				glBegin(GL_POLYGON);
				glVertex2i(x, wh - y);
				glVertex2i(x, wh - yp[0]);
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[0], wh - y);
				glEnd();
				count = 0;
			}
			break;
		}


		case (SAVE):
		{
			Save();
		}break;


		case (LOAD):
		{
			Load();
		}break;


		case (IMG):
		{
			Img();
		}


		}
		glPopAttrib();
		glFlush();
	}
}


void mouse_motion(int x, int y)
{
	if (DRAW == draw_mode)
	{
		glColor3f(r, g, b);
		y = wh - y;
		if (y < (wh - ww / 10))
		{
			glBegin(GL_POLYGON);
			for (float Angle = 0.0; Angle < 360; Angle++)
			{
				float radian = Angle * (PI / 180);
				glVertex2f(size * cos(radian) + x, size * sin(radian) + y);
			}
			glEnd();
			glFlush();
		}
	}
	else if (ERASE == draw_mode)
	{
		glColor3f(r2, g2, b2);
		y = wh - y;
		if (y < (wh - ww / 10) - size)
		{
			glBegin(GL_POLYGON);
			for (float Angle = 0.0; Angle < 360; Angle++)
			{
				float radian = Angle * (PI / 180);
				glVertex2f(size * cos(radian) + x, size * sin(radian) + y);
			}
			glEnd();
			glFlush();
		}
	}
	else if (SPRAY == draw_mode)
	{

		glColor3f(r, g, b);
		y = wh - y;
		if (y < (wh - ww / 10))
		{
			glBegin(GL_POINTS);

			for (float Angle = -30.0; Angle < 330; Angle = Angle + 45)
			{
				float radian = Angle * (PI / 180);
				glVertex2f(rand() % (int)(size * 4) * cos(radian) + x, rand() % (int)(size * 4) * sin(radian) + y);

			}
			glEnd();
			glFlush();
		}
	}
}

int pick(int x, int y)
{
	y = wh - y;
	if (y < wh - ww / 10) return 0;
	else if (x < ww / 10) return LINE;
	else if (x < 2 * ww / 10) return RECTANGLE;
	else if (x < 3 * ww / 10) return TRIANGLE;
	else if (x < 4 * ww / 10) return POINTS;
	else if (x < 5 * ww / 10) return Text;
	else if (x < 6 * ww / 10) return CIRCLE;
	else if (x < 7 * ww / 10) return ELLIPSE;
	else if (x < 8 * ww / 10) return SAVE;
	else if (x < 9 * ww / 10) return LOAD;
	else if (x < ww) return IMG;
	else return 0;
}

void screen_box(int x, int y, int s)
{
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x + s, y);
	glVertex2i(x + s, y + s);
	glVertex2i(x, y + s);
	glEnd();
}

void right_menu(int id)
{
	if (id == 1) exit(0);
}

void draw_menu(int id)
{
	if (id == 1)	draw_mode = DRAW;
	else if (id == 2)	draw_mode = SPRAY;
	else draw_mode = 0;

}

void erase_menu(int id)
{
	if (id == 1)	draw_mode = ERASE;
	else if (id == 2)	draw_mode = CUT;
	else display();
}

void line_mode_menu(int id)
{
	lm_idx = id;
}

void line_mode(int id)
{

	if (id == 1) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(4, 0xaaaa);
	}
	else if (id == 2) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x33ff);
	}
	else if (id == 3) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x57ff);
	}
	else
		glDisable(GL_LINE_STIPPLE);
}



void alias_menu(int id)
{
	am_idx = id;
}
void alias_mode(int id)
{
	if (id == 1) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

	}
	else {
		glDisable(GL_BLEND);

		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}
}


void filter_menu(int id)
{
	if (id == 1) Red();
	else if (id == 2)	Green();
	else if (id == 3)	Blue();
	else display();
}

void middle_menu(int id)
{

}

void color_menu(int id)
{
	if (id == 1) { r = 1.0; g = 0.0; b = 0.0; }
	else if (id == 2) { r = 0.0; g = 1.0; b = 0.0; }
	else if (id == 3) { r = 0.0; g = 0.0; b = 1.0; }
	else if (id == 4) { r = 0.0; g = 1.0; b = 1.0; }
	else if (id == 5) { r = 1.0; g = 0.0; b = 1.0; }
	else if (id == 6) { r = 1.0; g = 1.0; b = 0.0; }
	else if (id == 7) { r = 1.0; g = 1.0; b = 1.0; }
	else if (id == 8) { r = 0.0; g = 0.0; b = 0.0; }
}

//배경화면
void bcolor_menu(int id)
{
	if (id == 1) { r2 = 0.8; g2 = 0.0; b2 = 0.0; }
	else if (id == 2) { r2 = 0.0; g2 = 0.8; b2 = 0.0; }
	else if (id == 3) { r2 = 0.0; g2 = 0.0; b2 = 0.8; }
	else if (id == 4) { r2 = 0.0; g2 = 0.8; b2 = 0.8; }
	else if (id == 5) { r2 = 0.8; g2 = 0.0; b2 = 0.8; }
	else if (id == 6) { r2 = 0.8; g2 = 0.8; b2 = 0.0; }
	else if (id == 7) { r2 = 0.8; g2 = 0.8; b2 = 0.8; }
	else if (id == 8) { r2 = 0.0; g2 = 0.0; b2 = 0.0; }
	display();
}


void pixel_menu(int id)
{
	if (id == 1) size = 2 * size;
	else if (size > 1) size = size / 2;
}

void fill_menu(int id)
{
	if (id == 1) fill = 1;
	else fill = 0;
}

void key(unsigned char k, int xx, int yy)
{
	if (draw_mode != Text) return;
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i(rx, ry);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, k);
	/*glutStrokeCharacter(GLUT_STROKE_ROMAN,i); */
	rx += glutBitmapWidth(GLUT_BITMAP_9_BY_15, k);

}

void display(void)
{
	int shift = 0;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glClearColor(r2, g2, b2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	screen_box(0, wh - ww / 10, ww / 10);				// LINE
	glColor3f(1.0, 1.0, 0.0);
	screen_box(ww / 10, wh - ww / 10, ww / 10);			//RECTANGLE
	glColor3f(1.0, 0.0, 1.0);
	screen_box(2 * ww / 10, wh - ww / 10, ww / 10);		//TRIAGLES
	glColor3f(1.0, 0.0, 0.0);
	screen_box(3 * ww / 10, wh - ww / 10, ww / 10);		//POINTS
	glColor3f(0.0, 1.0, 1.0);
	screen_box(4 * ww / 10, wh - ww / 10, ww / 10);		//Text
	glColor3f(0.0, 1.0, 0.0);
	screen_box(5 * ww / 10, wh - ww / 10, ww / 10);		//CIRCLE




	glColor3f(0.3, 1.0, 0.8);
	screen_box(6 * ww / 10, wh - ww / 10, ww / 10);		//ELLIPSE

	glColor3f(0.3, 0.5, 0.8);
	screen_box(7 * ww / 10, wh - ww / 10, ww / 10);		//

	glColor3f(0.8, 1.0, 0.4);
	screen_box(8 * ww / 10, wh - ww / 10, ww / 10);

	glColor3f(0.5, 1.0, 0.4);
	screen_box(9 * ww / 10, wh - ww / 10, ww / 10);





	glColor3f(0.0, 0.0, 0.0);
	screen_box(ww / 10 + ww / 40, wh - ww / 10 + ww / 40, ww / 20);


	glBegin(GL_LINES);
	glVertex2i(wh / 40, wh - ww / 20);
	glVertex2i(wh / 40 + ww / 20, wh - ww / 20);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2i(ww / 5 + ww / 40, wh - ww / 10 + ww / 40);
	glVertex2i(ww / 5 + ww / 20, wh - ww / 40);
	glVertex2i(ww / 5 + 3 * ww / 40, wh - ww / 10 + ww / 40);
	glEnd();
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glVertex2i(3 * ww / 10 + ww / 20, wh - ww / 20);
	glEnd();



	glBegin(GL_POLYGON);
	for (int i = 0.0; i < 360; i++)
		glVertex3f(ww / 30 * cos(i) + 11 * ww / 20, ww / 30 * sin(i) + wh - ww / 20, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	for (int i = 0.0; i < 360; i++)
		glVertex3f(ww / 30 * cos(i) + 13 * ww / 20, ww / 45 * sin(i) + wh - ww / 20, 0.0);
	glEnd();






	glRasterPos2i(4 * ww / 10, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(4 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'B');
	glRasterPos2i(4 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');


	glRasterPos2i(7 * ww / 10, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'S');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'S');
	glRasterPos2i(7 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(7 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'V');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'V');
	glRasterPos2i(7 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'E');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'E');


	glRasterPos2i(8 * ww / 10, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'L');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'L');
	glRasterPos2i(8 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'O');
	glRasterPos2i(8 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(8 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'D');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'D');


	glRasterPos2i(9 * ww / 10, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'I');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'I');
	glRasterPos2i(9 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'M');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'M');
	glRasterPos2i(9 * ww / 10 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'G');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'G');











	glFlush();
	glPopAttrib();
}



int main(int argc, char** argv)
{
	int c_menu, p_menu, f_menu, d_menu, e_menu, b_menu, a_menu, l_menu, ff_menu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("square");
	glutDisplayFunc(display);
	b_menu = glutCreateMenu(bcolor_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Cyan", 4);
	glutAddMenuEntry("Magenta", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("White", 7);
	glutAddMenuEntry("Black", 8);
	c_menu = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Cyan", 4);
	glutAddMenuEntry("Magenta", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("White", 7);
	glutAddMenuEntry("Black", 8);
	p_menu = glutCreateMenu(pixel_menu);
	glutAddMenuEntry("increase pixel size", 1);
	glutAddMenuEntry("decrease pixel size", 2);
	f_menu = glutCreateMenu(fill_menu);
	glutAddMenuEntry("fill on", 1);
	glutAddMenuEntry("fill off", 2);
	d_menu = glutCreateMenu(draw_menu);
	glutAddMenuEntry("draw", 1);
	glutAddMenuEntry("spray", 2);
	glutAddMenuEntry("off", 3);
	e_menu = glutCreateMenu(erase_menu);
	glutAddMenuEntry("earse", 1);
	glutAddMenuEntry("cut", 2);
	glutAddMenuEntry("clear", 3);
	l_menu = glutCreateMenu(line_mode_menu);
	glutAddMenuEntry("_ _ _ _ _ _ _", 1);
	glutAddMenuEntry("_._._._._._._", 2);
	glutAddMenuEntry("__.__.__.__._", 3);
	glutAddMenuEntry("_____________", 4);
	a_menu = glutCreateMenu(alias_menu);
	glutAddMenuEntry("anti_aliasing on", 1);
	glutAddMenuEntry("anti_aliasing off", 2);

	ff_menu = glutCreateMenu(filter_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);







	glutCreateMenu(right_menu);
	glutAddMenuEntry("quit", 1);
	glutAddSubMenu("brush", d_menu);
	glutAddSubMenu("erase", e_menu);
	glutAddSubMenu("line_mode", l_menu);
	glutAddSubMenu("anti_alias", a_menu);
	glutAddSubMenu("filter", ff_menu);





	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutCreateMenu(middle_menu);
	glutAddSubMenu("background color", b_menu);
	glutAddSubMenu("Colors", c_menu);
	glutAddSubMenu("Pixel Size", p_menu);
	glutAddSubMenu("Fill", f_menu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	myinit();
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutMainLoop();

}
