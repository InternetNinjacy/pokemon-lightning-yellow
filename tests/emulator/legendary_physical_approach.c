// Focused actual directional walking from the authored Suicune/Mew map entries.
// No teleporting during each route; the bootstrap warp only stages the entry.
// This proves dry tiles only. Crossing water still requires a separate real
// controller-driven Surf action (not pretended by a fabricated avatar flag).
static void legendaryPhysicalDryApproachTests(void)
{
 static const unsigned maps[]={MAP_SEAFOAM_ISLANDS_B4F,MAP_SEVEN_ISLAND_TANOBY_RUINS};
 static const unsigned widths[]={38,144},heights[]={24,20};
 static const unsigned goalX[]={29,104},goalY[]={9,12};
 for(unsigned caseId=0;caseId<2;caseId++){
  CHECK(core->loadState(core,bootState));
  daycareFixture(1);
  CALL(FlagSet,FLAG_SYS_GAME_CLEAR);
  const unsigned map=maps[caseId],w=widths[caseId],h=heights[caseId];
  unsigned entryX=caseId?48:32,entryY=caseId?0:5;
  if(caseId){
   unsigned found=0;
   for(unsigned x=48;x<72;x++){
    fossilWarp(MAP_GROUP(map),MAP_NUM(map),x,0);
    if(legendaryNativeTilePassable(x,0,0)){entryX=x;found=1;break;}
   }
   CHECK(found);
  }
  fossilWarp(MAP_GROUP(map),MAP_NUM(map),entryX,entryY);
  CHECK(legendaryNativeTilePassable(entryX,entryY,0));
  int prev[2880], queue[2880],head=0,tail=0;
  CHECK(w*h<=2880);
  for(unsigned n=0;n<w*h;n++)prev[n]=-1;
  const unsigned first=entryY*w+entryX,target=goalY[caseId]*w+goalX[caseId];
  queue[tail++]=first;prev[first]=(int)first;
  unsigned best=first;
  unsigned bestDistance=(entryX>goalX[caseId]?entryX-goalX[caseId]:goalX[caseId]-entryX)
                       +(entryY>goalY[caseId]?entryY-goalY[caseId]:goalY[caseId]-entryY);
  while(head<tail){
   unsigned pos=queue[head++],cx=pos%w,cy=pos/w;
   unsigned distance=(cx>goalX[caseId]?cx-goalX[caseId]:goalX[caseId]-cx)
                     +(cy>goalY[caseId]?cy-goalY[caseId]:goalY[caseId]-cy);
   if(distance<bestDistance){best=pos;bestDistance=distance;}
   const int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
   for(unsigned d=0;d<4;d++){
    int xx=(int)cx+dx[d],yy=(int)cy+dy[d];
    if(xx<0||yy<0||xx>=(int)w||yy>=(int)h)continue;
    unsigned next=yy*w+xx;
    if(prev[next]>=0||!legendaryNativeTilePassable(xx,yy,0))continue;
    prev[next]=(int)pos;queue[tail++]=next;
   }
  }
  // Prefer entire dry route when possible, otherwise the best reachable
  // shoreline segment; never silently count water as walkable.
  unsigned end=prev[target]>=0?target:best;
  unsigned path[2880],count=0;
  for(unsigned pos=end;pos!=first;pos=(unsigned)prev[pos]){
   CHECK(prev[pos]>=0);
   path[count++]=pos;
   CHECK(count<w*h);
  }
  CHECK(count>0);
  unsigned at=first;
  for(unsigned step=0;step<count;step++){
   unsigned next=path[count-step-1],ax=at%w,ay=at/w,nx=next%w,ny=next/w;
   unsigned key;
   if(nx==ax+1&&ny==ay)key=16;
   else if(ax==nx+1&&ny==ay)key=32;
   else if(ny==ay+1&&nx==ax)key=128;
   else if(ay==ny+1&&nx==ax)key=64;
   else {fprintf(stderr,"Non-adjacent route step (%u,%u) -> (%u,%u)\n",ax,ay,nx,ny);CHECK(0);}
   unsigned framesUsed;
   for(framesUsed=0;framesUsed<60;framesUsed++){
    frames(1,key);
    CALL(PlayerGetDestCoords,scratch,scratch+2);
    unsigned px=core->busRead16(core,scratch),py=core->busRead16(core,scratch+2);
    if(px==nx+7&&py==ny+7)break;
   }
   if(framesUsed==60)fprintf(stderr,"D-pad movement stuck map%u from (%u,%u) toward (%u,%u) step %u/%u\n",
                             caseId,ax,ay,nx,ny,step+1,count);
   CHECK(framesUsed<60);
   frames(12,0);
   CALL(PlayerGetDestCoords,scratch,scratch+2);
   CHECK(core->busRead16(core,scratch)==nx+7);
   CHECK(core->busRead16(core,scratch+2)==ny+7);
   at=next;
  }
  printf("PASS legendary %u REAL D-pad from authored entry (%u,%u) reached (%u,%u) in %u dry steps; reached encounter=%u; remaining water/travel not certified\n",
         caseId,entryX,entryY,at%w,at/w,count,at==target);
 }
 CHECK(core->loadState(core,bootState));
}
