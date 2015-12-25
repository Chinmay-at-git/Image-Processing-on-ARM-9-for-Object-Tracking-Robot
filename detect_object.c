/*
		||SHREE||
===================================================================================
EDIT:23 Feb 14
Parent code file: main_22feb_optimized_working.c
EDIT,DEBUGGING completed :NO
Status: Editing or debugging
By: Chinmay
Why: Adding Motor driving function
What to change: To add motor drive function

Backup stored.
===================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
//#include <riflib.c>
//#include <math.h>
#include <time.h>
//#include "rif_lib.c" //  To be activated on ARM
//#include "motor_drive.c"
//#define INFILE "C:\\Users\\CHINMAY\\Documents\\MATLAB\\mobile.rif"
//#define OUTFILE "C:\\Users\\CHINMAY\\Documents\\MATLAB\\mobile_out.rif"
//#define OUT1FILE "C:\\Users\\CHINMAY\\Documents\\MATLAB\\mobile_neg.rif"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int detect_object(unsigned char *data,int w,int h,int d,int THR) {
   //FILE *fid;
   //clock_t t1,t2,t4,t3;
unsigned int imsize,flag=0, u,i,j,l,limit,k,m,n,g,z;
      int len;
unsigned char *out,*temp,array[d*d+1],t,*graydata;
   signed int x,y;
const char *INFILE,*OUTFILE;
   double alpha,beta, theta;
   //printf("Simple Object detection algorithm detect_object\n Developed for Final Y Project by SPC, \n Complied at "__TIME__" "__DATE__);
   // Input arguments : input file,output file, order(d)
   
   
   //printf("\nOrder of Median Filter: %d",d);
   
   //t1=clock();
   
   
imsize=w*h;
//printf("\nalloting mem to out alloting mem=%u",imsize);
out=(unsigned char *)malloc(imsize);
   if(!out)
{
//printf("Alloting mem to out failed, abort");
exit(1);
}
     
	graydata=rgb2gray_1(data,w,h);

/*for(i=w;i!=0;i--)
   {
   
     	for(j=h;j!=0;j--)
   		{
   			printf("%d %d %d\n",i,j,graydata[i*h+j]);
}}*/

//	for(i=0;i<76800;i++) ////printf(" %u ",graydata[i]);
	free(data);
	data=graydata;
	temp=data;   
	//printf("\nData fetching Completed... Negating the image... imsize=%u",imsize);
   //t2=clock();
	for(i=imsize/8;i!=0;i--)
	{
   		
   			t=*data;t=255-t;*data++=t;
   			t=*data;t=255-t;*data++=t;
   			t=*data;t=255-t;*data++=t;
   			t=*data;t=255-t;*data++=t;
   			
			t=*data;t=255-t;*data++=t;
   			t=*data;t=255-t;*data++=t;
   			t=*data;t=255-t;*data++=t;
   			t=*data;t=255-t;*data++=t;
// Unwrapping of loop   			
	}
	for(i=imsize&7;i!=0;i--)
	{
		t=*data;t=255-t;*data++=t;
	   			
	}
data=graydata;
   ////printf("\nImage Negation Completed... %ld to out, then Median Filter failed...",imsize);
 // Negation image saving
 /*fclose(fid);
 fid=fopen(OUT1FILE,"wb");
 
 fwrite(starter,sizeof(unsigned int),3,fid);
 fseek(fid,8l,SEEK_CUR);
 fwrite(data,1,starter[0]*starter[1],fid);
 
 *
 // 
   if(!out)
   {
   	////printf("malloc failed !!!");
   }
  
*/
   g=(d-1)/2;
    //w=2*g+1; // EDIT
   
   l=d*d/2;
	//printf("Checkpoint %d %d",g,l);  
 
//printf("Moving forward to opt. median filter");  

for(i=w-1;i!=0;i--)
   {
	////printf("Checkpoint %d",i); 
    	for(j=h-1;j!=0;j--)
   		{
				
           // //printf("\t%d-%d",i,j);
            limit=0;
            array[limit]=255;
            for(m=d;m!=0;m--)
            {	
		for(n=d;n!=0;n--)
            	{	
			
			// Take g=3, Hence we want to go from -3 to +3
			// w=2g+1 i.e 7. Hence, 7:1 to be mapped as 				3:-3 ::offset=-(g+1)
			// also, w-(g+1)=2g+1-(g+1)=g
            		x=i+m-g-1;
			y=j+n-g-1;
	
            		if(x<w-1 && x>-1 && y<h && y>-1)
            		{
				            			
				if(!((x*(h)+y)>=0 && (x*(h)+y)<=imsize-1))
				{//printf("GANDLA \t %d\t %d\t %d\n",x*(h)+y,x,y);
					exit(1);
				}			
				array[limit]=data[x*(h)+y];
						k=limit;
						////printf(" %d \n",(x*(h-1)+y));
            			 
            			while(k>0 && array[k]>array[k-1])
            			{
            				t=array[k];
            				array[k]=array[k-1];
            				array[--k]=t;
            				
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
			 
	out[i*h+j]=array[l];
   			
				
   			
	}
   		
   }
   //printf("Reached stage 2\n");
 //t3=clock();
 free(temp);
 
 //fclose(fid);// Actually pefectly correct in general case, but we are dealing with eccentric ARM. Gives segmentation error somehow.
////printf("Closed the file...");
 //fid=fopen(OUTFILE,"wb");
 //if(fid==NULL) //printf("\nFile opening problem...");
 //fwrite(starter,sizeof(unsigned int),3,fid);
 //fseek(fid,8l,SEEK_CUR);
 //fwrite(out,1,imsize,fid);
//fclose(fid);
////printf("\nFile Writing Complete...");
u=0;x=0;y=0;z=0;
 
  for(i=w;i!=0;i--)
   {
   	//printf("XXX");
     	for(j=h;j!=0;j--)
   		{
   			
			if(out[i*h+j]>THR)
   			{
   				if(!flag)
   				{
   					flag=1;
   					u=i;
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

	i=(u+y)/2;
	j=(x+z)/2;

	beta=(double)i-u;
	alpha=(double)j-h/2;
	if(alpha>0)
	k=90;
	else
	k=-90;
//theta=k+atan2(beta,alpha)*180/3.14159265;
//t4=clock();
	//printf("\nCoordinates(appro.): ROW=%d , COLUMN=%d \n alpha :%f beta: %f \n Angle theta : %f degrees \n",i,j,alpha,beta,theta);
free(out);
//free(data);



////printf("\n Timing Info in msec: 1]t3-t2 (Median Filter) :%g\n 2]t4-t1(Including Read-write overheads):%g",(double)(t3-t2)*1000/CLK_TCK,(double)(t4-t1)*1000/CLK_TCK);	

if(i==0 && j==0)
{
//motor_drive(0,0);
DRA=0xff;
printf("No Object");

}
else 
	{
		if(j<=(h/2-h/10))
		{DRA=0x07;

		printf("\nLeft %d",j);			 // left turn;
		}
		else if(j>=(h/2+h/10))

		{
			DRA=13;printf("\nRight %d",j);
		}// right turn;
		
		else //if((j>(h/2-h/10)) && (j<(h/2+h/10)))
		{
			DRA=6;
			printf("\nstraight %d",j);
		}// straight
		
		//	(j>(h/2-h/10) && j<(h/2+h/10))	
	}
		// 9-reverse 	6-forward 13-right	7 left
//motor_drive(1,k);
//return 0;
k=0x3fff;
printf("delay");
while(k--);
//

//DRA=15;

}

		
