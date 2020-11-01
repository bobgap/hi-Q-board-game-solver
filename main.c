#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define GOAL 1
#define RESTOREX 0
#define RESTOREY 0
#define DEBUG 0
#ifndef TRUE
	#define TRUE 1
	#define FALSE 0
#endif
/*prototypes***************************************************/
int checkmove ();
int printboard ();

/********globals*/
int firsttime=TRUE, firstcount = 0;
/*  This program calculates the solution for the game HI Q*/
	 	/*board definition -- the board should be looked at sideways
		1s are pegs, 2s are no holes at that spot, 0 is an empty hole*/

int  	level = -1,   /*level is the pointer to the current move (moves[level])*/
/*  This is the layout of the board, 7x7 square*/
  2 is not a hole
  1 is a hole with a peg
  0 is a hole without a peg*/
		board[7][7]={ 
			{2,2,1,1,1,2,2},
			{2,1,1,1,1,1,2},
			{1,1,1,1,1,1,1},
			{1,1,1,0,1,1,1},  /*top end --->*/
			{1,1,1,1,1,1,1},
			{2,1,1,1,1,1,2},
			{2,2,1,1,1,2,2}
			};
unsigned long totalcount = 0;
clock_t start;
int restorex = 0, restorey=0;
struct move_struct {
	int x;
	int y;
	};
struct move_struct moves[50];
struct move_struct moves_current[50];
/*begin main****************************************************/
int main()
{
 register int j,k;
  extern clock_t start;
  extern struct move_struct moves[50];
  extern unsigned long totalcount;
  FILE *saved;

 
 	/*Read the data from the file into moves[], else leave it zeroes (done automatically)*/
	saved = fopen( "saved","r");
	if (saved != NULL)
		{ 
		fscanf(saved, "%lu", &totalcount);
		for (j=0; j<50; j+=1)
		fscanf(saved, "%d %d", &moves[j].x, &moves[j].y);
		fclose (saved);
		}
printf("Start (x,y) = %d,%d\n",restorex, restorey);
  start = clock();
  for (j=0;j<7;j+=1)  /*count pieces remaining*/
		for (k=0;k<7;k+=1)
			firstcount +=(board[j][k]==1) ? 1: 0;
  checkmove ();printboard();
  printf("\n\nIt took %lu calls to checkmove to do this.\n", totalcount);
  return 0;
}
/*************************************************************/
int checkmove ()
	{
	extern int board[7][7], level;
	extern struct move_struct moves[50];
	extern unsigned long totalcount;
	extern clock_t start;
	FILE *saved;

register	int x, y, j, k, count = 0, xval,yval;
	level +=1;
	for (j=0;j<7;j+=1)  /*count pieces remaining*/
					for (k=0;k<7;k+=1)
						count +=(board[j][k]==1) ? 1: 0;
	if ((count == GOAL) && (board[3][3]==1))
	{   printf(" Srce  Dst  Rmv\n");
		printf("  x y  x y  x y\n");
		return 1;  }
	  if (++totalcount%10000000==0)
    {   printf("Passes = %lu, taking %lu seconds.\n\n", totalcount, (clock()-start)/CLOCKS_PER_SEC);
    /*write out data for resumption later*/
   		saved = fopen( "saved","w");
		if (saved == NULL)
			{ printf("File opening error!  Exiting.");
				exit(0);
		}
		fprintf(saved, "%lu\n", totalcount);
		for (j=0; j<50; j+=1)
			fprintf(saved, "%d %d\n",moves_current[j].x,moves_current[j].y);
		fclose (saved);
    
	  /*  printboard (); 
	   */ 
	   }
	    xval = moves[level].x;
	    yval = moves[level].y;
	    moves[level].x = moves[level].y = 0; /*restores value if needed, but only once*/
	for (x = xval; x<=6; x+=1)
		{	for (y=yval; y<=6; y+=1)
				if (board[x][y]==1)
				{
				moves_current[level].x=x;
				moves_current[level].y=y;
				/*vertical jumps*/
				if ((y<=4) && (board[x][y+1] == 1) && (board[x][y+2]==0))
						{board[x][y+1] = 0;board[x][y+2] = 1;board[x][y] = 0;
						if (checkmove()==1)
							{	printboard();
								printf("%3d,%1d%3d,%1d%3d,%1d\n",x,y,x,y+2,x,y+1);
								board[x][y+1] = 1;board[x][y+2] = 0;board[x][y] = 1;
								level-=1;
								return 1;}
						board[x][y+1] = 1;board[x][y+2] = 0;board[x][y] = 1;
						}
				if ((y>=2) && (board[x][y-1] == 1) && (board[x][y-2]==0))
						{board[x][y-1] = 0;board[x][y-2] = 1;board[x][y] = 0;
						if (checkmove()==1)
							{	printboard();
							printf("%3d,%1d%3d,%1d%3d,%1d\n",x,y,x,y-2,x,y-1);
							board[x][y-1] = 1;board[x][y-2] = 0;board[x][y] = 1;
							level-=1;
							return 1;
							}
						board[x][y-1] = 1;board[x][y-2] = 0;board[x][y] = 1;
						}
				/*horizontal jumps*/
				if ((x<=4) && ( board[x+1][y]==1) && (board[x+2][y]==0))  
					{
						board[x+1][y] = 0;board[x+2][y] = 1;board[x][y] = 0;
						if (checkmove()==1)
							{	printboard();
							printf("%3d,%1d%3d,%1d%3d,%1d\n",
							       x,y,x+2,y,x+1,y);
							board[x+1][y] = 1;board[x+2][y] = 0;board[x][y] = 1;
							level-=1;
							return 1;
							}
						board[x+1][y] = 1;board[x+2][y] = 0;board[x][y] = 1;
					}
				if ((x>=2) && (board[x-1][y]==1) && ( board[x-2][y]==0)) 
					{
						board[x-1][y] = 0;board[x-2][y] = 1;board[x][y] = 0;
						if (checkmove()==1)
							{	printboard();
								printf("%3d,%1d%3d,%1d%3d,%1d\n",
									x,y,x-2,y,x-2,y);
								board[x-1][y] = 1;board[x-2][y] = 0;board[x][y] = 1;
								level-=1;
								return 1;
							}
						board[x-1][y] = 1;board[x-2][y] = 0;board[x][y] = 1;
					}
				}	/*check to see if the location can jump*/
			
		}  /*end of x loop (y loop is one "if" statement)*/
		moves_current[level].x = moves_current[level].y = 0;
	level-=1;
	return 0; /* no luck at this level */
	}
/*************************************************************/
int printboard ( )
{	extern int board[7][7];
	int x,y;
	printf ("\n");
	printf ("   0 1 2 3 4 5 6\n\n");
	for (y=0;y<7;y+=1)
		{printf("%d ",y);
		for (x=0;x<7;x+=1)
			if (board[x][y]==2) printf ("  ");
				else printf("%2d",board[x][y]);
	printf("\n");
	}
	return 0;	
}	
/*************************************************************/

	
	
	
	
	
	
	
	
	
	
	
