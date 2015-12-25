#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<fcntl.h>
#include <ctype.h>
#define IOPORT_BASE 0x80840000
#define PORTA_DR_OFFSET 0x0
#define PORTA_DDR_OFFSET 0x10
#define PORTB_DR_OFFSET 0x4
#define PORTB_DDR_OFFSET 0x14
#define PORTE_DR_OFFSET 0x20
#define PORTE_DDR_OFFSET 0x24
unsigned char *PortAddressSpace;
#define DDRA ( *(unsigned char *)(PortAddressSpace+PORTA_DDR_OFFSET))
#define DRA ( *(unsigned char *)(PortAddressSpace+PORTA_DR_OFFSET))
#define DDRB ( *(unsigned char *)(PortAddressSpace+PORTB_DDR_OFFSET))
#define DRB ( *(unsigned char *)(PortAddressSpace+PORTB_DR_OFFSET))
#define DDRE ( *(unsigned char *)(PortAddressSpace+PORTE_DDR_OFFSET))
#define DRE ( *(unsigned char *)(PortAddressSpace+PORTE_DR_OFFSET))


#include "camera.c"
#include "rif_lib.c"
#include "detect_object.c"
#include "jpg2rif.c"


int xioctl(int fd, int request, void *arg)
{
int r;
do r = ioctl (fd, request, arg);
while (-1 == r && EINTR == errno);
return r;
}


int print_usage()
{
printf("\nTakes a picture from camera attached to /dev/video0 and detects object");
printf("\nUsage: spc_test1 outfile.jpg <order of median filter> <threshold>");
printf("\nInvalid Argument List");
return(1);
}


int main (int argc,char *argv[])
{


int fd,fdmem,buffer_used,w,h,THR=250,d=5;
unsigned char *rgbdata;
//const char *OUTFILE;

FILE *outfd;

struct video_mbuf mbuf;

if(argc<2||argc>4)
{
return print_usage();
}
fdmem = open("/dev/mem",O_RDWR);
PortAddressSpace = (unsigned char *)mmap(0,getpagesize(),PROT_READ | PROT_WRITE, MAP_SHARED,fdmem,IOPORT_BASE);
if (PortAddressSpace == MAP_FAILED) 
{
printf("\nUnable to map memory space\n");
return (-2);
}
fd = open("/dev/video0",O_RDWR);
if (fd == -1)
{
perror("Error : Opening video device");
return 1;
}
DDRA|=0xff;					
//OUTFILE=(const char *)"spc_test.rif";

if(argc>=3)
{
d=atoi(argv[2]);
}
if(argc>=4)
{THR=atoi(argv[3]);}

if(-1==xioctl(fd,VIDIOCGMBUF,&mbuf))
{
	perror("Error : VIDIOCGMBUF failed\n");
	return -1;
}

img_size=22000;
buffer=(unsigned char *)mmap(0,22000,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
#ifdef DEBUG	
	//printf("\nframes : %d\n",mbuf.frames);
	//printf("\nbuffer size : %d \toffset : %d\n",mbuf.size,mbuf.offsets[1]);	
	//printf("\nBuffer Address : %u",buffer);	
#endif

while(1)
{

if(capture(fd,&buffer_used)) return -3;
#ifdef DEBUG
//printf("\nCapture complete\n");
#endif


outfd=fopen("spc_test2.jpg", "wb");
if(outfd==NULL)
{
//printf("File opening error");
return -1;
}

#ifdef DEBUG
else //printf("File opened successfully\n");
#endif

fwrite(buffer,sizeof(unsigned char),buffer_used,outfd);
fclose(outfd);

rgbdata=jpg2rif("spc_test2.jpg",&w,&h,0);
detect_object(rgbdata,w,h,d,THR);

}
close(fd);
munmap(PortAddressSpace,getpagesize());
close(fdmem);

return 0;
}

