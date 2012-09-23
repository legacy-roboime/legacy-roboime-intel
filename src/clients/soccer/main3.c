#include "minimax.h"
#include "soccer_draw.h"
#ifdef HAVE_WINDOWS
#include <windows.h>
void sleep(unsigned int msec) {
    Sleep(msec);
}
#else
#include <unistd.h>
#endif

static int winWidth, winHeight;
static SoccerState *s, saux;
static char *image;

static void redraw( void );
static void reshape(int wid, int ht);
static void img_ppm_write( char *img, char *fname );

#define WIDTH 900
#define HEIGHT 300

int main( int argc, char **argv )
{
 image = ( char* )malloc( 3*WIDTH*HEIGHT );
 soccer_env_init();
 soccer_env_red_side( LEFT );
 s = sstate_alloc();
 minimax_init(s);
 glutInit(&argc, argv);
 glutInitWindowSize(WIDTH ,HEIGHT);
 glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |  GLUT_DEPTH);
 (void)glutCreateWindow("Soccer");
 glutReshapeFunc(reshape);
 glEnable(GL_DEPTH_TEST);
 glutDisplayFunc(redraw);
 glutIdleFunc(redraw);
 glutMainLoop();
 return 0;
}

void redraw( void ){
   static int count = 0;
   char fname[50];
   SoccerAction red_action, blue_action;

   saux = *s;
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   minimax_play( &saux, MINIMAX_MAX_LEVEL );
   red_action = *minimax_get_best_red_action();
   blue_action = *minimax_get_best_blue_action();
   saction_blue_act( &saux, &blue_action );
   saction_red_act( &saux, &red_action );
   glViewport(winWidth /2., 0, winWidth /2., winHeight ); 
   soccer_redraw( &saux ); 
   sleep(100);
   if(red_action.type == null_action)
	   printf("NULL ACTION\n");
   saction_simulate( s, &red_action, &blue_action, 
                     soccer_env()->sample_period );
   glViewport(0, 0, winWidth/2., winHeight );   
   soccer_redraw( s );
   glPixelStorei(GL_PACK_ALIGNMENT , 1);
   glReadBuffer(GL_BACK);
   glutSwapBuffers();
   glReadPixels( 0, 0,  WIDTH, HEIGHT, GL_RGB,
                 GL_UNSIGNED_BYTE, image );
  // sprintf( fname, "frame%i.ppm", count++ ); 
  // img_ppm_write( image, fname );
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


void img_ppm_write( char *img, char *fname )
{
 int i;

 FILE *fout = fopen( fname, "w" );
 fprintf(fout, "P6\n");
 fprintf(fout, "%d %d\n", WIDTH, HEIGHT );
 fprintf(fout, "%d\n", 255);

 for( i = 0; i < 3*WIDTH*HEIGHT; i++ )
    fputc( img[i], fout ); 

 fclose( fout );
}
