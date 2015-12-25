#include <errno.h>
#include <fcntl.h>
#include <linux/videodev.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#define VIDEO_PALETTE_JPEG  21
#define HEIGHT 240
#define WIDTH 320
unsigned char *buffer=NULL;
int img_size;
int get_jpegsize (unsigned char *buf, int insize)
{
 int i; 	
 for ( i= 3000 ; i< insize-1; i++) {
 	if ((buf[i] == 0xFF) && (buf[i+1] == 0xD9)){
//	printf("\n Jpeg size = %d",i+2);	
	return i+575;	// OFFSET 640X480-->>2	OFFSET 320X240-->>575	
	}
 }
 return -1;
}


int capture(int fd,int *buf)
{
	struct video_mmap mmap;
	memset(&mmap,0,sizeof(mmap));
	mmap.frame=0;
	mmap.height=HEIGHT;
	mmap.width=WIDTH;
	mmap.format=VIDEO_PALETTE_JPEG;
	if(-1==xioctl(fd,VIDIOCMCAPTURE,&mmap)) 
	{
		perror("\nError : VIDIOCMCAPTURE failed");
		return -1;
	}
	int r;
	do r = ioctl (fd,VIDIOCSYNC,&mmap.frame);
	while (-1 == r && EINTR == errno);
	if(r==-1)	
	{
		perror("Error : VIDIOCSYNC failed");
		return -1;
	}
	*buf=get_jpegsize(buffer,img_size);
	if(*buf==-1)
	{
		printf("JPEG Size not available");
		return -1;
	}
	#ifdef DEBUG
	printf("\nBuffer size used : %d",*buf);
	#endif
	return 0;
}
