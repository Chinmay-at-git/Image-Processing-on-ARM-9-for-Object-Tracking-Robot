#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	
	int i,j,k,count;;
unsigned char row,column,proto,seek;
unsigned int hist[256];	
unsigned char d2,d3,red,green,blue;
unsigned char *data,*temp;
	double redd,bluee,greenn;
	FILE *fid,*fhist;
	fid=fopen("/media/image.spc","rb");
	if(fid==NULL)
	printf("ERROR:File cannot open\n");
	fseek(fid,0,2);
	count=ftell(fid);
	rewind(fid);
	fscanf(fid,"%c%c%c",&row,&column,&proto);
	printf("%d %d %u",row,column,proto);
	//printf("unsigned Integer:%d unsigned short int %d\n unsigned char %d ",sizeof(unsigned int),sizeof(unsigned short int),sizeof(unsigned char));
	data=(unsigned char *)malloc(sizeof(unsigned char)*row*column*proto+10);  // + 10 for safeguarding.
	if(data==NULL)
	printf("ERROR: Memory cannot ne acquired,malloc returned NULL");
	temp=data;
   while(!feof(fid))
    {
    	*data=getc(fid);
        data++; 
    }
    fclose(fid);
    printf("\nJust finished the reading.\n");
   
	
	fid=fopen("/media/imagegray.spc","wb");
	fhist=fopen("/media/image.hist","wb");
	
	fprintf(fid,"%c%c%c",row,column,1);
	// 1 for gray scale.
	data=temp+3;
	//printf("\nftel=%d\n",ftell(fid)-(int)(data-temp));
	// for clearing hist array
	for(count=0;count<256;count++)
	{
		hist[count]=0;
	}
	for(i=0;i<row;i++)
	{
		for(j=0;j<column;j++)
		{
			red=*(data);
			data=data+1;
			green=*(data);
			data=data+1;
			blue=*(data);
			data=data+1;
			redd=0.18*red;
			greenn=0.72*green ;
			bluee=0.1*blue;
			// debug
			//d2=(0.21*red + 0.71*green + 0.07*blue);
            d2=(unsigned char)(redd+greenn+bluee);
			fprintf(fid,"%c",d2);
			hist[d2]++;
			
			//fprintf(fid,"%c%c%c",red,green,blue);
		
			
		}
	
	}
 for(count=0;count<2560;count++)
 putw(count,fhist);
 //fprintf(fhist,"%i",count);
	
	fclose(fid);
	fclose(fhist);
	//d3=getc(fid);
	printf("Done successfully! ");

	return 0;
}

// Takes a SPC format file ,outputs array of characters ending with Tripple zeros. As SPC file has sizes of image in pixel, 
// Host function must calculate size from there.
unsigned char * spc2gray(char *filename)
{
	FILE *fid;
	unsigned char row,column,proto,red,green,blue,*data,*temp;
	fid=fopen(filename,"rb");
		if(fid==NULL)
	printf("ERROR:File %s cannot open\n",filename);
	fscanf(fid,"%c%c%c",&row,&column,&proto);
	data=(unsigned char *)malloc(sizeof(unsigned char)*row*column+10);  // + 10 for safeguarding.
	if(data==NULL)
	printf("ERROR: Memory cannot ne acquired,malloc returned NULL");
	temp=data;
   while(!feof(fid))
    {
    	red=getc(fid);
         green=getc(fid);
         blue=getc(fid);
         *data=(unsigned char)0.18*(int)red+0.72*(int)green+0.1*(int)blue;
             data++;
    }
    *data++=0;
    *data++=0;
    *data=0;
    fclose(fid);
    return(temp);
	
}
unsigned char * maskit(unsigned char *indata,int row,int column)
{
	int i,j,k,hold,rvar,cvar;
	int mask[]={   1,   1,  1,
						    1,   -8, 1,
							1,   1,  1  };
    unsigned char rowmove[]={-1,  -1,  -1,
						0,    0,   0,
						1,     1,    1};
	unsigned char colmove[]={ -1,  0,   1,
						      -1,  0,   1,
						      -1,   0,    1};
	unsigned char *temp,*out;
	out=(unsigned char *)malloc(row*column+10); // 10 for safeguarding
	temp=out;						
	for(i=1;i<row;i++)
	{
		for(j=1;j<column;j++)
		{
			hold=0;
			for(k=0;k<9;k++)
			{
				rvar=i+rowmove[k];
				cvar=j+colmove[k];
				hold=hold + (int)(*(indata+rvar*cvar+j))*mask[k];
				
			}
			if(hold<0)
			hold=0;
			if(hold>255)
			hold=255;
			*out=hold;
			out++;
			
		}
	}
	return(temp);
}

