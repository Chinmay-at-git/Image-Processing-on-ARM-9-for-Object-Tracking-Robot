#include <stdio.h>
//#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
FILE *fid;
fid=fopen("c:\\Users\\CHINMAY\\Documents\\MATLAB\\try.spc","w");
if(fid==NULL)
printf("FOPEN Returned NULL");
else if(ferror(fid)!=0)
printf("An error Occured while file creation");
 
 fprintf(fid,"%d%d%d",100,89,80);
 



fclose(fid);

return 0;
}
