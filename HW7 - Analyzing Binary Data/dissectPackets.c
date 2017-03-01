// File: dissectPackets.c 
// Dissect IPv4/6 packet headers.
// @author Douglas Gliner <dgg5503>
// @date 4/22/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw7}
//
// // // // // // // // // // // // // // // // // // // // // // // 

// I tried to optimize this program to execute as fast as possible
// according to the neat tricks we learned in lecture :D!

// Defines and Macros
#define BUFFER_SIZE 2048

// XXXXXXXX
// 00001111 &
// ----------
// 0000XXXX <-- because little endian, we're done!
#define UPPER_NIBBLE(byte) ((byte) & 0x0F)

// XXXXXXXX
// 11110000 &
// ----------
// XXXX0000 <-- because little endian, we need to shift right 4 bits!!
// 0000XXXX now we're done!
#define LOWER_NIBBLE(byte) (((byte) & 0xF0) >> 4)

// 4 bytes, shift right 3 bits.
#define FLAGMASK 0xFFFF >> 3

// Includes
#include <errno.h>
#include <string.h>
#include "dissectPackets.h"

// statics
// Static string array of all the protocol types with their corresponding 
// ID numbers.
static char* protocolTypes[] = {
	[1] = "ICMP",
	[2] = "IGMP",
	[6] = "TCP",
	[9] = "IGRP",
	[17] = "UDP",
	[47] = "GRE",
	[50] = "ESP",
	[51] = "AH",
	[57] = "SKIP",
	[88] = "EIGRP",
	[89] = "OSPF",
	[115] = "L2TP"
};

// externs
// error number for file I/O
extern int errno;

/// Print out the header information for each packet provided in the given
/// packets file.
///
/// @param argc  number of arguments provided
/// @param argv  arguments in string form, expecting one file provided.
/// @returns errorCode  error Code; EXIT_SUCCESS if no error
///
int main(int argc, char* argv[])
{
	// ensure we have an arg, otherwise print usage
	if (argc == 1)
	{
		printf("usage: dissectPackets inputFile\n");
		return EXIT_FAILURE;
	}

	// open the file in binary read
	FILE* packetFile;
	if ((packetFile = fopen(argv[1], "rb")) == NULL)
	{
		ERR("Failed to open input file: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	// get number of packets to read
	unsigned int numOfPackets, packetLength;

	// NOTE: -127 - 127 FOR SIGNED CHAR, use unsigned char.....
	unsigned char packetData[BUFFER_SIZE];
	if (fread(&numOfPackets, sizeof(unsigned int), 1, packetFile) == 0)
	{
		ERR("Failed to read count of packets.\n");
		return EXIT_FAILURE;
	}

	// set up numbers within a packet
	unsigned int version, ihl, tos, totalLen;
	unsigned int id, ipFlag, fragOffset, flagFrag;
	unsigned int ttl, protocol, headerChecksum;
	//unsigned int sourceAddr, destAddr, ipOption;


	// print num of packets
	printf("==== File %s contains %d Packets.\n", argv[1], numOfPackets);

	// for loop through all packets
	for (unsigned int i = 0; i < numOfPackets; ++i)
	{
		// header
		printf("==>Packet %u\n", i + 1);

		// get packet data length
		fread(&packetLength, sizeof(unsigned int), 1, packetFile);

		// read data in
		fread(packetData, sizeof(char), packetLength, packetFile);

		// print version
		version = LOWER_NIBBLE(packetData[0]);
		printf("Version:\t\t0x%X (%d)\n",
			version, version);

		// print ihl
		ihl = UPPER_NIBBLE(packetData[0]);
		printf("IHL (Header Length):\t\t0x%X (%d)\n",
			ihl, ihl);

		// print tos
		tos = packetData[1];
		printf("Type of Service (TOS):\t\t0x%X (%d)\n",
			tos, tos);

		// print total len
		totalLen = uchar2UShort(&packetData[2]);
		printf("Total Length:\t\t0x%X (%d)\n",
			totalLen, totalLen);

		// print id
		id = uchar2UShort(&packetData[4]);
		printf("Identification:\t\t0x%X (%d)\n",
			id, id);

		// print flags (3 BITS WHAT????)
		flagFrag = uchar2UShort(&packetData[6]);

		// if this is a flag, only 1 of the 3 bits should ever be
		// turned on.
		ipFlag = flagFrag >> 8;

		fragOffset = flagFrag & FLAGMASK;
		printf("IP Flags:\t\t0x%X (%d)\n", ipFlag, ipFlag);
		printf("Fragment Offset:\t\t0x%X (%d)\n",
			fragOffset, fragOffset);

		// print time to live
		ttl = packetData[8];
		printf("Time To Live (TTL):\t\t0x%X (%d)\n",
			ttl, ttl);

		// print protocol and get associated string
		protocol = packetData[9];
		printf("Protocol:\t\t%s 0x%X (%d)\n",
			protocolTypes[protocol], protocol, protocol);

		// print header checksum
		headerChecksum = uchar2UShort(&packetData[10]);
		printf("Header Checksum:\t\t0x%X (%d)\n",
			headerChecksum, headerChecksum);

		// print source address
		printf("Source Address:\t\t%u.%u.%u.%u\n",
			packetData[12], packetData[13], packetData[14], packetData[15]);

		// print destination address
		printf("Destination Address:\t\t%u.%u.%u.%u\n",
			packetData[16], packetData[17], packetData[18], packetData[19]);
	}

	// close file
	fclose(packetFile);

	return EXIT_SUCCESS;
}

/// Converts 2 unsigned characters into a single short in the form
/// of an unsigned int.
///
/// @param bytes  a pointer to the start of the 2 byte array.
/// @returns finalValue  an unsigned integer with equivalent magnitude of
///						 the provided bytes.
///
inline unsigned int uchar2UShort(unsigned char bytes[2])
{
	unsigned int finalValue = bytes[0] << 8;
	finalValue |= bytes[1];
	return finalValue;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 