#include "soccer_draw.h"


#define NSIDES 50


static GLubyte red_color[NPLAYERS][3] = { 
      {128,0,128},{0,0,255},{0,255,0},
      {0,255,255},{0,128,128},{70,70,128}
};

static GLubyte blue_color[NPLAYERS][3] = {
      {255,0,255},{255,255,0},{128,128,128},
      {128,0,0},{255,0,0},{128,70,70}
};

static void draw_cone(float radius);
static void draw_circle(float radius);
static void draw_red_robots( SoccerState *s, int id );
static void draw_blue_robots( SoccerState *s, int id );
static void draw_red_kick_sector(SoccerState *s);
static void draw_blue_kick_sector(SoccerState *s);



void soccer_redraw( SoccerState *s )
{
 int i;
 glClearDepth(10);
 glClearColor(0,0,0,0);
// glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
 glPointSize(8.f);

 for(i = 0; i < NPLAYERS; i++) {
   /* Draw ball */
   glColor3ub(255,255,255);
   glPushMatrix();
   glTranslatef(s->ball.x,s->ball.y, 0.f);
   draw_circle(.04);
   glPopMatrix();
   glEnd();

   draw_red_robots( s, i );
   draw_blue_robots( s, i );
   draw_red_kick_sector(s);
   draw_blue_kick_sector(s);
 }
}


void draw_red_robots( SoccerState *s, int id )
{
 glPushMatrix();
 glTranslatef(s->red[id].x, s->red[id].y, 0.f);
 glColor3ub(red_color[id][0], red_color[id][1],
           red_color[id][2]);
 draw_cone( 100*soccer_env()->red_speed );
 glColor3ub(255,30,30);
 draw_circle(soccer_env()->robot_radius);
 glPopMatrix();
}


void draw_blue_robots( SoccerState *s, int id )
{
 glPushMatrix();
 glTranslatef(s->blue[id].x, s->blue[id].y, 0.f);
 glColor3ub(blue_color[id][0], blue_color[id][1],
           blue_color[id][2]);
 draw_cone( 100*soccer_env()->blue_speed );
 glColor3ub(30,30,255);
 draw_circle(soccer_env()->robot_radius);
 glPopMatrix();
}


void draw_cone(float radius)
{
 int i;
 float s,c; 

 glBegin(GL_TRIANGLE_FAN);
 glVertex3f(0.f, 0.f, 1.f);
 for(i = 0; i <= NSIDES; i++) {
    s = sinf((2.f*M_PI*i)/NSIDES)*radius;
    c = cosf((2.f*M_PI*i)/NSIDES)*radius;
    glVertex3f(s, c, 0.f);
 }
 glEnd();
}


void draw_circle(float radius)
{
 int i;
 float s,c; 

 glBegin(GL_TRIANGLE_FAN);
 glVertex3f(0.f, 0.f, 1.f);
 for(i = 0; i <= NSIDES; i++) {
    s = sinf((2.f*M_PI*i)/NSIDES)*radius;
    c = cosf((2.f*M_PI*i)/NSIDES)*radius;
    glVertex3f(s, c, 1.f);
 }
 glEnd();
}


void draw_red_kick_sector(SoccerState *s)
{
if( v2_norm( v2_sub( s->ball, v2_make( 
         -soccer_env()->hfield_w, 0 ))) < 
      soccer_env()->max_red_kick_dist )                  
        glColor3ub(255,100,100);
 else
        glColor3ub(255,255,255);
 glBegin(GL_LINE);
 if( s->red_ball_owner >= 0 ){
   glVertex3f(s->ball.x, s->ball.y, 1. );
   glVertex3f( -soccer_env()->hfield_w, 
               .5*soccer_env()->goal_size, 1. );
   glVertex3f(s->ball.x, s->ball.y, 1. );
   glVertex3f( -soccer_env()->hfield_w, 
               -.5*soccer_env()->goal_size, 1. );
 }  
 glEnd();
}



void draw_blue_kick_sector(SoccerState *s)
{
 if( v2_norm( v2_sub( s->ball, v2_make( 
         soccer_env()->hfield_w, 0 ))) < 
      soccer_env()->max_blue_kick_dist )                  
        glColor3ub(255,100,100);
 else
        glColor3ub(255,255,255);
 glBegin(GL_LINE);
 if( s->blue_ball_owner >= 0 ){
   glVertex3f(s->ball.x, s->ball.y, 1. );
   glVertex3f( soccer_env()->hfield_w, 
               .5*soccer_env()->goal_size, 1. );
   glVertex3f(s->ball.x, s->ball.y, 1. );
   glVertex3f( soccer_env()->hfield_w, 
               -.5*soccer_env()->goal_size, 1. );
 }  
 glEnd();
}



