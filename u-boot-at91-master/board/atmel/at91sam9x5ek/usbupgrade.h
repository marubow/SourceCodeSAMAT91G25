#ifndef __USBUPGRAD_H__
#define __USBUPGRAD_H__

typedef enum _FlashType {
	EM_FLASH_TYPE_DATAFLASH,
	EM_FLASH_TYPE_SERIALFLASH,
	EM_FLASH_TYPE_NANDFLASH,
	EM_FLASH_TYPE_NORFLASH,
	EM_FLASH_TYPE_INVALID
}FlashType_E;

typedef enum _ImageType {
	EM_IMAGE_TYPE_BOOTSTRAP,
	EM_IMAGE_TYPE_ENV,
	EM_IMAGE_TYPE_UBOOT,
	EM_IMAGE_TYPE_DTB,
	EM_IMAGE_TYPE_DATA,
	EM_IMAGE_TYPE_LOGO,
	EM_IMAGE_TYPE_KERNEL,
	EM_IMAGE_TYPE_ROOTFS,
	EM_IMAGE_TYPE_INVALID
}ImageType_E;

typedef struct _ImageInfo {
	ImageType_E m_eImageType;  //<! 镜像的类型bootstrap, uboot等
	int m_nImageOffset;        //<!镜像在生成文件中偏移量	
	int m_nImageSize;          //<!镜像的大小
}ImageInfo_S;

typedef struct _PartitionInfo {
	char m_acName[16];	      //<! 分区的名字
	FlashType_E m_eFlashType; //<!FLASH的类型
	ImageInfo_S m_sImageInfo; //<!镜像的信息
	int m_nBeginAddr;         //<!镜像在FLASH上的起始地址
	int m_nEndAddr;           //<!镜像在FLASH上的结束地址
} PartitionInfo_S;

typedef struct _PacketInfo {
	int m_nMagic;                           //<!魔数
	int m_nCrc;                             //<!32位CRC校验
	int m_nPartionCount;                    //<!分区的个数
	PartitionInfo_S m_sPartitionInfo[0];    //<!分区信息的首地址
}PacketInfo_S;

int doUsbUpgrade(void) ;

#endif

