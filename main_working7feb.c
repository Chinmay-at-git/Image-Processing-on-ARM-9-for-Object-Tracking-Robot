#include <stdio.h>
#include <stdlib.h>
//#include <riflib.c>
//#include <math.h>
#include <time.h>
#include "rif_lib.c" //  To be activated on ARM
//#define INFILE "C:\\Users\\CHINMAY\\Documents\\MATLAB\\mobile.rif"
//#define OUTFILE "C:\\Users\\CHINMAY\\Documents\\MATLAB\\mobile_out.rif"
//#define OUT1FILE "C:\\Users\\CHINMAY\\Documents\\MATLAB\\mobile_neg.rif"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int print_usage()
{
	printf("\nArgument list invalid");
	printf("\n detect_object [input RIF file] [output RIF file] <order of median filter>");
	printf("\n Order is optional , if not given , default order is 5, Range of Order is 0 to 50");
	printf("\n Outputs X and Y coordinates of object(appro.) and angle from midpoint of bottom row of image(appro)");
	printf("\n Outputs file with median filter applied");
    return 1;
}
int main(int argc, char *argv[]) {
   FILE *fid;
   clock_t t1,t2,t4,t3;
   unsigned char *data,*out,d=5;
   signed char m,n,g;
   unsigned char array[d*d+1],t,flag=0;
   const char *INFILE,*OUTFILE;
   unsigned int starter[3],imsize;
   int i,j,l,limit,k,w,x,y,z,count;
   double alpha,beta, theta;
   printf("Simple Object detection algorithm detect_object\n Developed for Final Y Project by SPC, \n Complied at "__TIME__" "__DATE__);
   // Input arguments : input file,output file, order(d)
   if(argc<3||argc>4)
   {
   	return print_usage();
   }
   INFILE=argv[1];
   OUTFILE=argv[2];
   
   if(argc==4)
   {
   	d=atoi(argv[3]);
   	if(d<0||d>50)
   	{
   		printf("Order argument invalid...Range is 0 to 50 , Taking d=5\n");
   		d=5;
   	}
   	
   }
   printf("\nSource File =\"%s\"  \nDestination File:\"%s\"",INFILE,OUTFILE);
   printf("\nOrder of Median Filter: %d",d);
   
   t1=clock();
   
   fid=fopen(INFILE,"rb");
   if(!fid)
   printf("\nInput File opening error\n");
   //printf("File opened");
   fread(starter,sizeof(unsigned int),3,fid);
   printf("\nalloting mem to out");
imsize=starter[0]*starter[1];
out=(unsigned char *)malloc(imsize+12);
   if(!out)
   {
   	printf("malloc failed !!!");
   }
   
   if(starter[2]==3)
   {
   // If image is rgb, decode it !
   fclose(fid);
   printf("\n Input File is of RGB Type. Decoding RGB to Gray...");
   
   data=rgb2gray(INFILE);
}
else if(starter[2]==1)
{
	printf("\nInput File is of GRAY Type. Extracting Gray Data...");
   
	fseek(fid,8l,SEEK_CUR);
   data=(unsigned char*)malloc(starter[0]*starter[1]);
   
   fread(data,sizeof(unsigned char),starter[0]*starter[1],fid);
   //printf("Did I read it? Starter: %d %d %d",starter[0],starter[1],starter[2]);
   fclose(fid);

}
else{
	printf("\nInput File is neither Gray nor RGB type. Other types not supported yet. \n ABORT and EXIT... ");
	exit(1);
}
starter[2]=1;
//printf("\nData fetching Completed... Negating the image...");
   t2=clock();
   for(i=0;i<starter[0];i++)
   {
   		for(j=0;j<starter[1];j++)
   		{
   			//printf("\n Is I In");
   			data[i*starter[1]+j]=255-data[i*starter[1]+j];
   			
   		}
   //		printf("\t %d",i);
   }
  // printf("\nImage Negation Completed... %ld to out, then Median Filter started...",imsize);
 // Negation image saving
 /*fclose(fid);
 fid=fopen(OUT1FILE,"wb");
 
 fwrite(starter,sizeof(unsigned int),3,fid);
 fseek(fid,8l,SEEK_CUR);
 fwrite(data,1,starter[0]*starter[1],fid);
 
 */
 // 
  
   g=(d-1)/2;
    
   
   l=(int)d*d/2.0;
   count=0;
   for(i=0;i<starter[0];i++)
   {
     	for(j=0;j<starter[1];j++)
   		{
            
            limit=0;
            array[limit]=255;
            for(m=-g;m <=g;m++)
            {	for(n=-g;n<=g;n++)
            	{
            		
            		if((i+m)<starter[0] && (i+m)>-1 && (j+n)<starter[1] && (j+n)>-1)
            		{
            			array[limit]=data[(i+m)*starter[1]+(j+n)];
						k=limit;
						
            			 
            			while(k>0 && array[k]>array[k-1])
            			{
            				t=array[k];
            				array[k]=array[k-1];
            				array[k-1]=t;
            				k--;
            			}
            	
						limit++;
						
            			
            		}
            		else
            		{
            			array[limit]=0;
            			limit++;
            		}
            		
            	}
	 		}
			 //	printf("\t %d",i*starter[1]+j);
	 		//l=d*d/2;
	 	//printf("\t l=%d array[l]= %d i,j= %d %d ",l,array[l],i,j);
			out[i*starter[1]+j]=array[l];
   //	printf("Start\t");
		//	   for(k=0;k<d*d;k++)
   			{
			//printf("\t %d",array[k]);
   				
   			}
   		}
   		
   }
   //printf("Reached stage 2\n");
 t3=clock();
 free(data);
 
 //fclose(fid);// Actually pefectly correct in general case, but we are dealing with eccentric ARM. Gives segmentation error somehow.
printf("Closed the file...");
 fid=fopen(OUTFILE,"wb");
 if(fid==NULL) printf("\nFile opening problem...");
 fwrite(starter,sizeof(unsigned int),3,fid);
 fseek(fid,8l,SEEK_CUR);
 fwrite(out,1,imsize,fid);
fclose(fid);
printf("\nFile Writing Complete...");
w=0;x=0;y=0;z=0;
 
  for(i=0;i<starter[0];i++)
   {
   //	printf("XXX");
     	for(j=0;j<starter[1];j++)
   		{
   		//	printf("W");
   			if(out[i*starter[1]+j]>250)
   			{
   				if(flag==0)
   				{
   					flag=1;
   					w=i;
   					x=j;
   				}
   				else
   				{
   					y=i;
   					z=j;
   				}
   			}
   			
  
 		}
	}
	
	i=(w+y)/2;
	j=(x+z)/2;
	beta=(double)i-starter[0];
	alpha=(double)j-starter[1]/2;
	if(alpha>0)
	k=90;
	else
	k=-90;
//theta=k+atan2(beta,alpha)*180/3.14159265;
t4=clock();
	printf("\nCoordinates(appro.): ROW=%d , COLUMN=%d \n alpha :%f beta: %f \n Angle theta : %f degrees",i,j,alpha,beta,theta);
free(out);
//free(data);


printf("\n Timing Info in msec: 1]t3-t2 (Median Filter) :%g\n 2]t4-t1(Including Read-write overheads):%g",(double)(t3-t2)*1000/CLK_TCK,(double)(t4-t1)*1000/CLK_TCK);	
	
	
	return 0;
}


