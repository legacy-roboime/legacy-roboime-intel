#include "soccer.h"

static Vector2 red_move_table[NPLAYERS];
static Vector2 blue_move_table[NPLAYERS];


Vector2 get_red_move_table( int i )
{
 return red_move_table[i];
}


Vector2 get_blue_move_table( int i )
{
 return blue_move_table[i];
}


void set_red_move_table( int i, Vector2 newpos  )
{
 red_move_table[i] = newpos;
}


void set_blue_move_table( int i, Vector2 newpos )
{
 blue_move_table[i] = newpos;
}

