#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct MntTableEntry
{
	char name[30];
	int index;
	int pindex;
} MNTTable[20];

struct MdtTableEntry
{
	char name[50];
} MDTTable[20];

struct FormalParameterEntry
{
	char name[50];
	int def;
	int MNTindex;
	char type;
	char actualp[50];
} FormalParameterTable[20];

int main()
{
	FILE * fp1,*fp2,*fp3,*fp4,*fp5;
	fp1=fopen("input.txt","r");
	fp2=fopen("pass1out.txt","w");
	fp3=fopen("mnttable.txt","w");
	fp4=fopen("mdttable.txt","w");
	fp5=fopen("parametertable.txt","w");
	
	

	if(fp1==NULL || fp2==NULL || fp3==NULL || fp4==NULL )
	{
		printf("Error: Input File not found ");
		return 0;
	}

	int i,macroflag1=0,macroflag2=0,mntcounter=1,mdtcounter=1,len=50,formalpcounter=1,tflag=0;
	char * line;

	while(getline(&line,&len,fp1)!=-1)
	{
		line[strlen(line)]='\0';
		int ch;
		char token1[20];
		char token2[20];
		char token3[20];
		char token4[20];
		char token5[20];

		ch=sscanf(line , "%s%s%s%s%s",token1,token2,token3,token4,token5);

		
		if(strcmp(token1,"END")==0)
		{
			
			fprintf(fp2, "%s\n", line );
			fclose(fp1);
			fclose(fp2);
			break;
		}
		if(strcmp(token1,"MEND")==0)
		{
			strcpy(MDTTable[mdtcounter++].name,line);
			macroflag1=0;
			macroflag2=0;
			continue;
		}
		if(macroflag2==1)
		{
			strcpy(MDTTable[mdtcounter++].name,line);
			continue;
		}
		if(macroflag1==1)
		{
			strcpy(MNTTable[mntcounter].name,token1);
			MNTTable[mntcounter].index=mdtcounter;
			MNTTable[mntcounter++].pindex=formalpcounter;
			macroflag2=1;

			if(tflag==1)
			{
				if(ch>=2)
				{

					int z=0 , j=0,defv=-1;
					char val[200],t2[100];

					if(token2[0]=='&')
					{
						FormalParameterTable[formalpcounter].type='p';
						
					}
					else
					{
						FormalParameterTable[formalpcounter].type='k';	
						
					}


					while(token2[j]!='=' && j<strlen(token2))
					{
						t2[z++]=token2[j++];
					}
					t2[z]='\0';

					z=0;j+=1;


					if(j!=strlen(token2))
						{
							while(j<strlen(token2))
								{
									val[z++]=token2[j++];
								}
							val[z]='\0';
							defv=atoi(val);
						}
				

										
					FormalParameterTable[formalpcounter].def = defv;
					strcpy(FormalParameterTable[formalpcounter].name,t2);
					FormalParameterTable[formalpcounter++].MNTindex=(mntcounter-1);

				}

				if(ch>=3)
				{
					int z=0 , j=0,defv=-1;
					char val[200],t3[100];

					if(token3[0]=='&')
					{
						FormalParameterTable[formalpcounter].type='p';
						j=1;
					}
					else
					{
						FormalParameterTable[formalpcounter].type='k';	
						j=0;
					}


					while(token3[j]!='=' && j<strlen(token3))
					{
						t3[z++]=token3[j++];
					}
					t3[z]='\0';

					z=0;j+=1;

					if(j!=strlen(token3))
						{
							while(j<strlen(token3))
								{
									val[z++]=token3[j++];
								}
							val[z]='\0';
							defv=atoi(val);
						}
					

					FormalParameterTable[formalpcounter].def = defv;
					strcpy(FormalParameterTable[formalpcounter].name,t3);
					FormalParameterTable[formalpcounter++].MNTindex=(mntcounter-1);

				}
				
			}

			tflag=0;

			continue;
		}

		if(strcmp(token1,"MACRO")==0)
		{

			macroflag1=1;
			tflag=1;
			continue;
		}

		fprintf(fp2, "%s", line );

	}

	
	for(i=1;i<mntcounter;i++)
		{
			fprintf(fp3,"%s %d %d\n",MNTTable[i].name,MNTTable[i].index,MNTTable[i].pindex);
		}

		
	for(i=1;i<mdtcounter;i++)
		{
			fprintf(fp4,"%s",MDTTable[i].name);
		}

	for(i=1;i<formalpcounter;i++)
		{
			fprintf(fp5,"%d	%s 	%d	 %c\n",FormalParameterTable[i].MNTindex ,FormalParameterTable[i].name, FormalParameterTable[i].def, FormalParameterTable[i].type);
		}

	fclose(fp3);
	fclose(fp4);
	fclose(fp5);


}
