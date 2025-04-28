#ifndef __RDA5807M_H__
#define __RDA5807M_H__

#ifdef __cplusplus
extern "C" {
#endif
    
/*****************************用户配置区开始***********************************/
#include "main.h"                                                               //包含I2C读写函数声明的头文件

/*****************************用户配置区结束***********************************/

#define RDA5807_WR_CONT_Addr   0x20     //连续写方式的地址
#define RDA5807_RD_CONT_Addr   0x21     //连续读方式的地址
#define RDA5807_Wr_REG_Addr    0x22     //单字节写方式的地址
#define RDA5807_Rd_REG_Addr    0x23     //单字节读方式的地址

//设置寄存器
#define RDA5807_REG02           0x02    //02H寄存器地址
#define RDA5807_REG02_MSK       0xFFFF  //02H寄存器有效位
#define RDA5807_REG02_DHIZ      0x8000  //关闭音频高阻输出
#define RDA5807_REG02_DMUTE     0x4000  //关闭静音
#define RDA5807_REG02_MONO      0x2000  //强制单声道模式
#define RDA5807_REG02_BASS      0x1000  //低音提升
#define RDA5807_REG02_RNCM      0x0800  //RCLK无校准，此时RCLK无需持续输入
#define RDA5807_REG02_RDIM      0x0400  //RCLK时钟直接由外部输入，无需晶振
#define RDA5807_REG02_SEEKUP    0x0200  //向上搜台
#define RDA5807_REG02_SEEK      0x0100  //启动搜台直到一个台被找到或者搜索了整个频段，此时SEEK位零而STC位置一
#define RDA5807_REG02_SKMODE    0x0080  //在频段上下限处停止搜索
#define RDA5807_REG02_CKMD_38M4 0x0070  //38.4MHz时钟
#define RDA5807_REG02_CKMD_19M2 0x0060  //19.2MHz时钟
#define RDA5807_REG02_CKMD_26M  0x0050  //26MHz时钟
#define RDA5807_REG02_CKMD_13M  0x0040  //13MHz时钟
#define RDA5807_REG02_CKMD_24M  0x0030  //24MHz时钟
#define RDA5807_REG02_CKMD_12M  0x0020  //12MHz时钟
#define RDA5807_REG02_CKMD_32K  0x0000  //32768Hz时钟
#define RDA5807_REG02_RDSEN     0x0008  //使能RDS模式
#define RDA5807_REG02_NDME      0x0004  //使用新解调方法
#define RDA5807_REG02_SRST      0x0002  //触发软件复位
#define RDA5807_REG02_ENABLE    0x0001  //上电使能

#define RDA5807_REG03           0x03    //03H寄存器地址
#define RDA5807_REG03_MSK       0xFFFF  //03H寄存器有效位
#define RDA5807_REG03_CHAN_MSK  0xFFC0  //频段寄存器有效位
#define RDA5807_REG03_DIR       0x0020  //直接控制模式，仅在测试时使用
#define RDA5807_REG03_TUNE      0x0010  //启动调谐，完成后自动置零而STC位置一
#define RDA5807_REG03_BAND3     0x000C  //65 –76 MHz （East Europe） or 50-65MHz
#define RDA5807_REG03_BAND2     0x0008  //76–108 MHz (world wide)
#define RDA5807_REG03_BAND1     0x0004  //76–91 MHz (Japan)
#define RDA5807_REG03_BAND0     0x0000  //87–108 MHz (US/Europe)
#define RDA5807_REG03_25K       0x0003  //25KHz
#define RDA5807_REG03_50K       0x0002  //50kHz
#define RDA5807_REG03_200K      0x0001  //200kHz
#define RDA5807_REG03_100K      0x0000  //100kHz

#define RDA5807_REG04           0x04    //04H寄存器地址
#define RDA5807_REG04_MSK       0x0B00  //04H寄存器有效位
#define RDA5807_REG04_DE        0x0800  //去加重，0 = 75µs; 1 = 50µs 
#define RDA5807_REG04_SMEN      0x0200  //使能软静音
#define RDA5807_REG04_AFCD      0x0100  //禁止AFC

#define RDA5807_REG05           0x05    //05H寄存器地址
#define RDA5807_REG05_MSK       0x8F0F  //05H寄存器有效位
#define RDA5807_REG05_INTM      0x8000  //持续拉低直到读取0CH 
#define RDA5807_REG05_SKTH_MSK  0x0F00  //搜索阈值有效位
#define RDA5807_REG05_VOL_MSK   0x000F  //音量有效位

#define RDA5807_REG06           0x06    //06H寄存器地址
#define RDA5807_REG06_OPEN      0x6000  //开启隐藏寄存器

#define RDA5807_REG07           0x07
#define RDA5807_REG07_MSK       0x7E00  //07H寄存器有效位
#define RDA5807_REG07_SBTH_MSK  0x7C00  //噪声软混合阈值有效位
#define RDA5807_REG07_65M       0x0200  //1 = 65~76 MHz; 0 = 50~76 MHz. 

//状态寄存器
#define RDA5807_REG00           0x00    //CHIP ID

#define RDA5807_REG0A           0x0A    //0AH寄存器地址
#define RDA5807_REG0A_MSK       0xFFFF  //0AH寄存器有效位
#define RDA5807_REG0A_RDSR      0x8000  //新的RDS组已准备好
#define RDA5807_REG0A_STC       0x4000  //搜台/调谐已完成
#define RDA5807_REG0A_SF        0x2000  //搜台失败
#define RDA5807_REG0A_RDSS      0x1000  //RDS解码器已同步
#define RDA5807_REG0A_BLKE      0x0800  //找到了Block E，仅使能RDS时有意义
#define RDA5807_REG0A_ST        0x0400  //立体声指示
#define RDA5807_REG0A_CHAN_MSK  0x03FF  //读取频段有效位

#define RDA5807_REG0B           0x0B    //0BH寄存器地址
#define RDA5807_REG0B_MSK       0xFF80  //07H寄存器有效位
#define RDA5807_REG0B_RSSI_MSK  0xFE00  //读取频段有效位
#define RDA5807_REG0B_STA       0x0100  //该频道有效
#define RDA5807_REG0B_RDY       0x0080  //FM就绪

#define RDA5807_REG0C   0x0C
#define RDA5807_REG0D   0x0D
#define RDA5807_REG0E   0x0E
#define RDA5807_REG0F   0x0F



#define READ	        1   
#define WRITE	        0

#define NOTUNE          0       //不调谐
#define TUNE            1       //调谐
#define MONO            0
#define STEREO          1
#define MUTE            0
#define NOTMUTE         1

#define SEEK_UP         1
#define SEEK_DOWN       0

struct RDA5807_t{
uint8_t Cur_Band;                           //当前频段(0-3)
uint32_t Band_Base[4][2];        //频段值

uint8_t Cur_Step;                           //当前步进值(0-3) =3时搜不到台
uint8_t Ch_Spacing[4];        //频率步进值（单位25KHz）

//uint32_t Cur_Frq = 0;                         //当前频率，没用Chan值为方便修改频段和步进值后的转换，需控制范围？？？何处控制？
uint8_t isSearching;                            //正在搜索频道中

uint8_t Cur_Vol;                            //当前音量(0-15)
};//RDA5807参数句柄，需要定时更新或上传

/* 声明给外部使用的变量 */
extern uint16_t Register[6];

/* 声明给外部使用的函数 */
//硬件控制
void RDA5807_Init(void);
void FM_WR_AllReg(void);
void RDA5807_PowerOn(void);
void RDA5807_PowerOff(void);
//声音控制
void RDA5807_SetMute(uint8_t Mute);     //
void RDA5807_SetMono(uint8_t Mono);     //声道控制
void RDA5807_SetVolume(uint8_t level);
uint8_t RDA5807_IncVolume(void);
uint8_t RDA5807_DecVolume(void);
//频率控制
void RDA5807_SetStartFreq(void);
//void RDA5807_SetFreq(uint32_t Frequency);
void RDA5807_SetFreq(uint16_t Frequency);
//uint32_t RDA5807_ChanToFreq(uint16_t Chan,uint8_t Band,uint8_t Step);
//uint32_t RDA5807_RegToFreq(uint16_t RegisterVal);
uint16_t RDA5807_ChanToFreq(uint16_t Chan,uint8_t Band,uint8_t Step);
uint16_t RDA5807_RegToFreq(uint16_t RegisterVal);
uint8_t RDA5807_CheckReg03(uint16_t Reg03);
void RDA5807_Reg03Tune(uint16_t Reg03);
//搜台控制
void RDA5807_SeekThSet(uint8_t RSSI);
void RDA5807_SeekDirection(uint8_t Direction);
void RDA5807_StartSeek(void);
void RDA5807_InterruptSeek(void);
uint16_t RDA5807_SeekChannel(void);
//获取信息
uint16_t RDA5807_GetFreqReg(void);
uint8_t RDA5807_GetSigLvl(void);
uint8_t RDA5807_GetStep(void);
void RDA5807_GetBand(uint8_t* Band);
uint8_t RDA5807_GetVolume(void);
uint8_t RDA5807_CheckSTC(void);

#ifdef __cplusplus
}
#endif

#endif