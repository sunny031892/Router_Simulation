#include <stdio.h>

struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

struct rtpkt send_to_0;
struct rtpkt send_to_1;
struct rtpkt send_to_2;

extern int TRACE;
extern int YES;
extern int NO;
extern void tolayer2(struct rtpkt packet);

struct distance_table 
{
  int costs[4][4];
} dt3;


extern void read3(FILE *file)
{
  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      dt3.costs[i][j] = 999;
    }
  }

  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt3.costs[0][0], &dt3.costs[1][1], &dt3.costs[2][2], &dt3.costs[3][3]);

}

extern void rtinit3()
{
  /* TODO */
  send_to_0.sourceid = 3;
  send_to_0.destid = 0;
  send_to_1.sourceid = 3;
  send_to_1.destid = 1;
  send_to_2.sourceid = 3;
  send_to_2.destid = 2;
  for(int i=0;i<4;i++){
    send_to_0.mincost[i] = dt3.costs[3][i];
    send_to_1.mincost[i] = dt3.costs[3][i];
    send_to_2.mincost[i] = dt3.costs[3][i];
  }
  tolayer2(send_to_0);
  tolayer2(send_to_2);
}

extern void rtupdate3(struct rtpkt *rcvdpkt)
{
  /* TODO */
  int sourceid = rcvdpkt->sourceid;
  int destid = rcvdpkt->destid;
  int mincost[4];
  for(int i=0;i<4;i++)
    mincost[i] = rcvdpkt->mincost[i];

  int flag = 0;
  int min = 0;

  for(int i=0;i<4;i++){
    if(mincost[i] != 999 && dt3.costs[3][sourceid] != 999){
      min = dt3.costs[3][sourceid] + mincost[i];
      if(min < dt3.costs[3][i]){
        dt3.costs[3][i] = min;
        flag = 1;
      }
    }
  }

  if(flag){
    send_to_0.sourceid = 3;
    send_to_0.destid = 0;
    send_to_1.sourceid = 3;
    send_to_1.destid = 1;
    send_to_2.sourceid = 3;
    send_to_2.destid = 2;
    for(int i=0;i<4;i++){
      send_to_0.mincost[i] = dt3.costs[3][i];
      send_to_1.mincost[i] = dt3.costs[3][i];
      send_to_2.mincost[i] = dt3.costs[3][i];
    }
    tolayer2(send_to_0);
    tolayer2(send_to_2);
  }

  flag = 0;
  
}

void printdt3(void) 
{
  struct distance_table *dtptr = &dt3;

  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}

extern void print_min_cost3()
{

  int min_cost[4];

  for (int i = 0; i < 4; i++)
  {
    int min = dt3.costs[i][i];
    for (int j = 0; j < 4; j++)
    {
      min = min > dt3.costs[i][j] ? dt3.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 3, min_cost[0], min_cost[1],
         min_cost[2], min_cost[3]);
}