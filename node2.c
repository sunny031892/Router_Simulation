#include <stdio.h>

struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

struct rtpkt send_to_0;
struct rtpkt send_to_1;
struct rtpkt send_to_3;

extern int TRACE;
extern int YES;
extern int NO;
extern void tolayer2(struct rtpkt packet);

struct distance_table 
{
  int costs[4][4];
} dt2;


extern void read2(FILE *file)
{
  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      dt2.costs[i][j] = 999;
    }
  }

  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt2.costs[0][0], &dt2.costs[1][1], &dt2.costs[2][2], &dt2.costs[3][3]);

}

extern void rtinit2()
{
  /* TODO */
  send_to_0.sourceid = 2;
  send_to_0.destid = 0;
  send_to_1.sourceid = 2;
  send_to_1.destid = 1;
  send_to_3.sourceid = 2;
  send_to_3.destid = 3;
  for(int i=0;i<4;i++){
    send_to_0.mincost[i] = dt2.costs[2][i];
    send_to_1.mincost[i] = dt2.costs[2][i];
    send_to_3.mincost[i] = dt2.costs[2][i];
  }
  tolayer2(send_to_0);
  tolayer2(send_to_1);
  tolayer2(send_to_3);
}

extern void rtupdate2(struct rtpkt *rcvdpkt)
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
    if(mincost[i] != 999 && dt2.costs[2][sourceid] != 999){
      min = dt2.costs[2][sourceid] + mincost[i];
      if(min < dt2.costs[2][i]){
        dt2.costs[2][i] = min;
        flag = 1;
      }
    }
  }

  if(flag){
    send_to_0.sourceid = 2;
    send_to_0.destid = 0;
    send_to_1.sourceid = 2;
    send_to_1.destid = 1;
    send_to_3.sourceid = 2;
    send_to_3.destid = 3;
    for(int i=0;i<4;i++){
      send_to_0.mincost[i] = dt2.costs[2][i];
      send_to_1.mincost[i] = dt2.costs[2][i];
      send_to_3.mincost[i] = dt2.costs[2][i];
    }
    tolayer2(send_to_0);
    tolayer2(send_to_1);
    tolayer2(send_to_3);
  }

  flag = 0;

}

void printdt2(void) 
{
  struct distance_table *dtptr = &dt2;

  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}

extern void print_min_cost2()
{

  int min_cost[4];

  for (int i = 0; i < 4; i++)
  {
    int min = dt2.costs[i][i];
    for (int j = 0; j < 4; j++)
    {
      min = min > dt2.costs[i][j] ? dt2.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 2, min_cost[0], min_cost[1],
         min_cost[2], min_cost[3]);
}
