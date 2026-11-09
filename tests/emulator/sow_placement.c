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

// Find an actually walkable no-Surf location between the Cascade/Thunder
// checkpoints rather than accepting the unwalkable first map-data candidate.
// All candidates are on Route23 below the first Surf crossing.
static void sowSelectRoute23LandTile(void)
{
 const unsigned width=24,height=160,startX=9,startY=151;
 int previous[3840],queue[3840],head=0,tail=0;
 fossilWarp(MAP_GROUP(MAP_ROUTE23),MAP_NUM(MAP_ROUTE23),startX,startY);
 for(unsigned k=0;k<3840;k++)previous[k]=-2;
 CHECK(sowWalkable(startX,startY));
 unsigned start=startY*width+startX;
 previous[start]=-1;queue[tail++]=start;
 const int dx[]={0,0,-1,1},dy[]={-1,1,0,0};
 while(head<tail){
  unsigned p=queue[head++];
  for(unsigned k=0;k<4;k++){
   int x=(int)(p%width)+dx[k],y=(int)(p/width)+dy[k];
   if(x<0||y<0||x>=(int)width||y>=(int)height)continue;
   unsigned id=(unsigned)y*width+(unsigned)x;
   if(previous[id]!=-2 || !sowWalkable(x,y))continue;
   previous[id]=p;queue[tail++]=id;
  }
 }
 for(unsigned offset=0;offset<7;offset++){
  unsigned ys[]={145,144,146,143,147,142,148};
  unsigned y=ys[offset];
  const unsigned xs[]={12,11,13,10,14,9,15,8,16,7,17,6};
  for(unsigned j=0;j<sizeof(xs)/sizeof(*xs);j++){
   unsigned x=xs[j],id=y*width+x;
   // Leave one free orthogonally adjacent tile for player interaction.
   if(previous[id]==-2 || !sowWalkable(x,y))continue;
   unsigned neighbors=0;
   if(x>0 && previous[id-1]!=-2)neighbors++;
   if(x+1<width && previous[id+1]!=-2)neighbors++;
   if(y>0 && previous[id-width]!=-2)neighbors++;
   if(y+1<height && previous[id+width]!=-2)neighbors++;
   if(neighbors<2)continue;
   // Guard the exact installed map object, not merely any walkable candidate.
   CHECK(x==13 && y==145);
   printf("SOW-008 SELECTED ROUTE23 TILE: x=%u y=%u, reachable on foot from (%u,%u), %u walkable neighbors\\n",
          x,y,startX,startY,neighbors);
   sowCheckLandRoute(MAP_ROUTE23,width,height,startX,startY,x,y);
   return;
  }
 }
 fprintf(stderr,"No walkable Route23 candidate between Cascade and Thunder badge guards\\n");
 CHECK(0);
}

static void sowPlacementTests(void)
{
 daycareFixture(1);
 // Original Route12 fishing-house entrance (12,86); approach from adjacent path.
 sowCheckLandRoute(MAP_ROUTE12,24,120,12,87,19,87);
 // Route21 northern grass, connected to the Pallet-side land entrance.
 sowCheckLandRoute(MAP_ROUTE21_NORTH,24,50,13,1,13,7);
 // Between Route23 Cascade and Thunder badge checkpoints, below Surf section.
 sowSelectRoute23LandTile();
 sowCheckLandRoute(MAP_ROUTE23,24,160,9,151,13,145);
 // Candidate above the arch, intentionally not installed until owner approval.
 sowCheckLandRoute(MAP_ROUTE23,24,160,9,151,13,142);
 // ISOLATED DIAGNOSTIC ONLY: candidate walkability near both sides of the
 // Cascade arch. Does not alter approved production object coordinate.
 fossilWarp(MAP_GROUP(MAP_ROUTE23),MAP_NUM(MAP_ROUTE23),9,151);
 for(unsigned y=137;y<=154;y++){
  printf("ROUTE23_PROBE y=%u ",y);
  for(unsigned x=7;x<=17;x++){
   unsigned attr=CALL(MapGridGetMetatileAttributeAt,x+7,y+7,ABI_META_BEHAVIOR);
   printf("%c", !CALL(MapGridGetCollisionAt,x+7,y+7)
     ? (CALL(MetatileBehavior_IsSurfable,attr)?'~':'.'):'#');
  }
  printf("\\n");
 }

 CHECK(core->loadState(core,bootState));
}
