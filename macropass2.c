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

void constructtables(int * mntcounter , int * paracounter)
{
	FILE * fp1 ;
	fp1=fopen("mnttable.txt","r");
	

	if(!fp1)
	{
		printf("\n Error opening MNT file");
	}
	char* line;
	line=(char*)calloc(50,sizeof(char));
	int len = 50;


	while(getline(&line ,(size_t *) &len , fp1)!=-1)
	{
		line[strlen(line)]='\0';
		int ch;
		char token1[20];
		char token2[20];
		char token3[20];
	

		ch=sscanf(line , "%s%s%s",token1,token2,token3);

		//printf("%s %s\n",token1 , token2);
		strcpy( MNTTable[*mntcounter].name ,token1);
		MNTTable[(*mntcounter)].index = atoi(token2);
		MNTTable[*mntcounter].pindex=atoi(token3);

		*mntcounter+=1;
	}

	fclose(fp1);


	fp1=fopen("parametertable.txt","r");

	if(!fp1)
	{
		printf("\n Error opening Parameter File");
	}

	while(getline(&line , (size_t *)&len , fp1)!=-1)
	{
		line[strlen(line)]='\0';
		int ch;
		char token1[20];
		char token2[20];
		char token3[20];
		char token4[20];
	

		ch=sscanf(line , "%s%s%s%s",token1,token2,token3,token4);

		//printf("%s %s\n",token1 , token2);

		strcpy( FormalParameterTable[*paracounter].name ,token2);
		FormalParameterTable[*paracounter].def=atoi(token3);
		FormalParameterTable[*paracounter].MNTindex=atoi(token1);
		FormalParameterTable[*paracounter].type=token4;

		*paracounter+=1;
	}

	fclose(fp1);
}

int main()
{
	int mntcounter=1,paracounter=1;
	constructtables(&mntcounter,&paracounter);

	char *line;
	line=(char*)calloc(50,sizeof(char));
	int ch,i,len=50;
	FILE * fp , *fp2 , *fp3;
	fp=fopen("pass1out.txt","r");

	fp2=fopen("pass2output.txt","w");

	while(getline(&line,(size_t *) &len,fp)!=1)
	{
		char token1[20] , token2[20] , token3[20] , token4[20];
		int mf=0 ;

		ch = sscanf(line ,"%s%s%s",token1,token2,token3);

		for(i=1;i<mntcounter;i++)
		{
			if(strcmp(token1,MNTTable[i].name)==0)
			{

				mf=1;

				if(ch>=2)
				{
					if(FormalParameterTable[MNTTable[i].pindex].name[0]=='&')
					{
						
						strcpy(FormalParameterTable[MNTTable[i].pindex].actualp,token2);
					}
					
					else
					{
						int j = MNTTable[i].pindex,z=0,k=0;
						int temp = i;
						char t2[50],val[50];

						while(token2[z]!='=')
						{
							t2[z]=token2[z];
							z++;
						}
						t2[z]='\0';
						z+=1;
						
						while(z<strlen(token2))
						{
							val[k++]=token2[z++];
						}
						val[k]='\0';

						while(FormalParameterTable[j].MNTindex==temp)
						{
							if(strcmp(t2,FormalParameterTable[j].name)==0)
							{
								
								strcpy(FormalParameterTable[MNTTable[j].pindex].actualp,val);
								break;
							}
							j++;
						}
					}					
				}


				if(ch>=3)
				{
					if(FormalParameterTable[MNTTable[i].pindex+1].name[0]=='&')
					{
						strcpy(FormalParameterTable[MNTTable[i].pindex+1].actualp,token3);
					}
					
					else
					{
						
						int j = MNTTable[i].pindex,z=0,k=0;
						int temp = i;
						char t3[50],val[50];

						while(token3[z]!='=')
						{
							t3[z]=token3[z];
							z++;
						}
						t3[z]='\0';
						z+=1;
						
						while(z<strlen(token3))
						{
							val[k++]=token3[z++];
						}
						val[k]='\0';
						
						

						while(FormalParameterTable[j].MNTindex==temp)
						{
							if(strcmp(t3,FormalParameterTable[j].name)==0)
							{

								strcpy(FormalParameterTable[j].actualp,val);
								break;
							}
							j++;
						}
					}	
				}



				fp3=fopen("mdttable.txt","r");
				char *mline;
				mline=(char*)calloc(20,sizeof(char));
				len =20;

				int z1=1;
				
				
				while(z1 < MNTTable[i].index)
				{
					getline(&mline , (size_t *)&len , fp3);
						
					z1++;
					
				}
				
				
				while(getline(&mline , (size_t *)&len , fp3)!=-1)
				{
					

					int ch,j;
					char *token1 , *token2,*token3 , *token4;
					token1=(char*)calloc(20,sizeof(char));
					token2=(char*)calloc(20,sizeof(char));
					token3=(char*)calloc(20,sizeof(char));
					token4=(char*)calloc(20,sizeof(char));




					ch = sscanf(mline , "%s%s%s%s",token1, token2 , token3 , token4);
					int temp = i , pflag=0;


					if(ch>=1)
					{
						if(strcmp(token1 , "MEND")==0)
							goto bk;
						
						for(j=MNTTable[i].pindex ; j<paracounter ; j++ )
						{
							if(FormalParameterTable[j].MNTindex!=temp )
								break;

							if(strcmp(FormalParameterTable[j].name , token1)==0)
							{
								pflag=1;
								break;
							}
						}

						
						if(pflag==1)
							fprintf(fp2, "%s ", FormalParameterTable[j].actualp );
						else 
							fprintf(fp2, "%s ", token1 );
					}

					if(ch>=2)
					
					{
						pflag=0;
						for(j=MNTTable[i].pindex ; j<paracounter ; j++ )
						{
							if(FormalParameterTable[j].MNTindex!=temp )
								break;

							if(strcmp(FormalParameterTable[j].name , token2)==0)
							{
								pflag=1;
								break;
							}
						}

						
						if(pflag==1)
							fprintf(fp2, "%s ", FormalParameterTable[j].actualp );
						else 
							fprintf(fp2, "%s ", token2 );
					}

					if(ch>=3)
					{
					pflag=0;
						for(j=MNTTable[i].pindex ; j<paracounter ; j++ )
						{
							if(FormalParameterTable[j].MNTindex!=temp )
								break;

							if(strcmp(FormalParameterTable[j].name , token3)==0)
							{
								pflag=1;
								break;
							}
						}

						
						if(pflag==1)
							fprintf(fp2, "%s ", FormalParameterTable[j].actualp );
						else 
							fprintf(fp2, "%s ", token3 );
					}


					if(ch>=4)
					{
						pflag=0;
						for(j=MNTTable[i].pindex ; j<paracounter ; j++ )
						{
							if(FormalParameterTable[j].MNTindex!=temp )
								break;

							if(strcmp(FormalParameterTable[j].name , token4)==0)
							{
								pflag=1;
								break;
							}
						}

						
						if(pflag==1)
							fprintf(fp2, "%s ", FormalParameterTable[j].actualp );
						else 
							fprintf(fp2, "%s ", token4 );
					}

					fprintf(fp2, "\n" );
				}
			}

		}

		bk:
		
		if(mf==0)
		{
			fprintf(fp2,"%s",line );
			continue;
		}

	}


	for(i=1;i<=3;i++)
	printf("%s %d %s\n",FormalParameterTable[i].name , FormalParameterTable[i].MNTindex , FormalParameterTable[i].actualp);
}
