//


#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define ESC 27// ASCII for escape

typedef struct {
    int y,x,hp;
}ent;

#define ENTS_ 15
ent ent_l[ENTS_];

#define Y_ 11
#define X_ 17
ent *ent_m[Y_][X_];

//increased the size of the map
char *map[]={
    "#################",
    "#      #        #",
    "#               #",
    "#      ###   ####",
    "#               #",
    "#### ###   #    #",
    "#          #    #",
    "#          #    #",
    "#               #",
    "#          #    #",
    "#################"
};

void init_ent() {
    for (int e=0;e<ENTS_;e++) {
        ent *ce=&ent_l[e];
        ce->hp=3;
        do {
            ce->y=rand()%Y_;	//changed to arc4random 
            ce->x=rand()%X_;	//changed to arc4random
        } while ( '#'==map[ce->y][ce->x] || NULL!=ent_m[ce->y][ce->x] );
        ent_m[ce->y][ce->x]=ce;
    }
}

//move player if there is no living entity on the way
void move_to(int *y,int *x,int dy,int dx) {
    //remove reference to the player's old position
    ent_m[*y][*x]=NULL;
   
    //if the destination tile has an entity in it
    if (NULL!=ent_m[*y+dy][*x+dx]) {
        //decrement hitpoints of the entity at the destination tile
        ent *de=ent_m[*y+dy][*x+dx];
        de->hp--;
        //if it's still alive don't move into its place
        if (0 < de->hp) {
            dy=0;
            dx=0;
        //if it's dead remove its reference
        } else {
            ent_m[*y+dy][*x+dx]=NULL;
        }
    }
    //update player's position
    *y+=dy;
    *x+=dx;

    //add reference to the player's new position
    ent_m[*y][*x]=&ent_l[0];
}

int main() {
    //initialize curses
    keypad(initscr(),1);
    curs_set(0);    // 0 = hidden, 1 = visible, 2 = very vissble

    //initialize entities
    //srand(time(0));      //not needed when using arc4random
    init_ent();

    //player's starting coordinates
    int *y=&ent_l[0].y;
    int *x=&ent_l[0].x;

    //last key pressed
    int c=0;
    do {
        //draw map
        for (int yy=0;yy<Y_;yy++)
            for (int xx=0;xx<X_;xx++)
                mvaddch(yy,xx,map[yy][xx]);

        //move player if there is no wall on the way
        if (KEY_UP==c && ' '==map[*y-1][*x])
            move_to(y,x,-1,0);
        if (KEY_DOWN==c && ' '==map[*y+1][*x])
            move_to(y,x,1,0);
        if (KEY_LEFT==c && ' '==map[*y][*x-1])
            move_to(y,x,0,-1);
        if (KEY_RIGHT==c && ' '==map[*y][*x+1])
            move_to(y,x,0,1);

        //draw entities
        for (int e=0;e<ENTS_;e++) {
            if (ent_l[e].hp>0)
                mvaddch(ent_l[e].y,ent_l[e].x, 0==e?'@':'m');
        }
    //quit when ESC is pressed
    } while ((ESC!=(c=getch())));

    //clean up after we've finished using curses
    endwin();
}
