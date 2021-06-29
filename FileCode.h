#ifndef FILE_CODE_H
#define FILE_CODE_H
#include <string.h>
/******************************************************************************
*                    MP3 编码
******************************************************************************/
void MP3_Decode(const char* data) {
	int headSize = ((data[6] & 0x7F) << 21)
		+ ((data[7] & 0x7F) << 14)
		+ ((data[8] & 0x7F) << 7)
		+ (data[9] & 0x7F);
	int audioSize = (data[10 + 4] << 24)
		+ (data[10 + 5] << 16)
		+ (data[10 + 6] << 8)
		+ data[10 + 7];
	data += 10 + headSize;
	//数据帧
	for (int i = 0; i < audioSize;) {
		unsigned char bitRate = (data[3] & 0xF0) >> 4;
		double sampleRate = (data[3] & 0x0C) >> 2 == 0x00 ? (44.1) : ((data[3] & 0x0C) >> 2 == 0x01 ? 48 : 32);
		unsigned char padding = (data[3] & 0x02) >> 1;
		int size = 144 * bitRate / sampleRate + padding;
		i += size; data += size;
	}
}
/******************************************************************************
*                    JPG 编码
******************************************************************************/
void JpgDecode() {

}
/******************************************************************************
*                    IPv4 编码
******************************************************************************/
//IPv4首部(20B+)
struct IPv4Header {
	unsigned char  Ver_HeadLen;	// 版本(4b) + 首部长度(4b)
	unsigned char  ServiceType;	// 服务类型(Type of service)(8b)
	unsigned short Length;		// 总长(Total length)(16b)
	unsigned short Identification;//标识(Identification)(16b)
	unsigned short Flag_FragmentOffset;// 标志位(Flags)(3b) + 段偏移量(Fragment offset)(13b)
	unsigned char  TTL;			// 存活时间(Time to live)(8b)
	unsigned char  Protocol;	// 协议(Protocol)(8b)
	unsigned short CRC;			// 首部校验和(Header checksum)(16b)
	unsigned int SourAddr;		// 源地址(Source address)(32b)
	unsigned int DestAddr;		// 目的地址(Destination address)(32b)
	unsigned int Option_Padding;// 选项与填充(Option + Padding)(+)
};
IPv4Header IPv4Decode(char* input, char*& data, int& dataLen) {
	IPv4Header head;
	memcpy(&head, input, sizeof(IPv4Header));
	data = input + 20;
	dataLen = head.Length - 20;
}
/******************************************************************************
*                    IPv6 编码
******************************************************************************/
//IPv6首部(40B)
struct IPv6Header {
	unsigned int  Ver_Traffic_Flow;	// 版本(4b) + 通信类量(Traffic Class)(8b) + 流编号(flow label)(20b)
	unsigned short Length;		// 总长(Total length)(16b)
	unsigned char  NextHead;	// 下一首部(Next Header)(8b)
	unsigned char  HopLim;		// 跳数限制(Hop Limit)(8b)
	unsigned short SourAddr[8];	// 源地址(Source address)(128b)
	unsigned short DestAddr[8];	// 目的地址(Destination address)(128b)
};
IPv6Header IPv6Decode(char* input, char*& data, int& dataLen) {
	IPv6Header head;
	memcpy(&head, input, sizeof(IPv6Header));
	data = input + 40;
	dataLen = head.Length - 40;
}
/******************************************************************************
*                    UDP 编码
******************************************************************************/
//UDP首部(8B)
struct UdpHeader {
	unsigned short SourPort;	// 源端口(Source port)(16b)
	unsigned short DestPort;	// 目的端口(Destination port)(16b)
	unsigned short Length;		// UDP数据包长度(Datagram length)(16b)
	unsigned short Checksum;	// 校验和(Checksum)(16b)
};
UdpHeader UdpDecode(char* input, char*& data, int& dataLen) {
	UdpHeader head;
	memcpy(&head, input, sizeof(UdpHeader));
	data = input + 8;
	dataLen = head.Length;
}
/******************************************************************************
*                    TCP 编码
******************************************************************************/
//TCP首部(20B+)
struct TCPHeader {
	unsigned short SourPort;	// 源端口(Source port)(16b)
	unsigned short DestPort;	// 目的端口(Destination port)(16b)
	unsigned int SeqNum;		// 序号(Sequence Number)(32b)
	unsigned int AckNum;		// 确认号(Acknowlegment Number)(32b)
	unsigned short DataOffset_;	// 数据偏移_保留_URC_ACK_PSH_RST_SYN_FIN(Data offset)(16b)
	unsigned short Window;		// 窗口(Window)(16b)
	unsigned short CRC;			// 校验和(Checksum)(16b)
	unsigned short UrgentPtr;	// 紧急指针(Urgent Pointer)(16b)
	unsigned int Option_Padding;// 选项与填充(Option + Padding)(+)
};
TCPHeader TCPDecode(char* input, char*& data, int& dataLen) {
	TCPHeader head;
	memcpy(&head, input, sizeof(TCPHeader));
	data = input + 20;
	dataLen = 1460;
}
#endif