/// \file firewall.c
/// \brief Reads IP packets from a named pipe, examines each packet,
/// and writes allowed packets to an output named pipe.
/// Author: Chris Dickens (RIT CS)
/// @author Douglas Gliner <dgg5503>
/// @date 5/10/2016
///
/// This file contains proprietary information. Distribution is limited
/// to Rochester Institute of Technology faculty, students currently enrolled
/// in CSCI243: The Mechanics of Programming, graders, and student lab
/// instructors. Further distribution requires written approval from the
/// Rochester Institute of Technology Computer Science department. The
/// content of this file is protected as an unpublished work.
///
/// Copyright 2015 Rochester Institute of Technology
///
///

// Pre-Reqs
#define _BSD_SOURCE
#define MAX_PACKET_LENGTH 2000
#define MAX_INPUT_LENGTH 3

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>

#include "utils.h"
#include "filter.h"


/// Type used to control the mode of the firewall
typedef enum FilterMode_e
{
   MODE_BLOCK_ALL,
   MODE_ALLOW_ALL,
   MODE_FILTER
} FilterMode;

/// wireshark GLOBAL HEADER
typedef struct pcap_hdr_s {
	unsigned int magic_number; /* magic number */
	unsigned short version_major; /* major version number */
	unsigned short version_minor; /* minor version number */
	int thiszone; /* GMT to local correction */
	unsigned int sigfigs; /* accuracy of timestamps */
	unsigned int snaplen; /* max length of captured packets, in octets */
	unsigned int network; /* data link type */
} pcap_hdr;

/// wireshark packet header
typedef struct pcaprec_hdr_s {
	unsigned int ts_sec; /* timestamp seconds */
	unsigned int ts_usec; /* timestamp microseconds */
	unsigned int incl_len; /* number of octets of packet saved in file */
	unsigned int orig_len; /* actual length of packet */
} pcaprec_hdr;

/// The input named pipe, "ToFirewall"
static FILE* InPipe = NULL;

/// The output named pipe, "FromFirewall"
static FILE* OutPipe = NULL;

/// The output file for the pcap file
static FILE* PCAPFile = NULL;

/// Temporary packet header to contain swapped values.
static pcaprec_hdr TempHeader = { 0, 0, 0, 0 };

/// Controls the mode of the firewall
volatile FilterMode Mode = MODE_FILTER;

/// Decides if we should verify checksum or not
static bool IsVerifyChecksum = false;

/// The main function that performs the actual packet read, filter, and write.
/// The return value and parameter must match those expected by pthread_create.
/// @param args A pointer to a filter
/// @return Always NULL
static void* FilterThread(void* args);


/// Displays the menu of commands that the user can choose from.
static void DisplayMenu(void);


/// Opens the input and output named files.
/// @return True if successful
static bool OpenPipes(void);


/// Reads a packet from the input name pipe.
/// @param buf Destination buffer to write the packet into
/// @param bufLength The length of the supplied destination buffer
/// @param len The length of the packet
/// @return True if successful
static bool ReadPacket(unsigned char* buf, int bufLength, int* len);

/// Tests a packet to determine if it should be blocked according to its
/// accompanied IPv4 checksum
/// @param fltCfg The filter configuration to use
/// @param pkt The IP packet that is to be evaluated
/// @return True if the packet passes inspection, False if it fails
static bool VerifyChecksum(unsigned char* pkt, int len);

/// Converts 2 unsigned characters into a single short in the form
/// of an unsigned int.
///
/// @param bytes  a pointer to the start of the 2 byte array.
/// @returns finalValue  an unsigned integer with equivalent magnitude of
///						 the provided bytes.
///
static unsigned int uchar2UShort(unsigned char bytes[2]);


/// The main function. Creates a filter, configures it, launches the
/// filtering thread, handles user input, and cleans up resources when
/// exiting.  The intention is to run this program with a command line
/// argument specifying the configuration file to use.
/// @param argc Number of command line arguments
/// @param argv Command line arguments
/// @return EXIT_SUCCESS or EXIT_FAILURE
int main(int argc, char* argv[])
{
	// usage
	if (argc == 1)
	{
		ERR("usage: ./firewall configFileName\n");
		return EXIT_FAILURE;
	}

	// get optional parameters
	int opt;
	while ((opt = getopt(argc, argv, "cl:")) != -1)
	{
		switch (opt)
		{
			case 'c':
				IsVerifyChecksum = true;
				break;

			case 'l':
				{
					// overwrite old packet file
					PCAPFile = fopen(optarg, "w");
					
					// write header to prepare for content
					fwrite(&((pcap_hdr){ 0xa1b2c3d4, 2, 4, 0, 0, 65535, 228 }),
						sizeof(pcap_hdr),
						1,
						PCAPFile);
				}
				break;

			default:
				break;
		}
	}

	// create a filter
	// Will exit if out of memory
	IpPktFilter ipPktFilter = CreateFilter();

	// configure it
	if (!ConfigureFilter(ipPktFilter, argv[optind]))
	{
		// failed, free the filter
		DestroyFilter(ipPktFilter);

		// close pcap file if not closed
		if (PCAPFile != NULL)
			fclose(PCAPFile);

		return EXIT_FAILURE;
	}

	

	// generate thread id array
	void* exitStatus;
	pthread_t filterThreadID;
	// display options
	DisplayMenu();
	// launch thread
	pthread_create(&filterThreadID,
		NULL,
		FilterThread,
		(void*)ipPktFilter);

	// handle input
	int choice = -1;
	char inputBuffer[MAX_INPUT_LENGTH] = { '\0' };
	while (choice != 0 &&
		fgets(inputBuffer, MAX_INPUT_LENGTH, stdin) != NULL)
	{
		// rid of \n
		inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

		// print table?
		switch ((choice = strtol(inputBuffer, NULL, 10)))
		{
			case 0:
				// cancel thread if exit.
				pthread_cancel(filterThreadID);
				printf("Exiting\n");
				break;

			case 1:
				Mode = MODE_BLOCK_ALL;
				printf("blocking all packets\n> ");
				break;

			case 2:
				Mode = MODE_ALLOW_ALL;
				printf("allowing all packets\n> ");
				break;

			case 3:
				Mode = MODE_FILTER;
				printf("filtering packets\n> ");
				break;
				
			default:
				ERR("Invalid choice\n> ");
				break;
		}
	}

	// join (will this work after a cancel?)
	pthread_join(filterThreadID, &exitStatus);

	// clean up resources
	DestroyFilter(ipPktFilter);

	if(InPipe != NULL)
		fclose(InPipe);

	if(OutPipe != NULL)
		fclose(OutPipe);

	if (PCAPFile != NULL)
		fclose(PCAPFile);

   return EXIT_SUCCESS;
}


/// Runs as a thread and handles each packet. It is responsible
/// for reading each packet in its entirety from the input pipe,
/// filtering it, and then writing it to the output pipe. The
/// single void* parameter matches what is expected by pthread.
/// @param args An IpPktFilter
/// @return Always NULL
static void* FilterThread(void* args)
{
	// cast arg as filter
	IpPktFilter filter = (IpPktFilter)args;
	assert(filter != NULL);

	// open pipes for reading/writing
	if (!OpenPipes())
	{
		ERR("Error: Failed to open pipes!\n");
		pthread_exit(NULL);
	}

	// current length of packet
	int packetLength = 0;

	// create VLA of bytes and init
	unsigned char packetBuffer[MAX_PACKET_LENGTH] = { 0 };

	while (1)
	{
		// read from InPipe
		if (!ReadPacket(packetBuffer, MAX_PACKET_LENGTH, &packetLength))
		{
			ERR("Error: error reading packet!\n");
			pthread_exit(NULL);
		}

		// abuse fall through
		switch (Mode)
		{
			case MODE_BLOCK_ALL:
				// write to PCAP FILE if not null!
				if (PCAPFile != NULL)
				{
					TempHeader.incl_len = packetLength;
					TempHeader.orig_len = packetLength;

					// write packer header
					fwrite(&TempHeader, sizeof(pcaprec_hdr), 1, PCAPFile);

					// write packet data
					fwrite(packetBuffer, sizeof(unsigned char),
						packetLength, PCAPFile);
				}
				break;

			case MODE_FILTER:
				if (!FilterPacket(filter, packetBuffer))
				{
					// write to PCAP FILE if not null!
					if (PCAPFile != NULL)
					{
						TempHeader.incl_len = packetLength;
						TempHeader.orig_len = packetLength;

						// write packer header
						fwrite(&TempHeader, sizeof(pcaprec_hdr), 1, PCAPFile);

						// write packet data
						fwrite(packetBuffer, sizeof(unsigned char),
							packetLength, PCAPFile);
					}
					break;
				}

			case MODE_ALLOW_ALL:
				// run verify checksum if true
				if (IsVerifyChecksum &&
					!VerifyChecksum(packetBuffer, packetLength))
				{
					printf("VERIFY FAILED!\n");
					break;
				}

				// TODO: check to see if packet write failed??
				// write packet length & then contents
				fwrite(&packetLength, sizeof(int), 1, OutPipe);
				fwrite(packetBuffer, sizeof(unsigned char),
					packetLength, OutPipe);
				fflush(OutPipe);
				break;

			default:
				ERR("Error: Invalid filter mode!\n");
				pthread_exit(NULL);
				break;
		}

		// ensure time conforms to standards provided
		if (TempHeader.ts_usec >= 1000000)
		{
			TempHeader.ts_sec++;
			TempHeader.ts_usec = 0;
		}
		else
		{
			// increment 50ms to simulate some time
			TempHeader.ts_usec += 50000; 
		}
	}
	pthread_exit(NULL);
}


 
/// Print a menu and a prompt to stdout
static void DisplayMenu(void)
{
   printf("\n1. Block All\n");
   printf("2. Allow All\n");
   printf("3. Filter\n");
   printf("0. Exit\n");
   printf("> ");
}


/// Open the input and output named pipes that are used for reading
/// and writing packets.
/// @return True if successful
static bool OpenPipes(void)
{
   InPipe = fopen("ToFirewall", "rb");
   if(InPipe == NULL)
   {
      perror("ERROR, failed to open pipe ToFirewall:");
      return false;
   }

   OutPipe = fopen("FromFirewall", "wb");
   if(OutPipe == NULL)
   {
      perror("ERROR, failed to open pipe FromFirewall:");
	  fclose(InPipe);
      return false;
   }

   return true;
}


/// Read an entire IP packet from the input pipe
/// @param buf Destination buffer for storing the packet
/// @param bufLength The length of the supplied destination buffer
/// @param len The length of the packet
/// @return True if a packet was successfully read
static bool ReadPacket(unsigned char* buf, int bufLength, int* len)
{
	// TODO FINISH THIS
	// mutex lock?

	// ensure inpipe is not null
	assert(InPipe != NULL);

	// read size of packet
	if (!fread(len, sizeof(int), 1, InPipe))
		return false;

	// ensure buf length is smaller then packet length
	assert(bufLength > *len);

	// read data
	if (fread(buf, sizeof(unsigned char), *len, InPipe))
		return true;

	return false;
}

/// Converts 2 unsigned characters into a single short in the form
/// of an unsigned int.
///
/// @param bytes  a pointer to the start of the 2 byte array.
/// @returns finalValue  an unsigned integer with equivalent magnitude of
///						 the provided bytes.
///
static inline unsigned int uchar2UShort(unsigned char bytes[2])
{
	unsigned int finalValue = bytes[0] << 8;
	finalValue |= bytes[1];
	return finalValue;
}

/// Tests a packet to determine if it should be blocked according to its
/// accompanied IPv4 checksum
/// @param fltCfg The filter configuration to use
/// @param pkt The IP packet that is to be evaluated
/// @return True if the packet passes inspection, False if it fails
bool VerifyChecksum(unsigned char * pkt, int len)
{
	// ensure we wont overflow
	assert(len > (int)sizeof(unsigned char) * 20);

	unsigned int addedValue = 0;

	// obtain first 5 unsigned ints
	for (unsigned int i = 0; i < 10; ++i)
		addedValue += uchar2UShort(&pkt[i * sizeof(unsigned short)]);

	unsigned int carry = (addedValue & 0xFFFF0000) >> 16;
	addedValue = ~((carry + addedValue)) & 0x0000FFFF;
	
	// failed checksum
	if (addedValue)
		return false;

	// passed!
	return true;
}


