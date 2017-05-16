/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: USB drive control */
//#include "atadrive.h"	/* Example: ATA drive control */
//#include "sdcard.h"		/* Example: MMC/SDC contorl */

/* Definitions of physical drive number for each media */
#define ATA		0
#define MMC		1
#define USB		2


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive number (0..) */
)
{
	pdrv = 0;
	return (DSTATUS)0;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number (0..) */
)
{
	pdrv = 0;
	return (DSTATUS)0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive number (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
	BYTE result;
	
	pdrv = 0;	//躲警告
	if (count == 0)
	{
		result = 0x00;
	}
	else if (count == 1)
	{
		result = SD_read_block(sector, buff);
	}
	else
	{
		result = SD_read_multiple_block(sector, count, (BYTE (*)[SD_SECTOR_SIZE])buff);
	}
	if (result == 0x00)
	{
		res = RES_OK;
	}
	else
	{
		res = RES_ERROR;
	}
	
	return res;
	/*
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		result = ATA_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

		result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}
	return RES_PARERR;
	*/
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
	BYTE result;
	
	pdrv = 0;	//躲警告
	if (count == 0)
	{
		result = 0x00;
	}
	else if (count == 1)
	{
		result = SD_write_block(sector, buff);
	}
	else
	{
		result = SD_write_multiple_block(sector, count, (const BYTE (*)[SD_SECTOR_SIZE])buff);
	}
	
	if (result == 0x00)
	{
		res = RES_OK;
	}
	else
	{
		res = RES_ERROR;
	}
	
	return res;
	/*
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		result = ATA_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

		result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}
	return RES_PARERR;
	*/
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive number (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	//这个函数仅仅在格式化的时候被使用，在调试读写的时候，这个函数直接让他返回0就OK 了。
	pdrv = 0;	//躲警告
	cmd = 0;	//躲警告
	buff = 0;	//躲警告
	
	return RES_OK;
	/*
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// pre-process here

		result = ATA_disk_ioctl(cmd, buff);

		// post-process here

		return res;

	case MMC :
		// pre-process here

		result = MMC_disk_ioctl(cmd, buff);

		// post-process here

		return res;

	case USB :
		// pre-process here

		result = USB_disk_ioctl(cmd, buff);

		// post-process here

		return res;
	}
	return RES_PARERR;
	*/
}


//-----------------------------------------------------------------------//
// 返回系统时间                                                          //
//-----------------------------------------------------------------------//
DWORD get_fattime(void)
{
	return ((DWORD)(2010 - 1980) << 25)	// Fixed to Jan. 1, 2010 //
	| ((DWORD)1 << 21)
	| ((DWORD)1 << 16)
	| ((DWORD)0 << 11)
	| ((DWORD)0 << 5)
	| ((DWORD)0 >> 1);
}


#endif
