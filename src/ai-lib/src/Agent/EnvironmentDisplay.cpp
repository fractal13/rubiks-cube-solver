#include <ai_agent.h>
#ifdef WIN32
#include <windows.h>
#define sleep(x) Sleep(x*1000);
#else
#include <unistd.h>
#endif

#ifdef DrawText
#undef DrawText
#endif

#ifdef USE_GLUT
#include <ai_glut.h>
#include <cstring>
#include <stdio.h>
#include <cmath>

namespace ai
{
  namespace Agent
  {

    EnvironmentDisplay::EnvironmentDisplay(Environment *env, int width, int height)
      : env_cache(env)
    {
      DBGP_DPY(LOG_STREAM(std::cerr) << "Constructor1 Start" << std::endl);
      Init(width, height, (char *)"AI Environment");
      DBGP_DPY(LOG_STREAM(std::cerr) << "Constructor1 End" << std::endl);
    }

    EnvironmentDisplay::EnvironmentDisplay(Environment *env, int width, int height, bool do_not_init)
      : env_cache(env)
    {
      DBGP_DPY(LOG_STREAM(std::cerr) << "Constructor2 Start" << std::endl);
      if(!do_not_init)
        {
          Init(width, height, (char *)"AI Environment");
        }
      DBGP_DPY(LOG_STREAM(std::cerr) << "Constructor2 End" << std::endl);
    }

    bool EnvironmentDisplay::Init(int width_in, int height_in, char *title_in)
    {
      DBGP_DPY(LOG_STREAM(std::cerr) << "Init Start" << std::endl);
      static bool initialized = false;
      int    argc = 0;
      char **argv = 0;
      width  = width_in;
      height = height_in;
      if(!initialized)
        {
          glutInit(&argc, argv);
          glutInitWindowPosition(0,0);
          glutInitWindowSize(width, height);
          initialized = true;
        }
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
      
      window = glutCreateWindow(title_in);

      glClearColor(1.0,1.0,1.0,1.0);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
      glutShowWindow();
      DBGP_DPY(LOG_STREAM(std::cerr) << "Init End" << std::endl);
      return true;
    }

    EnvironmentDisplay::~EnvironmentDisplay()
    {
      if(window)
        {
          glutDestroyWindow(window);
          window = 0;
        }
    }

    void EnvironmentDisplay::Redraw(Environment *env)
    {
      char *string = (char *) "Your environment doesn't have it's own EnvironmentDisplay";
      int   x = 5;
      int   y = height/2;
      GLfloat whiteMaterial[] = {1.0, 1.0, 1.0, 1.0};
      char  buf[128];
      sprintf(buf,
              "You have %zd objects and %zd agents.",
              env->objects.size(), env->agents.size());

      glutSetWindow(window);
      glViewport(0, 0, width, height);

      // go into 2D mode
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0, width, height, 0);
      glMatrixMode(GL_MODELVIEW);


      // redraw
      glClear(GL_COLOR_BUFFER_BIT);

      SetColor(whiteMaterial);

      DrawText(x, y, string);
      y -= 25;
      DrawText(x, y, buf);

      glutSwapBuffers();
      glutMainLoopEvent();
    }

    void EnvironmentDisplay::Redraw()
    {
      glutPostRedisplay();
      glutMainLoopEvent();
    }

    void EnvironmentDisplay::SetColor(float *material)
    {
      //glColor3fv(material);
      glColor4fv(material);
    }

    void EnvironmentDisplay::SetEnv(Environment *env_in)
    {
      this->env_cache = env_in;
    }

    Environment * EnvironmentDisplay::GetEnv() const
    {
      return this->env_cache;
    }

    void EnvironmentDisplay::DrawText(int x, int y, const char *str)
    {
      DrawText((double) x, (double) y, str);
    }

    void EnvironmentDisplay::DrawText(float x, float y, const char *str)
    {
      DrawText((double) x, (double) y, str);
    }

    void EnvironmentDisplay::DrawText(double x, double y, const char *str)
    {
      void *font = GLUT_BITMAP_9_BY_15;
      int   i;
      int   len;

      //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      //glEnable(GL_BLEND);

      glRasterPos2f(x, y);
      len = (int) strlen(str);
      for (i = 0; i < len; i++)
        {
          glutBitmapCharacter(font, str[i]);
        }

      //glDisable(GL_BLEND);
    }

    void EnvironmentDisplay::DrawLine(int x1, int y1, int x2, int y2)
    {
      DrawLine((float) x1, (float) y1, (float) x2, (float) y2);
    }

    void EnvironmentDisplay::DrawLine(float x1, float y1, float x2, float y2)
    {
      glBegin(GL_LINES);
      glVertex2f(x1,y1);
      glVertex2f(x2,y2);
      glEnd();
    }

    void EnvironmentDisplay::DrawCircle(int x1, int y1, int radius)
    {
      DrawCircle((double) x1, (double) y1, (double) radius);
    }

    void EnvironmentDisplay::DrawCircle(float x1, float y1, float radius)
    {
      DrawCircle((double) x1, (double) y1, (double) radius);
    }

    void EnvironmentDisplay::DrawCircle(double x1, double y1, double radius)
    {
      glBegin(GL_POLYGON);
      for(int i=0; i<32; i++)
        {
          double theta = (double)i/32.0 * 2.0 * 3.1415926;
          double x = x1 + radius * cos(theta);
          double y = y1 + radius * sin(theta);
          glVertex2f((GLfloat)x, (GLfloat)y);
        }
      glEnd();
    }


    void EnvironmentDisplay::DrawX(float x1, float y1, float radius)
    {
      this->DrawLine(x1-radius, y1-radius, x1+radius, y1+radius);
      this->DrawLine(x1-radius, y1+radius, x1+radius, y1-radius);
    }

    void EnvironmentDisplay::DrawO(float x1, float y1, float radius)
    {
      for(int i=0; i<32; i++)
        {
          double theta00 = (double)(i)/32.0 * 2.0 * 3.1415926;
          double x00 = x1 + radius * cos(theta00);
          double y00 = y1 + radius * sin(theta00);

          double theta01 = (double)(i+1)/32.0 * 2.0 * 3.1415926;
          double x01 = x1 + radius * cos(theta01);
          double y01 = y1 + radius * sin(theta01);

          glBegin(GL_LINES);
          glVertex2f((GLfloat)x00, (GLfloat)y00);
          glVertex2f((GLfloat)x01, (GLfloat)y01);
          glEnd();
        }
    }

    void EnvironmentDisplay::DrawRectangle(int x1, int y1, int x2, int y2)
    {
      glBegin(GL_QUADS);
      glVertex2f((GLfloat)x1,(GLfloat)y1);
      glVertex2f((GLfloat)x2,(GLfloat)y1);
      glVertex2f((GLfloat)x2,(GLfloat)y2);
      glVertex2f((GLfloat)x1,(GLfloat)y2);
      glEnd();
    }

    void EnvironmentDisplay::DrawRectangle(float x1, float y1, float x2, float y2)
    {
      glBegin(GL_QUADS);
      glVertex2f(x1,y1);
      glVertex2f(x2,y1);
      glVertex2f(x2,y2);
      glVertex2f(x1,y2);
      glEnd();
    }

    void EnvironmentDisplay::DrawTriangle(int x1, int y1, int x2, int y2,
                                          int x3, int y3)
    {
      glBegin(GL_TRIANGLES);
      glVertex2f((GLfloat)x1,(GLfloat)y1);
      glVertex2f((GLfloat)x2,(GLfloat)y2);
      glVertex2f((GLfloat)x3,(GLfloat)y3);
      glEnd();
    }

    void EnvironmentDisplay::DrawRotatedRectangle(float x1, float y1, float x2, float y2,
                                                  float x3, float y3, float x4, float y4) {
      glBegin(GL_TRIANGLES);
      
      glVertex2f(x1,y1);
      glVertex2f(x2,y2);
      glVertex2f(x3,y3);
      
      glVertex2f(x2,y2);
      glVertex2f(x3,y3);
      glVertex2f(x4,y4);
      
      glEnd();
    }

    void EnvironmentDisplay::DrawBox(float x1, float y1, float x2, float y2)
    {
      DrawLine(x1, y1, x1, y2);
      DrawLine(x1, y2, x2, y2);
      DrawLine(x2, y2, x2, y1);
      DrawLine(x2, y1, x1, y1);
    }

  }
}
#else /* no glut, just do nothing */

namespace ai
{
  namespace Agent
  {

    EnvironmentDisplay::EnvironmentDisplay(Environment *env, int width, int height)
    {
    }
    EnvironmentDisplay::EnvironmentDisplay(Environment *env, int width, int height, bool do_not_init)
    {
    }
    EnvironmentDisplay::~EnvironmentDisplay()
    {
    }
    void EnvironmentDisplay::Redraw(Environment *env)
    {
    }
    void EnvironmentDisplay::Redraw()
    {
    }

    void EnvironmentDisplay::SetColor(float *material)
    {
    }

    void EnvironmentDisplay::SetEnv(Environment *env_in)
    {
    }
    
    void EnvironmentDisplay::DrawText(int x, int y, const char *str)
    {
    }

    void EnvironmentDisplay::DrawLine(int x1, int y1, int x2, int y2)
    {
    }

    void EnvironmentDisplay::DrawCircle(int x1, int y1, int radius)
    {
    }

    void EnvironmentDisplay::DrawRectangle(int x1, int y1, int x2, int y2)
    {
    }

    void EnvironmentDisplay::DrawTriangle(int x1, int y1, int x2, int y2,
                                          int x3, int y3)
    {
    }
  }
}

#endif
