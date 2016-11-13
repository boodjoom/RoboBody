#define HOST_ADRESS			1		//Адрес мастера

#define START_FCN				0x05		//Команда разрешения работы привода
#define STOP_FCN				0x04		//
#define READ_ADC_FCN			0x03		//Команда чтения АЦП датчика привода *
#define WRITE_ANGLE_FCN			0x06		//Команда записи положения привода *
#define READ_ANGLE_FCN			0x07		//*
#define WRITE_MIN_ANG_FCN		0x08		//*
#define READ_MIN_ANG_FCN		0x09		//*
#define WRITE_MAX_ANG_FCN		0x0a		//*
#define READ_MAX_ANG_FCN		0x0b		//*
#define WRITE_MAX_SPEED_FCN		0x0c		//
#define READ_MAX_SPEED_FCN		0x0d		//
#define WRITE_NEW_ADR_FCN   	0x0e
#define WRITE_MAX_REFSPEED_FCN 	0x0f		//Команда записи верхнего ограничения задания скорости
#define READ_MAX_REFSPEED_FCN 	0x10		//Команда чтения верхнего ограничения задания скорости
#define WRITE_MIN_REFSPEED_FCN 	0x11		//Команда записи нижнего ограничения задания скорости
#define READ_MIN_REFSPEED_FCN 	0x12		//Команда чтения нижнего ограничения задания скорости
#define WRITE_REFSPEED_FCN		0x14		//Команда записи задания скорости
#define READ_REFSPEED_FCN		0x15		//Команда чтения задания скорости
#define READ_CURRSPEED_FCN		0x16		//Команда чтения скорости вращения двигателя
#define READ_DIRSETTING_FCN     0x17        //Команда чтения установки уровня для бита направления
#define WRITE_DIRSETTING_FCN    0x18        //Команда записи установки уровня для бита направления


#define WRITE_PTERM         0x21
#define READ_PTERM          0x22
#define WRITE_ITERM         0x23
#define READ_ITERM          0x24
#define WRITE_DTERM         0x25
#define READ_DTERM          0x26

#define READ_FROM_FLASH     0x2A
#define WRITE_TO_FLASH      0x2B

#define	DEV_ADRESS			0
#define FCN					1
#define DATA_H				2
#define DATA_L				3
#define CRC_H				4
#define CRC_L				5
