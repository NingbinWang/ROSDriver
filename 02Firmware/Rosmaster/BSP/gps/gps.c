/**
  ******************************************************************************
  * @file    gps.c
  * @author  alex
  * @version V1.0
  * @date    2016-07-xx
  * @brief   gps模块接口配置驱动
*/

#include "gpsconf.h"
#include "nmea/nmea.h"
#include "gps.h"


#if (GPS_ENBALE)


GPS_DMA_T  GPS_USART_DMA;
/**
  * @brief  GPS_SendDate 获取
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
  */
void GPS_SendDate(uint8_t *buffer, uint16_t length)
{
	HAL_StatusTypeDef ret;
    //发送数据
    ret = HAL_UART_Transmit(&GPS_USART, buffer, length,0);
    if(ret != HAL_OK)
    {
    	printf("GPS_SendDate err ret = %d\r\n",ret);
    }
}

void GPS_config(void)
{
	HAL_UART_RegisterRxEventCallback();
	HAL_UART_Receive_DMA(&GPS_USART, GPS_USART_DMA.recv_buf, GPS_RBUFF_SIZE);
}

void GPS_ProcessDMAIRQCallback(void)
{
	double deg_lat;//转换成[degree].[degree]格式的纬度
	double deg_lon;//转换成[degree].[degree]格式的经度

	nmeaINFO info;          //GPS解码后得到的信息
	nmeaPARSER parser;      //解码时使用的数据结构
	uint8_t new_parse=0;    //是否有新的解码数据标志

	nmeaTIME beiJingTime;    //北京时间

	    /* 设置用于输出调试信息的函数 */
	    nmea_property()->trace_func = &trace;
	    nmea_property()->error_func = &error;
	    nmea_property()->info_func = &gps_info;

}



/**
  * @brief  trace 在解码时输出捕获的GPS语句
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
  */
void trace(const char *str, int str_size)
{
  #ifdef __GPS_DEBUG    //在gps_config.h文件配置这个宏，是否输出调试信息
    uint16_t i;
    printf("\r\nTrace: ");
    for(i=0;i<str_size;i++)
      printf("%c",*(str+i));

    printf("\n");
  #endif
}

/**
  * @brief  error 在解码出错时输出提示消息
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
  */
void error(const char *str, int str_size)
{
    #ifdef __GPS_DEBUG   //在gps_config.h文件配置这个宏，是否输出调试信息

    uint16_t i;
    printf("\r\nError: ");
    for(i=0;i<str_size;i++)
      printf("%c",*(str+i));
    printf("\n");
    #endif
}

/**
  * @brief  error 在解码出错时输出提示消息
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
  */
void gps_info(const char *str, int str_size)
{

    uint16_t i;
    printf("\r\nInfo: ");
    for(i=0;i<str_size;i++)
      printf("%c",*(str+i));
    printf("\n");
}



/********************************************************************************************************
**     函数名称:            bit        IsLeapYear(uint8_t    iYear)
**    功能描述:            判断闰年(仅针对于2000以后的年份)
**    入口参数：            iYear    两位年数
**    出口参数:            uint8_t        1:为闰年    0:为平年
********************************************************************************************************/
static uint8_t IsLeapYear(uint8_t iYear)
{
    uint16_t    Year;
    Year    =    2000+iYear;
    if((Year&3)==0)
    {
        return ((Year%400==0) || (Year%100!=0));
    }
     return 0;
}

/********************************************************************************************************
**    函数名称:            void    GMTconvert(uint8_t *DT,uint8_t GMT,uint8_t AREA)
**    功能描述:            格林尼治时间换算世界各时区时间
**    入口参数：            *DT:    表示日期时间的数组 格式 YY,MM,DD,HH,MM,SS
**                        GMT:    时区数
**                        AREA:    1(+)东区 W0(-)西区
********************************************************************************************************/
void    GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA)
{
    uint32_t    YY,MM,DD,hh,mm,ss;        //年月日时分秒暂存变量

    if(GMT==0)    return;                //如果处于0时区直接返回
    if(GMT>12)    return;                //时区最大为12 超过则返回

    YY    =    SourceTime->year;                //获取年
    MM    =    SourceTime->mon;                 //获取月
    DD    =    SourceTime->day;                 //获取日
    hh    =    SourceTime->hour;                //获取时
    mm    =    SourceTime->min;                 //获取分
    ss    =    SourceTime->sec;                 //获取秒

    if(AREA)                        //东(+)时区处理
    {
        if(hh+GMT<24)    hh    +=    GMT;//如果与格林尼治时间处于同一天则仅加小时即可
        else                        //如果已经晚于格林尼治时间1天则进行日期处理
        {
            hh    =    hh+GMT-24;        //先得出时间
            if(MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10)    //大月份(12月单独处理)
            {
                if(DD<31)    DD++;
                else
                {
                    DD    =    1;
                    MM    ++;
                }
            }
            else if(MM==4 || MM==6 || MM==9 || MM==11)                //小月份2月单独处理)
            {
                if(DD<30)    DD++;
                else
                {
                    DD    =    1;
                    MM    ++;
                }
            }
            else if(MM==2)    //处理2月份
            {
                if((DD==29) || (DD==28 && IsLeapYear(YY)==0))        //本来是闰年且是2月29日 或者不是闰年且是2月28日
                {
                    DD    =    1;
                    MM    ++;
                }
                else    DD++;
            }
            else if(MM==12)    //处理12月份
            {
                if(DD<31)    DD++;
                else        //跨年最后一天
                {
                    DD    =    1;
                    MM    =    1;
                    YY    ++;
                }
            }
        }
    }
    else
    {
        if(hh>=GMT)    hh    -=    GMT;    //如果与格林尼治时间处于同一天则仅减小时即可
        else                        //如果已经早于格林尼治时间1天则进行日期处理
        {
            hh    =    hh+24-GMT;        //先得出时间
            if(MM==2 || MM==4 || MM==6 || MM==8 || MM==9 || MM==11)    //上月是大月份(1月单独处理)
            {
                if(DD>1)    DD--;
                else
                {
                    DD    =    31;
                    MM    --;
                }
            }
            else if(MM==5 || MM==7 || MM==10 || MM==12)                //上月是小月份2月单独处理)
            {
                if(DD>1)    DD--;
                else
                {
                    DD    =    30;
                    MM    --;
                }
            }
            else if(MM==3)    //处理上个月是2月份
            {
                if((DD==1) && IsLeapYear(YY)==0)                    //不是闰年
                {
                    DD    =    28;
                    MM    --;
                }
                else    DD--;
            }
            else if(MM==1)    //处理1月份
            {
                if(DD>1)    DD--;
                else        //新年第一天
                {
                    DD    =    31;
                    MM    =    12;
                    YY    --;
                }
            }
        }
    }

    ConvertTime->year   =    YY;                //更新年
    ConvertTime->mon    =    MM;                //更新月
    ConvertTime->day    =    DD;                //更新日
    ConvertTime->hour   =    hh;                //更新时
    ConvertTime->min    =    mm;                //更新分
    ConvertTime->sec    =    ss;                //更新秒
}

/**
  * @brief  nmea_decode_test 解码GPS模块信息
  * @param  无
  * @retval 无
  */
int nmea_decode_test(void)
{
	double deg_lat;//转换成[degree].[degree]格式的纬度
	double deg_lon;//转换成[degree].[degree]格式的经度

    nmeaINFO info;          //GPS解码后得到的信息
    nmeaPARSER parser;      //解码时使用的数据结构
    uint8_t new_parse=0;    //是否有新的解码数据标志

    nmeaTIME beiJingTime;    //北京时间

    /* 设置用于输出调试信息的函数 */
    nmea_property()->trace_func = &trace;
    nmea_property()->error_func = &error;
    nmea_property()->info_func = &gps_info;

    /* 初始化GPS数据结构 */
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);

    while(1)
    {
      if(GPS_HalfTransferEnd)     /* 接收到GPS_RBUFF_SIZE一半的数据 */
      {
        /* 进行nmea格式解码 */
        nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);

        GPS_HalfTransferEnd = 0;   //清空标志位
        new_parse = 1;             //设置解码消息标志
      }
      else if(GPS_TransferEnd)    /* 接收到另一半数据 */
      {

        nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);

        GPS_TransferEnd = 0;
        new_parse =1;
      }

      if(new_parse )                //有新的解码消息
      {
        /* 对解码后的时间进行转换，转换成北京时间 */
        GMTconvert(&info.utc,&beiJingTime,8,1);

        /* 输出解码得到的信息 */
	   printf("\r\n时间%d-%02d-%02d,%d:%d:%d\r\n", beiJingTime.year+1900, beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);

		//info.lat lon中的格式为[degree][min].[sec/60]，使用以下函数转换成[degree].[degree]格式
	    deg_lat = nmea_ndeg2degree(info.lat);
	    deg_lon = nmea_ndeg2degree(info.lon);
	    printf("\r\n纬度：%f,经度%f\r\n",deg_lat,deg_lon);
        printf("\r\n海拔高度：%f 米 ", info.elv);
        printf("\r\n速度：%f km/h ", info.speed);
        printf("\r\n航向：%f 度", info.direction);

		printf("\r\n正在使用的GPS卫星：%d,可见GPS卫星：%d",info.satinfo.inuse,info.satinfo.inview);

		printf("\r\n正在使用的北斗卫星：%d,可见北斗卫星：%d",info.BDsatinfo.inuse,info.BDsatinfo.inview);
	    printf("\r\nPDOP：%f,HDOP：%f，VDOP：%f",info.PDOP,info.HDOP,info.VDOP);

        new_parse = 0;
      }

	}

    /* 释放GPS数据结构 */
    // nmea_parser_destroy(&parser);


    //  return 0;
}




#endif
