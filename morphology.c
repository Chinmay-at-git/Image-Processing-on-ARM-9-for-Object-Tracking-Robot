
int morpho() {
	int i,j,row,col;
	double percent;
FILE *fid;
unsigned char *indata,*out,*out1;
unsigned char mask[]={255,255,255,
					 255,255,255,255,255,255,
					 255,255,255,255,255,255,
					 255,255,255,255,255,255,
					 255,255,255,255};
row=256;col=256; // Fixed ,for circles.png(.spc)
indata=malloc(row*col);
fid=fopen("c:\\Users\\CHINMAY\\Documents\\MATLAB\\circles.spc","rb");
if(fid==NULL)
printf("\nfopen returned NULL");
fseek(fid,3l,0);// The problem of int compatibility in files, not solved. Hence positions rendered useless.

for(i=0;i<(row*col);i++)
indata[i]=getc(fid);

fclose(fid);
//out=dilate(indata,row,col,mask,5);
out=erode(indata,row,col,mask,5);
out1=logical_xor(out,indata,row,col);
free(out);
percent=measure_object(out1,row,col);
fid=fopen("c:\\Users\\CHINMAY\\Documents\\MATLAB\\imagemasked.spc","wb");
if(fid==NULL)
printf("\nfopen -3returned NULL");
printf("\n Area:- %lf",percent);
fprintf(fid,"%c%c%c",row,col,1);
for(i=0;i<(row*col);i++)
{
	fprintf(fid,"%c",*(out1+i));
}	
	fclose(fid);


	return 0;
}
