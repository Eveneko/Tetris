//jpeg解码程序

//目前仅支持JFIF格式的JPEG/JPG
//参考网上代码(创新论坛：超级大苹果)

//占用SRAM : 14060 Bytes  
//正点原子@HYW
//2009.03.21
//V1.0
#include "mygirl.h"
#include "mygirl1.h"	  
#include "mygirl2.h"
#include "mygirl3.h"
#include "mine.h"
#include "stm32f1xx_hal.h"
//#include "mygirl4.h"
//#include "mygirl5.h"

#define M_SOF0  0xc0
#define M_DHT   0xc4
#define M_EOI   0xd9
#define M_SOS   0xda
#define M_DQT   0xdb
#define M_DRI   0xdd
#define M_APP0  0xe0
//开始显示的坐标值	  
long   CurX=00;
long   CurY=00;

static int Zig_Zag[8][8]={{0,1,5,6,14,15,27,28},
					  	  {2,4,7,13,16,26,29,42},
						  {3,8,12,17,25,30,41,43},
						  {9,11,18,24,37,40,44,53},
						  {10,19,23,32,39,45,52,54},
						  {20,22,33,38,46,51,55,60},
						  {21,34,37,47,50,56,59,61},
						  {35,36,48,49,57,58,62,63}
						 };

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */


//*************************************************************************************
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
					   
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;	  

typedef int HFILE;
typedef CHAR *LPSTR, *PSTR;

#define FALSE 0
#define TRUE 1  

/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L    

typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;
typedef RGBQUAD * LPRGBQUAD;


#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
	   
//宏定义					   
#define WIDTHBYTES(i)    ((i+31)/32*4)//??????????
#define PI 3.1415926535
//函数返回值定义
#define FUNC_OK 0
#define FUNC_MEMORY_ERROR 1
#define FUNC_FILE_ERROR 2
#define FUNC_FORMAT_ERROR 3


//////////////////////////////////////////////////
//JPEG 解码函数
BOOL LoadJpegFile(const unsigned char *BmpFileName);
void showerror(int funcret);
int  InitTag(void);
void InitTable(void);
int  Decode(void);
int  DecodeMCUBlock(void);
int  HufBlock(BYTE dchufindex,BYTE achufindex);
int  DecodeElement(void);
void IQtIZzMCUComponent(short flag);
void IQtIZzBlock(short  *s ,int * d,short flag);
void GetYUV(short flag);
void StoreBuffer(void);

BYTE ReadByte(void);
void Initialize_Fast_IDCT(void);
void Fast_IDCT(int * block);
void idctrow(int * blk);
void idctcol(int * blk);
//////////////////////////////////////////////////
//全局变量声明			    
DWORD              NumColors;
DWORD              LineBytes;
DWORD              ImgWidth=0 , ImgHeight=0;
char*             lpPtr;
//////////////////////////////////////////////////
//在JPEG函数里面用到的变量
short			SampRate_Y_H,SampRate_Y_V;
short			SampRate_U_H,SampRate_U_V;
short			SampRate_V_H,SampRate_V_V;
short			H_YtoU,V_YtoU,H_YtoV,V_YtoV;
short			Y_in_MCU,U_in_MCU,V_in_MCU;
unsigned char   *lpJpegBuf;
unsigned char   *lp;
short			qt_table[3][64];
short			comp_num;
BYTE			comp_index[3];
BYTE		    YDcIndex,YAcIndex,UVDcIndex,UVAcIndex;
BYTE			HufTabIndex;
short		    *YQtTable,*UQtTable,*VQtTable;
BYTE			And[9]={0,1,3,7,0xf,0x1f,0x3f,0x7f,0xff};
short		    code_pos_table[4][16],code_len_table[4][16];
unsigned short	code_value_table[4][256];
unsigned short	huf_max_value[4][16],huf_min_value[4][16];
short			BitPos,CurByte;
short			rrun,vvalue;
short			MCUBuffer[10*64];
int				QtZzMCUBuffer[10*64];
short			BlockBuffer[64];
short			ycoef,ucoef,vcoef;
BOOL			IntervalFlag;
short			interval=0;
int				Y[4*64],U[4*64],V[4*64];
DWORD		    sizei,sizej;
short 			restart;
static  long	iclip[1024];
static  long	*iclp;
	  
//读取JPEG代码，并解码
BOOL LoadJpegFile (const unsigned char *JpegFileName)
{								  
    DWORD			    JpegBufSize; 
	const unsigned char *hJpegBuf;
	int				    funcret;
 
 	hJpegBuf=JpegFileName;//JPEG文件头
	lpJpegBuf=(unsigned char*)hJpegBuf;//转换为变量指针  
	InitTable();  
	if((funcret=InitTag())!=FUNC_OK)//初始化表头不成功 
	{					 
		showerror(funcret);//错误信息显示
		return FALSE;
	}    
	if((SampRate_Y_H==0)||(SampRate_Y_V==0))
	{	    
		return FALSE ;
	}	   
	funcret=Decode();
	if(funcret==FUNC_OK)//解码成功
	{   
		return TRUE;
	}
	else
	{ 
		return FALSE;
	}
}
//错误提示函数
void showerror(int funcret)
{
	switch(funcret)
	{
		case FUNC_MEMORY_ERROR:
	 //   	printf("Error alloc memory\n!");
			break;
		case FUNC_FILE_ERROR:
	 //   	printf("File not found!\n");
			break;
		case FUNC_FORMAT_ERROR:
	//		printf("File format error!\n");
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////
int InitTag()
{
	BOOL finish=FALSE;
	BYTE id;
	short  llength;
	short  i,j,k;
	short  huftab1,huftab2;
	short  huftabindex;
	BYTE hf_table_index;
	BYTE qt_table_index;
	BYTE comnum;

	unsigned char  *lptemp;
	short  colorount;

	lp=lpJpegBuf+2;//跳过两个字节SOI(0xFF，0xD8 Start of Image) 

	while (!finish)
	{
		id=*(lp+1);//取出低位字节(高位在前，低位在后) 
		lp+=2;	   //跳过取出的字节
		switch (id)
		{
			case M_APP0: //JFIF APP0 segment marker (0xE0) 
				llength=MAKEWORD(*(lp+1),*lp);
				lp+=llength;
				break;
			case M_DQT: //定义量化表(0xFF,0xDB) 
				llength=MAKEWORD(*(lp+1),*lp);//(量化表长度) 
				qt_table_index=(*(lp+2))&0x0f;//量化表信息bit 0..3: QT 号(0..3, 否则错误) 
											  //bit 4..7: QT 精度, 0 = 8 bit, 否则 16 bit
				lptemp=lp+3;  				  //n 字节的 QT, n = 64*(精度+1) 
				if(llength<80)				  //精度为 8 bit 
				{
					for(i=0;i<64;i++)
						qt_table[qt_table_index][i]=(short)*(lptemp++);
				}
				else						  //精度为 16 bit 
				{
					for(i=0;i<64;i++)qt_table[qt_table_index][i]=(short)*(lptemp++);   
	                qt_table_index=(*(lptemp++))&0x0f;
	  				for(i=0;i<64;i++)qt_table[qt_table_index][i]=(short)*(lptemp++);	    
	  			}
	  			lp+=llength;                  		//跳过量化表 		
				break;
			case M_SOF0:					        //帧开始 (baseline JPEG 0xFF,0xC0)
		 		llength=MAKEWORD(*(lp+1),*lp);		//长度 (高字节, 低字节), 8+components*3 
		 		ImgHeight=MAKEWORD(*(lp+4),*(lp+3));//图片高度 (高字节, 低字节), 如果不支持 DNL 就必须 >0 
		 		ImgWidth=MAKEWORD(*(lp+6),*(lp+5));	//图片宽度 (高字节, 低字节), 如果不支持 DNL 就必须 >0 
	            comp_num=*(lp+7);//components 数量(1 byte), 灰度图是 1, YCbCr/YIQ 彩色图是 3, CMYK 彩色图是 4 
			    if((comp_num!=1)&&(comp_num!=3))return FUNC_FORMAT_ERROR;	 
				if(comp_num==3)						//YCbCr/YIQ 彩色图
				{
					comp_index[0]=*(lp+8);	    //component id (1 = Y, 2 = Cb, 3 = Cr, 4 = I, 5 = Q) 
		  			SampRate_Y_H=(*(lp+9))>>4;	//水平采样系数 
		  			SampRate_Y_V=(*(lp+9))&0x0f;//垂直采样系数 
		  			YQtTable=(short *)qt_table[*(lp+10)];//通过量化表号取得量化表地址 
	
					comp_index[1]=*(lp+11);				 //component id 
					SampRate_U_H=(*(lp+12))>>4;			 //水平采样系数
		  			SampRate_U_V=(*(lp+12))&0x0f;		 //垂直采样系数 
		  			UQtTable=(short *)qt_table[*(lp+13)];//通过量化表号取得量化表地址
	
		  			comp_index[2]=*(lp+14);				 //component id 
		  			SampRate_V_H=(*(lp+15))>>4;			 //水平采样系数
		  			SampRate_V_V=(*(lp+15))&0x0f;		 //垂直采样系数 
					VQtTable=(short *)qt_table[*(lp+16)];//通过量化表号取得量化表地址
		  		}
				else								     //component id 
				{
		  			comp_index[0]=*(lp+8);
					SampRate_Y_H=(*(lp+9))>>4;
		  			SampRate_Y_V=(*(lp+9))&0x0f;
		  			YQtTable=(short *)qt_table[*(lp+10)];//灰度图的量化表都一样
	
					comp_index[1]=*(lp+8);
		  			SampRate_U_H=1;
		  			SampRate_U_V=1;
		  			UQtTable=(short *)qt_table[*(lp+10)];
	
					comp_index[2]=*(lp+8);
					SampRate_V_H=1;
		  			SampRate_V_V=1;
		  			VQtTable=(short *)qt_table[*(lp+10)];
				}
	  			lp+=llength;						    
				break;
			case M_DHT: //定义 Huffman Table(0xFF,0xC4)             
				llength=MAKEWORD(*(lp+1),*lp);//长度 (高字节, 低字节)
				if (llength<0xd0)			  // Huffman Table信息 (1 byte) 
				{
					huftab1=(short)(*(lp+2))>>4;     //huftab1=0,1(HT 类型,0 = DC 1 = AC)
			 		huftab2=(short)(*(lp+2))&0x0f;   //huftab2=0,1(HT 号  ,0 = Y  1 = UV)
					huftabindex=huftab1*2+huftab2;	 //0 = YDC 1 = UVDC 2 = YAC 3 = UVAC 
			 		lptemp=lp+3;
					for (i=0; i<16; i++)             //16 bytes: 长度是 1..16 代码的符号数
						code_len_table[huftabindex][i]=(short)(*(lptemp++));//码长为i的码字个数   
					j=0;
					for (i=0; i<16; i++)			 //得出HT的所有码字的对应值 
					{
						if(code_len_table[huftabindex][i]!=0)
						{
							k=0;
							while(k<code_len_table[huftabindex][i])
							{
								code_value_table[huftabindex][k+j]=(short)(*(lptemp++));
								k++;
							}
							j+=k;	
						}
					}
					i=0;
					while (code_len_table[huftabindex][i]==0)i++;	   
					for (j=0;j<i;j++)
					{
						huf_min_value[huftabindex][j]=0;
						huf_max_value[huftabindex][j]=0;
					}
					huf_min_value[huftabindex][i]=0;
					huf_max_value[huftabindex][i]=code_len_table[huftabindex][i]-1;
					for (j=i+1;j<16;j++)
					{
						huf_min_value[huftabindex][j]=(huf_max_value[huftabindex][j-1]+1)<<1;
						huf_max_value[huftabindex][j]=huf_min_value[huftabindex][j]+code_len_table[huftabindex][j]-1;
					}
					code_pos_table[huftabindex][0]=0;
					for (j=1;j<16;j++)
			  			code_pos_table[huftabindex][j]=code_len_table[huftabindex][j-1]+code_pos_table[huftabindex][j-1];
			  		lp+=llength;
				}  //if
				else
				{
		 			hf_table_index=*(lp+2);
					lp+=2;
					while (hf_table_index!=0xff)
					{
						huftab1=(short)hf_table_index>>4;     //huftab1=0,1
				 		huftab2=(short)hf_table_index&0x0f;   //huftab2=0,1
						huftabindex=huftab1*2+huftab2;
						lptemp=lp+1;
						colorount=0;
						for (i=0; i<16; i++)
						{
							code_len_table[huftabindex][i]=(short)(*(lptemp++));
							colorount+=code_len_table[huftabindex][i];
						}
						colorount+=17;	
						j=0;
						for (i=0; i<16; i++)
						{
							if(code_len_table[huftabindex][i]!=0) 
							{
								k=0;
								while(k<code_len_table[huftabindex][i])
								{
									code_value_table[huftabindex][k+j]=(short)(*(lptemp++));
									k++;
								}
								j+=k;
							}
						}
						i=0;
						while (code_len_table[huftabindex][i]==0)i++;
						for (j=0;j<i;j++)
						{
							huf_min_value[huftabindex][j]=0;
							huf_max_value[huftabindex][j]=0;
						}
						huf_min_value[huftabindex][i]=0;
						huf_max_value[huftabindex][i]=code_len_table[huftabindex][i]-1;
						for (j=i+1;j<16;j++)
						{
							huf_min_value[huftabindex][j]=(huf_max_value[huftabindex][j-1]+1)<<1;
							huf_max_value[huftabindex][j]=huf_min_value[huftabindex][j]+code_len_table[huftabindex][j]-1;
						}
						code_pos_table[huftabindex][0]=0;
						for (j=1;j<16;j++)
							code_pos_table[huftabindex][j]=code_len_table[huftabindex][j-1]+code_pos_table[huftabindex][j-1];
						lp+=colorount;
						hf_table_index=*lp;
					}  //while
				}  //else
				break;
			case M_DRI:
				llength=MAKEWORD(*(lp+1),*lp);
				restart=MAKEWORD(*(lp+3),*(lp+2));
				lp+=llength;
				break;
			case M_SOS:
				llength=MAKEWORD(*(lp+1),*lp);
				comnum=*(lp+2);
				if(comnum!=comp_num)
					return FUNC_FORMAT_ERROR;
				lptemp=lp+3;
				for (i=0;i<comp_num;i++)//每组件的信息 
				{
					if(*lptemp==comp_index[0])
					{
						YDcIndex=(*(lptemp+1))>>4;   //Y 使用的 Huffman 表 
						YAcIndex=((*(lptemp+1))&0x0f)+2;
					}
					else
					{
						UVDcIndex=(*(lptemp+1))>>4;   //U,V
						UVAcIndex=((*(lptemp+1))&0x0f)+2;
					}
					lptemp+=2;
				}
				lp+=llength;
				finish=TRUE;
				break;
			case M_EOI://图片结束     
				return FUNC_FORMAT_ERROR;
				break;
			default:
	 			if ((id&0xf0)!=0xd0)
				{
					llength=MAKEWORD(*(lp+1),*lp);
		 			lp+=llength;
				}
				else lp+=2;
				break;
  		}  //switch
	} //while
	return FUNC_OK;
}
//初始化量化表，全部清零
void InitTable()
{
	short i,j;
	sizei=sizej=0;
	ImgWidth=ImgHeight=0;
	rrun=vvalue=0;
	BitPos=0;
	CurByte=0;
	IntervalFlag=FALSE;
	restart=0;
	for(i=0;i<3;i++) //量化表
		for(j=0;j<64;j++)
			qt_table[i][j]=0;
	comp_num=0;
	HufTabIndex=0;
	for(i=0;i<3;i++)
		comp_index[i]=0;
	for(i=0;i<4;i++)
		for(j=0;j<16;j++){
			code_len_table[i][j]=0;
			code_pos_table[i][j]=0;
			huf_max_value[i][j]=0;
			huf_min_value[i][j]=0;
		}
	for(i=0;i<4;i++)
		for(j=0;j<256;j++)
			code_value_table[i][j]=0;
	
	for(i=0;i<10*64;i++){
		MCUBuffer[i]=0;
		QtZzMCUBuffer[i]=0;
	}
	for(i=0;i<64;i++){
		Y[i]=0;
		U[i]=0;
		V[i]=0;
		BlockBuffer[i]=0;
	}
	ycoef=ucoef=vcoef=0;
}
//调用顺序: Initialize_Fast_IDCT() :初始化 
//          DecodeMCUBlock()       Huffman Decode 
//          IQtIZzMCUComponent()   反量化、反DCT 
//          GetYUV()               Get Y U V 
//          StoreBuffer()          YUV to RGB 
int Decode()
{
	int funcret;

	Y_in_MCU=SampRate_Y_H*SampRate_Y_V;//YDU YDU YDU YDU
	U_in_MCU=SampRate_U_H*SampRate_U_V;//cRDU 
	V_in_MCU=SampRate_V_H*SampRate_V_V;//cBDU
	H_YtoU=SampRate_Y_H/SampRate_U_H;
	V_YtoU=SampRate_Y_V/SampRate_U_V;
	H_YtoV=SampRate_Y_H/SampRate_V_H;
	V_YtoV=SampRate_Y_V/SampRate_V_V;
	Initialize_Fast_IDCT();
	while((funcret=DecodeMCUBlock())==FUNC_OK) //After Call DecodeMCUBUBlock()
	{
		interval++;						      //The Digital has been Huffman Decoded and 
		if((restart)&&(interval % restart==0))//be stored in MCUBuffer(YDU,YDU,YDU,YDU		        
			 IntervalFlag=TRUE;				  // UDU,VDU) Every DU := 8*8 
		else
			IntervalFlag=FALSE;
		IQtIZzMCUComponent(0); //反量化 and IDCT The Data in QtZzMCUBuffer 
		IQtIZzMCUComponent(1);
		IQtIZzMCUComponent(2);
		GetYUV(0);			   //得到Y cR cB 
		GetYUV(1);
		GetYUV(2);
		StoreBuffer();		   //To RGB
		sizej+=SampRate_Y_H*8;
		if(sizej>=ImgWidth)
		{
			sizej=0;
			sizei+=SampRate_Y_V*8;
		}
		if ((sizej==0)&&(sizei>=ImgHeight))break;	  
	}
	return funcret;
}
// 入口 QtZzMCUBuffer 出口 Y[] U[] V[] 
//得到YUV色彩空间   
void  GetYUV(short flag)
{
	short	H,VV;
	short	i,j,k,h;
	int		*buf;
	int		*pQtZzMCU;

	switch(flag)
	{
		case 0://亮度分量
			H=SampRate_Y_H;
			VV=SampRate_Y_V;
			buf=Y;
			pQtZzMCU=QtZzMCUBuffer;
			break;
		case 1://红色分量
			H=SampRate_U_H;
			VV=SampRate_U_V;
			buf=U;
			pQtZzMCU=QtZzMCUBuffer+Y_in_MCU*64;
			break;
		case 2://蓝色分量
			H=SampRate_V_H;
			VV=SampRate_V_V;
			buf=V;
			pQtZzMCU=QtZzMCUBuffer+(Y_in_MCU+U_in_MCU)*64;
			break;
	}
	for (i=0;i<VV;i++)
		for(j=0;j<H;j++)
			for(k=0;k<8;k++)
				for(h=0;h<8;h++)
					buf[(i*8+k)*SampRate_Y_H*8+j*8+h]=*pQtZzMCU++;
}


extern uint16_t POINT_COLOR;
//将解出的字按RGB形式存储 lpbmp (BGR),(BGR) ......入口Y[] U[] V[] 出口lpPtr		 
void StoreBuffer()
{
	short i,j;			    
	unsigned char R,G,B;
	int y,u,v,rr,gg,bb;
	uint16_t color;
	for(i=0;i<SampRate_Y_V*8;i++)
	{
		if((sizei+i)<ImgHeight)// sizei表示行 sizej 表示列 
		{																					    
			for(j=0;j<SampRate_Y_H*8;j++)
			{
				if((sizej+j)<ImgWidth)
				{
					y=Y[i*8*SampRate_Y_H+j];
					u=U[(i/V_YtoU)*8*SampRate_Y_H+j/H_YtoU];
					v=V[(i/V_YtoV)*8*SampRate_Y_H+j/H_YtoV];
					rr=((y<<8)+18*u+367*v)>>8;
					gg=((y<<8)-159*u-220*v)>>8;
					bb=((y<<8)+411*u-29*v)>>8;
					R=(unsigned char)rr;
					G=(unsigned char)gg;
					B=(unsigned char)bb;
					if (rr&0xffffff00) if (rr>255) R=255; else if (rr<0) R=0;
					if (gg&0xffffff00) if (gg>255) G=255; else if (gg<0) G=0;
					if (bb&0xffffff00) if (bb>255) B=255; else if (bb<0) B=0;  
				    color=R>>3;
					color=color<<6;
					color |=(G>>2);
					color=color<<5;
					color |=(B>>3);
					//在这里送给LCD显示
					POINT_COLOR=color;	
					LCD_DrawPoint(sizej+j+CurX,sizei+i+CurY);//显示图片				  
				}
				else  break;
			} 	    
		}
		else break;
	}
}
//Huffman Decode   MCU 出口 MCUBuffer  入口Blockbuffer[  ] 
int DecodeMCUBlock()
{
	short *lpMCUBuffer;
	short i,j;
	int funcret;

	if (IntervalFlag)//差值复位 
	{
		lp+=2;
		ycoef=ucoef=vcoef=0;
		BitPos=0;
		CurByte=0;
	}
	switch(comp_num)
	{
		case 3:	 //comp_num 指图的类型（彩色图、灰度图） 
			lpMCUBuffer=MCUBuffer;
			for (i=0;i<SampRate_Y_H*SampRate_Y_V;i++)  //Y
			{
				funcret=HufBlock(YDcIndex,YAcIndex);//解码4 * (8*8) 
				if (funcret!=FUNC_OK)
					return funcret;
				BlockBuffer[0]=BlockBuffer[0]+ycoef;//直流分量是差值，所以要累加。
				ycoef=BlockBuffer[0];
				for (j=0;j<64;j++)
					*lpMCUBuffer++=BlockBuffer[j];
			}
			for (i=0;i<SampRate_U_H*SampRate_U_V;i++)  //U
			{
				funcret=HufBlock(UVDcIndex,UVAcIndex);
				if (funcret!=FUNC_OK)
					return funcret;
				BlockBuffer[0]=BlockBuffer[0]+ucoef;
				ucoef=BlockBuffer[0];
				for (j=0;j<64;j++)
					*lpMCUBuffer++=BlockBuffer[j];
			}
			for (i=0;i<SampRate_V_H*SampRate_V_V;i++)  //V
			{
				funcret=HufBlock(UVDcIndex,UVAcIndex);
				if (funcret!=FUNC_OK)
					return funcret;
				BlockBuffer[0]=BlockBuffer[0]+vcoef;
				vcoef=BlockBuffer[0];
				for (j=0;j<64;j++)
					*lpMCUBuffer++=BlockBuffer[j];
			}
			break;
		case 1:	//Gray Picture 
			lpMCUBuffer=MCUBuffer;
			funcret=HufBlock(YDcIndex,YAcIndex);
			if (funcret!=FUNC_OK)
				return funcret;
			BlockBuffer[0]=BlockBuffer[0]+ycoef;
			ycoef=BlockBuffer[0];
			for (j=0;j<64;j++)
				*lpMCUBuffer++=BlockBuffer[j];
			for (i=0;i<128;i++)
				*lpMCUBuffer++=0;
			break;
		default:
			return FUNC_FORMAT_ERROR;
	}
	return FUNC_OK;
}
//Huffman Decode （8*8） DU   出口 Blockbuffer[ ] 入口 vvalue 
int HufBlock(BYTE dchufindex,BYTE achufindex)
{
	short count=0;
	short i;
	int funcret;

	//dc
	HufTabIndex=dchufindex;
	funcret=DecodeElement();
	if(funcret!=FUNC_OK)
		return funcret;

	BlockBuffer[count++]=vvalue;//解出的直流系数 
	//ac
	HufTabIndex=achufindex;
	while (count<64)
	{
		funcret=DecodeElement();
		if(funcret!=FUNC_OK)
			return funcret;
		if ((rrun==0)&&(vvalue==0))
		{
			for (i=count;i<64;i++)
				BlockBuffer[i]=0;
			count=64;
		}
		else
		{						    
			for (i=0;i<rrun;i++)	 	//前面的零
				BlockBuffer[count++]=0;
			BlockBuffer[count++]=vvalue;//解出的值 
		}
	}
	return FUNC_OK;
}
//Huffman 解码  每个元素   出口 vvalue 入口 读文件ReadByte 
int DecodeElement()
{
	int thiscode,tempcode;
	unsigned short temp,valueex;
	short codelen;
	BYTE hufexbyte,runsize,tempsize,sign;
	BYTE newbyte,lastbyte;

	if(BitPos>=1) //BitPos指示当前比特位置 
	{
		BitPos--;
		thiscode=(BYTE)CurByte>>BitPos;//取一个比特
		CurByte=CurByte&And[BitPos];   //清除取走的比特位 
	}
	else							   //取出的一个字节已用完
	{								   //新取
		lastbyte=ReadByte();		   //读出一个字节
		BitPos--;					   //and[]:=0x0,0x1,0x3,0x7,0xf,0x1f,0x2f,0x3f,0x4f 
		newbyte=CurByte&And[BitPos];
		thiscode=lastbyte>>7;
		CurByte=newbyte;
	}
	codelen=1;
	//与Huffman表中的码字匹配，直自找到为止 
	while ((thiscode<huf_min_value[HufTabIndex][codelen-1])||
		  (code_len_table[HufTabIndex][codelen-1]==0)||
		  (thiscode>huf_max_value[HufTabIndex][codelen-1]))
	{
		if(BitPos>=1)//取出的一个字节还有
		{
			BitPos--;
			tempcode=(BYTE)CurByte>>BitPos;
			CurByte=CurByte&And[BitPos];
		}
		else
		{
			lastbyte=ReadByte();
			BitPos--;
			newbyte=CurByte&And[BitPos];
			tempcode=(BYTE)lastbyte>>7;
			CurByte=newbyte;
		}
		thiscode=(thiscode<<1)+tempcode;
		codelen++;
		if(codelen>16)return FUNC_FORMAT_ERROR;	   
	}  //while
	temp=thiscode-huf_min_value[HufTabIndex][codelen-1]+code_pos_table[HufTabIndex][codelen-1];
	hufexbyte=(BYTE)code_value_table[HufTabIndex][temp];
	rrun=(short)(hufexbyte>>4);	 //一个字节中，高四位是其前面的零的个数。
	runsize=hufexbyte&0x0f;		 //后四位为后面字的尺寸
	if(runsize==0)
	{
		vvalue=0;
		return FUNC_OK;
	}
	tempsize=runsize;
	if(BitPos>=runsize)
	{
		BitPos-=runsize;
		valueex=(BYTE)CurByte>>BitPos;
		CurByte=CurByte&And[BitPos];
	}
	else
	{
		valueex=CurByte;
		tempsize-=BitPos;
		while(tempsize>8)
		{
			lastbyte=ReadByte();
			valueex=(valueex<<8)+(BYTE)lastbyte;
			tempsize-=8;
		}  //while
		lastbyte=ReadByte();
		BitPos-=tempsize;
		valueex=(valueex<<tempsize)+(lastbyte>>BitPos);
		CurByte=lastbyte&And[BitPos];
	}  //else
	sign=valueex>>(runsize-1);
	if(sign)vvalue=valueex;//解出的码值 
	else
	{
		valueex=valueex^0xffff;
		temp=0xffff<<runsize;
		vvalue=-(short)(valueex^temp);
	}
	return FUNC_OK;
}
//反量化MCU中的每个组件   入口 MCUBuffer 出口 QtZzMCUBuffer 
void IQtIZzMCUComponent(short flag)
{
	short H,VV;
	short i,j;
	int *pQtZzMCUBuffer;
	short  *pMCUBuffer;

	switch(flag){
	case 0:
		H=SampRate_Y_H;
		VV=SampRate_Y_V;
		pMCUBuffer=MCUBuffer;
		pQtZzMCUBuffer=QtZzMCUBuffer;
		break;
	case 1:
		H=SampRate_U_H;
		VV=SampRate_U_V;
		pMCUBuffer=MCUBuffer+Y_in_MCU*64;
		pQtZzMCUBuffer=QtZzMCUBuffer+Y_in_MCU*64;
		break;
	case 2:
		H=SampRate_V_H;
		VV=SampRate_V_V;
		pMCUBuffer=MCUBuffer+(Y_in_MCU+U_in_MCU)*64;
		pQtZzMCUBuffer=QtZzMCUBuffer+(Y_in_MCU+U_in_MCU)*64;
		break;
	}
	for(i=0;i<VV;i++)
		for (j=0;j<H;j++)
			IQtIZzBlock(pMCUBuffer+(i*H+j)*64,pQtZzMCUBuffer+(i*H+j)*64,flag);
}
//要量化的字 
//反量化 8*8 DU 
void IQtIZzBlock(short  *s ,int * d,short flag)
{
	short i,j;
	short tag;
	short *pQt;
	int buffer2[8][8];
	int *buffer1;
	short offset;

	switch(flag)
	{
		case 0:	  //亮度
			pQt=YQtTable;
			offset=128;
			break;
		case 1:	  //红
			pQt=UQtTable;
			offset=0;
			break;
		case 2:	  //蓝 
			pQt=VQtTable;
			offset=0;
			break;
	}

	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
		{
			tag=Zig_Zag[i][j];
			buffer2[i][j]=(int)s[tag]*(int)pQt[tag];
		}
	buffer1=(int *)buffer2;
	Fast_IDCT(buffer1);//反DCT
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			d[i*8+j]=buffer2[i][j]+offset;
}
//快速反DCT
void Fast_IDCT(int * block)
{
	short i;		 
	for (i=0; i<8; i++)idctrow(block+8*i);	    
	for (i=0; i<8; i++)idctcol(block+i);    
}
//从源文件读取一个字节
BYTE  ReadByte()
{
	BYTE  i;

	i=*(lp++);// lp 为解码的起始位置 
	if(i==0xff)lp++;   
	BitPos=8;
	CurByte=i;
	return i;
}
//初始化快速反DCT
void Initialize_Fast_IDCT()
{
	short i;

	iclp = iclip+512;
	for (i= -512; i<512; i++)
		iclp[i] = (i<-256) ? -256 : ((i>255) ? 255 : i);
}
////////////////////////////////////////////////////////////////////////
void idctrow(int * blk)
{
	int x0, x1, x2, x3, x4, x5, x6, x7, x8;
	//intcut
	if (!((x1 = blk[4]<<11) | (x2 = blk[6]) | (x3 = blk[2]) |
		(x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
	{
		blk[0]=blk[1]=blk[2]=blk[3]=blk[4]=blk[5]=blk[6]=blk[7]=blk[0]<<3;
		return;
	}
	x0 = (blk[0]<<11) + 128; // for proper rounding in the fourth stage 
	//first stage
	x8 = W7*(x4+x5);
	x4 = x8 + (W1-W7)*x4;
	x5 = x8 - (W1+W7)*x5;
	x8 = W3*(x6+x7);
	x6 = x8 - (W3-W5)*x6;
	x7 = x8 - (W3+W5)*x7;
	//second stage
	x8 = x0 + x1;
	x0 -= x1;
	x1 = W6*(x3+x2);
	x2 = x1 - (W2+W6)*x2;
	x3 = x1 + (W2-W6)*x3;
	x1 = x4 + x6;
	x4 -= x6;
	x6 = x5 + x7;
	x5 -= x7;
	//third stage
	x7 = x8 + x3;
	x8 -= x3;
	x3 = x0 + x2;
	x0 -= x2;
	x2 = (181*(x4+x5)+128)>>8;
	x4 = (181*(x4-x5)+128)>>8;
	//fourth stage
	blk[0] = (x7+x1)>>8;
	blk[1] = (x3+x2)>>8;
	blk[2] = (x0+x4)>>8;
	blk[3] = (x8+x6)>>8;
	blk[4] = (x8-x6)>>8;
	blk[5] = (x0-x4)>>8;
	blk[6] = (x3-x2)>>8;
	blk[7] = (x7-x1)>>8;
}
//////////////////////////////////////////////////////////////////////////////
void idctcol(int * blk)
{
	int x0, x1, x2, x3, x4, x5, x6, x7, x8;
	//intcut
	if (!((x1 = (blk[8*4]<<8)) | (x2 = blk[8*6]) | (x3 = blk[8*2]) |
		(x4 = blk[8*1]) | (x5 = blk[8*7]) | (x6 = blk[8*5]) | (x7 = blk[8*3])))
	{
		blk[8*0]=blk[8*1]=blk[8*2]=blk[8*3]=blk[8*4]=blk[8*5]
			=blk[8*6]=blk[8*7]=iclp[(blk[8*0]+32)>>6];
		return;
	}
	x0 = (blk[8*0]<<8) + 8192;
	//first stage
	x8 = W7*(x4+x5) + 4;
	x4 = (x8+(W1-W7)*x4)>>3;
	x5 = (x8-(W1+W7)*x5)>>3;
	x8 = W3*(x6+x7) + 4;
	x6 = (x8-(W3-W5)*x6)>>3;
	x7 = (x8-(W3+W5)*x7)>>3;
	//second stage
	x8 = x0 + x1;
	x0 -= x1;
	x1 = W6*(x3+x2) + 4;
	x2 = (x1-(W2+W6)*x2)>>3;
	x3 = (x1+(W2-W6)*x3)>>3;
	x1 = x4 + x6;
	x4 -= x6;
	x6 = x5 + x7;
	x5 -= x7;
	//third stage
	x7 = x8 + x3;
	x8 -= x3;
	x3 = x0 + x2;
	x0 -= x2;
	x2 = (181*(x4+x5)+128)>>8;
	x4 = (181*(x4-x5)+128)>>8;
	//fourth stage
	blk[8*0] = iclp[(x7+x1)>>14];
	blk[8*1] = iclp[(x3+x2)>>14];
	blk[8*2] = iclp[(x0+x4)>>14];
	blk[8*3] = iclp[(x8+x6)>>14];
	blk[8*4] = iclp[(x8-x6)>>14];
	blk[8*5] = iclp[(x0-x4)>>14];
	blk[8*6] = iclp[(x3-x2)>>14];
	blk[8*7] = iclp[(x7-x1)>>14];
}
			 
