#include "minimax.h"
#include "soccer_draw.h"

static int winWidth, winHeight;
static SoccerState *s, saux;


void redraw( void ){
   int i;
   
   SoccerAction red_action, blue_action;
   saux = *s;
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   minimax_use_next_red_robot();
   minimax_use_next_blue_robot();
   minimax_play( &saux, MINIMAX_MAX_LEVEL );
   red_action = *minimax_get_best_red_action();
   blue_action = *minimax_get_best_blue_action();
   saction_blue_act( &saux, &blue_action );
   saction_red_act( &saux, &red_action );
   glViewport(winWidth /2., 0, winWidth /2., winHeight ); 
   soccer_redraw( &saux ); 
   //blue_action = saction_blue_make(s); 
   saction_simulate( s, &red_action, &blue_action, .1 );
   glViewport(0, 0, winWidth/2., winHeight );   
   soccer_redraw( s );
   glutSwapBuffers();
   //usleep(800000);
}


void reshape(int wid, int ht)
{
 winWidth = wid;
 winHeight = ht;
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-soccer_env()->hfield_w, soccer_env()->hfield_w, 
         -soccer_env()->hfield_h, soccer_env()->hfield_h, -1.f, 1.f);
 glMatrixMode(GL_MODELVIEW);
}


int main( int argc, char **argv )
{
 soccer_env_init();
 s = sstate_alloc();
 minimax_init(s);
 glutInit(&argc, argv);
 glutInitWindowSize(1200, 400);
 glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
 (void)glutCreateWindow("Soccer");
 glutReshapeFunc(reshape);
 glEnable(GL_DEPTH_TEST);
 glutDisplayFunc(redraw);
 glutIdleFunc(redraw);
 glutMainLoop();
 return 0;
}
