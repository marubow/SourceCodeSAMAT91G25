#include <common.h>
#include <linux/mtd/mtd.h>
#include <command.h>
#include <malloc.h>
#include <spi_flash.h>
#include <part.h>
#include <fat.h>
#include <nand.h>
#include <usb.h>
#include <lcd.h>
#include <asm/arch/hardware.h>
#include <mmc.h>

#include "compiler.h"
#include "usbupgrade.h"

#ifndef CONFIG_USB_OHCI_NEW
#error "must define CONFIG_USB_OHCI_NEW"
#endif

#ifndef CONFIG_USB_STORAGE
#error "must define CONFIG_USB_STORAGE"
#endif

#if !defined(CONFIG_CMD_FAT)
#error "must define CONFIG_CMD_FAT"
#endif


#undef AU_DEBUG

#undef debug
#ifdef	AU_DEBUG
#define debug(fmt,args...)	printf (fmt ,##args)
#else
#define debug(fmt,args...)
#endif	/* AU_DEBUG */

#define TRUE	(0)
#define FALSE	(-1)

#define LOAD_RAM_ADDRESS	(0x20000000)

static void printPartionInfo(int nIndex, PartitionInfo_S *psPartitionInfo) 
{
	printf("[%d]={\n\t.m_acName = \"%s\",\n\t.m_eFlashType = %d,\n"
		"\t.m_sImageInfo = {\n\t\t.m_eImageType = %d,\n\t\t.m_nImageOffset = 0x%x,\n"
		"\t\t.m_nImageSize = 0x%x,\n\t},\n"
	"\t.m_nBeginAddr = 0x%x,\n\t.m_nEndAddr = 0x%x,\n},\n", nIndex,  
	psPartitionInfo->m_acName, psPartitionInfo->m_eFlashType,
	psPartitionInfo->m_sImageInfo.m_eImageType, psPartitionInfo->m_sImageInfo.m_nImageOffset, 
	psPartitionInfo->m_sImageInfo.m_nImageSize, psPartitionInfo->m_nBeginAddr,	 
	psPartitionInfo->m_nEndAddr);
}

static PacketInfo_S *parsePacketInfo(const void *pvBuf, int nLength)
{
	int i;
	int nMagicNum = 0;
	int nCrcValue = 0;
	int nPartNum = 0;
	const char *pvTempBuf = (const char *)pvBuf;
	
	PartitionInfo_S *psPartitionInfo = NULL;
	PacketInfo_S *psPacketInfo = NULL;

	if (pvBuf==NULL||nLength<sizeof(PacketInfo_S)+sizeof(PartitionInfo_S)) {
		printf("error param.\n");
		return NULL;
	}
	
	nMagicNum = be32_to_cpu(*(int *)pvTempBuf);
	if (nMagicNum != 0xAAAA5555) {
		printf("Expect magic number(0x%x) result magic number(0x%x).\n", 0xAA55, nMagicNum);
		return NULL;
	}

	pvTempBuf += 4;
	nCrcValue = be32_to_cpu(*(int *)pvTempBuf); 
	pvTempBuf += 4;
	nPartNum = be32_to_cpu(*(int *)pvTempBuf); 
	if (nPartNum==0) {		
		printf("nPartNum (0x%x).\n", nPartNum);
		return NULL;
	}
	
	psPacketInfo = (PacketInfo_S *)malloc (sizeof(PacketInfo_S)+sizeof(PartitionInfo_S)*nPartNum);
	if (psPacketInfo==NULL) {
		printf("malloc error.\n");
		return NULL;
	}

	psPacketInfo->m_nMagic = nMagicNum;
	psPacketInfo->m_nCrc = nCrcValue;
	psPacketInfo->m_nPartionCount = nPartNum;
	memcpy(psPacketInfo->m_sPartitionInfo, pvBuf+sizeof(PacketInfo_S), sizeof(PartitionInfo_S)*nPartNum);

	psPartitionInfo = psPacketInfo->m_sPartitionInfo;
	for (i=0; i<nPartNum; i++, psPartitionInfo++) {
		psPartitionInfo->m_eFlashType = be32_to_cpu(psPartitionInfo->m_eFlashType);
		psPartitionInfo->m_sImageInfo.m_eImageType = be32_to_cpu(psPartitionInfo->m_sImageInfo.m_eImageType);
		psPartitionInfo->m_sImageInfo.m_nImageOffset = be32_to_cpu(psPartitionInfo->m_sImageInfo.m_nImageOffset);
		psPartitionInfo->m_sImageInfo.m_nImageSize = be32_to_cpu(psPartitionInfo->m_sImageInfo.m_nImageSize);
		psPartitionInfo->m_nBeginAddr = be32_to_cpu(psPartitionInfo->m_nBeginAddr);
		psPartitionInfo->m_nEndAddr = be32_to_cpu(psPartitionInfo->m_nEndAddr);
	}
/*
	psPartitionInfo = psPacketInfo->m_sPartitionInfo;
	for (i=0; i<psPacketInfo->m_nPartionCount; i++) {
		printPartionInfo(i, psPartitionInfo++);		
	}
*/
	return psPacketInfo;			
}

static int current_usb_storage_device = -1;
static block_dev_desc_t *stor_dev = NULL;

static int initUsbDevice(void)
{
	/* start USB */
	if (usb_stop() < 0) {
		debug ("Stop USB  failed\n");
		return FALSE;
	}
	if (usb_init() < 0) {
		debug ("Init USB failed\n");
		return FALSE;
	}

	current_usb_storage_device = usb_stor_scan(0);
	if (current_usb_storage_device == -1) {
		debug ("No USB device found. Not initialized?\n");
		return FALSE;
	}

	stor_dev = get_dev("usb", current_usb_storage_device);
	if (stor_dev == NULL) {
		debug ("usb get uknown device type\n");
		return FALSE;	
	}
	
	return TRUE;
}

static int initMMCDevice(void)
{
	int i = 0;
	
	current_usb_storage_device = get_mmc_num();
	if (current_usb_storage_device < 0) {
		debug ("No MMC device found. Not initialized?\n");
		return FALSE;
	}
	for (i=0; i<current_usb_storage_device; i++) {
		stor_dev = get_dev("mmc", i);
		if (stor_dev != NULL) {	
			return TRUE;
		} else {
			debug ("mmc get uknown device type(%d)\n", i);
		}
	}	
	
	return FALSE;
}


static int initFatFileSystem(void)
{
	if (fat_register_device(stor_dev, 0) != 0) {
		debug ("Unable to use %d:%d for fat \n", current_usb_storage_device, 0);
		if (fat_register_device(stor_dev, 1) != 0) {
			debug ("Unable to use %d:%d for fat \n", current_usb_storage_device, 1);
			return FALSE;
		}
	}

	if (file_fat_detectfs() != 0) {
		debug ("file_fat_detectfs failed\n");
		return FALSE;
	}

	return TRUE;
}

static int loadImageFromUsbToRam(const char *pcFileName, void *pvOutBuffer, int *pnOutLength)
{
	long nReadFileSize = 0;
	int nMaxLength = 0;

	if (pcFileName==NULL || pvOutBuffer == NULL || pnOutLength==NULL) {
		debug("[loadImageFromUsbToRam] pcFileName: %p pvOutBuffer: %p pnOutLength: %p\n", pcFileName, pvOutBuffer, pnOutLength);
		return FALSE;
	}

#ifdef CONFIG_ATMEL_LCD
	lcd_printf("System is reading image from USB storage, please wait");
#endif

	nMaxLength = *pnOutLength;
	memset(pvOutBuffer, 0xFF, nMaxLength);
	nReadFileSize = file_fat_read(pcFileName, pvOutBuffer, nMaxLength);
	if (nReadFileSize < 0) {
		debug("[loadImageFromUsbToRam] pcFileName: %p pvOutBuffer: %p pnOutLength: %p\n", pcFileName, pvOutBuffer, pnOutLength);
		return FALSE;
	}

	debug("[loadImageToRam] pcFileName: %s pvOutBuffer: %d nReadFileSize: %d\n", pcFileName, pvOutBuffer, nReadFileSize);

	*pnOutLength = nReadFileSize;

#ifdef CONFIG_ATMEL_LCD
	lcd_putc('\n');
#endif
	
	return TRUE;
	
}

static int writeImageToSerialFlash(PartitionInfo_S *psPartitionInfo, void *pvBuf, int nBufLength)
{
	int nRet;
	unsigned int nBus = 0;
	unsigned int nCS = 0;
	unsigned int nSpeed = 1000000;
	unsigned int nMode = SPI_MODE_3;
	ImageInfo_S *psImageInfo = &psPartitionInfo->m_sImageInfo;
	
	struct spi_flash *psFlash;
	int nOffset = psPartitionInfo->m_nBeginAddr;
	int nLength = psPartitionInfo->m_nEndAddr-psPartitionInfo->m_nBeginAddr;

	psFlash = spi_flash_probe(nBus, nCS, nSpeed, nMode);
	if (!psFlash) {
		printf("Failed to initialize SPI flash at %u:%u\n", nBus, nCS);
		return FALSE;
	}

	if (nOffset + nLength > psFlash->size) {
		printf("ERROR: attempting past flash size (%#x)\n", psFlash->size);
		return FALSE;
	}

	nRet = spi_flash_erase(psFlash, nOffset&(~(psFlash->sector_size-1)), nLength&(~(psFlash->sector_size-1)));
	if (nRet) {
		printf("SPI flash erase failed\n");
		return FALSE;
	}

	nLength = psImageInfo->m_nImageSize;
	if  (psImageInfo->m_eImageType==EM_IMAGE_TYPE_BOOTSTRAP) {
		*(int *)(pvBuf+psImageInfo->m_nImageOffset+0x14) = nLength;
	}
	
	nRet = spi_flash_write(psFlash, nOffset, nLength, pvBuf+psImageInfo->m_nImageOffset);
	if (nRet) {
		printf("SPI flash write failed\n");
		return FALSE;
	}
	
	spi_flash_free(psFlash);

	return TRUE;
}

static int eraseNandFlash(int nOffsetAddr, int nLength)
{
	int nRet = -1;
		
	nand_info_t *nand = &nand_info[nand_curr_device];
	nand_erase_options_t opts;

	memset(&opts, 0, sizeof(opts));
	opts.offset = nOffsetAddr;
	opts.length = nLength;
	opts.jffs2	= 0;
	opts.quiet	= 1;
	opts.spread = 0;

	nRet = nand_erase_opts(nand, &opts);
	printf("Erase NAND flash 0x%x 0x%x %s\n", nOffsetAddr, nLength, nRet?"ERROR":"OK");
		
	return (nRet?FALSE:TRUE );
}

static int writeNandFlash(char *pcSourceBuffer, int nStartAddr, int nLength, ImageType_E type)
{
	int nRet = -1;
		
	nand_info_t *nand = &nand_info[nand_curr_device];

	if (type==EM_IMAGE_TYPE_BOOTSTRAP) {
		int i = 0;
		char *pcImageBuffer = (char *)malloc(nLength+52*4);
		int *pnImageBuffer = NULL;
		if (pcImageBuffer == NULL) {
			return FALSE;
		}

		pnImageBuffer = (int *)pcImageBuffer;

		for (i=0; i<52; i++) {
			pnImageBuffer[i] = 0xc0c00405;
		}

		memcpy(pcImageBuffer+52*4, pcSourceBuffer, nLength);
		nLength += 52*4;

		nRet = nand_write_skip_bad(nand, nStartAddr, (unsigned int *)&nLength, (u_char *)pcImageBuffer, 0);
	} else {
		nRet = nand_write_skip_bad(nand, nStartAddr, (unsigned int *)&nLength, 
			(u_char *)pcSourceBuffer, (int)(type==EM_IMAGE_TYPE_ROOTFS?WITH_DROP_FFS:0));
	}
	
	return nRet?FALSE:TRUE;
}

static int writeImageToNandFlash(PartitionInfo_S *psPartitionInfo, void *pvBuffer, int nBufferLength)
{
	int nRet = FALSE;

	ImageInfo_S *psImageInfo = &psPartitionInfo->m_sImageInfo;
	int nOffset = psPartitionInfo->m_nBeginAddr;
	int nLength = psPartitionInfo->m_nEndAddr-psPartitionInfo->m_nBeginAddr;

	nRet = eraseNandFlash(nOffset, nLength);
	if (nRet == FALSE) {
		debug("[writeImageToNandFlash] eraseNandFlash(%s, %d, %d) failed.\n", 
					psPartitionInfo->m_acName, psPartitionInfo->m_nBeginAddr, psPartitionInfo->m_nEndAddr);
		return FALSE;
	}

	nRet = writeNandFlash(pvBuffer+psImageInfo->m_nImageOffset, nOffset, psImageInfo->m_nImageSize, psImageInfo->m_eImageType);
	if (nRet == FALSE) {
		debug("[writeImageToNandFlash] writeNandFlash(%s, %d, %d) failed.\n", 
					psPartitionInfo->m_acName, psPartitionInfo->m_nBeginAddr, psPartitionInfo->m_nEndAddr);
		return FALSE;
	}

	return TRUE;
}


static int writeImageFromRamToFlash(PacketInfo_S * psPacketInfo, void *pvBuffer, int nLength)
{
	int i;
	int nRet = FALSE;
	PartitionInfo_S *psPartitionInfo = NULL;

	if (psPacketInfo==NULL||pvBuffer==NULL||nLength==0) {
		debug("[writeImageFromRamToFlash] psPacketInfo: %p pvBuffer: %p nLength: %d\n", psPacketInfo, pvBuffer, nLength);
		return FALSE;
	}
	
#ifdef CONFIG_ATMEL_LCD
	lcd_printf("System is updated image, please wait");
#endif

	psPartitionInfo = psPacketInfo->m_sPartitionInfo;
	for (i=0; i<psPacketInfo->m_nPartionCount; i++, psPartitionInfo++) {
		if (psPartitionInfo->m_eFlashType==EM_FLASH_TYPE_SERIALFLASH) {
			nRet = writeImageToSerialFlash(psPartitionInfo, pvBuffer, nLength);
			if (nRet == FALSE) {
				debug("[writeImageFromRamToFlash] writeImageToSerialFlash(%s, %d, %d) failed.\n", 
					psPartitionInfo->m_acName, psPartitionInfo->m_nBeginAddr, psPartitionInfo->m_nEndAddr);
				return FALSE;
			}
		} else if (psPartitionInfo->m_eFlashType==EM_FLASH_TYPE_NANDFLASH) {
			nRet = writeImageToNandFlash(psPartitionInfo, pvBuffer, nLength);
			if (nRet == FALSE) {
				debug("[writeImageFromRamToFlash] writeImageToNandFlash(%s, %d, %d) failed.\n", 
					psPartitionInfo->m_acName, psPartitionInfo->m_nBeginAddr, psPartitionInfo->m_nEndAddr);
				return FALSE;
			}
		}
#ifdef CONFIG_ATMEL_LCD
		lcd_putc('.');
#endif
	}

#ifdef CONFIG_ATMEL_LCD
	lcd_putc('\n');
#endif

	return TRUE;
}

static int __doUsbUpgrade(int nStorMode) 
{
	int nRet = FALSE;
	int nOldCtrlc = 0;
	int nReadLength = 0;
	PacketInfo_S * psPacketInfo = NULL;
	char acReadFileName[32]={0};
	char *pcDeviceName = NULL;

	unsigned int extension_id = get_extension_chip_id();
	switch (extension_id) {
	case ARCH_EXID_AT91SAM9G25:
		pcDeviceName = "sam9g25ek"; break;
	case ARCH_EXID_AT91SAM9G35:
		pcDeviceName = "sam9g35ek"; break;
	case ARCH_EXID_AT91SAM9X25:
		pcDeviceName = "sam9x25ek"; break;
	case ARCH_EXID_AT91SAM9X35:
		pcDeviceName = "sam9x35ek"; break;
	default:
		printf("Warning: extension_id[0x%X] not support! Take sam9x25ek.dtb as default.", extension_id);
		pcDeviceName = "sam9x25ek";
		break;
	}
	
	memset(acReadFileName, 0, sizeof(acReadFileName));
	sprintf(acReadFileName, "usbupgrad_%s.dt", pcDeviceName);

	if (nStorMode==1) {
		nRet = initMMCDevice();
		if (nRet == FALSE) {
			printf("MMC upgrade initialize MMC failed.\n");
			return FALSE;
		}
	} else {
		nRet = initUsbDevice();
		if (nRet == FALSE) {
			printf("USB upgrade initialize USB failed.\n"); 	
			return FALSE;						
		}
	}

	nRet = initFatFileSystem();
	if (nRet == FALSE) {
		printf("USB upgrade initialize FAT filesystem failed.\n");
		return FALSE;
	}
	
	nOldCtrlc = disable_ctrlc(0);
	nReadLength = 100*1024*1024;
	nRet = loadImageFromUsbToRam(acReadFileName, (void *)LOAD_RAM_ADDRESS, &nReadLength);
	if (nRet == FALSE) {
		printf("[doUsbUpgrade] loadImageFromUsbToRam upgrade usbupgrad.dt  failed.\n");
		return FALSE;
	}

	psPacketInfo = parsePacketInfo((const void *)LOAD_RAM_ADDRESS, nReadLength);
	if (psPacketInfo==NULL) {
		printf("[doUsbUpgrade] parsePacketInfo upgrade usbupgrad.dt  failed.\n");
		return FALSE;
	}

	nRet = writeImageFromRamToFlash(psPacketInfo, (void *)LOAD_RAM_ADDRESS, nReadLength);
	if (nRet == FALSE) {
		printf("[doUsbUpgrade] writeImageFromRamToFlash upgrade usbupgrad.dt (%p) failed.\n", (void *)LOAD_RAM_ADDRESS);
		free(psPacketInfo);
		return FALSE;
	}
	
	free(psPacketInfo);
	disable_ctrlc(nOldCtrlc);
	
	return 0;
}

int doUsbUpgrade(void) 
{
	return __doUsbUpgrade(0);
}

int doMMCUpgrade(void) 
{
	return __doUsbUpgrade(1);
}


int usb_update(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	return doUsbUpgrade();
}

U_BOOT_CMD(usbupgrade, 1, 1, usb_update,
	"usbupgrade - USB update images", "");

int mmc_update(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	return doMMCUpgrade();
}

U_BOOT_CMD(mmcupgrade, 1, 1, mmc_update,
	"mmcupgrade - MMC update images", "");

