/*
|| SHREE ||
Author : Chinmay A. Joshi.
Date : 18-Dec-13 
For Final year project work
"Image processing on embedded platform"

RIF : Raw Image Format
RIF is nothing but format made to hold raw data extracted from standard image file formats. 
The previously used "SPC" format is superseded by RIF.
Contents of RIF file :
	unsigned int Starter[3]: contains row, column and proto(color map type) info of a image. 
							Unsigned int is implemented as 32 bit unsigned variable.
							Matlab Equivalent : 'uint32'
   unsigned char notused[8] :8 byte Space which may be used in future.   
   unsigned char data[inf] : Image raw data . Size is determined by Starter array. 
   							Matlab Equivalent : 'uint8'	
   unsigned char EOF[8]    End of file sequence for end of data identification. 
   						   5,25,125,250,0,0,255,255

Functions to be used for I/O 
	The file is binary one.  Open with Wb/ rb only.
	For C environment :	
				fopen,fclose, fseek
				fwrite and fread for file writing and reading.
				DO NOT Use fscanf and fprintf or any other function
	For Matlab Environment:
				fopen,fclose, fseek
				fwrite and fread
				Do not use any other function. 
				When you create or read varibales , Matlab convert them into double. This is not harmful, 
				as this does not distort the info. BUT when you read or write into file make sure you have defined precision that is 
				uint8/32
	Preferably store the file with .rif.  			
*/


unsigned char * rgb2gray(char *filename)
{
	// Checked at 05/08/13 22:48, Function working properly.
	// Inputs RIF File, outputs gray data;
	FILE *fid;
	unsigned char rgb[3],*data,*temp,intsize,charsize;
	unsigned int row,column,proto,starter[3];
	unsigned int imsize,count;
	intsize=sizeof(unsigned int);
	charsize=sizeof(unsigned char);
	fid=fopen(filename,"rb");
		if(fid==NULL)
	printf("\nERROR in rif2gray :File %s cannot open\n",filename);
	fread(starter,intsize,3,fid);
	row=starter[0];
column=starter[1];
imsize=row*column;
	if(starter[2]!=3)
	{
		printf("\n Error in rgb2gray() The image provided is not RGB. \n ABORT and Return");
		return(NULL);
	}
	data=(unsigned char *)malloc(charsize*imsize+12);  // + 12 for EOF sequence and safeguarding
	if(data==NULL)
	{
	
	printf("ERROR in rgb2gray() : malloc returned NULL \n Abort and return");
	return(NULL);
    
    }
	temp=data;
	fseek(fid,8l,SEEK_CUR); // 8 Byte unused space
   for(count=0;count<imsize;count++)
    {
    	fread(rgb,charsize,3,fid);
         *data=0.18*(float)rgb[0]+0.72*(float)rgb[1]+0.1*(float)rgb[2];
             data++;
    }
    // EOF Sequence (Proprietary for RIF format)
    *data++=5;
    *data++=25;
    *data++=125;
    *data++=250;
    *data++=0;
    *data++=0;
    *data++=255;
    *data=255;
    // End of EOF Sequence 
    
    
	fclose(fid);
    return(temp);
	
}
unsigned char * maskit(unsigned char *indata,double *mask,unsigned int row,unsigned int column)
{
	// checked at 05/08/13 23:26
// DO NOT use unsigned int for mask,rowmove,colmove, as we are using signed no.s
	int i,j,k,hold,rvar,cvar;
//	double mask[]={   5.0,   5,  5.0,
//						    -3.0,   0.0, -3.0,
//							-3.0,   -3.0,  -3.0  };
    int rowmove[]={-1,  -1,  -1,
						0,    0,   0,
						1,     1,    1};
	int colmove[]={ -1,  0,   1,
						      -1,  0,   1,
						      -1,   0,    1};
	unsigned char *temp,*out;
	out=(unsigned char *)malloc(row*column+12); // 10 for safeguarding
     if(out!=NULL)
	printf("\n IN maskit function, malloc worked well");
	temp=out;						
	for(i=1;i<row-1;i++)
	{
		for(j=1;j<column-1;j++)
		{
			hold=0;
			for(k=0;k<9;k++)
			{
				rvar=i+rowmove[k];
				cvar=j+colmove[k];
				hold=hold + (int)(*(indata+(rvar*column+cvar)))*mask[k];
				// debug printf("  hold=%d  ",hold);
			}
			if(hold<0)
			hold=0;
			if(hold>255)
			hold=255;
			out=temp+(i*column+j);
			*out=(unsigned char)hold;
			
			// debug printf("\t %d",i*column+j);
		}
	}
	return(temp);
}
unsigned char *makebinary(unsigned char *indata,int row,int column)
{
	unsigned char *out;
	out=(unsigned char *)malloc(row*column);
	int i;
	for(i=0;i<(row*column);i++)
	{
		if(indata[i]<120)
		out[i]=0;
		else
		out[i]=255;
		
	
	}
	return(out);
}
unsigned char *add(unsigned char *data1,unsigned char *data2,int row,int col)
{
	int i;
	unsigned char *out=malloc(row*col);
	for(i=0;i<row*col;i++)
	{
		out[i]=data1[i]/2+data2[i]/2;
		
	}
	return(out);
}
unsigned char *subthre(unsigned char *indata,int row,int col)
{
	int i,j;
	int ori,nxti,nxtj;
	unsigned char *out;
	out=malloc(row*col);
	for(i=0;i<row-1;i++)
	{
		for(j=0;j<col-1;j++)
		{
			if(i*col+j>=row*col)
			continue;
			if(i*col+j+1>=row*col)
			continue;
			if((i+1)*col+j>=row*col)
			continue;
		//	printf("\nI have entered in second for at %d",i*col+j);
			ori=*(indata+(i*col+j));
			nxti=*(indata+((i+1)*col+j));
			nxtj=*(indata+(i*col+j+1));
		if(ori>nxti)	
			{
				if((ori-nxti>50))
					ori=0;
				else
					ori=255;
			}
		else
			{
				if((nxti-ori>50))
					ori=0;
				else
					ori=255;
			}
			*(out+(i*col+j))=ori;
				
		}
	}
	return(out);
}
//===============================================================================\\
//=======Morphology=============================\\
//========EROSION=====//
unsigned char *erode(unsigned char *indata,int row,int col,unsigned char *mask,int msize)
{
	// msize == msize X msize mask  NOTE: msize must be ODD
	// indata is supposed to be binary one , containing 0 & 255 only.
	int g,i,j,masklim,mcount=0,imcount=0,m,n,pnti,pntf;
	unsigned char *out;
	g=msize/2+1;
	masklim=msize*msize;
    for(i=0;i<masklim;i++)
	     {
	     	if(mask[i]==255)
	     	mcount++;
	     }
    
	out=malloc(row*col);
	
	
	for(i=0;i<row;i++)
    {
    	for(j=0;j<col;j++)
    	{
    		imcount=0;
    		for(m=0-g;m <g-1;m++)
    		{
    			for(n=0-g;n<g-1;n++)
    			{
    				if (m+i>=0 && n+j>=0 && m+i<row && n+j<col && g+m <msize && g+n<msize)
                     {
                     	pnti=(i+m)*col+(j+n);
                     	pntf=(g+m)*msize+(g+n);
					    if (mask[pntf]==255 && indata[pnti]==255)                      
					    imcount++;
                     }
                
    			}
    		}
    	if(imcount==mcount)
    	{
    	           out[(i*col)+j]=255;
    	           
    	          //if (indata[(i*col)+j]==255)
    	         	//out[(i*col)+j]=0;// To get Edge detection.
    	           
    	           
        }
			else
			{
    		out[(i*col)+j]=0;
    		//if (indata[(i*col)+j]==255)
    		//out[(i*col)+j]=255;
    		
	    }
		}
    }
    return(out);

}


/////DILATION////////

unsigned char *dilate(unsigned char *indata,int row,int col,unsigned char *mask,int msize)
{
	// msize == msize X msize mask  NOTE: msize must be ODD
	// indata is supposed to be binary one , containing 0 & 255 only.
	int g,i,j,masklim,mcount=0,imcount=0,m,n,pnti,pntf;
	unsigned char *out,flg=0;
	g=msize/2+1;
	masklim=msize*msize;
   /* for(i=0;i<masklim;i++)
	     {
	     	if(mask[i]==255)
	     	mcount++;
	     }
    */
	out=malloc(row*col);
	
	
	for(i=0;i<row;i++)
    {
    	for(j=0;j<col;j++)
    	{
    		imcount=0;
    		for(m=0-g;m <g-1;m++)
    		{
    			for(n=0-g;n<g-1;n++)
    			{
    				if (m+i>=0 && n+j>=0 && m+i<row && n+j<col && g+m <msize && g+n<msize)
                     {
                     	pnti=(i+m)*col+(j+n);
                     	pntf=(g+m)*msize+(g+n);
					    if (mask[pntf]==255 && indata[pnti]==255)                      
					    {
					    	out[(i*col)+j]=255;
							flg=1;
							break;
						}
                     }
                
    			}
    		
				if (flg==1)
    				{
    					flg=0;
    					break;
    				}
    			else
	    			out[(i*col)+j]=0;
    		}
	    	    
		}
    }
    return(out);

}

///////EDGE DETECTION///////

unsigned char *logical_xor(unsigned char *data1,unsigned char *data2,int row,int col)
{
	// indata is supposed to be binary one , containing 0 & 255 only.
	    	           
    int i;
    unsigned char *out;
    out=malloc(row*col);
    for(i=0;i<row*col;i++)
    {
    	if(data1[i]==data2[i])
    	out[i]=0;
    	else
    	out[i]=255;
    	
    }
    return(out);
return(out);
}
////////////////////////////*Object surface area % detection*
// Assumed that : 1. Input data contains 0 (black) background and 255(white) EGDE (Edge detected previously)
////Assumed : Input data is binary (0,255)
/////Outputs percentage in double.
double measure_object(unsigned char *indata,int row,int col)
{
	int colcnt,wholecnt=0; // Must see what is INT size for ARM !!! 2^31 is sufficintly high!
	int i,j;
	double out;
	char flg=0,trans=0;
	unsigned char temp;
	for(i=0;i<row;i++)
	{
		flg=0;
		colcnt=0;
		for(j=0;j<col;j++)
		{
		    temp=indata[i*col+j];
		    if(temp==255 && trans==0)
		    {
		    	trans=1;
		    	
		    }
		    if(temp==0 && trans==1)
		    {
		    	trans=0;
		    	flg++; 
		    }
		    if((flg%2)!=0)
		    {
		    	//printf("\n Is I am here !%d %d",i,j);
		    	colcnt++;
		    }
		}
		wholecnt=wholecnt+colcnt;
	}
	out=wholecnt/(double)(row*col)*100.0;
	return(out);
}
//////       HistoGram EQUaliazation : Simple Method///
//    Input & Output: Gray Image raw data.
// 5/9/13 Chinmay 
unsigned char *histeq(unsigned char *indata,int row,int col)
{
	unsigned char *out,max=0,min=255;
	int i;
	float k;
	out=malloc(row*col);
	for(i=0;i<row*col;i++)
	{
		if(indata[i]>max)
		max=indata[i];
		if(indata[i]<min)
		min=indata[i];
	}
	k=255.0/(max-min);
	for(i=0;i<row*col;i++)
	{
		out[i]=k*(indata[i]-min);
	}
	return(out);
}

