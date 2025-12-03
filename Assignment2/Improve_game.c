#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define LANES 5
#define OBST 3
#define ROWS 12
#define FRAME 120

typedef struct { int lane,y,active; } O;
HANDLE con;

void setCur(int x,int y){ COORD p={(SHORT)x,(SHORT)y}; SetConsoleCursorPosition(con,p); }

int main(){
    con = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci; GetConsoleCursorInfo(con,&cci); cci.bVisible=FALSE; SetConsoleCursorInfo(con,&cci);
    srand((unsigned)time(NULL));

    O obs[OBST]; int player=LANES/2, score=0, lives=3, speed=FRAME;
    for(int i=0;i<OBST;i++){ obs[i].active=1; obs[i].lane=rand()%LANES; obs[i].y=-(rand()%ROWS); }

    // print an initial full frame so the buffer size is set
    for(int r=0;r<ROWS+7;r++) puts("");

    while(1){
        // input
        if(_kbhit()){
            int c=getch();
            if(c==0||c==224){ int c2=getch(); if(c2==75&&player>0) player--; if(c2==77&&player<LANES-1) player++; }
            else{ if((c=='a'||c=='A')&&player>0) player--; if((c=='d'||c=='D')&&player<LANES-1) player++; if(c=='q'||c=='Q') break; }
        }

        // rewind and draw (no clearing)
        setCur(0,0);
        int width = LANES*3;
        // top border and title
        printf("+"); for(int i=0;i<width;i++) printf("-"); printf("+   Game\n");
        // rows
        for(int row=0; row<ROWS; row++){
            printf("|");
            for(int lane=0; lane<LANES; lane++){
                int printed=0;
                for(int j=0;j<OBST;j++) if(obs[j].active && obs[j].y==row && obs[j].lane==lane){ printf(" * "); printed=1; break; }
                if(!printed) printf("   ");
            }
            printf("|");
            if(row==1) printf("   Score: %d", score);
            if(row==3) printf("   Lives: %d", lives);
            if(row==5) printf("   Press Q to quit");
            printf("\n");
        }
        // player row
        printf("|");
        for(int lane=0; lane<LANES; lane++) printf(lane==player ? " # " : "   ");
        printf("|   Speed: %d ms\n", speed);
        // bottom border
        printf("+"); for(int i=0;i<width;i++) printf("-"); printf("+\n");

        // collision
        for(int j=0;j<OBST;j++){
            if(obs[j].active && obs[j].y==ROWS && obs[j].lane==player){
                lives--; obs[j].active=0;
                if(lives==0){
                    printf("\nGAME OVER!\n");
                    goto end;
                }
            }
        }

        Sleep(speed);

        // move obstacles
        for(int j=0;j<OBST;j++){
            if(!obs[j].active){ obs[j].active=1; obs[j].y=-(rand()%5); obs[j].lane=rand()%LANES; continue; }
            obs[j].y++;
            if(obs[j].y>ROWS){ obs[j].y=-(rand()%5); obs[j].lane=rand()%LANES; score++; }
        }
        if(score>0 && score%5==0 && speed>60) speed-=5;
    }

end:
    cci.bVisible=TRUE; SetConsoleCursorInfo(con,&cci);
    printf("Press any key to exit...");
    getch();
    return 0;
}
