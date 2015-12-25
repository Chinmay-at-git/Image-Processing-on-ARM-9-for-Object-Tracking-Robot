
/*
Edit: By Chinmay
at 23rd of Feb 14

Why: To add this program as a function in detect_object code 

/********************************************************************************************************
# Project Code : SBC9302
# Created on Thu, 25 Jul 2013 20:06:17 +0900
#
# New Application
#
# Copyright (C) 2008 SPJ Embedded Technologies Pvt. Ltd. (www.spjsystems.com)
# All rights reserved.
#
# -------
# History
# -------
# Date       |Release|Author| Changes
# DD-MM-YYYY |       |      |
# ----------------------------------------------------------------------------
#            |       |      | 
#            |       |      | 
#            |       |      | 
********************************************************************************************************/

/* include header files here */

/* main function */
#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
//#include<stdio.h>
#include<fcntl.h>
#include <ctype.h>
//#include <stdlib.h>
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
int motor_drive(int ifrun,int how) {
int FileHandle,i,what;

//printf("\nNote: This program must be run with root privilege\n");

printf("\nIn Motor_drive function:The pagesize for this processor is %d\n",getpagesize());
PortAddressSpace = (unsigned char *)mmap(0,getpagesize(), \
PROT_READ | PROT_WRITE, MAP_SHARED, \
FileHandle,IOPORT_BASE);

if (PortAddressSpace == MAP_FAILED) 
{
printf("\nUnable to map memory space\n");
return (-2);
}
if(ifrun==0)
{
printf("\nIFRUN==0");

return 0;
}
//printf("\nPort Address %u",PortAddressSpace);
DDRA|=0xff;					
DDRB|=0xff;
//printf("\nMotor running by condition %x \n",how);
	//DRB=0x03;
	DRA=how;
	//printf("OF<--");	
	/*for(i=1;i<665000;i++);
	DRA=0xf0;
	DRB=0x0f;	
	printf("-->FO");
	for(i=1;i<665000;i++);
*/



munmap(PortAddressSpace,getpagesize());
//printf("\n");
close(FileHandle);
return(0);
}

