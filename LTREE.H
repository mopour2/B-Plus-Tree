// L TREE HEADER

struct Tree
{
   char DescNod[MaxDesc+2];
   char VoiceFileName[MaxFileName+2];
   fpos_t FatherPoint;
   fpos_t ChildPoint[MaxChild];
   unsigned int CountChild;
 };


typedef struct Tree StructTree;
