#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <algorithm>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

using namespace std;

const int MAXN=1e3+1;
int col[MAXN],c1=COLOR(0,0,255),c2=COLOR(255,0,0),c12=1;
int n,m,R=40,rx=50,r2x=30,timeloss=120,r3x=16;
int brdg[MAXN][MAXN],cnt;
int MAXX,MAXY,mode,plrnum=2,player=1,SETING=0;
char M[2000000];
int pointer,zed[MAXN*2];
int CLOCK=0,CHRO=0,CTRL=0,SCORE=0;
char CHEAT[]="CHT";
int cht,chflag,gametime,firstplayer=0;
FILE *fp;
char playername[30];
char TIME[30];
time_t mytime,lasttime=time(NULL);
int onetime=0,twotime=0,r=13;

void turn(int x,int t){
    setcolor(WHITE);
    int COL;
    if(t==1){
        COL=c1;
        brdg[col[x]][x]=1;
    }else{
        COL=c2;
        brdg[col[x]][x]=2;
    }
    zed[pointer++]=col[x];
    zed[pointer++]=x;
    int y=R+rx+r2x,X=(2*x-1)*R,one=1,LEN=(2*col[x]-1)*R+rx+r2x,END=LEN;
    while(LEN){
        while(y<=END){
            getimage(X-R,y-R,X+R,y+R,M);
            setfillstyle(1,COL);
            fillellipse(X,y,R,R);
            int j;
            j=(y-(rx+r2x))/(2*R)+1;
            if(j!=n){
                setfillstyle(1,BROWN);
                bar(0,rx+r2x+2*R*j-r3x/2,MAXX,rx+r2x+2*R*j+r3x/2);
            }
            y++;
            for(int I=0;I<100000;++I);
            putimage(X-R,y-R-1,M,0);
        }
        LEN/=2;
        while(y>=END-LEN){
            getimage(X-R,y-R,X+R,y+R,M);
            setfillstyle(1,COL);
            fillellipse(X,y,R,R);
            int j;
            j=(y-(rx+r2x))/(2*R)+1;
            if(j!=n){
                setfillstyle(1,BROWN);
                bar(0,rx+r2x+2*R*j-r3x/2,MAXX,rx+r2x+2*R*j+r3x/2);
            }
            y--;
            for(int I=0;I<100000;++I);
            putimage(X-R,y-R+1,M,0);
        }
    }
    setfillstyle(1,COL);
    fillellipse(X,y,R,R);
    int j;
    j=(y-(rx+r2x))/(2*R)+1;
    setfillstyle(1,BROWN);
    bar(0,rx+r2x+2*R*j-r3x/2,MAXX,rx+r2x+2*R*j+r3x/2);
    j=(y-(rx+r2x))/(2*R);
    setfillstyle(1,BROWN);
    bar(0,rx+r2x+2*R*j-r3x/2,MAXX,rx+r2x+2*R*j+r3x/2);
    col[x]--;
}

void draw(int x){
    int X[4];
    X[1]=(x-1)*2*R;
    X[2]=x*2*R;
    X[3]=(X[1]+X[2])/2;
    int shape[]={
        X[1],r2x,
        X[2],r2x,
        X[3],rx+r2x
    };
    line(X[1],r2x,X[3],rx+r2x);
    line(X[2],r2x,X[3],rx+r2x);
    line(X[1],r2x,X[2],r2x);
    setfillstyle(1,COLOR(0,255,0));
    fillpoly(3,shape);

}

int check(){
    for(int i=1;i<=n-3;++i){
        for(int j=1;j<=m;j++){
            if(brdg[i][j]==brdg[i+1][j] && brdg[i][j]==brdg[i+2][j] && brdg[i][j]==brdg[i+3][j] && brdg[i][j]!=0){
                return brdg[i][j];
            }
        }
    }
    for(int i=1;i<=n;++i){
        for(int j=1;j<=m-3;j++){
            if(brdg[i][j]==brdg[i][j+1] && brdg[i][j]==brdg[i][j+2] && brdg[i][j]==brdg[i][j+3] && brdg[i][j]!=0){
                return brdg[i][j];
            }
        }
    }
    for(int i=1;i<=n-3;++i){
        for(int j=1;j<=m-3;j++){
            if(brdg[i][j]==brdg[i+1][j+1] && brdg[i][j]==brdg[i+2][j+2] && brdg[i][j]==brdg[i+3][j+3] && brdg[i][j]!=0){
                return brdg[i][j];
            }
        }
    }
    for(int i=1;i<=n-3;++i){
        for(int j=4;j<=m;j++){
            if(brdg[i][j]==brdg[i+1][j-1] && brdg[i][j]==brdg[i+2][j-2] && brdg[i][j]==brdg[i+3][j-3] && brdg[i][j]!=0){
                return brdg[i][j];
            }
        }
    }
    return 0;
}

int recf(int t,int ct,int TT){
    int CH=check();
    if(CH==1)   return -1;
    else if(CH==2)        return 1;
    if(t==2){
        int ansx=0,ans=1000,zero=0,wone=0,wans=0;
        if(ct==0)   return 0;
        for(int x=1;x<=m;++x){
            if(col[x]){
                brdg[col[x]][x]=2;
                col[x]--;
                int temp=recf(t%2+1,ct-1,TT);
                col[x]++;
                brdg[col[x]][x]=0;
                if(temp>0){
                    if(ans>temp+1){
                        ans=temp+1;
                        ansx=x;
                    }
                }else if(temp==0){
                    zero=x;
                }else{
                    if(wone>temp){
                        wone=temp;
                        wans=x;
                    }
                }
            }
        }
        if(ansx!=0){
            if(ct==TT)  return ansx;
            else    return ans;
        }else{
            if(zero!=0){
                if(ct==TT)  return zero;
                else    return 0;
            }else{
                if(ct==TT)  return wans;
                else    return wone;
            }
        }
    }else{
        int wone=-1000,ans=0,zero=0;
        for(int x=1;x<=m;++x){
            if(col[x]){
                brdg[col[x]][x]=1;
                col[x]--;
                int temp=recf(t%2+1,ct,TT);
                col[x]++;
                brdg[col[x]][x]=0;
                if(temp<0){
                    if(wone<temp-1){
                        wone=temp-1;
                    }
                }else if(temp==0){
                    zero=x;
                }else{
                    if(ans<temp){
                        ans=temp;
                    }
                }
            }
        }
        if(wone!=-1000){
            return wone;
        }
        if(zero!=0) return 0;
        return ans;
    }
}

void cmpturn(int mode,int x){
    if(mode==1){
        int i=rand()%m+1;
        while(col[i]==0){
            i=rand()%m+1;
        }
        turn(i,2);
    }else{
        int i=recf(2,mode-1,mode-1);
        if(i==0){
            cmpturn(1,x);
        }else{
            turn(i,2);
        }
    }
}

int givech(){
    int b=check();
    if(b){
        setcolor(GREEN);
        if(plrnum==2){
            settextstyle(0,0,2);
            sprintf(TIME,"PLAYER NUMBER %d WON!",b);
            outtextxy(MAXX/8,MAXY/8,TIME);
            getch();
            return 1;
        }
        setcolor(COLOR(0,255,0));
        if(b==1){
            fp=fopen("scoreboard.txt","a");
            fprintf(fp,"%s %d\n",playername,cnt);
            fclose(fp);
            settextstyle(0,0,4);
            outtextxy(MAXX/4,MAXY/4,"YOU WON!");
            getch();
        }else{
            settextstyle(0,0,4);
            outtextxy(MAXX/4,MAXY/4,"YOU LOST!");
            getch();
        }
        return 1;
    }
    return 0;
}

void czed(){
    if(pointer==0)  return;
    int x1=zed[--pointer],y1=zed[--pointer];
    int x2=zed[--pointer],y2=zed[--pointer];
    brdg[y1][x1]=0;
    brdg[y2][x2]=0;
    col[x1]++;
    col[x2]++;
    setfillstyle(2,COLOR(100,100,100));
    setcolor(WHITE);
    fillellipse((2*x1-1)*R,(2*y1-1)*R+rx+r2x,R,R);
    fillellipse((2*x2-1)*R,(2*y2-1)*R+rx+r2x,R,R);
    int j;
    j=y1;
    setfillstyle(1,BROWN);
    bar(0,rx+r2x+2*R*j-r3x/2,MAXX,rx+r2x+2*R*j+r3x/2);
    j=y2;
    setfillstyle(1,BROWN);
    bar(0,rx+r2x+2*R*j-r3x/2,MAXX,rx+r2x+2*R*j+r3x/2);
    if(y1!=1){
        j=y1-1;
        setfillstyle(1,BROWN);
        bar(0,rx+r2x+2*R*j-r3x/2,MAXX,rx+r2x+2*R*j+r3x/2);
    }
    if(y2!=1){
        j=y2-1;
        setfillstyle(1,BROWN);
        bar(0,rx+r2x+2*R*j-r3x/2,MAXX,rx+r2x+2*R*j+r3x/2);
    }
}

void seting(int r){
    setfillstyle(1,WHITE);
    if(CLOCK==1)    setfillstyle(1,GREEN);
    bar(MAXX-80,r,MAXX-80+r,2*r);
    outtextxy(MAXX-80+r+2,r,"Clock");
    setfillstyle(1,WHITE);
    if(CHRO==1)    setfillstyle(1,GREEN);
    bar(MAXX-200,r,MAXX-200+r,2*r);
    outtextxy(MAXX-200+r+2,r,"Chronometer");
    setfillstyle(1,WHITE);
    if(CTRL==1)    setfillstyle(1,GREEN);
    bar(MAXX-280,r,MAXX-280+r,2*r);
    outtextxy(MAXX-280+r+2,r,"Ctrl+Z");
    setfillstyle(1,WHITE);
    if(SCORE==1)    setfillstyle(1,GREEN);
    bar(MAXX-400,r,MAXX-400+r,2*r);
    outtextxy(MAXX-400+r+2,r,"ScoreBoard");
}

void board(int O=0){
    onetime=0;
    twotime=0;
    gametime=0;
    setfillstyle(1,BROWN);
    setcolor(WHITE);
    bar(0,0,MAXX,MAXY);
    setfillstyle(2,COLOR(100,100,100));
    for(int i=1;i<=m;++i){
        for(int j=1;j<=n;++j){
            fillellipse((2*i-1)*R,rx+r2x+(2*j-1)*R,R,R);
        }
    }
    for(int j=0;j<=n;++j){
        setfillstyle(1,BROWN);
        bar(0,rx+r2x+2*R*j-r3x/2,MAXX,rx+r2x+2*R*j+r3x/2);
    }
    for(int i=1;i<=m;++i)   col[i]=n;
    for(int i=1;i<=m;++i){
        for(int j=1;j<=n;++j){
            brdg[j][i]=0;
        }
    }
    cnt=0;
    if(O==0)    return;
    printf("Please enter color of the first one (1 = Blue and 2 = Red) : ");
    {
        int tmp;
        scanf("%d",&tmp);
        if(tmp!=c12){
            swap(c1,c2);
            c12=tmp;
        }
    }
}

int main(){
    srand(time(0));
    //drawing the board
    printf("Please enter Dimensions (n and m) : ");
    scanf("%d%d",&n,&m);
    printf("Please enter color of the first one (1 = Blue and 2 = Red) : ");
    {
        int tmp;
        scanf("%d",&tmp);
        if(tmp!=c12){
            swap(c1,c2);
            c12=tmp;
        }
    }
    printf("Please enter number of players : ");
    scanf("%d",&plrnum);
    if(plrnum==1){
        printf("Please enter mode (EASY:1/MEDIUM:2/HARD:3/EXPERT:4) : ");
        scanf("%d",&mode);
        printf("Please enter player name : ");
        scanf(" %s",playername);
        printf("Who should be the first one (YOU:0/COMPUTER:1) : ");
        scanf("%d",&firstplayer);
    }
    MAXX=m*2*R+m;
    MAXY=2*n*R+n+rx+r2x;
    initwindow(MAXX,MAXY);

    board();

    //Gaming
    int x=1,x0=mousex()/(2*R)+1;
    char c=1,N[200000],N2[200000],N3[1000000];
    getimage(0,r2x,MAXX,rx+r2x,N);
    draw(x);

    getimage(0,0,MAXX,r2x-1,N2);

    while(c!=27){
        mytime=time(NULL);
        if(mytime!=lasttime){
            if(onetime==timeloss){
                for(int i=1;i<=4;++i)   brdg[1][i]=2;
            }else if(twotime==timeloss){
                for(int i=1;i<=4;++i)   brdg[1][i]=1;
            }
            if(givech())    return 0;
            gametime++;
            putimage(0,0,N2,0);
            if(c12==1){
                if(player==1)setcolor(BLUE);
                if(player==2)setcolor(RED);
            }else{
                if(player==1)setcolor(RED);
                if(player==2)setcolor(BLUE);
            }
            if(player==1)   outtextxy(MAXX/2+140,0,"PLAYER ONE");
            else    outtextxy(MAXX/2+140,0,"PLAYER TWO");
            if(CLOCK==1)    outtextxy(0,0,ctime(&mytime));
            if(CHRO==1){
                if(player==1){
                    onetime++;
                    sprintf(TIME,"%02d:%02d",onetime/60,onetime%60);
                }else{
                    twotime++;
                    sprintf(TIME,"%02d:%02d",twotime/60,twotime%60);
                }
                outtextxy(0,r2x/2,TIME);
            }
            lasttime=mytime;
            sprintf(TIME,"GAME TIME -> %02d:%02d",gametime/60,gametime%60);
            outtextxy(MAXX/2-30,0,TIME);
        }

        if(plrnum==1){
            if(firstplayer==1){
                cmpturn(mode,x);
                cnt++;
                if(givech()){
                    return 0;
                }
            }
            firstplayer=0;
        }
        if(kbhit()){
            putimage(0,r2x,N,0);
            c=getch();
            c=toupper(c);
            if(c=='A'){
                if(x>1){
                    x--;
                }
            }else if(c=='D'){
                if(x<m){
                    x++;
                }
            }
            draw(x);
            if(c==' '){
                if(col[x]){
                    turn(x,player);
                    cnt++;
                    if(givech()){
                            return 0;
                    }
                    if(plrnum==1){
                        if(chflag==0){
                            if(cnt<n*m){
                                cmpturn(mode,x);
                                cnt++;
                                if(givech()){
                                    return 0;
                                }
                            }
                        }else{
                            chflag--;
                        }
                    }else{
                        player%=2;
                        player+=1;
                    }
                }
            }else if(c=='Z'){
                if(CTRL==1) czed();
            }else if(c=='S'){
                seting(r);
                char c=1;
                while(c!='S'){
                    if(kbhit()){
                        c=getch();
                        c=toupper(c);
                    }
                    if(ismouseclick(WM_LBUTTONDOWN)){
                        clearmouseclick(WM_LBUTTONDOWN);
                        if(mousex()>=MAXX-80 && mousex()<=MAXX-80+r && mousey()>=r && mousey()<=2*r){
                            CLOCK+=1;
                            CLOCK%=2;
                        }else if(mousex()>=MAXX-200 && mousex()<=MAXX-200+r && mousey()>=r && mousey()<=2*r){
                            CHRO+=1;
                            CHRO%=2;
                        }else if(mousex()>=MAXX-280 && mousex()<=MAXX-280+r && mousey()>=r && mousey()<=2*r){
                            CTRL+=1;
                            CTRL%=2;
                        }else if(mousex()>=MAXX-400 && mousex()<=MAXX-400+r && mousey()>=r && mousey()<=2*r){
                            SCORE+=1;
                            SCORE%=2;
                        }
                        seting(r);
                    }
                }
            }else if(c=='R'){
                board(1);
            }else if(c=='B'){
                if(SCORE==1){
                    getimage(MAXX/4,MAXY/4,3*MAXX/4,3*MAXY/4,N3);
                    setfillstyle(1,COLOR(130,130,130));
                    bar(MAXX/4,MAXY/4,3*MAXX/4,3*MAXY/4);
                    fp=fopen("scoreboard.txt","r");
                    int X=MAXX/4,Y=MAXY/4;
                    while(!feof(fp)){
                        char NAME[20],NAME2[20];
                        int scr;
                        fscanf(fp,"%s%s",NAME,NAME2);
                        if(feof(fp)){
                            break;
                        }
                        strcat(NAME,"   ");
                        strcat(NAME,NAME2);
                        setcolor(GREEN);
                        outtextxy(X+10,Y+10,NAME);
                        Y+=20;
                    }
                    fclose(fp);
                    getch();
                    putimage(MAXX/4,MAXY/4,N3,0);
                }
            }else{
                if(CHEAT[cht]==c)    cht++;
                else cht=0;
                if(cht==strlen(CHEAT)){
                    cht=0;
                    chflag=1;
                }
            }
        }


        int xmos=mousex()/(2*R)+1;
        if(xmos!=x0){
            putimage(0,r2x,N,0);
            draw(xmos);
            x0=xmos;
            x=xmos;
        }
        if(ismouseclick(WM_LBUTTONDOWN)){
            clearmouseclick(WM_LBUTTONDOWN);
            if(mousey()>=r2x){
                if(col[x]){
                    turn(x,player);
                    cnt++;
                    if(givech()){
                            return 0;
                    }
                    if(plrnum==1){
                        if(chflag==0){
                            if(cnt<n*m){
                                cmpturn(mode,x);
                                cnt++;
                                if(givech()){
                                    return 0;
                                }
                            }
                        }else{
                            chflag--;
                        }
                    }else{
                        player%=2;
                        player+=1;
                    }
                }
            }
        }
        if(ismouseclick(WM_RBUTTONDOWN)){
            clearmouseclick(WM_RBUTTONDOWN);
            board(1);
        }
        if(cnt>=n*m){
                settextstyle(0,0,5);
                outtextxy(MAXX/4,MAXY/4,"draw!");
                getch();
                return 0;
        }
    }
    return 0;
}
