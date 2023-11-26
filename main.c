#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

#define MAX_CHARNAME       200
#define N_PLAYER             3
#define MAX_DIE              6
#define N_BOARD		        15 
#define MAX_COIN_PER_BOARD   5

#define N_PLAYER	    	3
#define PLAYERSTATUS_LIVE   0
#define PLAYERSTATUS_DIE    1
#define PLAYERSTATUS_END    2

int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];
char player_statusString[3][MAX_CHARNAME] = {"LIVE","DIE","END"};


void opening(void)
{
	printf("==========================================================\n");
	printf("**********************************************************\n");
	printf("                      SharkGame                 \n");
	printf("**********************************************************\n");
	printf("==========================================================\n\n\n"); 
}


int rolldie(void)
{
    return rand()%MAX_DIE+1;
}



int game_end(void)
{
	int i;
	int flag_end = 1;

	//if all the players are dead
	for (i=0; i<N_PLAYER; i++)
	{
		if (player_status[i] == PLAYERSTATUS_LIVE)
		{
			flag_end = 0;
			break;
		}
	}

	return flag_end;
}


int getAlivePlayer(void)
{
	int i;
	int cnt = 0;
	for (i=0; i<N_PLAYER; i++)
	{
		if (player_status[i] == PLAYERSTATUS_END)
			cnt++;
	}

	return cnt;
}


int getWinner(void)
{
	int i;
	int winner = 0;
	int max_coin =-1;

	for (i=0;i<N_PLAYER;i++)
	{
		if (player_coin[i] > max_coin)
		{
			max_coin = player_coin[i];
			winner = i;
		}
	}

	return winner;
}

void printPlayerPosition(int player)
{
     int i;
     for(i=0;i<N_BOARD;i++)
     {
     	 printf("|");
         if (i == player_position[player])
            printf("%c", player_name[player][0]);
         else
         {
             if (board_getBoardStatus(i) == BOARDSTATUS_NOK)
                printf("X");
             else
                printf(" ");
         }                 
     }
     printf("|\n");
}

void printPlayerStatus(void)
{
     int i=0;
     printf("------player status ------\n");
     
     for (i=0;i<N_PLAYER;i++)
     {
         printf("%s : pos %i, coin %i, status : %s\n",player_name[i], player_position[i], player_coin[i], player_statusString[player_status[i]]);
     }
     printf("-----------------\n");
}

void checkDie(void)
{
     int i;
     for (i=0;i<N_PLAYER;i++)
         if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)
            player_status[i] = PLAYERSTATUS_DIE;
}

int main(int argc, char *argv[])
{
  int pos=0;
  int i;
  int turn = 0;
  
  srand((unsigned)time(NULL));
  
  //0. opening
  opening();
  
  //1. ???? & ?÷???? ??? ????
  board_initboard();
  //1-2.
  for (i=0;i<N_PLAYER;i++)
  {
      player_position[i] = 0;
      player_coin[i] = 0;
      player_status[i] = PLAYERSTATUS_LIVE;
      //player_name
      printf("Player %i's name:", i);
      scanf("%s", player_name[i]);
  }
  
  
  
  
  
  //2. ????? (?÷???? ??)
  do {
    int step;
    int coinResult;
    char c;
    
    if (player_status[turn] != PLAYERSTATUS_LIVE)
    {
       turn = (turn + 1)%N_PLAYER;
       continue;
    }
    
    //2-1. ????&?÷?????? ???? ???
    board_printBoardStatus();
    for (i=0;i<N_PLAYER;i++)
        printPlayerPosition(i);
    printPlayerStatus();
    
    
    //2-2. ????? ??????
    printf("%s turn!! ", player_name[turn]);
    printf("press any key to roll a die!\n");
    scanf("%d", &c);
    fflush(stdin);
    step = rolldie();
    
    //2-3. ???
    player_position[turn] += step;
    if (player_position[turn] >= N_BOARD)
       player_position[turn] = N_BOARD-1;
    
    if (player_position[turn] == N_BOARD-1)
       player_status[turn] = PLAYERSTATUS_END;
       printf("%d만큼 이동해 %d로 이동했습니다.\n", step, player_position[turn]);
		
    
    //2-4. ???? ???
    coinResult = board_getBoardCoin(player_position[turn]);
    player_coin[turn] += coinResult;
    printf("%s가  %i코인을 얻었습니다.\n",player_name[turn],coinResult);
    
    //2-5. ??????
    turn = (turn + 1)%N_PLAYER; //wrap around
    
    //2-6. if (????:??? ?÷???? ????? ???? ??)
    if (turn == 0)
    {
       //??? ????
       int shark_pos = board_stepShark();
       printf("상어가 이동했습니다.\n");
       checkDie();
    } 
if(game_end()==1) break;

  } while(1);
  //3. ???? (???? ???, ??? ??) 
  if(getAlivePlayer()==0) 
  {
  	printf("우승자가 없습니다!\n");
  }
  else 
  	printf("우승자는 %s 입니다!\n",player_name[getWinner()]); 
  system("PAUSE");	
  return 0;
}
