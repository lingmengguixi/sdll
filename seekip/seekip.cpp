// seekip.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdio.h>  
#include <WinSock2.h>  
#include <WS2tcpip.h>  
#include <stdlib.h>  
#include <Windows.h>  
#include <string.h> 
#include <pcap.h>
using namespace std;

// 用户保存4字节的IP地址
typedef struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

// 用于保存IPV4的首部
typedef struct ip_header {
	u_char ver_ihl;
	u_char tos;
	u_short tlen;
	u_short identification;
	u_short flags_fo;
	u_char ttl;
	u_char proto;
	u_short crc;
	ip_address saddr;
	ip_address daddr;
	u_int op_pad;
}ip_header;

// 保存UDP首部
typedef struct udp_header {
	u_short sport;
	u_short dport;
	u_short len;
	u_short crc;
}udp_header;


int main(int argc, char *argv[])
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	pcap_t *adhandle;
	u_int netmask;
	ip_header *ih;
	time_t local_tv_sec;
	char timestr[16];
	struct pcap_pkthdr *header;
	struct tm *ltime;
	const u_char *pkt_data;
	int res;
	char packet_filter[] = "ip and tcp";
	struct bpf_program fcode;
	int i = 0;
	int inum;
	char errbuf[PCAP_ERRBUF_SIZE];
	
	if (pcap_findalldevs(&alldevs, errbuf) == -1){

		printf("Error in pcap_findalldevs_ex: %s\n", errbuf);

		return 1;

	}
	for (d = alldevs; d != NULL; d = d->next)

	{

		printf("%d. %s", ++i, d->name);

		if (d->description)

			printf(" (%s)\n", d->description);

		else

			printf(" (No description available)\n");

	}
	if (0 == i) {
		printf("\nNo interface found!Make sure WinPcap is installed\n");
		return -1;
	}
	scanf_s("%d", &inum);
	if (inum < 1 || inum > i) {
		printf("\nInterface number out of range.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

	// 跳转到该设备，打开适配器

	// 设备名，要捕捉的数据包的部分（65536保证能捕获到不同数据链路层上的每个数据包的全部内容），混杂模式，读取超时时间，错误缓冲池
	if ((adhandle = pcap_open_live(d->name, 65536, 1, 1000, errbuf)) == NULL) {
		printf("\nUnable to open the adapter.%s is not supported by WinPcap\n", errbuf);
		pcap_freealldevs(alldevs);
		return -1;
	}
	// 检查数据链路层（只考虑了以太网）
	if (pcap_datalink(adhandle) != DLT_EN10MB) {
		printf("\nThis program works only on Ethernet networks.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}

	if (d->addresses != NULL) {
		// 获得接口的第一个地址的掩码
		netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	}
	else {
		netmask = 0xffffff;
	}
	// 编译过滤器
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0) {
		printf("\nUnable to compile the packet filter.Check the syntax\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	printf("\nlistenting on %s...\n", d->description);
	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0) {
		// 请求超时
		if (0 == res) {
			continue;
		}
		// 将时间戳转换成可识别的格式
		local_tv_sec = header->ts.tv_sec;
		ltime = localtime(&local_tv_sec);
		strftime(timestr, sizeof(timestr), "%H:%M:%S", ltime);
		ih = (ip_header *)(pkt_data + 14); //以太网头部长度

		// 数据包的处理部分
		printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);
		printf(" [%d] ", ih->ver_ihl);
		printf("%d.%d.%d.%d -> %d.%d.%d.%d",
			ih->saddr.byte1,
			ih->saddr.byte2,
			ih->saddr.byte3,
			ih->saddr.byte4,
			ih->daddr.byte1,
			ih->daddr.byte2,
			ih->daddr.byte3,
			ih->daddr.byte4);
		printf(" %d\n",ih->proto);
	}
	if (-1 == res) {
		printf("Error reading the packet:%s\n", pcap_geterr(adhandle));
		return -1;
	}
	pcap_freealldevs(alldevs);
	system("pause");
}
