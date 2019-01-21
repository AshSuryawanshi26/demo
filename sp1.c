#include<stdio.h>
#include<stdlib.h>
#include<string.h>



int iSt=0,iLt=0,iIc=0,lc=0,iPt,pt[20],cntToken,nmot=28;
char label[10],s1[10],s2[10],s3[10];

struct MOT
{
    char mnemonic[6];
    int Class;
    char opcode[3];

};


static struct MOT mt[28]={{"STOP",1,"00"},{"ADD",1,"01"},{"SUB",1,"02"},
   		   {"MULT",1,"03"},{"MOVER",1,"04"},{"MOVEM",1,"05"},
   			{"COMP",1,"06"},{"BC",1,"07"},{"DIV",1,"08"},
   			{"READ",1,"09"},{"PRINT",1,"10"},{"DS",2,"01"},
   			{"DC",2,"02"},{"AREG",4,"01"},{"BREG",4,"02"},
   			{"CREG",4,"03"},{"START",3,"01"},{"END",3,"02"},
   			{"ORIGIN",3,"03"},{"EQU",3,"04"},{"LTORG",3,"01"},
   			{"EQ",5,"01"},{"LT",5,"02"},{"GT",5,"03"},
   			{"LE",5,"04"},{"GE",5,"05"},{"NE",5,"06"},
   			{"ANY",5,"07"}};

struct SymTab
{
    char symbol[10];
    int address;
    int index;
}st[20];

struct IcTab
{
	int lc;
	int code1,type1;
	int code2,type2;
	int code3,type3;
}ic[30];

int search_MOT(char s[10])
{
    int i;
    for(i=0;i<nmot;i++)
    {
   	 if(strcmp(s,mt[i].mnemonic)==0)
   	 {
   		 return i;
   	 }
    }
    return -1;
}

int search_symTab(char s[10])
{
    int i,flag;
    for(i=0;i<iSt;i++)
    {
   	 if(strcmp(s,st[i].symbol)==0)
   	 {
   		 return i;
   		 break;
   	 }
    }
    return -1;
}

int insert_symTab(char symbol[10],int addr)
{
    strcpy(st[iSt].symbol,symbol);
    st[iSt].address=addr;
    st[iSt].index=iSt;
    iSt++;
    return(iSt-1);
}



void directive()
{
	 if(strcmp(s1,"START")==0)
	    {
	   	 START();
	   	 return;
	    }
	    if(strcmp(s1,"EQU")==0)
	    {
	   	 EQU();
	   	 return;
	    }

 //printf("I am in directive statement\n");

}


void DC()
{
    int index;
    index=search_MOT(s1);
    ic[iIc].type1=ic[iIc].type2=ic[iIc].type3=0;   	 //initialise
    ic[iIc].lc=lc;
    ic[iIc].code1=index;
    ic[iIc].type1=mt[index].Class;
    ic[iIc].type2=6;
    ic[iIc].code2=atoi(s2);
    index=search_symTab(label);
    if(index==-1)
   	 index=insert_symTab(label,lc);
    st[index].address=lc;
    lc++;
    iIc++;
}

void DS()
{
    int index;
    index=search_MOT(s1);
    ic[iIc].type1=ic[iIc].type2=ic[iIc].type3=0;   	 //initialise
    ic[iIc].lc=lc;
    ic[iIc].code1=index;
    ic[iIc].type1=mt[index].Class;
    ic[iIc].type2=6;
    ic[iIc].code2=atoi(s2);
    index=search_symTab(label);
    if(index==-1)
   	 index=insert_symTab(label,0);
    st[index].address=lc;
    lc=lc+atoi(s2);
    iIc++;
}


void START()
{
    int index;
    index=search_MOT(s1);
    ic[iIc].type1=ic[iIc].type2=ic[iIc].type3=0;   	 //initialise
    ic[iIc].lc=lc;
    ic[iIc].code1=index;
    ic[iIc].type1=mt[index].Class;
    ic[iIc].type2=6;
    ic[iIc].code2=atoi(s2);
    lc=atoi(s2);
    iIc++;
}


void EQU()
{
  int index,index_new;
  index=search_MOT(s1);
    ic[iIc].type1=ic[iIc].type2=ic[iIc].type3=0;   	 //initialise
    ic[iIc].lc=lc;
    ic[iIc].code1=index;
    ic[iIc].type1=mt[index].Class;
    ic[iIc].type2=6;

    index=search_symTab(label);

    index_new=search_symTab(s2);

    st[index].address=st[index_new].address;

    ic[iIc].code2=st[index_new].address;
    iIc++; //lc not increment

}

void declarative()
{
	  if(strcmp(s1,"DC")==0)
	    {
	   	 DC();
	   	 return;
	    }
	    if(strcmp(s1,"DS")==0)
	   	 DS();
   //printf("I am in declarative statement\n");
}

void imperative()
{
	int index;
	index=search_MOT(s1);
	ic[iIc].type1=ic[iIc].type2=ic[iIc].type3=0;   	 //initialise
	ic[iIc].lc=lc;
	ic[iIc].code1=index;
	ic[iIc].type1=mt[index].Class;
	lc++;
	if(cntToken>1)
		{
			index=search_MOT(s2);
			if(index==-1)
			{   	//it is a variable
				index=search_symTab(s2);
				if(index==-1)
					index=insert_symTab(s2,lc);
					ic[iIc].code2=index;
					ic[iIc].type2=7;
			}
			else
			{
				ic[iIc].code2=index;
				ic[iIc].type2=mt[index].Class;
			}
		}

	if(cntToken>2)
	{
		 index=search_symTab(s3);
		 if(index==-1)
		 index=insert_symTab(s3,lc);

		 ic[iIc].type3=7;
		 ic[iIc].code3=st[index].index;

	}
iIc++;
//printf("\n I am in imperative statement");
}

void printSt()
{
    int i;
    printf("\n\n===========Symbol Table=========================");
    printf("\n\nNo       	Symbol       	Address");
    printf("\n==================================================");
    for(i=0;i<iSt;i++)
    {
   	 printf("\n%d\t%s\t%d",st[i].index,st[i].symbol,st[i].address);
    }
}
void print_opcode()
{
    int i;
    printf("\n\n=============Mnemonic Opcode Table================");
    for(i=0;i<nmot;i++)
    {
   	 if(mt[i].Class==1)
   	 {
   		 printf("\n%6s   %2s",mt[i].mnemonic,mt[i].opcode);
   	 }
    }
}

void Icode()
{
    int i;
    char decode[9][3]={" ","IS","DL","AD","RG","CC","C","S","L"};

    printf("\n\n==============Intermediate Code================ ");
    printf("\n=================================================");
    for(i=0;i<iIc;i++)
    {
   	 printf("\n%3d) (%s, %2s)",ic[i].lc,decode[ic[i].type1],mt[ic[i].code1].opcode);
   	 if(ic[i].type2!=0)
   	 {
   		 if(ic[i].type2<6)
   			 printf("(%s, %2s)",decode[ic[i].type2],mt[ic[i].code2].opcode);
   		 else
   			 printf("(%s, %2d)",decode[ic[i].type2],ic[i].code2);
   	 }
   	 if(ic[i].type3!=0)
   		 printf("(%s, %2d)",decode[ic[i].type3],ic[i].code3);
    }
}

int main()
{
	FILE *fp;
    int i=0,j,len,temp,errorType;
    char b_line[80];


    fp=fopen("input.txt","r");
    while(!feof(fp))
    {
   	 i=0;
   	 b_line[i]=fgetc(fp);
   	 while(b_line[i]!='\n'&&b_line[i]!=EOF)
   	 {
   		 if(!isalnum(b_line[i]))
   			 b_line[i]=' ';

   		 else
   			 b_line[i]=toupper(b_line[i]);
   		 i++;
   		 b_line[i]=fgetc(fp);
   	 }

   	 b_line[i]='\0';
   	 sscanf(b_line,"%s",s1);

   	 if(strcmp(s1,"END")==0)
   		 break;

   	   if(search_MOT(s1)==-1)
   	 	{
   		 if(search_symTab(s1)==-1)
   			 insert_symTab(s1,lc);
   		 //separate opcodes and operands
   		 cntToken=sscanf(b_line,"%s%s%s%s",label,s1,s2,s3);
   		 cntToken--;
   	  	}
   	 	else
   		 cntToken=sscanf(b_line,"%s%s%s",s1,s2,s3);
   	  if(cntToken==0)
   	  continue; //go to the beginning of the loop

   	 i=search_MOT(s1);
   	 if(i==-1)
   	 {
   		 printf("\nWrong Opcode....%s",s1);
   		 continue;
   	 }

   	 switch(mt[i].Class)
   	 {
   		 case 1:imperative();
   			 break;
   		 case 2:declarative();
   			 break;
   		 case 3:directive();
   			 break;
}
    }
    fclose(fp);
       printSt();
       print_opcode();
       Icode();


    return 0;
}




