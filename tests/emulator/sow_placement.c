// Direct native map-collision and water-behavior acceptance for proposed event tiles.
// 7 is the engine virtual-map border offset, as exercised by museumWalk.
static unsigned sowWalkable(unsigned x,unsigned y)
{
 unsigned attr=CALL(MapGridGetMetatileAttributeAt,x+7,y+7,ABI_META_BEHAVIOR);
 return !CALL(MapGridGetCollisionAt,x+7,y+7)
     && !CALL(MetatileBehavior_IsSurfable,attr);
}
static void sowCheckLandRoute(unsigned map,unsigned width,unsigned height,
                              unsigned startX,unsigned startY,unsigned x,unsigned y)
{
 CHECK(width*height<=3840 && startX<width && startY<height && x<width && y<height);
 fossilWarp(MAP_GROUP(map),MAP_NUM(map),startX,startY);
 int previous[3840],queue[3840],head=0,tail=0;
 for(unsigned k=0;k<width*height;k++)previous[k]=-2;
 CHECK(sowWalkable(startX,startY));
 CHECK(sowWalkable(x,y));
 unsigned start=startY*width+startX,goal=y*width+x;
 previous[start]=-1;queue[tail++]=start;
 const int dx[]={0,0,-1,1},dy[]={-1,1,0,0};
 while(head<tail && previous[goal]==-2){
  unsigned p=queue[head++];
  for(unsigned k=0;k<4;k++){
   int nx=(int)(p%width)+dx[k],ny=(int)(p/width)+dy[k];
   if(nx<0||ny<0||nx>=(int)width||ny>=(int)height)continue;
   unsigned index=(unsigned)ny*width+(unsigned)nx;
   if(previous[index]!=-2 || !sowWalkable(nx,ny))continue;
   previous[index]=p;queue[tail++]=index;
  }
 }
 if(previous[goal]==-2)
  fprintf(stderr,"SOW candidate not walk-reachable without Surf: map=%u start=(%u,%u) target=(%u,%u) target-collision=%u\n",
          map,startX,startY,x,y,CALL(MapGridGetCollisionAt,x+7,y+7));
 CHECK(previous[goal]!=-2);
 unsigned steps=0;
 for(unsigned p=goal;p!=start;p=previous[p]){CHECK(previous[p]>=0);steps++;}
 printf("PASS: SOW map%u proposed object (%u,%u) is collision-free, non-Surf and %u walking tiles from (%u,%u)\n",
        map,x,y,steps,startX,startY);
}
static void sowPlacementTests(void)
{
 daycareFixture(1);
 // Original Route12 fishing-house entrance (12,86); approach from adjacent path.
 sowCheckLandRoute(MAP_ROUTE12,24,120,12,87,19,87);
 // Route21 northern grass, connected to the Pallet-side land entrance.
 sowCheckLandRoute(MAP_ROUTE21_NORTH,24,50,13,1,13,7);
 // Between Route23 Cascade and Thunder badge checkpoints, below Surf section.
 sowCheckLandRoute(MAP_ROUTE23,24,160,9,151,12,145);
 CHECK(core->loadState(core,bootState));
}
