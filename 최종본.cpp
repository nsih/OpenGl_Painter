/* This program illustrates the use of the glut library for
interfacing with a window system */

#pragma comment (lib,"glut32.lib")

#define NULL 0
#define LINE 1
#define RECTANGLE 2
#define TRIANGLE  3
#define POINTS 4
#define TEXT 5
#define POLYGON 6
#define CIRCLE 7

#include <glut.h>

#include <stdlib.h>
#include <math.h>
#include <iostream>

using std::cout;
using std::endl;

//#define PI 3.14159265 
const float PI = 3.14159265;

void mouse(int, int, int, int);
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
int pick(int, int);

/* globals */

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat wwh = 500.0, www = 500.0;
GLfloat size = 3.0;   /* half side length of square */
int draw_mode = 0; /* drawing mode */
int rx, ry; /*raster position*/

GLfloat r = 1.0, g = 1.0, b = 1.0; /* drawing color */
int fill = 0; /* fill flag */

int TopLeftX, TopLeftY, BottomRightX, BottomRightY;

GLfloat LineWidth = 0.5f;
bool Line_Flag = false;
int xl[50], yl[50];
int cline;
int pfirst, cfirst;
int ccount;
void drawLine()
{

    glColor3f(r, g, b);
    //glLineWidth(LineWidth);
    if (cline < 1)
        return;
    cout << "drawLine  " << cline << endl;
    glBegin(GL_LINES);
    for (int i = 0; i < cline - 1; i++)
    {
        glVertex2i(xl[i], wh - yl[i]);
        glVertex2i(xl[i + 1], wh - yl[i + 1]);
    }

    glEnd();

}


void drawLine1()
{
    //cout << "drawLine1" << endl;
    glColor3f(r, g, b);
    //glLineWidth(LineWidth);
    glBegin(GL_LINES);

    glVertex2i(TopLeftX, wh - TopLeftY);
    glVertex2i(BottomRightX, wh - BottomRightY);

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

void drawSquare(int x, int y)
{

    y = wh - y;
    //glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);
    //glColor3ub(255, 0, 0);//R, G, B
    glBegin(GL_POLYGON);
    glVertex2f(x + size, y + size);
    glVertex2f(x - size, y + size);
    glVertex2f(x - size, y - size);
    glVertex2f(x + size, y - size);
    glEnd();
}




/* rehaping routine called whenever window is resized
or moved */

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
    glClearColor(0.8, 0.8, 0.8, 1.0);
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

    glClearColor(0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}



void mouse(int btn, int state, int x, int y)
{
    static int count;
    static int xp[1], yp[1];
    int where;

    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        where = pick(x, y);
        cout << "where " << where << endl;
        glColor3f(r, g, b);
        if (where != 0)
        {
            count = 0;
            ccount = 0;
            draw_mode = where;
        }

        switch (draw_mode)
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
            Line_Flag = false;
            cout << "Re " << endl;
            if (cline)
                cline = 0;
            if (pfirst)
                pfirst = 0;
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
            cout << "Triangle " << endl;
            Line_Flag = false;
            if (cline)
                cline = 0;
            if (pfirst)
                pfirst = 0;
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
            cout << "Po " << endl;
            Line_Flag = false;
            if (cline)
                cline = 0;
            if (pfirst)
                pfirst = 0;
            drawSquare(x, y);
            //drawCircle(x, y);
            count++;
        }
        break;
        case(TEXT):
        {
            cout << "Te " << endl;
            if (cline)
                cline = 0;
            if (pfirst)
                pfirst = 0;
            Line_Flag = false;
            rx = x;
            ry = wh - y;
            glRasterPos2i(rx, ry);
            count = 0;
        }
        case (POLYGON):

            if (pfirst == 0)
            {
                //Line_Flag = true;
                pfirst++;
                break;
            }

            cout << "Line choose " << pfirst++ << endl;;
            Line_Flag = true;
            TopLeftX = x;
            TopLeftY = y;
            xl[cline] = x;
            yl[cline] = y;
            cline++;
            //drawLine();
           // glFlush();

            break;
        case (CIRCLE):
            Line_Flag = false;
            if (cline)
                cline = 0;
            if (pfirst)
                pfirst = 0;
            cout << "circle : cfirst : " << cfirst << endl;
            if (cfirst == 0)
            {
                cfirst++;
                break;
            }
            if (ccount == 0)
            {
                cout << ccount << "(" << xp[0] << "," << yp[0] << ")" << endl;
                xp[0] = x;
                yp[0] = wh - y;
                cout << ccount << "(" << xp[0] << "," << yp[0] << ")" << endl;
                ccount++;
            }
            else {
                cout << ccount << "(" << x << "," << wh - y << ")" << endl;
                drawCircle(xp[0], yp[0], x, wh - y);
                ccount = 0;
            }

            break;
        default:
            cout << "De " << endl;
            Line_Flag = false;
        }

        glPopAttrib();
        glFlush();
    }
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (Line_Flag == true)
        {
            cout << "UP " << cline << " " << Line_Flag << endl;
            xl[cline] = x;
            yl[cline] = y;
            cline++;
            TopLeftX = BottomRightX;  //처음 마우스를 눌렀을 때 top x, y에 현 좌표를 저장.
            TopLeftY = BottomRightY;
            display();
            glFlush();
            drawLine();
            glFlush();
        }

    }

}

void mouseDrag(GLint X, GLint Y)
{
    if (Line_Flag)
    {
        BottomRightX = X;
        BottomRightY = Y;
        //glutPostRedisplay();
        //drawLine();
        //glFlush();
        drawLine1();
        glFlush();
        //glutPostRedisplay();

        TopLeftX = BottomRightX;
        TopLeftY = BottomRightY;
    }

}

int pick(int x, int y)
{
    y = wh - y;
    if (y < wh - ww / 10) return 0;
    else if (x < ww / 10) return LINE;
    else if (x < ww / 5) return RECTANGLE;
    else if (x < 3 * ww / 10) return TRIANGLE;
    else if (x < 2 * ww / 5) return POINTS;
    else if (x < ww / 2) return TEXT;
    else if (x < 6 * ww / 10) return POLYGON;
    else if (x < 7 * ww / 10) return CIRCLE;
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
    else
    {
        cline = 0;
        display();
    }

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
    if (draw_mode != TEXT) return;
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
    glClearColor(0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    screen_box(0, wh - ww / 10, ww / 10);
    glColor3f(1.0, 0.0, 0.0);
    screen_box(ww / 10, wh - ww / 10, ww / 10);
    glColor3f(0.0, 1.0, 0.0);
    screen_box(ww / 5, wh - ww / 10, ww / 10);
    glColor3f(0.0, 0.0, 1.0);
    screen_box(3 * ww / 10, wh - ww / 10, ww / 10);
    glColor3f(1.0, 1.0, 0.0);
    screen_box(2 * ww / 5, wh - ww / 10, ww / 10);
    glColor3f(1.0, 0.0, 1.0);
    screen_box(5 * ww / 10, wh - ww / 10, ww / 10);
    glColor3f(0.0, 1.0, 1.0);
    screen_box(6 * ww / 10, wh - ww / 10, ww / 10);
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
    glRasterPos2i(2 * ww / 5, wh - ww / 20);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
    shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
    glRasterPos2i(2 * ww / 5 + shift, wh - ww / 20);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
    shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'B');
    glRasterPos2i(2 * ww / 5 + shift, wh - ww / 20);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');

    glBegin(GL_LINE_STRIP);
    glVertex2i(5 * wh / 10 + ww / 50, wh - ww / 10 + 15);
    glVertex2i(5 * wh / 10 + 20, wh - ww / 10 + 35);
    glVertex2i(5 * wh / 10 + 30, wh - ww / 10 + 10);
    glVertex2i(5 * wh / 10 + 40, wh - ww / 10 + 30);
    glEnd();
    drawCircle(6 * wh / 10 + ww / 20, wh - ww / 20, 6 * wh / 10 + ww / 20, wh - ww / 50);
    glFlush();
    glPopAttrib();

}


int main(int argc, char** argv)
{
    int c_menu, p_menu, f_menu;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("square");
    glutDisplayFunc(display);
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
    glutCreateMenu(right_menu);
    glutAddMenuEntry("quit", 1);
    glutAddMenuEntry("clear", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutCreateMenu(middle_menu);
    glutAddSubMenu("Colors", c_menu);
    glutAddSubMenu("Pixel Size", p_menu);
    glutAddSubMenu("Fill", f_menu);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
    myinit();
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseDrag);
    glutMainLoop();

}