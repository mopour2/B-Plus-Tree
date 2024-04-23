//#include <stdio.h>
//#include <io.h>
//#include <stdlib.h>
//#include <process.h>
//#include <alloc.h>

#include <libbptr.h>
//**************************************************************************
struct Key_S
{
   char f;  /*flag del logic*/
   char   key[MAX_L_KEY];
   DWord index;
 };
struct B_P_Tr
 {
    Byte Flag;
    Word Conter;
    DWord Pointer[Point_Num_Nod+2];
    struct Key_S key[Key_Num_Nod+2];
    DWord  Next;
    DWord  Prev;
/*    Byte   Spac[Space];*/
 };
 struct Head_Point
  {
    Byte  Flag_Lock;
    DWord Rec_Conter;
    DWord Pointer_Root;
    DWord Conter_Rec_In_File_Dat;
  };

union B_P_Tree
 {
   struct Head_Point Head;
   struct B_P_Tr     Nod;
 }*Nod;

struct New_S
 {
  Byte Flag_Split;
  DWord New_Pointer_Nod;
  struct Key_S Small_Key;
 }New;

/*---------------------------------------------------*/
struct Key_S key;

/*=======================================================================*/

//**************************************************************************
Creat_Bp(char Boxid[],char Boxda[],char CountVoi[],char DriveName[])
{
 unsigned long int L;
 union B_P_Tree Record;
 FILE *fdata,*fp;
 char BoxIdx[100];
 char BoxDat[100];
 char CountVoic[100];
 FILE *FCountVoic;

    strcpy(BoxIdx,DriveName);
    strcat(BoxIdx,Boxid);

    strcpy(BoxDat,DriveName);
    strcat(BoxDat,Boxda);

    strcpy(CountVoic,DriveName);
    strcat(CountVoic,CountVoi);

    if (access(BoxIdx, 0) == 0)
      return 0;
    else  fp= fopen(BoxIdx,"wb");
    if ( fp== NULL )
      return 0;
    else
      {
	if (access(BoxDat, 0) == 0)
	  return 0;

	FCountVoic=fopen(CountVoic,"wb");
	L=1;
	fwrite(&L,sizeof(unsigned long int),1,FCountVoic);
	fclose(FCountVoic);

	fdata=fopen(BoxDat,"wb");
	fclose(fdata);

	Record.Head.Rec_Conter=2;
	Record.Head.Flag_Lock=0;
	Record.Head.Pointer_Root=1;
	Record.Head.Conter_Rec_In_File_Dat=0;
	fwrite(&Record,sizeof(union B_P_Tree),1,fp);

	Record.Nod.Conter=0;
	Record.Nod.Flag=Leave;
	Record.Nod.Next=0;
	Record.Nod.Prev=0;
	fseek(fp,sizeof(union B_P_Tree)*1,SEEK_SET);
	fwrite(&Record,sizeof(union B_P_Tree),1,fp);

	fclose(fp);

	return 1;
      }
}

/*=======================================================================*/
/*                                *                                       */
compare(struct Key_S *Key1,struct Key_S *Key2)
{
     return(strcmp(Key1->key , Key2->key));

}
/*=======================================================================*/
/*                                                                       */
/*                               *                                       */
/*                                                                       */
/*=======================================================================*/
void copy_key_to_key(struct Key_S *key1,struct Key_S *key2)
{
  strcpy(key1->key,key2->key);
  key1->index=key2->index;
  key1->f=key2->f;
}
/*=======================================================================*/
/*                                                                       */
/*                               *                                       */
/*                                                                       */
/*=======================================================================*/
//void copy1_data_data(char far *Data2,char far *Data,unsigned long Lent)
//{
//int i;
//  for (i=0;i<Lent;i++)
//  {
//    *(Data2+i)= *(Data+i);
//  }
//}
/*-------------------------------------------------------------------------------*/
/*=======================================================================*/
write_data(const void *Data,unsigned long Lent,DWord rec_num,char fnamedata[])
{
FILE *f;
int i;

while ((f=fopen(fnamedata,"r+b"))==NULL)
{
    printf("This Is File Used");
;
}
fseek(f,(DWord)( Lent )*rec_num,SEEK_SET);
i=fwrite(Data,Lent,1,f);
fclose(f);
return(i);
}
/*-------------------------------------------------------------------------------*/
/*=======================================================================*/

read_data(const void *Data,unsigned long Lent,DWord rec_num,char fnamedata[])
{
FILE *f;
int i;

while ((f=fopen(fnamedata,"r+b"))==NULL)
{
    printf("This Is File Used");
		  ;
}
fseek(f,(DWord)(Lent )*rec_num,SEEK_SET);
i=fread(Data,Lent,1,f);
fclose(f);
return(i);
}
/*=======================================================================*/
write_nod(union B_P_Tree *Nod,DWord rec_num,FILE *fp)
{

fseek(fp,(DWord)(sizeof(union B_P_Tree)*rec_num),SEEK_SET);
return(fwrite(Nod,sizeof(union B_P_Tree),1,fp));
}

/*=======================================================================*/
read_nod(union B_P_Tree *Nod,DWord rec_num,FILE *fp)
{
int i;
i=fseek(fp,(DWord)(sizeof(union B_P_Tree)*rec_num),SEEK_SET);
i=fread(Nod,sizeof(union B_P_Tree),1,fp);
return(i);
}

/*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
split(struct Key_S *key,union B_P_Tree *Nod,DWord Nod_Num
       ,union B_P_Tree *Head,FILE *fp)
{
  Word i,j;
  union B_P_Tree *new_nod_split,*NodT;
  DWord rec_t;

  struct Key_S temp[Key_Num_Nod +2];

  if(((new_nod_split=(union B_P_Tree *)malloc(sizeof(union B_P_Tree )))!=NULL)
       &&((NodT=(union B_P_Tree *)malloc(sizeof(union B_P_Tree )))!=NULL))
  {

  for (i =0;i<Key_Num_Nod;i++)
    if(compare(key,&Nod->Nod.key[i])>0)
	    copy_key_to_key(&temp[i],&Nod->Nod.key[i]);
    else  break;
  copy_key_to_key(&temp[i],key);

   for(j=i+1;j<Key_Num_Nod +1;j++)
       copy_key_to_key(&temp[j],&Nod->Nod.key[j-1]);
   for(i=0;i<Order;i++)
     copy_key_to_key(&Nod->Nod.key[i],&temp[i]);
  for(i=0;i<Order+1;i++)
     copy_key_to_key(&new_nod_split->Nod.key[i],&temp[Order+i]);

   Nod->Nod.Conter= Order ;
rec_t=Nod->Nod.Next;
if(rec_t)
 {
   read_nod(NodT,rec_t,fp);
   NodT->Nod.Prev=Head->Head.Rec_Conter;
   write_nod(NodT,rec_t,fp);
 }
 else ;


   new_nod_split->Nod.Flag=Leave;

   new_nod_split->Nod.Conter= Order + 1    ;
   new_nod_split->Nod.Next=Nod->Nod.Next;
   Nod->Nod.Next=Head->Head.Rec_Conter;
   new_nod_split->Nod.Prev=Nod_Num;
   write_nod(new_nod_split,Head->Head.Rec_Conter,fp);
   write_nod(Nod,Nod_Num,fp);

   New.Flag_Split=Split;
   New.New_Pointer_Nod=Head->Head.Rec_Conter;
   copy_key_to_key(&New.Small_Key,&new_nod_split->Nod.key[0]);

   Head->Head.Rec_Conter++;
   write_nod(Head,0,fp);
  free(new_nod_split);
  free(NodT);
 }else printf("NOT ALLOCATION");
  return 0;

}
/*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
ins(struct Key_S *key,union B_P_Tree *Nod,DWord Nod_Num,FILE *fp)
{
  int i,j;
   if(Nod->Nod.Conter==0)
      copy_key_to_key(&Nod->Nod.key[0],key);
   else {
	  for (i=0;i<Nod->Nod.Conter;i++)
	    if(compare(key,&Nod->Nod.key[i])<=0)
	      break;
	  for(j=Nod->Nod.Conter - 1;j>=i;j--)
	    copy_key_to_key(&Nod->Nod.key[j+1],&Nod->Nod.key[j]);
	  copy_key_to_key(&Nod->Nod.key[i],key);
	}
  Nod->Nod.Conter++;
   write_nod(Nod,Nod_Num,fp);
return 0;
}

/*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
insertt(struct Key_S *key,union B_P_Tree *Nod,DWord Nod_Num
	    ,union B_P_Tree *Head,FILE *fp)
 {
   if(Nod->Nod.Conter==Key_Num_Nod)
   {
     split(key,Nod,Nod_Num,Head,fp);

   }
   else
       {
	 ins(key,Nod,Nod_Num,fp);
	 New.Flag_Split=Off;
       }
   return(0);
 }

/*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
split_parent(struct New_S *New,union B_P_Tree *Nod
	      ,union B_P_Tree *Head,FILE *fp,DWord Nod_Num)
{
  Word i,j;
  union B_P_Tree *new_nod_split;
  struct Key_S temp[Key_Num_Nod +4];
  DWord pointer[Point_Num_Nod+4];
 if(((new_nod_split=(union B_P_Tree *)malloc(sizeof(union B_P_Tree )))!=NULL))
 {
	  for (i =0;i<Key_Num_Nod;i++)
	    if(compare(&New->Small_Key,&Nod->Nod.key[i])>0)
		    {
		      copy_key_to_key(&temp[i],&Nod->Nod.key[i]);
		      pointer[i]=Nod->Nod.Pointer[i];
		    }
	    else  break;
	  pointer[i]=Nod->Nod.Pointer[i];
	  copy_key_to_key(&temp[i],&New->Small_Key);
	  pointer[i+1]=New->New_Pointer_Nod;

	   for(j=i+1;j<Key_Num_Nod + 1;j++)
	     {
	       copy_key_to_key(&temp[j],&Nod->Nod.key[j-1]);
	       pointer[j+1]=Nod->Nod.Pointer[j];
	     }

	   for(i=0;i<Order;i++)
	   {
	     copy_key_to_key(&Nod->Nod.key[i],&temp[i]);
	     Nod->Nod.Pointer[i]=pointer[i];
	   }
	   Nod->Nod.Pointer[i]=pointer[i];

	   copy_key_to_key(&New->Small_Key,&temp[Order]);

	  for(i=1;i<Order+1;i++)
	  {
	     copy_key_to_key(&new_nod_split->Nod.key[i-1],&temp[Order+i]);
	     new_nod_split->Nod.Pointer[i-1]=pointer[Order+i];
	  }
	  new_nod_split->Nod.Pointer[i-1]=pointer[Order+i];

	   Nod->Nod.Conter= Order ;
	   write_nod(Nod,Nod_Num,fp);

	   new_nod_split->Nod.Flag=Inter_Nod;
	   new_nod_split->Nod.Conter= Order;
	   new_nod_split->Nod.Next=0;
	   new_nod_split->Nod.Prev=0;

	   write_nod(new_nod_split,Head->Head.Rec_Conter,fp);

	   New->Flag_Split=Split;
	   New->New_Pointer_Nod=Head->Head.Rec_Conter;


	   Head->Head.Rec_Conter++;
	   write_nod(Head,0,fp);
  free(new_nod_split);
}
else printf("NOT ALLOCATION");
  return 0;

}
/*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
ins_parent(struct New_S *New,union B_P_Tree *Nod,Word i,FILE *fp,DWord  Nod_Num)
{
    int j;
     j=Nod->Nod.Conter - 1;
	     while( j >= i && j >=0 )
	     {
	       copy_key_to_key(&Nod->Nod.key[j+1],&Nod->Nod.key[j]);
		Nod->Nod.Pointer[j+2]=Nod->Nod.Pointer[j+1];
	       j--;

	     }

	  copy_key_to_key(&Nod->Nod.key[i],&New->Small_Key);
     Nod->Nod.Pointer[i+1]=New->New_Pointer_Nod;
     Nod->Nod.Conter++;
     write_nod(Nod,Nod_Num,fp);
return 0;
}
 /*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
 insert_parent(struct New_S *New,union B_P_Tree *Nod,Word i
	       ,union B_P_Tree *Head,FILE *fp,DWord Nod_Num)
 {
   if(Nod->Nod.Conter==Key_Num_Nod)
     split_parent(New,Nod,Head,fp,Nod_Num);
   else
       {
	 ins_parent(New,Nod,i,fp,Nod_Num);
	 New->Flag_Split=Off;
       }
   return(0);
 }

/*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
 insert_parent2(struct New_S *New ,union B_P_Tree *Head,FILE *fp)
 {
  union B_P_Tree *Nod;
  if((Nod=(union B_P_Tree *)malloc(sizeof(union B_P_Tree)))!=NULL)
  {
   Nod->Nod.Flag=Inter_Nod;
   Nod->Nod.Conter=1;
   Nod->Nod.Pointer[0]=Head->Head.Pointer_Root;
   Nod->Nod.Pointer[1]=New->New_Pointer_Nod;
   copy_key_to_key(&Nod->Nod.key[0],&New->Small_Key);
   Nod->Nod.Next=0;
   Nod->Nod.Prev=0;

   write_nod(Nod,Head->Head.Rec_Conter,fp);

   free(Nod);

   Head->Head.Pointer_Root=Head->Head.Rec_Conter;
   Head->Head.Rec_Conter++;

   write_nod(Head,0,fp);

   New->Flag_Split=UnSplit;
  }
  else  printf("Btree SICK");
 return 0;
 }

/*=======================================================================*/
Word search_pointer(struct Key_S *key,union B_P_Tree *Nod)
{
 Word i;
  for (i=0;i<Nod->Nod.Conter;i++)
     if(compare(key,&Nod->Nod.key[i])<=0)
	return i;
     else ;

 return  Nod->Nod.Conter;
}
/*=======================================================================*/
 b_p_t_insert(struct Key_S *key,DWord Nod_Num ,union B_P_Tree *Head,FILE *fp)
 {
  Word  i;
  DWord Rec_Num;
  union B_P_Tree *Nod;
  if((Nod=(union B_P_Tree *)malloc(sizeof(union B_P_Tree)))!=NULL)
  {
   read_nod(Nod,Nod_Num,fp);
   if(Nod->Nod.Flag==Leave)
    {
      insertt(key,Nod,Nod_Num,Head,fp);
      free(Nod);
      return(0);
    }
   else
    {
	  i= search_pointer(key,Nod);

	b_p_t_insert(key,Nod->Nod.Pointer[i],Head,fp);

	  if(New.Flag_Split==Split)
	    insert_parent(&New,Nod,i,Head,fp,Nod_Num);
	  else ;
     }
  free(Nod);
 }else printf("Btree SICK");
 return 0;
 }

/*=======================================================================*/
cheklock(FILE *fp)
{
  union B_P_Tree *Nod;
  if((Nod=(union B_P_Tree *)malloc(sizeof(union B_P_Tree)))!=NULL)
  {
    read_nod(Nod,0,fp);
    if(Nod->Head.Flag_Lock==UnLock)
	{free(Nod);return 1;}
    else {free(Nod);return 0;}
  }else printf("NOT ALLOCATION");
return 0;
}
/*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
lockb(FILE *fp)
{
  union B_P_Tree *Nod;
    if((Nod=(union B_P_Tree *)malloc(sizeof(union B_P_Tree)))!=NULL)
   {
	   read_nod(Nod,0,fp);

	   if(Nod->Head.Flag_Lock==UnLock)
	      {
		Nod->Head.Flag_Lock=Lock;
		write_nod(Nod,0,fp);
		free(Nod);
		return 1;
	      }
	    else {free(Nod);return 0;}
  }
  else printf("NOT ALLOCATION");
return 0;
}
/*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
unlockb(FILE *fp)
{
   union B_P_Tree *Nod;
  if((Nod=(union B_P_Tree *)malloc(sizeof(union B_P_Tree)))!=NULL)
  {
	   read_nod(Nod,0,fp);
	   if(Nod->Head.Flag_Lock==Lock)
	      {
		Nod->Head.Flag_Lock=UnLock;
		write_nod(Nod,0,fp);
		free(Nod);
		return 1;
	      }
	    else {free(Nod); return 0;}
 }
  else printf("NOT ALLOCATION");
return 0;
}
/*=======================================================================*/
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*=======================================================================*/
long  B_insert(struct Key_S *key,char fname[],void *Data,unsigned long Lent,char fnamedata[])
 {
 long err;
  int i,j;
  union B_P_Tree * Nod;

  FILE *fp;
  char s[20];
  err=-1;
  if((Nod=(union B_P_Tree *)malloc(sizeof(union B_P_Tree)))!=NULL)
  {

  if((fp=fopen(fname,"rb+"))!=NULL)
  {
    if(lockb(fp))
    {
     read_nod(Nod,0,fp);
     key->index=Nod->Head.Conter_Rec_In_File_Dat;
     err=b_p_t_insert(key,Nod->Head.Pointer_Root,Nod,fp);
    if(New.Flag_Split==Split)
     err=insert_parent2(&New,Nod,fp);
    else ;
     err=Nod->Head.Conter_Rec_In_File_Dat;
//     Data->fdel=ActivRecCod;
     write_data(Data,Lent,Nod->Head.Conter_Rec_In_File_Dat,fnamedata);
    Nod->Head.Conter_Rec_In_File_Dat++;
    write_nod(Nod,0,fp);
     unlockb(fp);
   } else {free(Nod); printf("ERR_LOCK");}
   fclose(fp);
   free(Nod);
   return err;
  }
 else {free(Nod);  printf("ERR_OP_FILE");}
 return -1;
}
  else printf("NOT ALLOCATION");
return 0;
}
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
long searchleave3(char str[],union B_P_Tree *Nod,DWord k,FILE *fp)
{
  int i;
   if(Nod->Nod.Conter==0)
      return -1;
   else {
           for (i=0;i<Nod->Nod.Conter;i++)
           {
	     if(strcmp(str,Nod->Nod.key[i].key)==0)
	       {
                if(Nod->Nod.key[i].f==ActivRecCod)
	   {
                     Nod->Nod.key[i].f=DelRecCod;
	     write_nod(Nod,k,fp);
                     return Nod->Nod.key[i].index;
	   }
                else ;
               }
               else;
          }
	 return -1;
        }
}
/*=======================================================================*/
long searchparent3(char str[],union B_P_Tree *Nod,DWord *rec_num,DWord k,FILE *fp)
{
 Word i;
  for (i=0;i<Nod->Nod.Conter;i++)
  {
     if(strcmp(str,Nod->Nod.key[i].key)<0)
	{ *rec_num=Nod->Nod.Pointer[i];;return -1;}
     else    {
		 if(strcmp(str,Nod->Nod.key[i].key)==0)
		  {
		    if( Nod->Nod.key[i].f==ActivRecCod)
		    {
			Nod->Nod.key[i].f=DelRecCod;
			write_nod(Nod,k,fp);
//                                              return Nod->Nod.key[i].index;
		    }
		    else ;
		  }
	      }
   }
 *rec_num= Nod->Nod.Pointer[Nod->Nod.Conter];
 return -1;
 }
/*----------------------------------------------------------------------*/
long searchdelidx(char str[],char fname[])
{
  int err,i,j,brk;
  DWord rec_num,rec_idx;
  union B_P_Tree *Nod;
  FILE *fp;
  if((Nod=(union B_P_Tree *)malloc(sizeof(union B_P_Tree)))!=NULL)
  {
   if((fp=fopen(fname,"rb+"))!=NULL)
   {
    if(cheklock(fp))
    {
     read_nod(Nod,0,fp);
     rec_num=Nod->Head.Pointer_Root;
     brk=1;
     while(brk)
     {
       read_nod(Nod,rec_num,fp);
       if(Nod->Nod.Flag==Leave)
	{rec_idx=searchleave3(str,Nod,rec_num,fp);brk=0;}
	else  rec_idx=searchparent3(str,Nod,&rec_num,rec_num,fp);

	if(rec_idx!=-1)
	 brk=0;
	else;

     }
     } else ;
    free(Nod);
   } else printf("NOT openfilename");
   }else printf("NOT ALLOCATION");

  fclose(fp);
  return rec_idx;
}

/*----------------------------------------------------------------------*/
long searchleave(char str[],union B_P_Tree *Nod)
{
  int i;
   if(Nod->Nod.Conter==0)
      return -1;
   else {
	  for (i=0;i<Nod->Nod.Conter;i++)
	  {
	    if(strcmp(str,Nod->Nod.key[i].key)==0)
	    {
	       if( Nod->Nod.key[i].f==ActivRecCod)
		   return Nod->Nod.key[i].index;
	       else ;
	    }else;
	  }
	  return -1;
	}
}
/*=======================================================================*/
long searchparent(char str[],union B_P_Tree *Nod,DWord *rec_num)
{
 Word i;
  for (i=0;i<Nod->Nod.Conter;i++)
  {
     if(strcmp(str,Nod->Nod.key[i].key)<0)
	{ *rec_num=Nod->Nod.Pointer[i];;return -1;}
     else    {
		 if(strcmp(str,Nod->Nod.key[i].key)==0)
		  {
		    if( Nod->Nod.key[i].f==ActivRecCod)
		       return Nod->Nod.key[i].index;
		    else ;
		  }
	      }
   }
 *rec_num= Nod->Nod.Pointer[Nod->Nod.Conter];
 return -1;
 }
/*=======================================================================*/
long search(char str[],char fname[])
{
  int err,i,j,brk;
  DWord rec_num,rec_idx;
  union B_P_Tree *Nod;
  FILE *fp;
  if((Nod=(union B_P_Tree *)malloc(sizeof(union B_P_Tree)))!=NULL)
  {
   if((fp=fopen(fname,"rb+"))!=NULL)
   {
    if(cheklock(fp))
    {
     read_nod(Nod,0,fp);
     rec_num=Nod->Head.Pointer_Root;
     brk=1;
     while(brk)
     {
       read_nod(Nod,rec_num,fp);
       if(Nod->Nod.Flag==Leave)
	{rec_idx=searchleave(str,Nod);brk=0;}
	else  rec_idx=searchparent(str,Nod,&rec_num);

	if(rec_idx!=-1)
	 brk=0;
	else;

     }
     } else ;
    free(Nod);
   } else printf("NOT openfilename");
   }else printf("NOT ALLOCATION");

  fclose(fp);
  return rec_idx;
}

/*----------------------------------------------*/
