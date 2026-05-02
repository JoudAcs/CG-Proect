#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <gl/glew.h>
#include <GL/freeglut.h>
#endif
#include<cmath>
#include<cstdlib>
#include<iostream>

using namespace std;


GLint width = 1280;
GLint height =720;

GLdouble PI = 3.14159265359;

// State Variables

float firsttimer = 0.0f;
int firstline = 0;
float textalpha = 1.0f;
float greyalpha =0.0f;
bool fadingtext =false;
bool showscene = false;
float suny=0.92f;
bool settimgsun =false;
float scalepond =1.0f;
bool growingpond =true;

float suny4 =0.20f;
bool settingsun4=false;

int elemPhase = 0;
float elemTimer = 0.0f;
const float PHASE_DURATION = 5.0f;

int sceneNumber = 0; // 0=intro, 3=Realization, 4=Union & Restoration
float scene3timer =0.0f;
const float SCENE3_DURATION =8.0f;


//buffer
GLuint textureprojectroad;
char roadpath[] = "C:/Users/Juxia/cg/stone.BMP";

GLuint textureprojectbrick;
char brickpath[] = "C:/Users/Juxia/cg/red brick.bmp";

// animation time
float ta = 0.0f;

//particles
struct Particles{
    float x,y,vx,vy,life,maxlife,size;
};
const int nump = 80;
Particles Particle[nump];

void resetParticle(Particles&p){
    float headcx =0.0f;
        float headcy =0.2f;
            float headrx =0.92f + 0.032f;
                 float headry =0.103f + 0.032f;
                 float a= PI *(rand()%100)/100.0f;
                 p.x =headcx + headrx * cos(a);
                 p.y =headcy + headry * sin(a);
                 float spd = 0.004f +(rand()%100)/12000.0f;
                 p.vx = cos(a)* spd * 0.4f + ((rand()%100)-50)/6000.0f;
                 p.vy =sin(a) * spd;
                 p.life =0.0f;
                 p.maxlife =0.35f + (rand()%100)/ 220.0f;
                 p.size =0.010f + (rand()%100)/ 12000.0f;
}

void InitParticle(){
    for (int i = 0; i < nump; i++)
    {
       resetParticle(Particle[i]);
       Particle[i].life = ((float)(rand()%100))/100.0f *Particle[i].maxlife; 
    }
    
}
void updateParticle(){
    for (int i = 0; i < nump; i++)
    {
        Particle[i].life += 1.0f/60.0f;
        Particle[i].x += Particle[i].vx;
                Particle[i].y += Particle[i].vy;
                Particle[i].vy *=0.995f;
                Particle[i].vx += sin(ta*3.0f +i)*0.0001f;
                if(Particle[i].life >=Particle[i].maxlife)
                resetParticle(Particle[i]);

    }
    
}

void Drawcircle(float cx,float cy, float rx,float ry, int segment, float r,float g, float b){
    glBegin(GL_POLYGON);
    glColor3f(r,g,b);
    for (int i = 0; i < segment; i++)
    {
        float angle = 2.0f * PI * i / segment;
        glVertex2f(cx +rx * cos(angle),cy + ry *sin(angle));
    }
    glEnd();
    
}
void Drawcirclea(float cx,float cy, float rx,float ry, int seg, float r,float g, float b, float a){
    glBegin(GL_POLYGON);
    glColor4f(r,g,b,a);
    for (int i = 0; i < seg; i++)
    {
        float angle = 2.0f * PI * i / seg;
        glVertex2f(cx +rx * cos(angle),cy + ry *sin(angle));
    }
    glEnd();
    
}

void drawParticlesmini(){
    for (int i = 0; i < nump; i++)
    {
        Particles& p = Particle[i];
        float ratio = p.life /p.maxlife;
        float a2,r2,b2,g2;
        if (ratio < 0.3f){
                  float f = ratio / 0.3f;
            r2=1.0f; g2=0.9f-0.4f*f; b2=0.2f*(1-f); a2=ratio/0.3f;
        } 
        else if (ratio < 0.7f) {
            float f=(ratio-0.3f)/0.4f;
            r2=1.0f; g2=0.5f-0.4f*f; b2=0.0f; a2=1.0f;
        } 
        else {
            float f=(ratio-0.7f)/0.3f;
            r2=0.8f; g2=0.1f; b2=0.0f; a2=1.0f-f;
        }
        float flicker = 0.9f + 0.1f * sin(ta * 10.0f + i);
        glColor4f(r2, g2*flicker, b2, a2*0.85f);
        float s = p.size * (1.0f - ratio * 0.5f);
        glBegin(GL_POLYGON);
        for (int j = 0; j < 6; j++) {
            float angle = 2.0f * PI * j / 6;
            glVertex2f(p.x + cos(angle)*s, p.y + sin(angle)*s*1.5f);
        }
        glEnd();
    }
}

void DrawFlameHairMini() {
    float headCX=0.0f, headCY=0.02f;
    float headRX=0.092f, headRY=0.103f;
    float crownH=0.032f;
    float crownTopRX = headRX+crownH, crownTopRY = headRY+crownH;
    int count=13;
    for (int i=0; i<count; i++) {
        float frac=(float)i/(count-1);
        float a=PI*frac;
        float baseX=headCX+crownTopRX*cos(a);
        float baseY=headCY+crownTopRY*sin(a);
        float dirX=cos(a), dirY=sin(a);
        float midness=sin(PI*frac);
        float h=0.08f+0.14f*midness;
        float flame=sin(ta*5.0f+i*0.8f)*0.020f;
        float sway=sin(ta*3.5f+i*0.5f)*0.012f;
        float tipX=baseX+dirX*(h+flame)+(-dirY)*sway;
        float tipY=baseY+dirY*(h+flame)+(dirX)*sway;
        float hw=0.018f;
        glBegin(GL_TRIANGLES);
            glColor4f(0.9f,0.12f,0.0f,0.88f);
            glVertex2f(baseX+(-dirY)*hw,  baseY+dirX*hw);
            glVertex2f(baseX+(-dirY)*-hw, baseY+dirX*-hw);
            glColor4f(1.0f,0.75f,0.0f,0.0f);
            glVertex2f(tipX+dirX*0.018f, tipY+dirY*0.018f);
        glEnd();
        float hw2=0.011f;
        glBegin(GL_TRIANGLES);
            glColor4f(1.0f,0.45f,0.0f,0.92f);
            glVertex2f(baseX+(-dirY)*hw2,  baseY+dirX*hw2);
            glVertex2f(baseX+(-dirY)*-hw2, baseY+dirX*-hw2);
            glColor4f(1.0f,0.95f,0.3f,0.0f);
            glVertex2f(tipX, tipY);
        glEnd();
    }
}

void DrawPrinceMini() {
    
    glBegin(GL_QUADS); glColor3f(0.75f,0.30f,0.0f);
        glVertex2f(-0.06f,-0.42f); glVertex2f(-0.01f,-0.42f);
        glVertex2f(-0.01f,-0.60f); glVertex2f(-0.06f,-0.60f);
    glEnd();
    
    glBegin(GL_QUADS); glColor3f(0.75f,0.30f,0.0f);
        glVertex2f(0.01f,-0.42f); glVertex2f(0.06f,-0.42f);
        glVertex2f(0.06f,-0.60f); glVertex2f(0.01f,-0.60f);
    glEnd();

    glBegin(GL_QUADS); glColor3f(0.85f,0.38f,0.0f);
        glVertex2f(-0.10f,-0.42f); glVertex2f(0.10f,-0.42f);
        glVertex2f(0.10f,-0.10f);  glVertex2f(-0.10f,-0.10f);
    glEnd();
    
    glLineWidth(1.0f);
    glBegin(GL_LINES); glColor3f(1.0f,0.65f,0.10f);
        glVertex2f(-0.10f,-0.25f); glVertex2f(0.10f,-0.25f);
        glVertex2f(0.0f,-0.10f);   glVertex2f(0.0f,-0.42f);
    glEnd();
    
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.9f,0.1f);
        glVertex2f(-0.03f,-0.32f); glVertex2f(0.03f,-0.32f);
        glColor3f(1.0f,0.3f,0.0f);
        glVertex2f(0.0f,-0.18f);
    glEnd();
    
    glBegin(GL_QUADS); glColor3f(0.80f,0.33f,0.0f);
        glVertex2f(-0.16f,-0.42f); glVertex2f(-0.10f,-0.42f);
        glVertex2f(-0.10f,-0.13f); glVertex2f(-0.16f,-0.13f);
    glEnd();

    glBegin(GL_QUADS); glColor3f(0.80f,0.33f,0.0f);
        glVertex2f(0.10f,-0.42f); glVertex2f(0.16f,-0.42f);
        glVertex2f(0.16f,-0.13f); glVertex2f(0.10f,-0.13f);
    glEnd();

    Drawcircle(0.0f,0.02f,0.09f,0.10f,40,0.95f,0.72f,0.50f);

    Drawcircle(-0.032f,0.04f,0.015f,0.015f,20,0.15f,0.05f,0.0f);
    Drawcircle( 0.032f,0.04f,0.015f,0.015f,20,0.15f,0.05f,0.0f);
    Drawcirclea(-0.032f,0.04f,0.007f,0.007f,12,1.0f,0.5f,0.0f,1.0f);
    Drawcirclea( 0.032f,0.04f,0.007f,0.007f,12,1.0f,0.5f,0.0f,1.0f);

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP); glColor3f(0.4f,0.10f,0.0f);
    for (int i=0; i<=14; i++) {
        float a=PI+(PI*i/14.0f);
        glVertex2f(0.0f+0.040f*cos(a), -0.028f+0.016f*sin(a));
    }
    glEnd();

    float headCX=0.0f, headCY=0.02f;
    float headRX=0.092f, headRY=0.103f, crownH=0.032f;
    glBegin(GL_QUAD_STRIP);
    for (int i=0; i<=40; i++) {
        float a=PI*i/40.0f;
        float cosA=cos(a), sinA=sin(a);
        glColor3f(1.0f,0.78f,0.0f);
        glVertex2f(headCX+(headRX+crownH)*cosA, headCY+(headRY+crownH)*sinA);
        glColor3f(0.85f,0.55f,0.0f);
        glVertex2f(headCX+headRX*cosA, headCY+headRY*sinA);
    }
    glEnd();

    for (int i=0; i<7; i++) {
        float frac=(float)i/6.0f;
        float a=PI*frac;
        float bx=headCX+(headRX+crownH)*cos(a);
        float by=headCY+(headRY+crownH)*sin(a);
        float toothH=0.025f+0.015f*sin(PI*frac);
        float nx=cos(a), ny=sin(a);
        glBegin(GL_TRIANGLES);
            glColor3f(1.0f,0.85f,0.0f);
            glVertex2f(bx-ny*0.012f, by+nx*0.012f);
            glVertex2f(bx+ny*0.012f, by-nx*0.012f);
            glColor3f(1.0f,0.55f,0.0f);
            glVertex2f(bx+nx*toothH, by+ny*toothH);
        glEnd();
    }
    
    float gemY=headCY+headRY+crownH*0.5f;
    Drawcirclea(0.0f,gemY,0.014f,0.014f,16,1.0f,0.2f,0.0f,1.0f);
    Drawcirclea(0.0f,gemY,0.007f,0.007f,12,1.0f,0.9f,0.5f,1.0f);
}



void DrawFirePrinceAt(float px, float py, float scale) {
    glPushMatrix();
    glTranslatef(px, py, 0.0f);
    glScalef(scale, scale, 1.0f);
    drawParticlesmini();
    DrawPrinceMini();
    DrawFlameHairMini();
    glPopMatrix();
}


void drawsky(){
    glBegin(GL_QUADS);
    glColor3f(0.40f,0.75f,0.95f);
    glVertex2f(-1.0f,1.25f);
    glVertex2f(1.0f,1.25f);

    glColor3f(0.55f,0.85f,0.98f);
    glVertex2f(1.0f,0.0f);
    glVertex2f(-1.0f,0.0f);
    glEnd();
}

void drawground(){
 glBegin(GL_QUADS);
    glColor3f(0.36f,0.70f,0.20f);
    glVertex2f(-1.0f,0.0f);
    glVertex2f(1.0f,0.0f);

     glColor3f(0.30f,0.60f,0.15f);
    glVertex2f(1.0f,-1.0f);
    glVertex2f(-1.0f,-1.0f);
     glEnd();
}

void DrawSun(){
    glPushMatrix();
    glTranslatef(-0.55f,suny,0.0f);
    glRotatef(ta*20.0f,0.0f,0.0f,1.0f);
    glTranslatef(0.55f,-0.92f,0.0f);
  Drawcircle(-0.55f,0.92f,0.115f,0.115f,60,1.0f,0.98f,0.60f);  
  Drawcircle(-0.55f,0.92f,0.09f,0.09f,60,1.0f,0.98f,0.10f);
  glPopMatrix();  
}

void DrawSun4(){
    glPushMatrix();
    glTranslatef(-0.55f,suny4,0.0f);
    glRotatef(ta*20.0f,0.0f,0.0f,1.0f);
    glTranslatef(0.55f,-0.92f,0.0f);
  Drawcircle(-0.55f,0.92f,0.115f,0.115f,60,1.0f,0.98f,0.60f);  
  Drawcircle(-0.55f,0.92f,0.09f,0.09f,60,1.0f,0.98f,0.10f);
  glPopMatrix();  
}

void DrawClouds(float cx,float cy, float rx, float ry){
    Drawcircle(cx,cy,rx,ry,40,1.0f,1.0f,1.0f);

}
void drawcloudpuff(float cx,float cy,float rx,float ry)
{Drawcircle(cx,cy,rx,ry,40,1.0f,1.0f,1.0f);
}

void drawcloud(float x, float y, float scale){
    drawcloudpuff(x,y,0.10f*scale,0.065f*scale);
    drawcloudpuff(x+0.08f*scale,y+0.02f*scale,0.085f*scale,0.060f*scale);
    drawcloudpuff(x+0.16f*scale,y+0.08f*scale,0.055f*scale,0.040f*scale);
    drawcloudpuff(x+0.07f*scale,y-0.01f*scale,0.070f*scale,0.045f*scale);
}
void drawclouds(){
    drawcloud(0.10f,1.00f,1.0f);
        drawcloud(0.50f,0.90f,.75f);
            drawcloud(-0.20f,0.85f,0.60f);


}
void DrawHillbackLeft(){
    glBegin(GL_POLYGON);
    glColor3f(0.25f,0.60f,0.18f);

    glVertex2f(-1.0f,0.0f);
        glVertex2f(-1.0f,0.10f);
            glVertex2f(0.95f,0.20f);

                glVertex2f(-0.87f,0.31f);
    glVertex2f(-0.78f,0.40f);
    glVertex2f(-0.70f,0.46f);

        glVertex2f(-0.62f,0.51f);
    glVertex2f(-0.54f,0.53f);
    glVertex2f(-0.46f,0.51f);

        glVertex2f(-0.38f,0.45f);
    glVertex2f(-0.31f,0.37f);
    glVertex2f(-0.004f,0.50f);

        glVertex2f(-0.10f,0.0f);
        glEnd();

}
void DrawHillbackRight(){
      glBegin(GL_POLYGON);
    glColor3f(0.25f,0.60f,0.18f);

    glVertex2f(0.10f,0.0f);
        glVertex2f(0.04f,0.50f);
            glVertex2f(0.31f,0.37f);

                glVertex2f(0.38f,0.45f);
    glVertex2f(0.46f,0.51f);
    glVertex2f(0.54f,0.53f);

        glVertex2f(0.62f,0.51f);
    glVertex2f(0.70f,0.46f);
    glVertex2f(0.78f,0.40f);

        glVertex2f(0.87f,0.31f);
    glVertex2f(0.95f,0.20f);
    glVertex2f(1.0f,0.10f);

        glVertex2f(1.0f,0.0f);
        glEnd();

}

void DrawHillbackMid(){
 glBegin(GL_POLYGON);
    glColor3f(0.28f,0.65f,0.20f);

    glVertex2f(-0.65f,0.0f);
        glVertex2f(-0.63f,0.08f);
            glVertex2f(-0.58f,0.17f);

                glVertex2f(-0.51f,0.26f);
    glVertex2f(-0.43f,0.34f);
    glVertex2f(-0.34f,0.41f);

        glVertex2f(-0.24f,0.46f);
    glVertex2f(-0.13f,0.50f);
    glVertex2f(-0.04f,0.52f);

        glVertex2f(0.00f,0.53f);
    glVertex2f(0.04f,0.52f);
    glVertex2f(0.13f,0.50f);

    glVertex2f(0.24f,0.46f);
    glVertex2f(0.34f,0.41f);
    glVertex2f(0.43f,0.34f);

    glVertex2f(0.51f,0.26f);
    glVertex2f(0.58f,0.17f);
    glVertex2f(0.63f,0.08f);

        glVertex2f(0.65f,0.00f);
        glEnd();
}

void DrawDrawHillfrontLeft(){
       glBegin(GL_POLYGON);
    glColor3f(0.22f,0.52f,0.12f);

    glVertex2f(-1.0f,-0.05f);
        glVertex2f(-1.0f,0.08f);
            glVertex2f(-0.90f,0.20f);

                glVertex2f(-0.78f,0.31f);
    glVertex2f(-0.66f,0.40f);
    glVertex2f(-0.54f,0.47f);

        glVertex2f(-0.42f,0.52f);
    glVertex2f(-0.30f,0.54f);
    glVertex2f(-0.20f,0.53f);

        glVertex2f(-0.10f,0.48f);
    glVertex2f(-0.03f,0.54f);
    glVertex2f(0.02f,0.30f);

        glVertex2f(0.05f,0.18f);
        glVertex2f(0.06f,0.05f);
        glVertex2f(0.06f,-0.05f);
        glEnd();

}

void DrawDrawHillfrontRight(){

 glBegin(GL_POLYGON);
    glColor3f(0.22f,0.52f,0.12f);

    glVertex2f(-0.06f,-0.05f);
        glVertex2f(-0.06f,0.05f);
            glVertex2f(-0.05f,0.18f);

                glVertex2f(-0.02f,0.30f);
    glVertex2f(0.03f,0.40f);
    glVertex2f(-0.10f,0.48f);

        glVertex2f(0.20f,0.53f);
    glVertex2f(0.30f,0.54f);
    glVertex2f(0.42f,0.52f);

        glVertex2f(0.54f,0.47f);
    glVertex2f(0.63f,0.40f);
    glVertex2f(0.78f,0.31f);

        glVertex2f(0.90f,0.20f);
        glVertex2f(1.0f,0.08f);
        glVertex2f(1.0f,-0.05f);
        glEnd();

}


void DrawRoad(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textureprojectroad);
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);
    glVertex2f(-0.22f,-1.0f);

      glTexCoord2f(1,0);
        glVertex2f(0.22f,-1.0f);

      glTexCoord2f(1,1);
    glVertex2f(0.022f,0.50f);
      glTexCoord2f(0,1);
        glVertex2f(-0.022f,0.50f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

void drawtree(float x,float y,float scale){
    glBegin(GL_QUADS);
    glColor3f(0.40f,0.25f,0.10f);
    glVertex2f(x-0.012f*scale,y);
        glVertex2f(x+0.012f*scale,y);
            glVertex2f(x+0.012f*scale,y+0.07f*scale);
                        glVertex2f(x-0.012f*scale,y+0.07f*scale);

    glEnd();
Drawcircle(x,y+0.13f*scale,0.065f*scale,0.065f*scale,40,0.15f,0.42f,0.08f);
Drawcircle(x,y+0.15f*scale,0.045f*scale,0.045f*scale,40,0.22f,0.55f,0.12f);

}

void DrawTrees(){
   drawtree(-0.82f,-0.40f,1.1f);
   drawtree(-0.70f,-0.38f,1.0f);
    drawtree(-0.60f,-0.42f,0.9f);
        drawtree(0.60f,-0.38f,1.0f);
            drawtree(0.70f,-0.40f,1.1f);
                drawtree(0.80f,-0.36f,0.95f);
                    drawtree(0.88f,-0.42f,0.85f);
    drawtree(-0.75f,-0.10f,0.55f);
    drawtree(-0.68f,-0.08f,0.50f);
    drawtree(0.60f,-0.08f,0.50f);
    drawtree(0.76f,-0.10f,0.55f);
}

void drawpound(float cx,float cy,float rx,float ry){
  Drawcircle(cx,cy,rx*1.25f,ry*1.6f,40,0.28f,0.58f,0.14f);
  Drawcircle(cx,cy,rx,ry,40,0.08f,0.30f,0.50f);
  Drawcircle(cx,cy+ry*0.1f,rx*0.85f,ry*0.75f,40,0.15f,0.48f,0.72f);
  Drawcircle(cx-rx*0.20f,cy+ry*0.10f,rx*0.22f*scalepond,ry*0.18f*scalepond,30,0.40f,0.72f,0.88f);
  Drawcircle(cx+rx*0.15f,cy-ry*0.05f,rx*0.12f*scalepond,ry*0.10f*scalepond,20,0.55f,0.82f,0.95f);
}

void DrawPonds(){
   drawpound(-0.68f,-0.58f,0.22f,0.09f);
   drawpound(0.68f,-0.58f,0.22f,0.09f); 
}

void DrawBushes(){
    Drawcircle(-0.72f,-0.22f,0.040f,0.030f,30,0.18f,0.48f,0.10f);
    Drawcircle(-0.64f,-0.21f,0.035f,0.028f,30,0.20f,0.52f,0.12f);
        Drawcircle(0.64f,0.21f,0.035f,0.028f,30,0.18f,0.48f,0.10f);
    Drawcircle(0.72f,-0.22f,0.040f,0.030f,30,0.20f,0.52f,0.12f);

}

void drawtowerroof(float cx, float basey,float halfw,float height,float r,float g, float b){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textureprojectbrick);
    glColor3f(r,g,b);
    
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f,0.0f);
    glVertex2f(cx-halfw,basey);
    glTexCoord2f(1.0f,0.0f);
    glVertex2f(cx+halfw,basey);
    glTexCoord2f(0.5f,1.0f);
    glVertex2f(cx,basey+height);
            glEnd();
            glDisable(GL_TEXTURE_2D);
}

void drawflag(float x, float y){
glLineWidth(1.5f);
glBegin(GL_LINES);
glColor3f(0.55f,0.35f,0.15f);
glVertex2f(x,y);
glVertex2f(x,y+0.055f);
glEnd();
glBegin(GL_TRIANGLES);
glColor3f(0.85f,0.10f,0.10f);
glVertex2f(x,y+0.055f);
glVertex2f(x+0.030f,y+0.040f);
glVertex2f(x,y+0.025f);
glEnd();
}

void DrawCastle(){
    const float A=0.50f;
    glBegin(GL_QUADS);
    glColor3f(0.82f,0.72f,0.58f);
    glVertex2f(-.18f,A);
    glVertex2f(0.18f,A);
        glVertex2f(0.18f,A+0.20f);
        glVertex2f(-0.18f,A+0.20f);
        glEnd();

        glColor3f(0.75f,0.65f,0.52f);
        float battelx[]={-0.18f,-0.13f,-0.08f,-0.03f,0.02f,0.07f,0.12f};
        for(int i=0;i<7;i++){
            glBegin(GL_QUADS);
            glVertex2f(battelx[i],A+0.20f);
            glVertex2f(battelx[i]+0.03f,A+0.20f);
            glVertex2f(battelx[i]+0.03f,A+0.235f);
            glVertex2f(battelx[i],A+0.235f);
            glEnd();
        }

        glBegin(GL_QUADS);
        glColor3f(0.80f,0.70f,0.56f);
        glVertex2f(-0.17f,A);
        glVertex2f(-0.08f,A);
        glVertex2f(-0.08f,A+0.34f);
        glVertex2f(-0.17f,A+0.34f);
        glEnd();
        glColor3f(0.72f,0.62f,0.50f);
        for(int i =0;i<3;i++){
            float ax=-0.170f+i*0.032f;
        glBegin(GL_QUADS);
        glVertex2f(ax,A+0.34f);
        glVertex2f(ax+0.022f,A+0.34f);
        glVertex2f(ax+0.022f,A+0.365f);
        glVertex2f(ax,A+0.365f);
        glEnd();
        }

        drawtowerroof(-0.125f,A+0.365f,0.055f,0.095f,0.75f,0.12f,0.12f);
        glBegin(GL_QUADS);
        glColor3f(0.80f,0.70f,0.56f);
        glVertex2f(0.08f,A);
        glVertex2f(0.17f,A);
        glVertex2f(0.17f,A+0.34f);
        glVertex2f(0.08f,A+0.34f);
        glEnd();

        glColor3f(0.72f,0.62f,0.50f);
        for(int i =0;i<3;i++){
            float ax=0.082f+i*0.032f;
            glBegin(GL_QUADS);
            glVertex2f(ax,A+0.34f);
            glVertex2f(ax+0.022f,A+0.34f);
            glVertex2f(ax+0.022f,A+0.365f);
            glVertex2f(ax,A+0.365f);
            glEnd();
        }

       drawtowerroof(0.125f,A+0.365f,0.055f,0.095f,0.75f,0.12f,0.12f);
       glBegin(GL_QUADS);
       glColor3f(0.85f,0.75f,0.60f);
       glVertex2f(-0.055f,A);
       glVertex2f(0.055f,A);
       glVertex2f(0.055f,A+0.41f);
 glVertex2f(-0.055f,A+0.41f);
 glEnd();

 glColor3f(0.74f,0.64f,0.51f);
 for(int i =0;i<4;i++){
    float ax=-0.054f+i*0.028f;
    glBegin(GL_QUADS);
    glVertex2f(ax,A+0.41f);
    glVertex2f(ax+0.018f,A+0.41f);
    glVertex2f(ax+0.018f,A+0.435f);
    glVertex2f(ax,A+0.435f);
    glEnd();
 }

 drawtowerroof(0.0f,A+0.435f,0.065f,0.115f,0.78f,0.10f,0.10f);
 glBegin(GL_QUADS);
 glColor3f(0.50f,0.38f,0.22f);
 glVertex2f(-0.038f,A);
 glVertex2f(0.038f,A);
  glVertex2f(0.038f,A+0.135f);
  glVertex2f(-0.038f,A+0.135f);
  glEnd();

  glBegin(GL_QUADS);
  glColor3f(0.25f,0.18f,0.10f);
  glVertex2f(-0.028f,A);
  glVertex2f(0.028f,A);
  glVertex2f(0.028f,A+0.118f);
  glVertex2f(-0.028f,A+0.118f);
  glEnd();

  Drawcircle(0.0f,A+0.118f,0.028f,0.025f,30,0.25f,0.18f,0.10f);
Drawcircle(-0.125f,A+0.215f,0.018f,0.022f,20,0.28f,0.20f,0.12f);
Drawcircle(0.125f,A+0.215f,0.018f,0.022f,20,0.28f,0.20f,0.12f);
  Drawcircle(0.0f,A+0.235f,0.016f,0.020f,20,0.28f,0.20f,0.12f);
    Drawcircle(0.0f,A+0.290f,0.014f,0.018f,20,0.28f,0.20f,0.12f);

    drawflag(-0.125f,A+0.460f);
    drawflag(0.125f,A+0.460f);
     drawflag(0.0f,A+0.550f);

Drawcircle(-0.215f,A+0.020f,0.030f,0.038f,25,0.18f,0.48f,0.10f);
Drawcircle(0.215f,A+0.020f,0.030f,0.038f,25,0.18f,0.48f,0.10f);


}

void Drawtext(float x, float y, const char* str,void* font,float r, float g, float b , float a){
    glColor4f(r,g,b,a);
    glRasterPos2f(x,y);
for(const char* c =str;*c;c++)glutBitmapCharacter(font,*c);    
    
}


void DrawElementPrinceBody(float bodyR, float bodyG, float bodyB,
                           float darkR, float darkG, float darkB,
                           float accentR, float accentG, float accentB,
                           char symbol){
    // Legs - same proportions as fire character
    glBegin(GL_QUADS); 
    glColor3f(darkR,darkG,darkB);
    glVertex2f(-0.06f,-0.42f); glVertex2f(-0.01f,-0.42f);
    glVertex2f(-0.01f,-0.60f); glVertex2f(-0.06f,-0.60f);

    glVertex2f(0.01f,-0.42f); glVertex2f(0.06f,-0.42f);
    glVertex2f(0.06f,-0.60f); glVertex2f(0.01f,-0.60f);
    glEnd();

    // Body - same square shape as fire
    glBegin(GL_QUADS); 
    glColor3f(bodyR,bodyG,bodyB);
    glVertex2f(-0.10f,-0.42f); glVertex2f(0.10f,-0.42f);
    glVertex2f(0.10f,-0.10f);  glVertex2f(-0.10f,-0.10f);
    glEnd();

    // Armor lines
    glLineWidth(1.0f);
    glBegin(GL_LINES); 
    glColor3f(accentR,accentG,accentB);
    glVertex2f(-0.10f,-0.25f); glVertex2f(0.10f,-0.25f);
    glVertex2f(0.0f,-0.10f);   glVertex2f(0.0f,-0.42f);
    glEnd();

    // Element symbol on chest
    if(symbol == 'W'){
        Drawcircle(0.0f,-0.27f,0.030f,0.040f,30,accentR,accentG,accentB);
        glBegin(GL_TRIANGLES);
        glColor3f(accentR,accentG,accentB);
        glVertex2f(-0.025f,-0.255f); glVertex2f(0.025f,-0.255f); glVertex2f(0.0f,-0.190f);
        glEnd();
    }
    else if(symbol == 'A'){
        glLineWidth(2.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(accentR,accentG,accentB);
        for(int i=0;i<=18;i++){
            float xx=-0.055f+i*0.006f;
            float yy=-0.27f+sin(i*0.8f+ta*4.0f)*0.012f;
            glVertex2f(xx,yy);
        }
        glEnd();
    }
    else if(symbol == 'E'){
        Drawcircle(-0.018f,-0.265f,0.035f,0.018f,24,accentR,accentG,accentB);
        Drawcircle(0.018f,-0.265f,0.035f,0.018f,24,accentR,accentG,accentB);
        glBegin(GL_LINES);
        glColor3f(0.10f,0.45f,0.10f);
        glVertex2f(0.0f,-0.32f); glVertex2f(0.0f,-0.21f);
        glEnd();
    }

    // Arms - same as fire
    glBegin(GL_QUADS); 
    glColor3f(bodyR*0.92f, bodyG*0.92f, bodyB*0.92f);
    glVertex2f(-0.16f,-0.42f); glVertex2f(-0.10f,-0.42f);
    glVertex2f(-0.10f,-0.13f); glVertex2f(-0.16f,-0.13f);

    glVertex2f(0.10f,-0.42f); glVertex2f(0.16f,-0.42f);
    glVertex2f(0.16f,-0.13f); glVertex2f(0.10f,-0.13f);
    glEnd();

    // Head and facial features - same as fire
    Drawcircle(0.0f,0.02f,0.092f,0.103f,50,0.95f,0.76f,0.55f);
    Drawcircle(-0.035f,0.03f,0.010f,0.010f,20,0.05f,0.03f,0.02f);
    Drawcircle(0.035f,0.03f,0.010f,0.010f,20,0.05f,0.03f,0.02f);

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.10f,0.05f,0.02f);
    for(int i=0;i<=16;i++){
        float a=PI + PI*i/16.0f;
        glVertex2f(cos(a)*0.035f, -0.005f + sin(a)*0.018f);
    }
    glEnd();
}

void DrawElementCrown(char type, float r, float g, float b){
    // Same crown/hair size as the fire character, with element colors
    float headCX=0.0f, headCY=0.02f;
    float headRX=0.092f, headRY=0.103f;
    float crownH=0.032f;
    float crownTopRX = headRX+crownH, crownTopRY = headRY+crownH;
    int count=13;

    for (int i=0; i<count; i++) {
        float frac=(float)i/(count-1);
        float a=PI*frac;
        float baseX=headCX+crownTopRX*cos(a);
        float baseY=headCY+crownTopRY*sin(a);
        float dirX=cos(a), dirY=sin(a);
        float midness=sin(PI*frac);
        float h=0.08f+0.14f*midness;
        float move=sin(ta*4.0f+i*0.7f)*0.015f;
        float sway=sin(ta*3.0f+i*0.5f)*0.010f;
        float tipX=baseX+dirX*(h+move)+(-dirY)*sway;
        float tipY=baseY+dirY*(h+move)+(dirX)*sway;

        if(type=='E'){
            Drawcircle(tipX,tipY,0.025f,0.014f,24,r,g,b);
        }
        else if(type=='A'){
            Drawcirclea(tipX,tipY,0.024f,0.014f,24,r,g,b,0.75f);
        }
        else{
            float hw=0.018f;
            glBegin(GL_TRIANGLES);
            glColor4f(r,g,b,0.88f);
            glVertex2f(baseX+(-dirY)*hw,  baseY+dirX*hw);
            glVertex2f(baseX+(-dirY)*-hw, baseY+dirX*-hw);
            glColor4f(0.80f,0.95f,1.0f,0.0f);
            glVertex2f(tipX+dirX*0.018f, tipY+dirY*0.018f);
            glEnd();
        }
    }
}

void DrawWaterCharacterAt(float x, float y, float scale){
    glPushMatrix();
    glTranslatef(x, y + sin(ta*2.5f)*0.006f, 0.0f);
    glScalef(scale , scale , 1.0f);
    DrawElementPrinceBody(0.05f,0.42f,0.95f, 0.02f,0.22f,0.65f, 0.70f,0.95f,1.0f, 'W');
    DrawElementCrown('W',0.05f,0.55f,1.0f);
    glPopMatrix();

}



void DrawAirCharacterAt(float x, float y, float scale){
    glPushMatrix();
    glTranslatef(x, y + sin(ta*3.0f)*0.008f, 0.0f);
    glScalef(scale, scale , 1.0f);
    DrawElementPrinceBody(0.78f,0.84f,0.92f, 0.55f,0.62f,0.70f, 0.95f,0.98f,1.0f, 'A');
    DrawElementCrown('A',0.92f,0.98f,1.0f);
    glPopMatrix();

}


void DrawEarthCharacterAt(float x, float y, float scale){
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale , scale,  1.0f);
    DrawElementPrinceBody(0.23f,0.68f,0.18f, 0.12f,0.42f,0.10f, 0.95f,0.80f,0.20f, 'E');
    DrawElementCrown('E',0.12f,0.62f,0.15f);
    glPopMatrix();

}



void Drawintrotext(){
    float t=textalpha;
    if(t<=0.0f)
    return;
    glBegin(GL_QUADS);
    glColor4f(0.0f,0.0f,0.0f,t*0.50f);
    glVertex2f(-1.0f,0.32f);
    glVertex2f(1.0f,0.32f);
    glVertex2f(1.0f,-0.58f);
    glVertex2f(-1.0f,-0.58f);
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor4f(0.85f,0.70f,0.28f,t*0.9f);
    glVertex2f(-0.52f,0.24f);
    glVertex2f(0.52f,0.24f);
    glEnd();
DrawFirePrinceAt(-0.72f,0.14f,0.19f);
    DrawWaterCharacterAt(-0.24f,0.14f,0.19f);
DrawEarthCharacterAt(0.24f,0.14f,0.19f);
   DrawAirCharacterAt(0.72f,0.14f,0.19f);

    if(firstline>=2) Drawtext(-0.55f,-0.12f,"Once, the Four Elements (FIRE,WATER,EARTH,AIR) stood together...",GLUT_BITMAP_HELVETICA_18,0.95f,0.92f,0.82f,t);
    if(firstline>=3) Drawtext(-0.60f,-0.26f,"And the world was full of life, color, and wonder.",GLUT_BITMAP_HELVETICA_18,0.85f,0.80f,0.70f,t);
    if(firstline>=4) Drawtext(-0.42f,-0.40f,"But then... they parted ways.",GLUT_BITMAP_HELVETICA_18,0.65f,0.60f,0.55f,t);
    if(firstline>=5) Drawtext(-0.50f,-0.54f,"And the world lost its colors forever.",GLUT_BITMAP_HELVETICA_18,0.50f,0.45f,0.40f,t);

    glBegin(GL_LINES);
    glColor4f(0.85f,0.70f,0.28f,t*0.9f);
glVertex2f(-0.52f,-0.54f);
glVertex2f(0.52f,-0.54f);
glEnd();

if(firstline>=5){
        float blink=0.5f+0.5f*sinf(firsttimer*3.0f);
        Drawtext(-0.28f,-0.80f,"Press SPACE to continue...",GLUT_BITMAP_HELVETICA_12,0.0f,0.0f,0.0f,t*blink);
    }
}

void drawgreyoverlay(){
    if (greyalpha<=0.0f)return;
   glBegin(GL_QUADS);
   glColor4f(0.12f,0.12f,0.15f,greyalpha);
   glVertex2f(-1.0f,1.25f);
      glVertex2f(1.0f,1.25f);
         glVertex2f(1.0f,-1.0f);
                  glVertex2f(-1.0f,-1.0f);
glEnd();
   
}

//REVAL
float lerpf(float a, float b, float t){ return a+(b-a)*t; }
float clamp01(float v){ return v<0?0:v>1?1:v; }

void lerpcol3(float r,float g,float b,float sat,float &or2,float &og,float &ob){
    float grey=r*0.3f+g*0.59f+b*0.11f;
    or2=lerpf(grey,r,sat); og=lerpf(grey,g,sat); ob=lerpf(grey,b,sat);
}


void drawskyS(float sat){
    float r1,g1,b1,r2,g2,b2;
    lerpcol3(0.40f,0.75f,0.95f,sat,r1,g1,b1);
    lerpcol3(0.55f,0.85f,0.98f,sat,r2,g2,b2);
    glBegin(GL_QUADS);
    glColor3f(r1,g1,b1);glVertex2f(-1.0f,1.25f);glVertex2f(1.0f,1.25f);
    glColor3f(r2,g2,b2);glVertex2f(1.0f,0.0f);glVertex2f(-1.0f,0.0f);
    glEnd();
}
void drawgroundS(float sat){
    float r1,g1,b1,r2,g2,b2;
    lerpcol3(0.36f,0.70f,0.20f,sat,r1,g1,b1);
    lerpcol3(0.30f,0.60f,0.15f,sat,r2,g2,b2);
    glBegin(GL_QUADS);
    glColor3f(r1,g1,b1);glVertex2f(-1.0f,0.0f);glVertex2f(1.0f,0.0f);
    glColor3f(r2,g2,b2);glVertex2f(1.0f,-1.0f);glVertex2f(-1.0f,-1.0f);
    glEnd();
}


void DrawSceneColored(float sat){
    drawskyS(sat);
    drawgroundS(sat);

    float r,g,b;
    lerpcol3(1.0f,0.98f,0.60f,sat,r,g,b); Drawcircle(-0.55f,0.92f,0.115f,0.115f,60,r,g,b);
    lerpcol3(1.0f,0.98f,0.10f,sat,r,g,b); Drawcircle(-0.55f,0.92f,0.09f,0.09f,60,r,g,b);

    drawclouds();

    lerpcol3(0.25f,0.60f,0.18f,sat,r,g,b);
    glBegin(GL_POLYGON);glColor3f(r,g,b);
    glVertex2f(-1.0f,0.0f);glVertex2f(-1.0f,0.10f);glVertex2f(0.95f,0.20f);glVertex2f(-0.87f,0.31f);glVertex2f(-0.78f,0.40f);glVertex2f(-0.70f,0.46f);glVertex2f(-0.62f,0.51f);glVertex2f(-0.54f,0.53f);glVertex2f(-0.46f,0.51f);glVertex2f(-0.38f,0.45f);glVertex2f(-0.31f,0.37f);glVertex2f(-0.004f,0.50f);glVertex2f(-0.10f,0.0f);
    glEnd();

    glBegin(GL_POLYGON);glColor3f(r,g,b);
    glVertex2f(0.10f,0.0f);glVertex2f(0.04f,0.50f);glVertex2f(0.31f,0.37f);glVertex2f(0.38f,0.45f);glVertex2f(0.46f,0.51f);glVertex2f(0.54f,0.53f);glVertex2f(0.62f,0.51f);glVertex2f(0.70f,0.46f);glVertex2f(0.78f,0.40f);glVertex2f(0.87f,0.31f);glVertex2f(0.95f,0.20f);glVertex2f(1.0f,0.10f);glVertex2f(1.0f,0.0f);
    glEnd();

    float r2,g2,b2;
    lerpcol3(0.28f,0.65f,0.20f,sat,r2,g2,b2);
    glBegin(GL_POLYGON);glColor3f(r2,g2,b2);
    glVertex2f(-0.65f,0.0f);glVertex2f(-0.63f,0.08f);glVertex2f(-0.58f,0.17f);glVertex2f(-0.51f,0.26f);glVertex2f(-0.43f,0.34f);glVertex2f(-0.34f,0.41f);glVertex2f(-0.24f,0.46f);glVertex2f(-0.13f,0.50f);glVertex2f(-0.04f,0.52f);glVertex2f(0.00f,0.53f);glVertex2f(0.04f,0.52f);glVertex2f(0.13f,0.50f);glVertex2f(0.24f,0.46f);glVertex2f(0.34f,0.41f);glVertex2f(0.43f,0.34f);glVertex2f(0.51f,0.26f);glVertex2f(0.58f,0.17f);glVertex2f(0.63f,0.08f);glVertex2f(0.65f,0.00f);
    glEnd();

    DrawRoad();
    DrawCastle();

    float rf,gf,bf;
    lerpcol3(0.22f,0.52f,0.12f,sat,rf,gf,bf);
    glBegin(GL_POLYGON);glColor3f(rf,gf,bf);
    glVertex2f(-1.0f,-0.05f);glVertex2f(-1.0f,0.08f);glVertex2f(-0.90f,0.20f);glVertex2f(-0.78f,0.31f);glVertex2f(-0.66f,0.40f);glVertex2f(-0.54f,0.47f);glVertex2f(-0.42f,0.52f);glVertex2f(-0.30f,0.54f);glVertex2f(-0.20f,0.53f);glVertex2f(-0.10f,0.48f);glVertex2f(-0.03f,0.54f);glVertex2f(0.02f,0.30f);glVertex2f(0.05f,0.18f);glVertex2f(0.06f,0.05f);glVertex2f(0.06f,-0.05f);
    glEnd();
    glBegin(GL_POLYGON);glColor3f(rf,gf,bf);
    glVertex2f(-0.06f,-0.05f);glVertex2f(-0.06f,0.05f);glVertex2f(-0.05f,0.18f);glVertex2f(-0.02f,0.30f);glVertex2f(0.03f,0.40f);glVertex2f(-0.10f,0.48f);glVertex2f(0.20f,0.53f);glVertex2f(0.30f,0.54f);glVertex2f(0.42f,0.52f);glVertex2f(0.54f,0.47f);glVertex2f(0.63f,0.40f);glVertex2f(0.78f,0.31f);glVertex2f(0.90f,0.20f);glVertex2f(1.0f,0.08f);glVertex2f(1.0f,-0.05f);
    glEnd();

    DrawRoad();

    float pe1r,pe1g,pe1b; lerpcol3(0.28f,0.58f,0.14f,sat,pe1r,pe1g,pe1b);
    float pw1r,pw1g,pw1b; lerpcol3(0.08f,0.30f,0.50f,sat,pw1r,pw1g,pw1b);
    float pw2r,pw2g,pw2b; lerpcol3(0.15f,0.48f,0.72f,sat,pw2r,pw2g,pw2b);
    float pw3r,pw3g,pw3b; lerpcol3(0.40f,0.72f,0.88f,sat,pw3r,pw3g,pw3b);
    float pw4r,pw4g,pw4b; lerpcol3(0.55f,0.82f,0.95f,sat,pw4r,pw4g,pw4b);

    Drawcircle(-0.68f,-0.58f,0.22f*1.25f,0.09f*1.6f,40,pe1r,pe1g,pe1b);
    Drawcircle(-0.68f,-0.58f,0.22f,0.09f,40,pw1r,pw1g,pw1b);
    Drawcircle(-0.68f,-0.58f+0.09f*0.1f,0.22f*0.85f,0.09f*0.75f,40,pw2r,pw2g,pw2b);
    Drawcircle(-0.68f-0.22f*0.20f,-0.58f+0.09f*0.10f,0.22f*0.22f,0.09f*0.18f,30,pw3r,pw3g,pw3b);
    Drawcircle(-0.68f+0.22f*0.15f,-0.58f-0.09f*0.05f,0.22f*0.12f,0.09f*0.10f,20,pw4r,pw4g,pw4b);
    Drawcircle(0.68f,-0.58f,0.22f*1.25f,0.09f*1.6f,40,pe1r,pe1g,pe1b);
    Drawcircle(0.68f,-0.58f,0.22f,0.09f,40,pw1r,pw1g,pw1b);
    Drawcircle(0.68f,-0.58f+0.09f*0.1f,0.22f*0.85f,0.09f*0.75f,40,pw2r,pw2g,pw2b);
    Drawcircle(0.68f-0.22f*0.20f,-0.58f+0.09f*0.10f,0.22f*0.22f,0.09f*0.18f,30,pw3r,pw3g,pw3b);
    Drawcircle(0.68f+0.22f*0.15f,-0.58f-0.09f*0.05f,0.22f*0.12f,0.09f*0.10f,20,pw4r,pw4g,pw4b);

    float trr,trg,trb; lerpcol3(0.40f,0.25f,0.10f,sat,trr,trg,trb);
    float tl1r,tl1g,tl1b; lerpcol3(0.15f,0.42f,0.08f,sat,tl1r,tl1g,tl1b);
    float tl2r,tl2g,tl2b; lerpcol3(0.22f,0.55f,0.12f,sat,tl2r,tl2g,tl2b);

    struct _TS { static void draw(float x,float y,float sc,
                                  float r1,float g1,float b1,
                                  float r2,float g2,float b2,
                                  float r3,float g3,float b3){
        glBegin(GL_QUADS);glColor3f(r1,g1,b1);
        glVertex2f(x-0.012f*sc,y);glVertex2f(x+0.012f*sc,y);
        glVertex2f(x+0.012f*sc,y+0.07f*sc);glVertex2f(x-0.012f*sc,y+0.07f*sc);glEnd();
        Drawcircle(x,y+0.13f*sc,0.065f*sc,0.065f*sc,40,r2,g2,b2);
        Drawcircle(x,y+0.15f*sc,0.045f*sc,0.045f*sc,40,r3,g3,b3);
    }};
#define drawTreeS(x,y,sc) _TS::draw(x,y,sc,trr,trg,trb,tl1r,tl1g,tl1b,tl2r,tl2g,tl2b)
    drawTreeS(-0.82f,-0.40f,1.1f);drawTreeS(-0.70f,-0.38f,1.0f);drawTreeS(-0.60f,-0.42f,0.9f);
    drawTreeS(0.60f,-0.38f,1.0f);drawTreeS(0.70f,-0.40f,1.1f);drawTreeS(0.80f,-0.36f,0.95f);drawTreeS(0.88f,-0.42f,0.85f);
    drawTreeS(-0.75f,-0.10f,0.55f);drawTreeS(-0.68f,-0.08f,0.50f);drawTreeS(0.60f,-0.08f,0.50f);drawTreeS(0.76f,-0.10f,0.55f);
#undef drawTreeS

    float bs1r,bs1g,bs1b; lerpcol3(0.18f,0.48f,0.10f,sat,bs1r,bs1g,bs1b);
    float bs2r,bs2g,bs2b; lerpcol3(0.20f,0.52f,0.12f,sat,bs2r,bs2g,bs2b);
    Drawcircle(-0.72f,-0.22f,0.040f,0.030f,30,bs1r,bs1g,bs1b);
    Drawcircle(-0.64f,-0.21f,0.035f,0.028f,30,bs2r,bs2g,bs2b);
    Drawcircle(0.64f,0.21f,0.035f,0.028f,30,bs1r,bs1g,bs1b);
    Drawcircle(0.72f,-0.22f,0.040f,0.030f,30,bs2r,bs2g,bs2b);
}

void DrawColorSpotlight(float cx,float cy,float radius,float cr,float cg,float cb,float strength){
    if(strength<=0.01f) return;
    int rings=40;
    for(int i=rings;i>=1;i--){
        float f=(float)i/rings;
        float ringR=radius*f;
        float edgeFade=1.0f-f;
        float a=edgeFade*edgeFade*strength*0.55f;
        if(a<0.005f) continue;
        Drawcirclea(cx,cy,ringR,ringR*0.42f,48,cr,cg,cb,a);
    }
}

void DrawHalo(float cx,float cy,float radius,float r,float g,float b,float alpha){
    for(int i=8;i>=1;i--){
        float f=(float)i/8.0f;
        Drawcirclea(cx,cy,radius*f*1.2f,radius*f*0.5f,40,r,g,b,alpha*(1.0f-f)*0.6f);
    }
    Drawcirclea(cx,cy,radius*0.35f,radius*0.15f,40,1.0f,1.0f,1.0f,alpha*0.5f);
}


void DrawFireEffect(float cx,float cy,float alpha){
    for(int i=0;i<18;i++){
        float angle=-0.6f+1.2f*(float)i/17.0f;
        float h=0.28f+0.22f*sinf(ta*3.5f+i*0.7f);
        float w=0.055f+0.025f*sinf(ta*2.0f+i*1.1f);
        float bx=cx+sinf(angle)*0.18f;
        float by=cy-0.30f;
        glBegin(GL_TRIANGLES);
        glColor4f(1.0f,0.35f+0.15f*sinf(ta+i),0.0f,alpha*0.80f);
        glVertex2f(bx-w,by);glVertex2f(bx+w,by);
        glColor4f(1.0f,0.90f,0.10f,0.0f);
        glVertex2f(bx+sinf(ta*2.5f+i)*0.02f,by+h);
        glEnd();
        glBegin(GL_TRIANGLES);
        glColor4f(1.0f,0.85f,0.40f,alpha*0.55f);
        glVertex2f(bx-w*0.45f,by);glVertex2f(bx+w*0.45f,by);
        glColor4f(1.0f,1.0f,0.9f,0.0f);
        glVertex2f(bx,by+h*0.55f);
        glEnd();
    }
    Drawcirclea(cx,cy-0.15f,0.32f,0.14f,40,1.0f,0.45f,0.0f,alpha*0.18f);
    Drawcirclea(cx,cy-0.15f,0.18f,0.08f,32,1.0f,0.75f,0.1f,alpha*0.22f);
}

void DrawRainEffect(float alpha){
    for(int i=0;i<80;i++){
        float rx=-1.0f+(float)((i*137)%200)/100.0f;
        float speed=0.8f+(float)((i*73)%100)/200.0f;
        float offset=(float)((i*53)%100)/100.0f;
        float ry=1.25f-fmodf(ta*speed+offset*2.25f,2.25f);
        float len=0.07f+(float)((i*31)%100)/2000.0f;
        float thick=0.004f;
        glBegin(GL_QUADS);
        glColor4f(0.55f,0.82f,1.0f,alpha*0.85f);
        glVertex2f(rx-thick,ry);glVertex2f(rx+thick,ry);
        glColor4f(0.70f,0.90f,1.0f,0.0f);
        glVertex2f(rx+thick*0.3f,ry-len);glVertex2f(rx-thick*0.3f,ry-len);
        glEnd();
    }
    for(int i=0;i<10;i++){
        float wx=-0.90f+i*0.20f;
        float ws=0.020f+0.010f*sinf(ta*5.0f+i*1.4f);
        Drawcirclea(wx,-0.42f,ws,ws*0.30f,16,0.55f,0.82f,1.0f,alpha*0.50f);
    }
}

void DrawEarthEffect(float cx,float cy,float alpha){
    float growT=fmodf(ta*0.5f,1.0f);
    float gr=0.35f+0.65f*growT;
    float positions[6]={-0.55f,-0.38f,-0.20f,0.20f,0.38f,0.55f};
    for(int i=0;i<6;i++){
        float tx=positions[i],ty=-0.42f;
        float h=0.22f*gr,tw=0.055f;
        glBegin(GL_QUADS);
        glColor4f(0.38f,0.22f,0.08f,alpha);
        glVertex2f(tx-tw*0.5f,ty);glVertex2f(tx+tw*0.5f,ty);
        glVertex2f(tx+tw*0.4f,ty+h*0.45f);glVertex2f(tx-tw*0.4f,ty+h*0.45f);
        glEnd();
        float cr2=0.085f*gr*(0.85f+0.15f*sinf(ta*2.0f+i));
        Drawcirclea(tx,ty+h*0.45f+cr2*0.7f,cr2*1.1f,cr2*0.85f,28,0.18f,0.55f,0.10f,alpha*0.9f);
        Drawcirclea(tx,ty+h*0.45f+cr2*0.9f,cr2*0.75f,cr2*0.60f,24,0.25f,0.68f,0.14f,alpha*0.8f);
    }
    for(int i=0;i<12;i++){
        float px2=cx-0.30f+(float)i*0.055f;
        float py2=-0.42f+fmodf(ta*0.35f+(float)i*0.18f,0.55f);
        float ps=0.012f+0.006f*sinf(ta+i);
        Drawcirclea(px2,py2,ps,ps,8,0.42f,0.28f,0.10f,alpha*0.45f*(1.0f-py2*1.2f));
    }
}

void DrawWindEffect(float cx,float cy,float alpha){
    for(int ring=0;ring<3;ring++){
        float rr=0.28f+ring*0.16f;
        float offset=ta*(1.8f-ring*0.4f)+(float)ring*2.1f;
        int pts=28+ring*6;
        glLineWidth(2.0f-ring*0.4f);
        glBegin(GL_LINE_STRIP);
        for(int j=0;j<=pts;j++){
            float fj=(float)j/pts;
            float ang=fj*2.0f*PI+offset;
            float wobble=0.04f*sinf(ang*3.0f+ta*4.0f);
            float rx2=(rr+wobble)*cosf(ang);
            float ry2=(rr+wobble)*0.42f*sinf(ang);
            glColor4f(0.82f,0.93f,1.0f,alpha*(0.6f-ring*0.15f)*fj*(1.0f-fj)*4.0f);
            glVertex2f(cx+rx2,cy-0.10f+ry2);
        }
        glEnd();
    }
    glLineWidth(1.0f);
    for(int i=0;i<20;i++){
        float ang2=ta*2.2f+(float)i*0.314f;
        float dist=0.15f+0.28f*((float)i/20.0f);
        float px2=cx+cosf(ang2)*dist;
        float py2=cy-0.10f+sinf(ang2)*dist*0.40f;
        float ps=0.008f+0.005f*sinf(ta*5.0f+i);
        Drawcirclea(px2,py2,ps,ps,6,0.88f,0.96f,1.0f,alpha*0.55f);
    }
}

struct ElemState {
    float rise,haloAlpha,haloRadius,spotStrength,charAlpha,label_a;
};

ElemState computeElemState(float t){
    ElemState s; s.rise=0;s.haloAlpha=0;s.haloRadius=0;s.spotStrength=0;s.charAlpha=0;s.label_a=0;
    if(t<0.0f) return s;
    if(t<0.8f){ float f=t/0.8f; s.rise=f;s.charAlpha=f;s.haloAlpha=f*0.7f;s.haloRadius=0.10f*f;s.spotStrength=0.0f; }
    else if(t<1.6f){ float f=(t-0.8f)/0.8f; s.rise=1.0f;s.charAlpha=1.0f;s.haloAlpha=0.7f+0.2f*sinf(f*PI*2.0f);s.haloRadius=0.12f;s.spotStrength=0.0f;s.label_a=f; }
    else if(t<2.8f){ float f=(t-1.6f)/1.2f; s.rise=1.0f;s.charAlpha=1.0f;s.haloAlpha=0.9f;s.haloRadius=0.12f+0.43f*f;s.spotStrength=f*0.85f;s.label_a=1.0f; }
    else if(t<3.8f){ float f=(t-2.8f)/1.0f; s.rise=1.0f;s.charAlpha=1.0f-f*0.25f;s.haloAlpha=0.9f*(1.0f-f);s.haloRadius=0.55f*(1.0f-f)*(1.0f-f);s.spotStrength=0.85f*(1.0f-f)*(1.0f-f);s.label_a=1.0f-f; }
    else if(t<5.0f){ float f=(t-3.8f)/1.2f; s.rise=1.0f-f;s.charAlpha=(1.0f-f)*0.75f;s.haloAlpha=0;s.haloRadius=0;s.spotStrength=0;s.label_a=0; }
    return s;
}

const char* elemMessages[4]={
    "I can restore the warmth...",
    "I can bring life to the rivers...",
    "I can make the earth bloom...",
    "I can fill the world with breath..."
};
const char* elemFail[4]={
    "But warmth alone is not enough...",
    "But water alone cannot color the world...",
    "But earth alone cannot make it whole...",
    "But wind alone cannot restore what was lost..."
};


void DrawElementScene(){
    if(elemPhase==0) return;
    int idx=elemPhase-1;
    if(idx<0||idx>3) return;
    ElemState es=computeElemState(elemTimer);
    float charY=-1.5f+es.rise*1.5f;
    float charX=0.0f, charScale=0.50f;
    float hr[4]={1.0f,0.20f,0.25f,0.85f};
    float hg[4]={0.45f,0.65f,0.68f,0.92f};
    float hb[4]={0.0f,0.95f,0.12f,1.0f};
    float spotCY=charY-0.60f*charScale;
    if(es.spotStrength>0.01f) DrawColorSpotlight(charX,spotCY,es.haloRadius,hr[idx],hg[idx],hb[idx],es.spotStrength);
    if(es.haloAlpha>0.01f){ float pulse=1.0f+0.07f*sinf(ta*5.0f); DrawHalo(charX,spotCY,0.10f*pulse,hr[idx],hg[idx],hb[idx],es.haloAlpha); }
    float ea=es.charAlpha;
    if(ea>0.01f){
        if(idx==0) DrawFireEffect(charX,charY,ea);
        else if(idx==1) DrawRainEffect(ea);
        else if(idx==2) DrawEarthEffect(charX,charY,ea);
        else if(idx==3) DrawWindEffect(charX,charY,ea);
    }
    if(idx==0) DrawFirePrinceAt(charX,charY,charScale);
    else if(idx==1) DrawWaterCharacterAt(charX,charY,charScale);
    else if(idx==2) DrawEarthCharacterAt(charX,charY,charScale);
    else if(idx==3) DrawAirCharacterAt(charX,charY,charScale);
    if(es.label_a>0.01f){
        if(elemTimer>0.8f&&elemTimer<2.8f)
            Drawtext(-0.52f,-0.78f,elemMessages[idx],GLUT_BITMAP_HELVETICA_18,hr[idx]*0.9f+0.1f,hg[idx]*0.9f+0.05f,hb[idx]*0.9f+0.05f,es.label_a);
        if(elemTimer>2.8f&&elemTimer<3.8f){
            float fa=1.0f-(elemTimer-2.8f)/1.0f;
            Drawtext(-0.52f,-0.78f,elemFail[idx],GLUT_BITMAP_HELVETICA_18,0.60f,0.55f,0.50f,fa);
        }
    }
}

// ================= SCENE 3 & SCENE 4 ADDITIONS =================

void DrawSmile(float cx, float cy, float rx, float ry, float r, float g, float b){
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(r,g,b);
    for(int i=0;i<=16;i++){
        float a = PI + (PI * i / 16.0f);
        glVertex2f(cx + rx*cos(a), cy + ry*sin(a));
    }
    glEnd();
}






void DrawCharacterSimple(float x, float y, float scale, float r, float g, float b, const char* label){
    // Unified character dispatcher:
    // Fire uses the original fire-prince drawing from the main scene.
    if(label[0]=='F'){
        DrawFirePrinceAt(x, y + 0.04f, scale * 0.34f);
      
    }
    else if(label[0]=='W'){
        DrawWaterCharacterAt(x, y + 0.04f, scale * 0.34f);
    }
    else if(label[0]=='A'){
        DrawAirCharacterAt(x, y + 0.04f, scale * 0.34f);
    }
    else{
        DrawEarthCharacterAt(x, y + 0.04f, scale * 0.34f);
    }
}


void DrawDialogueBox(float x1, float y1, float x2, float y2){
    glBegin(GL_QUADS);
    glColor4f(0.02f,0.02f,0.02f,0.58f);
    glVertex2f(x1,y1); glVertex2f(x2,y1); glVertex2f(x2,y2); glVertex2f(x1,y2);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glColor4f(0.95f,0.85f,0.45f,0.9f);
    glVertex2f(x1,y1); glVertex2f(x2,y1); glVertex2f(x2,y2); glVertex2f(x1,y2);
    glEnd();
}


void DrawScene3Realization(){
    // slightly dim but alive environment: realization stage before full restoration
    drawsky();
    drawground();
    drawclouds();
    DrawHillbackLeft();
    DrawHillbackRight();
    DrawHillbackMid();
    DrawRoad();
    DrawCastle();
    DrawDrawHillfrontLeft();
    DrawDrawHillfrontRight();
    DrawPonds();
    DrawTrees();
    DrawBushes();

    // soft gray overlay to show that the world is not fully restored yet
    glBegin(GL_QUADS);
    glColor4f(0.08f,0.08f,0.10f,0.82f);
    glVertex2f(-1.0f,1.25f); glVertex2f(1.0f,1.25f); glVertex2f(1.0f,-1.0f); glVertex2f(-1.0f,-1.0f);
    glEnd();

    Drawtext(-0.26f,1.08f,"Scene 3: Realization",GLUT_BITMAP_HELVETICA_18,1.0f,0.95f,0.70f,1.0f);
    Drawtext(-0.57f,0.96f,"The elements finally meet and understand cooperation.",GLUT_BITMAP_HELVETICA_18,0.95f,0.95f,0.88f,1.0f);

    // meeting circle
    Drawcirclea(0.0f,-0.25f,0.44f,0.18f,80,0.0f,0.0f,0.0f,0.20f);
    Drawcirclea(0.0f,-0.25f,0.36f,0.13f,80,0.95f,0.85f,0.45f,0.18f);

    // four elements meet around the center
    DrawCharacterSimple(-0.46f,-0.10f,0.72f,1.0f,0.25f,0.05f,"Fire");
    DrawCharacterSimple(-0.15f,-0.10f,0.72f,0.15f,0.55f,1.0f,"Water");
    DrawCharacterSimple(0.15f,-0.10f,0.72f,0.86f,0.88f,0.92f,"Air");
    DrawCharacterSimple(0.46f,-0.10f,0.72f,0.30f,0.70f,0.20f,"Earth");

    // dialogue
    DrawDialogueBox(-0.78f,-0.67f,0.78f,-0.91f);
    Drawtext(-0.70f,-0.73f,"Earth: Alone, none of us can restore life.",GLUT_BITMAP_HELVETICA_18,0.75f,1.0f,0.62f,1.0f);
    Drawtext(-0.70f,-0.82f,"Water: Together, we can bring balance back.",GLUT_BITMAP_HELVETICA_18,0.62f,0.82f,1.0f,1.0f);

    Drawtext(-0.79f,-0.98f,"Press 4 for Union & Restoration  |  Press 1 for Intro",GLUT_BITMAP_HELVETICA_12,0.90f,0.85f,0.65f,1.0f);
}

void DrawEnergyRay(float x1, float y1, float x2, float y2, float r, float g, float b){
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glColor4f(r,g,b,0.75f);
    glVertex2f(x1,y1); glVertex2f(x2,y2);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor4f(1.0f,1.0f,1.0f,0.65f);
    glVertex2f(x1,y1); glVertex2f(x2,y2);
    glEnd();
}

void DrawScene4UnionRestoration(){
    // full color restoration scene
    
    drawsky();
    drawground();
    DrawSun4();
    drawclouds();
    DrawHillbackLeft();
    DrawHillbackRight();
    DrawHillbackMid();
    DrawRoad();
    DrawCastle();
    DrawDrawHillfrontLeft();
    DrawDrawHillfrontRight();
    DrawRoad();
    DrawPonds();
    DrawTrees();
    DrawBushes();

    Drawtext(-0.35f,1.08f,"Scene 4: Union & Restoration",GLUT_BITMAP_HELVETICA_18,1.0f,0.95f,0.70f,1.0f);
    Drawtext(-0.63f,0.96f,"The elements unite. Colors return, movement returns, and life begins again.",GLUT_BITMAP_HELVETICA_18,0.95f,0.95f,0.88f,1.0f);

    float pulse = 0.04f + 0.035f * sin(ta * 4.0f);

    // glowing circle of energy in the center
    Drawcirclea(0.0f,-0.16f,0.42f+pulse,0.42f+pulse,100,1.0f,0.85f,0.10f,0.22f);
    Drawcirclea(0.0f,-0.16f,0.31f+pulse,0.31f+pulse,100,0.25f,0.70f,1.0f,0.28f);
    Drawcirclea(0.0f,-0.16f,0.20f+pulse,0.20f+pulse,100,0.35f,1.0f,0.35f,0.30f);
    Drawcirclea(0.0f,-0.16f,0.09f+pulse,0.09f+pulse,100,1.0f,1.0f,1.0f,0.65f);

    // colorful rays from elements to the center
    DrawEnergyRay(-0.48f,-0.08f,0.0f,-0.16f,1.0f,0.25f,0.05f);
    DrawEnergyRay(0.48f,-0.08f,0.0f,-0.16f,0.15f,0.55f,1.0f);
    DrawEnergyRay(0.0f,0.28f,0.0f,-0.16f,0.90f,0.92f,1.0f);
    DrawEnergyRay(0.0f,-0.58f,0.0f,-0.16f,0.30f,0.75f,0.18f);

    // four elements around the energy circle
    DrawCharacterSimple(-0.58f,-0.08f,0.68f,1.0f,0.25f,0.05f,"Fire");
    DrawCharacterSimple(0.58f,-0.08f,0.68f,0.15f,0.55f,1.0f,"Water");
    DrawCharacterSimple(0.0f,0.33f,0.68f,0.86f,0.88f,0.92f,"Air");
    DrawCharacterSimple(0.0f,-0.57f,0.68f,0.30f,0.70f,0.20f,"Earth");

    // extra movement: small sparkles around the circle
    for(int i=0;i<18;i++){
        float a = 2.0f * PI * i / 18.0f + ta * 1.8f;
        float rr = 0.30f + 0.06f * sin(ta*3.0f + i);
        float x = cos(a) * rr;
        float y = -0.16f + sin(a) * rr;
        Drawcirclea(x,y,0.012f,0.012f,18,1.0f,0.95f,0.30f,0.75f);
    }

    DrawDialogueBox(-0.77f,-0.82f,0.77f,-0.99f);
    Drawtext(-0.68f,-0.89f,"Together, the four elements restore harmony and teamwork.",GLUT_BITMAP_HELVETICA_18,1.0f,0.95f,0.75f,1.0f);
    Drawtext(-0.79f,-1.07f,"Press 3 for Realization  |  Press 1 for Intro",GLUT_BITMAP_HELVETICA_12,0.90f,0.85f,0.65f,1.0f);
}
// ================= END SCENE 3 & SCENE 4 ADDITIONS =================

void drawgreyoverlay(float alpha){
    if(alpha<=0.0f) return;
    glBegin(GL_QUADS);
    glColor4f(0.12f,0.12f,0.15f,alpha);
    glVertex2f(-1.0f,1.25f);glVertex2f(1.0f,1.25f);
    glVertex2f(1.0f,-1.0f);glVertex2f(-1.0f,-1.0f);
    glEnd();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();

    if(sceneNumber==3){
        DrawScene3Realization();
        glutSwapBuffers();
        return;
    }
    if(sceneNumber==4){
        DrawScene4UnionRestoration();
        glutSwapBuffers();
        return;
    }

    drawsky();
    drawground();
    DrawSun();
    drawclouds();
    DrawHillbackLeft();
    DrawHillbackRight();
    DrawHillbackMid();
    DrawRoad();
    DrawCastle();
    DrawDrawHillfrontLeft();
     DrawDrawHillfrontRight();
     DrawRoad();
     DrawPonds();
     DrawTrees();
     DrawBushes();
     drawgreyoverlay(greyalpha);
     

     if(!showscene){
Drawintrotext();
     }

     else{
        ElemState es;
        float spotX=0.0f,spotY=0.0f,spotRadius=0.0f;
        if(elemPhase>=1&&elemPhase<=4){
            es=computeElemState(elemTimer);
            float charY=-1.5f+es.rise*1.5f;
            spotY=charY-0.60f*0.50f;
            spotRadius=es.haloRadius;
        }
        if(spotRadius>0.01f&&es.spotStrength>0.01f){
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_ALWAYS,1,0xFF);
            glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
            glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
            glBegin(GL_POLYGON);
            for (int i = 0; i < 80; i++)
            {
                float a=2.0f*PI*i/80;
                glVertex2f(spotX+spotRadius*cos(a),spotY+spotRadius*0.42f*sin(a));

            }
            glEnd();
            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glStencilFunc(GL_EQUAL,1,0xFF);
            glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
            DrawSceneColored(1.0f);
            glDisable(GL_STENCIL_TEST);
            

        }
        DrawElementScene();
     }
     glutSwapBuffers();

}
void timer(int){
    float dt=1.0f/60.0f;
    firsttimer+=dt;
    ta+=dt;
    updateParticle();
    if(firstline==0 && firsttimer>0.5f)  firstline=1;
    if(firstline==1 && firsttimer>1.8f)  firstline=2;
    if(firstline==2 && firsttimer>3.5f)  firstline=3;
    if(firstline==3 && firsttimer>5.2f)  firstline=4;
    if(firstline==4 && firsttimer>6.8f)  firstline=5;
    if(fadingtext){
        textalpha-=dt*1.0f;
        if(textalpha<=0.0f){textalpha=0.0f;fadingtext=false;}
    }
    if(!fadingtext && textalpha<=0.0f && !showscene && greyalpha<0.82f){
    greyalpha+=dt*0.3f;
    if(greyalpha>0.82f) greyalpha=0.82f;
}
if(!fadingtext && textalpha<=0.0f && greyalpha>=0.82f && !showscene){
    showscene=true;
    elemPhase=1;
    elemTimer=0.0f;
}
    if(settimgsun&&suny>0.2f){
        suny -=0.003f;
    }
    if(growingpond){
        scalepond +=0.008f;
        if(scalepond >=1.3f)
        growingpond =false;
    }
    else{
     scalepond -=0.008f;
     if(scalepond<=0.7f)
     growingpond =true;   
    }

    if(showscene){
        
   if(showscene && sceneNumber==0){
        elemTimer+=dt;
        if(elemTimer>=PHASE_DURATION && elemPhase>=1 && elemPhase<=4){
            elemPhase++;
            elemTimer=0.0f;
            if(elemPhase>4){
                sceneNumber=3;
                scene3timer=0.0f;
            }
        }
    }

    if(sceneNumber==3){
        scene3timer+=dt;
        if(scene3timer>=SCENE3_DURATION){
            sceneNumber=4;
            
        }
    }
    if(sceneNumber ==4){
        settingsun4 =true;
    }
    if(settingsun4 && suny4 <0.92f){
        suny4 +=0.0025f;
        if(suny4>0.92f)
        suny4 =0.92f;
    }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16,timer,0);
}

void keyboard(unsigned char key,int,int){
    if(key==' ' && firstline>=5 && !fadingtext && textalpha>0.0f) {
    fadingtext=true;
    settimgsun=true;
    }

    switch(key){
    case 7:
     exit(0);
break;

case '1':
sceneNumber=0;
showscene=false;
fadingtext =false;
textalpha=1.0f;
greyalpha=0.0f;
firsttimer=0.0f;
firstline=0;
elemPhase=0;
elemTimer=0.0f;
scene3timer=0.0f;
suny=0.92f;
settimgsun=false;
break;

case '2':
sceneNumber=0;
showscene=true;
fadingtext =false;
textalpha=0.0f;
greyalpha=0.82f;
elemPhase=1;
elemTimer=0.0f;
scene3timer=0.0f;
break;

case '3':
sceneNumber=3;
scene3timer=0.0f;
showscene=true;
break;

case '4':
sceneNumber=4;
showscene=true;
settingsun4=true;
break;

}
}
GLuint LoadTexture( const char * filename, int width, int height )
{
    GLuint texture;
    unsigned char*data;

    FILE* file =fopen(filename,"rb");

    
    if ( file == NULL )
    {
        cout <<"Unable to open the image file" <<endl ;
       cout<< "Program will exit :("<<endl;
        exit(0);
        return 0;
    }

    data = (unsigned char *)malloc( width * height * 3 );
    fread( data, width * height * 3, 1, file );
    
    fclose( file );

    for(int i = 0; i < width * height ; ++i)
    {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];
        
        data[index] = R;
        data[index+2] = B;
        
    }

    glGenTextures( 1, &texture );            //generate the texture with the loaded data
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //set texture environment parameters
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
        

    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    /////////////////////////////////////////
    
    free( data ); //free the texture array
    
    if (glGetError()!=GL_NO_ERROR)
        printf("GLError in genTexture()\n");
    
    return texture; //return whether it was successfull  

}

GLuint LoadTextureMod( const char * filename, int width, int height )
{
    GLuint texture;
    unsigned char*data;

    FILE* file =fopen(filename,"rb");

    
    if ( file == NULL )
    {
        cout <<"Unable to open the image file" <<endl ;
       cout<< "Program will exit :("<<endl;
        exit(0);
        return 0;
    }

    data = (unsigned char *)malloc( width * height * 3 );
    fread( data, width * height * 3, 1, file );
    
    fclose( file );

    for(int i = 0; i < width * height ; ++i)
    {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];
        
        data[index] = R;
        data[index+2] = B;
        
    }

    glGenTextures( 1, &texture );            //generate the texture with the loaded data
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
        

    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    /////////////////////////////////////////
    
    free( data ); //free the texture array
    
    if (glGetError()!=GL_NO_ERROR)
        printf("GLError in genTexture()\n");
    
    return texture; //return whether it was successfull  

}

void init(){
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0,1.0,-1.0,1.25);
    glMatrixMode(GL_MODELVIEW);
        textureprojectroad=LoadTexture(roadpath,512,512);
                textureprojectbrick=LoadTextureMod(brickpath,512,512);


    srand(42);
    InitParticle();
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_STENCIL);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(50,50);
    glutCreateWindow("CG Project");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16,timer,0);
    glutMainLoop();
    return 0;

}
