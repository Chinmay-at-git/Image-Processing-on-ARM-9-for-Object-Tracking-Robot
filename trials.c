#include <stdio.h>
//#include <stdlib.h>
#include <c:\Users\CHINMAY\Documents\Final project DEV C\spclib.c>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
#include <c:\Users\CHINMAY\Documents\Final project DEV C\morphology.c>
int main(int argc, char *argv[]) {
unsigned char *gray,*masked;
int row,column,proto;
	double mask[]={   1,   1.0,  1,
				    1.0,   -8, 1.0,
					1.0,   1.0,  1.0  };
int i,j;
FILE *fid;
morpho();
fid=fopen("c:\\Users\\CHINMAY\\Documents\\MATLAB\\circles.spc","rb");
if(fid==NULL)
printf("\nfopen returned NULL");
fscanf(fid,"%c%c%c",&row,&column,&proto);
printf("%d %d %d",row,column,proto);
row=256;column=256;proto=3;
//fclose(fid);// Commented for gray image input
//gray=spc2gray("c:\\Users\\CHINMAY\\Documents\\MATLAB\\image.spc",row,column);
// The Following code is for gray image read
gray=malloc(row*column);

for(i=0;i<row*column;i++)
{
	gray[i]=getc(fid);
}
fclose(fid);
/// Special code for gray image read ends
fid=fopen("c:\\Users\\CHINMAY\\Documents\\MATLAB\\imagegray.spc","wb");
if(fid==NULL)
printf("\nfopen -2returned NULL");


fprintf(fid,"%c%c%c",row,column,1);  
for(i=0;i<((int)row*column);i++)
{
	fprintf(fid,"%c",*(gray+i));
}	
	fclose(fid);
	printf("\nHey , masking image now !!");
//masked=subthre(gray,row,column);
//masked=maskit(gray,mask,(int)row,(int)column);
masked=maskit(gray,mask,row,column);

//masked=maskit(masked,mask,(int)row,(int)column);
printf("\nout of maskit func");
// Writing masked file in spc format
fid=fopen("c:\\Users\\CHINMAY\\Documents\\MATLAB\\imagemasked1.spc","wb");
if(fid==NULL)
printf("\nfopen -3returned NULL");

printf("File imagemasked opened successfully");
fprintf(fid,"%c%c%c",row,column,1);
for(i=0;i<((int)row*column);i++)
{
	fprintf(fid,"%c",*(masked+i));
}	
	fclose(fid);
	
	return 0;
}

