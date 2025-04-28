/* 包含自定义头文件 */
#include "rda5807m.h"

/* 全局变量 */
struct RDA5807_t RDA5807 = {
    .Cur_Band = 0, 
    .Band_Base = {{87, 108}, {76, 91}, {76, 108}, {65, 76}}, 
    .Cur_Step = 0,
    .Ch_Spacing = {4, 8, 2, 1}, 
    .isSearching = 0, 
    .Cur_Vol = 1
};//RDA5807对象的实例

uint16_t Register[6] = {
    (RDA5807_REG02_DHIZ| RDA5807_REG02_DMUTE| RDA5807_REG02_BASS| RDA5807_REG02_RDIM| RDA5807_REG02_SEEKUP| RDA5807_REG02_SKMODE| RDA5807_REG02_CKMD_32K| RDA5807_REG02_NDME| RDA5807_REG02_ENABLE),
    (RDA5807_REG03_TUNE| RDA5807_REG03_BAND0| RDA5807_REG03_100K),
    0x0040,
    (RDA5807_REG05_INTM| (RDA5807_REG05_SKTH_MSK& 0x0800)| 0x0080| (RDA5807_REG05_VOL_MSK& 0x0000)),
    0x8000,
    0x5F1A
};//初始化，频率预设要把tune打开

/*******************************************************************************
 * 名    称: FM_WR_AllReg
 * 功    能: FM一次性写入寄存器
 * 形    参: 无
 * 返    回: 无
 * 说    明: 
 * 更新日期：2020-1-10
 ******************************************************************************/
void FM_WR_AllReg(void)
{
  I2C_Transmit(RDA5807_WR_CONT_Addr,(uint8_t*)Register,12);
}

/*******************************************************************************
 * 名    称: FM_WR_Reg
 * 功    能: FM写入寄存器
 * 形    参: Register：寄存器地址，Word：内容
 * 返    回: 无
 * 说    明: 
 * 更新日期：2020-1-10
 ******************************************************************************/
void FM_WR_Reg(uint8_t Register,uint16_t Word)
{
  uint8_t Data[3];
  
  Data[0] = Register;
  Data[1] = Word >> 8;
  Data[2] = Word & 0xFF;
  I2C_Transmit(RDA5807_Wr_REG_Addr,Data,3);
}

/*******************************************************************************
 * 名    称: FM_RD_Reg
 * 功    能: FM读取寄存器
 * 形    参: 
 * 返    回: 寄存器值
 * 说    明: 
 * 更新日期：2021-1-5
 ******************************************************************************/
uint16_t FM_RD_Reg(uint8_t Register)
{
    uint16_t Data;
    uint8_t Read_Buff[2];
    Read_Buff[0] = Register;
    I2C_Transmit(RDA5807_Wr_REG_Addr,Read_Buff,1);
    I2C_Receive(RDA5807_Wr_REG_Addr,Read_Buff,2);
    Data = (Read_Buff[0]<<8) + Read_Buff[1];
    return Data;
}

/*******************************************************************************
 * 名    称: RDA5807_WaitForSTC
 * 功    能: 等待STC信号
 * 形    参: 无
 * 返    回: 无
 * 说    明: 无
 * 更新日期：2022-5-9
 ******************************************************************************/
void RDA5807_WaitForSTC(void)
{
  while((FM_RD_Reg(RDA5807_REG0A) & RDA5807_REG0A_STC) == 0 )            //等待STC位置一 表示搜索完成
  {
    vTaskDelay(pdMS_TO_TICKS(10));                             //延时10ms
  }
}

uint8_t RDA5807_CheckSTC(void)
{
    if((FM_RD_Reg(RDA5807_REG0A) & RDA5807_REG0A_STC) == 0 ) return 0;//等待STC位置一 表示搜索完成
    else return 1;
}

/*******************************************************************************
 * 名    称: RDA5807_Init
 * 功    能: RDA5807初始化
 * 形    参: 无
 * 返    回: 无
 * 说    明: 无 
 * 更新日期：2021-1-1
 ******************************************************************************/
void RDA5807_Init(void){ 
    FM_WR_Reg(RDA5807_REG02, RDA5807_REG02_SRST);    //SOFT_RESET
    osDelay(pdMS_TO_TICKS(50));
    FM_WR_Reg(RDA5807_REG02, Register[0]);           //Power Up，Audio Output High-Z高阻:Normal operation;不静音;
    osDelay(pdMS_TO_TICKS(400));
    FM_WR_Reg(RDA5807_REG03, Register[1]);           //初始化频率，
    FM_WR_Reg(RDA5807_REG04, Register[2]);           //Bit[11]:De-emphasis（去加重:解调后对高频分量的压低）：50us；Bit[9]:softmute；Bit[8]:AFC自动频率控制
    FM_WR_Reg(RDA5807_REG05, Register[3]);           //SEEKTH[11:8]:搜台信噪比阈值；VOLUME[3:0]:DAC Gain Control Bits音量控制
    //FM_WR_Reg(RDA5807_REG06, Register[4]);           //OPEN_MODE[14:13]:打开保留寄存器，11:打开保留寄存器读写功能；其他: 只有读功能
    //FM_WR_Reg(RDA5807_REG07, Register[5]);           //TH_SOFRBLEND[14:10]:Threshold for noise soft blend setting；Bit9:65M_50M MODE(频段设置为11时，1=65~76MHz;0=50~76MHz)
    RDA5807_WaitForSTC();                           //等待STC标志
}

/*******************************************************************************
 * 名    称: RDA5807_CheckFreq
 * 功    能: 频率校验
 * 形    参: 无
 * 返    回: 是否正确，0为错误，1为正确
 * 说    明: 无
 * 更新日期：2022-5-6
 ******************************************************************************/
uint8_t RDA5807_CheckFreq(uint16_t Frequency)
{
  if ((Frequency < RDA5807.Band_Base[RDA5807.Cur_Band][0]*40) || (Frequency > RDA5807.Band_Base[RDA5807.Cur_Band][1]*40))
    return 0;
  else
    return 1;
}

/*******************************************************************************
 * 名    称: RDA5807_CheckReg03
 * 功    能: 频率校验
 * 形    参: 无
 * 返    回: 是否正确，0为错误，1为正确
 * 说    明: 无
 * 更新日期：2022-5-6
 ******************************************************************************/
uint8_t RDA5807_CheckReg03(uint16_t Reg03)
{
  uint16_t Chan;
  
  Chan = Reg03>>6;
  if (Chan > (RDA5807.Band_Base[RDA5807.Cur_Band][1]- RDA5807.Band_Base[RDA5807.Cur_Band][0])* 40/ RDA5807.Ch_Spacing[RDA5807.Cur_Step])         //Chan值大于最高频的Chan值
  //if (Chan > (Band_Base[Cur_Band][1]-Band_Base[Cur_Band][0])*1000/Ch_Spacing[Cur_Step])         //Chan值大于最高频的Chan值
    return 0;
  else
    return 1;
}

/*******************************************************************************
 * 名    称: RDA5807_FreqToChan
 * 功    能: 频率转寄存器值
 * 形    参: Frequency：频率值，单位KHz，Tune：是否调谐
 * 返    回: 无
 * 说    明: Chan[15:6]:     频率
             DIRECT MODE[5]: 直接控制模式
             TUNE[4]:        1:调谐开始，调谐结束后，STC位置1，TUNE值自动清0
             BAND[3:2]:      频段
             Ch_Spacing[1:0]:步进值
             频率计算：Ch_Spacing * Chan + 频段起始频率 
 * 更新日期：2022-5-26
 ******************************************************************************/
uint16_t RDA5807_FreqToReg(uint16_t Frequency,uint8_t Tune)
{
  uint16_t temp=0;
  uint16_t Chan;
  
  Chan = (Frequency - RDA5807.Band_Base[RDA5807.Cur_Band][0]* 40) / RDA5807.Ch_Spacing[RDA5807.Cur_Step];
  temp |= (uint16_t)Chan << 6;                    //Chan[15:6]
  if (Tune == TUNE)
    temp |= 1<<4;		                        //tune enable
  else
    temp &= ~(1<<4);		                        //tune disable
  temp |= RDA5807.Cur_Band << 2;
  temp |= RDA5807.Cur_Step;
  return temp;
}

/*******************************************************************************
 * 名    称: RDA5807_ChanToFreq
 * 功    能: 计算频率
 * 形    参: RegisterVal：寄存器值
 * 返    回: 无
 * 说    明: Chan[15:6]:     频率
             DIRECT MODE[5]: 直接控制模式
             TUNE[4]:        1:调谐开始，调谐结束后，STC位置1，TUNE值自动清0
             BAND[3:2]:      频段
             Ch_Spacing[1:0]:步进值
             频率计算：Ch_Spacing * Chan + 频段起始频率 
 * 更新日期：2021-1-2
 ******************************************************************************/
uint16_t RDA5807_RegToFreq(uint16_t RegisterVal) 
{
  uint8_t Band,Step;                            //取值1-4
  uint16_t Chan;

  Chan = (RegisterVal & RDA5807_REG03_CHAN_MSK) >> 6;
  Band = (uint8_t)((RegisterVal & 0x000C) >> 2);
  Step = (uint8_t)(RegisterVal & 0x0003);
  return RDA5807_ChanToFreq(Chan,Band,Step);
}

/*******************************************************************************
 * 名    称: RDA5807_ChanToFreq
 * 功    能: 计算频率
 * 形    参: Chan[9:0],Band[1:0],Step[1:0]：频道，波段，步进值；
 * 返    回: 以25K为单位频率值
 * 说    明: 
 * 更新日期：2021-1-5
 ******************************************************************************/
uint16_t RDA5807_ChanToFreq(uint16_t Chan,uint8_t Band,uint8_t Step) 
{
  return RDA5807.Band_Base[Band][0]* 40+ Chan* RDA5807.Ch_Spacing[Step];
}

/*******************************************************************************
 * 名    称: RDA5807_SetStartFreq
 * 功    能: 设置初始频率
 * 形    参: 无
 * 返    回: 无
 * 说    明: 无
 * 更新日期：2022-5-26
 ******************************************************************************/
void RDA5807_SetStartFreq(void){
    Register[1] &= 0x003F;
}

/*******************************************************************************
 * 名    称: RDA5807_SetFreq
 * 功    能: 频率设置
 * 形    参: 无
 * 返    回: 无
 * 说    明: 无
 * 更新日期：2022-5-7
 ******************************************************************************/
void RDA5807_SetFreq(uint16_t Frequency)
{
  if(RDA5807_CheckFreq(Frequency))                     //检验频率
  {
    Register[1] = RDA5807_FreqToReg(Frequency, TUNE);         //频率转寄存器值
    FM_WR_Reg(RDA5807_REG03, Register[1]);
    //RDA5807_WaitForSTC();
  }
}

/*******************************************************************************
 * 名    称: RDA5807_RegTune
 * 功    能: 根据寄存器进行调谐
 * 形    参: 无
 * 返    回: 无
 * 说    明: 无
 * 更新日期：2022-5-29
 ******************************************************************************/
void RDA5807_Reg03Tune(uint16_t Reg03){
  Reg03 |= RDA5807_REG03_TUNE;		                        //tune enable
  Register[1] = Reg03;
  FM_WR_Reg(RDA5807_REG03,Register[1]);
  RDA5807_WaitForSTC();
}

/*******************************************************************************
 * 名    称: RDA5807_SetMute
 * 功    能: 静音开关
 * 形    参: mute
 * 返    回: 无
 * 说    明: Reg02[14]：0为静音
 * 更新日期：2021-1-2
 ******************************************************************************/
void RDA5807_SetMute(uint8_t Mute){
    if(Mute == MUTE)
        Register[0] &= ~RDA5807_REG02_DMUTE;            //清零
    else
        Register[0] |= RDA5807_REG02_DMUTE;               //置1
    FM_WR_Reg(RDA5807_REG02,Register[0]);
}

/*******************************************************************************
 * 名    称: RDA5807_SetMomo
 * 功    能: 声道控制开关
 * 形    参: Momo
 * 返    回: 无
 * 说    明: Reg02[13]：0为单声道
 * 更新日期：2021-1-2
 ******************************************************************************/
void RDA5807_SetMono(uint8_t Mono)
{
  if(Mono == MONO)
    Register[0] &= ~RDA5807_REG02_MONO;            //清零
  else
    Register[0] |= RDA5807_REG02_MONO;               //置1
  FM_WR_Reg(RDA5807_REG02,Register[0]);  
}

/*******************************************************************************
 * 名    称: RDA5807_SetVolume
 * 功    能: 音量设置
 * 形    参: level：0-15
 * 返    回: 无
 * 说    明: Reg05[3:0]
 * 更新日期：2021-1-2
 ******************************************************************************/
void RDA5807_SetVolume(uint8_t level)
{
  Register[3] = (Register[3] & ~RDA5807_REG05_VOL_MSK) + level;               //置1
  FM_WR_Reg(RDA5807_REG05,Register[3]);
}

/*******************************************************************************
 * 名    称: RDA5807_IncVolume
 * 功    能: 音量增加
 * 形    参: level：0-15
 * 返    回: 无
 * 说    明: Reg05[3:0]
 * 更新日期：2022-5-8
 ******************************************************************************/
uint8_t RDA5807_IncVolume(void)
{
  if (RDA5807.Cur_Vol < 15)
  {
    RDA5807.Cur_Vol++;
    RDA5807_SetVolume(RDA5807.Cur_Vol);
  }
  return RDA5807.Cur_Vol;
}

/*******************************************************************************
 * 名    称: RDA5807_DecVolume
 * 功    能: 音量增加
 * 形    参: level：0-15
 * 返    回: 无
 * 说    明: Reg05[3:0]
 * 更新日期：2022-5-8
 ******************************************************************************/
uint8_t RDA5807_DecVolume(void)
{
  if (RDA5807.Cur_Vol > 0)
  {
    RDA5807.Cur_Vol--;
    RDA5807_SetVolume(RDA5807.Cur_Vol);
  }
  return RDA5807.Cur_Vol;
}

/*******************************************************************************
 * 名    称: RDA5807_GetSigLvl
 * 功    能: 获取信号强弱值
 * 形    参: 无
 * 返    回: 无
* 说    明: 获取RSSI，Reg05[15:9]，为一七位值，最大127
 * 更新日期：2021-1-5
 ******************************************************************************/
uint8_t RDA5807_GetSigLvl(void)
{
  return (uint8_t)(FM_RD_Reg(RDA5807_REG0B)>>9);
}

/*******************************************************************************
 * 名    称: RDA5807_GetStep
 * 功    能: 获取当前频率步进值
 * 形    参: 无
 * 返    回: Step值，单位KHz
 * 说    明: 无
 * 更新日期：2022-5-8
 ******************************************************************************/
uint8_t RDA5807_GetStep(void)
{
  return RDA5807.Ch_Spacing[RDA5807.Cur_Step];
}

/*******************************************************************************
 * 名    称: RDA5807_GetBand
 * 功    能: 获取当前频频段
 * 形    参: 无
 * 返    回: 频段值，单位MHz
 * 说    明: 无
 * 更新日期：2022-5-8
 ******************************************************************************/
void RDA5807_GetBand(uint8_t* Band)
{
  Band[0] = RDA5807.Band_Base[RDA5807.Cur_Band][0];
  Band[1] = RDA5807.Band_Base[RDA5807.Cur_Band][1];
}

/*******************************************************************************
 * 名    称: RDA5807_GetVolume
 * 功    能: 获取当前音量值
 * 形    参: 无
 * 返    回: 无
 * 说    明: 无
 * 更新日期：2022-5-8
 ******************************************************************************/
uint8_t RDA5807_GetVolume(void)
{
  return (uint8_t)(Register[3] & RDA5807_REG05_VOL_MSK);
}

/*******************************************************************************
 * 名    称: RDA5807_GetFreqReg
 * 功    能: 获取频率Reg值
 * 形    参: 无
 * 返    回: Register_03格式的频率值
 * 说    明: 无
 * 更新日期：2022-5-7
 ******************************************************************************/
uint16_t RDA5807_GetFreqReg(void)
{
  uint16_t Chan;
  
  Chan = FM_RD_Reg(RDA5807_REG0A) & RDA5807_REG0A_CHAN_MSK;
  Register[1] = (Register[1] & ~RDA5807_REG03_CHAN_MSK) + (Chan << 6);   //更改现有频率
  return Register[1];
}

/*******************************************************************************
 * 名    称: RDA5807_PowerOff
 * 功    能: 禁用芯片
 * 形    参: 无
 * 返    回: 无
 * 说    明: Reg02[0]
 * 更新日期：2021-1-2
 ******************************************************************************/
void RDA5807_PowerOff(void)
{
    Register[0] &= ~(RDA5807_REG02_ENABLE| RDA5807_REG02_DMUTE| RDA5807_REG02_DHIZ);
    FM_WR_Reg(RDA5807_REG02, Register[0]);
    LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_NOCLOCK, LL_RCC_MCO1_DIV_1);//关闭外部的时钟源以降低功耗
}

/*******************************************************************************
 * 名    称: RDA5807_PowerOn
 * 功    能: 使能芯片
 * 形    参: 无
 * 返    回: 无
 * 说    明: Reg02[0]
 * 更新日期：2021-1-2
 ******************************************************************************/
void RDA5807_PowerOn(void)
{
    LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_LSE, LL_RCC_MCO1_DIV_1);//开启外部的时钟源以降低功耗
    Register[0] |= (RDA5807_REG02_ENABLE| RDA5807_REG02_DMUTE| RDA5807_REG02_DHIZ);
    Register[0] &= ~RDA5807_REG02_SEEK;                       //SEEK位清零
    Register[1] |= RDA5807_REG03_TUNE;                       //Tune位置一
    FM_WR_Reg(RDA5807_REG02, Register[0]);
    FM_WR_Reg(RDA5807_REG03,Register[1]);
}

/*******************************************************************************
 * 名    称: RDA5807_SeekThSet
 * 功    能: 搜台阈值设置
 * 形    参: RSSI：阈值，0-15
 * 返    回: 无
 * 说    明: Reg05[11:8]
 * 更新日期：2021-1-5
 ******************************************************************************/
void RDA5807_SeekThSet(uint8_t RSSI)
{
  uint16_t Temp;
  Temp = RSSI << 8;
  Register[3] = (Register[3] & 0xF0FF) | Temp;
  FM_WR_Reg(RDA5807_REG05,Register[3]);
}

/*******************************************************************************
 * 名    称: RDA5807_SeekDirection
 * 功    能: 搜台方向设置
 * 形    参: RSSI：阈值，0-15
 * 返    回: 无
 * 说    明: Reg02[9]
 * 更新日期：2022-6-3
 ******************************************************************************/
void RDA5807_SeekDirection(uint8_t Direction)
{
  if (Direction == SEEK_DOWN) Register[0] &= ~RDA5807_REG02_SEEKUP;
  else Register[0] |= RDA5807_REG02_SEEKUP;
  FM_WR_Reg(RDA5807_REG02,Register[0]);
}

void RDA5807_StartSeek(void){
    if(RDA5807_CheckSTC()){
        Register[0] |= RDA5807_REG02_SEEK;                        //SEEK位置一，使能自动搜台
        Register[1] &= ~RDA5807_REG03_TUNE;                       //Tune位清零
        FM_WR_Reg(RDA5807_REG02,Register[0]);
        FM_WR_Reg(RDA5807_REG03,Register[1]);
    }
}

void RDA5807_InterruptSeek(void){
    Register[0] &= ~RDA5807_REG02_SEEK;                       //SEEK位清零
    Register[1] |= RDA5807_REG03_TUNE;                       //Tune位置一
    FM_WR_Reg(RDA5807_REG02,Register[0]);
    FM_WR_Reg(RDA5807_REG03,Register[1]);
}

/*******************************************************************************
 * 名    称: RDA5807_SeekChannel
 * 功    能: 搜台
 * 形    参: 无
 * 返    回: 返回搜到电台（以RDA5807_REG03格式体现？）
 * 说    明: 
 * 更新日期：2022-5-26
 ******************************************************************************/
uint16_t RDA5807_SeekChannel(void){
  Register[0] |= RDA5807_REG02_SEEK;                        //SEEK位置一，使能自动搜台
  Register[1] &= ~RDA5807_REG03_TUNE;                       //Tune位清零
  FM_WR_Reg(RDA5807_REG02,Register[0]);
  FM_WR_Reg(RDA5807_REG03,Register[1]);
  RDA5807_WaitForSTC();                         //等待STC标志
  if((FM_RD_Reg(RDA5807_REG0B) & RDA5807_REG0B_STA) == 0)         //核实是否搜到台
    return 0;
  else
    return RDA5807_GetFreqReg();
}


