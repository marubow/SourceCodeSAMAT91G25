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
	ImageType_E m_eImageType;  //<! ���������bootstrap, uboot��
	int m_nImageOffset;        //<!�����������ļ���ƫ����	
	int m_nImageSize;          //<!����Ĵ�С
}ImageInfo_S;

typedef struct _PartitionInfo {
	char m_acName[16];	      //<! ����������
	FlashType_E m_eFlashType; //<!FLASH������
	ImageInfo_S m_sImageInfo; //<!�������Ϣ
	int m_nBeginAddr;         //<!������FLASH�ϵ���ʼ��ַ
	int m_nEndAddr;           //<!������FLASH�ϵĽ�����ַ
} PartitionInfo_S;

typedef struct _PacketInfo {
	int m_nMagic;                           //<!ħ��
	int m_nCrc;                             //<!32λCRCУ��
	int m_nPartionCount;                    //<!�����ĸ���
	PartitionInfo_S m_sPartitionInfo[0];    //<!������Ϣ���׵�ַ
}PacketInfo_S;

int doUsbUpgrade(void) ;

#endif

