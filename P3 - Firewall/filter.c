/// \file filter.c
/// \brief Filters IP packets based on settings in a user supplied
/// configuration file.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "filter.h"
#include "pktUtility.h"
#include "utils.h"

#define MAX_LINE_LEN  256

/// The type used to hold the configuration settings for a filter
typedef struct FilterConfig_S
{
   unsigned int localIpAddr;
   unsigned int localMask;
   bool blockInboundEchoReq;
   unsigned int numBlockedInboundTcpPorts;
   unsigned int* blockedInboundTcpPorts;
   unsigned int numBlockedIpAddresses;
   unsigned int* blockedIpAddresses;
} FilterConfig;


/// Adds an IP address to the blocked list
/// @param fltCfg The filter configuration to add the IP address to
/// @param ipAddr The IP address that is to be blocked
static void AddBlockedIpAddress(FilterConfig* fltCfg, unsigned int ipAddr);


/// Adds a TCP port to the list of blocked inbound TCP ports
/// @param fltCfg The filter configuration to add the TCP port to
/// @param The TCP port that is to be blocked
static void AddBlockedInboundTcpPort(FilterConfig* fltCfg, unsigned int port);


/// Helper function that calls strtok and sscanf to read the decimal point
/// separated IP address octets
/// @param ipAddr The destination into which the IP address octets are stored
static void ParseRemainderOfStringForIp(unsigned int* ipAddr);


/// Tests a packet to determine if it should be blocked due to either
/// the source or destination IP addresses.
/// @param fltCfg The filter configuration to use
/// @param addr The IP address to test
/// @return True if the packet is to be blocked
static bool BlockIpAddress(FilterConfig* fltCfg, unsigned int addr);


/// Tests a packet to determine if it should be blocked due to the destination
/// TCP port.
/// @param fltCfg The filter configuration to use
/// @param port The TCP port to test
/// @return True if the packet is to be blocked
static bool BlockInboundTcpPort(FilterConfig* fltCfg, unsigned int port);

/// Tests a packet's source and destination IP addresses against the local
/// network's IP address and net mask to determine if a packet is coming
/// into the network from the outside world.
/// @param fltCfg The filter configuration to use
/// @param srcAddr The source IP address that has been extracted from a packet
/// @param dstAddr The destination IP address that has been extracted
/// from a packet
static bool PacketIsInbound(FilterConfig* fltCfg,
	unsigned int srcAddr,
	unsigned int dstAddr);


/// Creates an instance of a filter by allocating memory for a FilterConfig
/// and initializing its member variables.
/// @return A pointer to the new filter
IpPktFilter CreateFilter(void)
{
	// allocate memory and create the filter
	FilterConfig* allocFilter = malloc(sizeof(FilterConfig));
	
	if (allocFilter == NULL)
	{
		ERR("ERROR: Failed to create filter, out of memory!\n");
		exit(EXIT_FAILURE);
	}

	// init members to 0 or NULL
	allocFilter->blockedInboundTcpPorts = NULL;
	allocFilter->blockedIpAddresses = NULL;
	allocFilter->blockInboundEchoReq = false;
	allocFilter->localIpAddr = 0;
	allocFilter->localMask = 0;
	allocFilter->numBlockedInboundTcpPorts = 0;
	allocFilter->numBlockedIpAddresses = 0;

	// return our pointer
	return allocFilter;
}


/// Destroys an instance of a filter by freeing all of the dynamically
/// allocated memory associated with the filter.
/// @param filter The filter that is to be destroyed
void DestroyFilter(IpPktFilter filter)
{
   FilterConfig* fltCfg = filter;

   if(fltCfg->blockedIpAddresses != NULL)
      free(fltCfg->blockedIpAddresses);

   if(fltCfg->blockedInboundTcpPorts != NULL)
      free(fltCfg->blockedInboundTcpPorts);

   free(filter);
}


/// Configures a filter instance using the specified configuration file.
/// Reads the file line by line and uses strtok, strcmp, and sscanf to 
/// parse each line.  After each line is successfully parsed the result
/// is stored in the filter.  Blank lines are skipped.  When the end of
/// the file is encountered, the file is closed and the function returns.
/// @param filter The filter that is to be configured
/// @param filename The full path/filename of the configuration file that
/// is to be read.
/// @return True when successful
bool ConfigureFilter(IpPktFilter filter, char* filename)
{
   char buf[MAX_LINE_LEN];
   FILE* pFile;
   char* pToken;
   char* success;
   unsigned int ipAddr[4];
   unsigned int temp;
   unsigned int mask;
 
   FilterConfig *fltCfg = (FilterConfig*)filter;

   if(filename == NULL ||
	   (pFile = fopen(filename, "r")) == NULL)
   {
      ERR("ERROR, invalid config file\n");
      return false;
   }

   while(1)
   {
      success = fgets(buf, MAX_LINE_LEN, pFile);
      if(success == NULL)
         break;  // end of file found

      pToken = strtok(buf, ":\n");
      if( pToken == NULL )
      {
         // empty line encountered
      }
      else if(strcmp(pToken, "LOCAL_NET") == 0)
      {
		 // ensure local net not already set
		 if (fltCfg->localIpAddr != 0)
		 {
	 		 // supplied more than one time!!!
			 ERR("ERROR, invalid config file\n");
			 fclose(pFile);
			 return false;
		 }
         ParseRemainderOfStringForIp(ipAddr);
         temp = ConvertIpUIntOctetsToUInt(ipAddr);
         fltCfg->localIpAddr = temp;
        
         pToken = strtok(NULL, "/");
         sscanf(pToken, "%u", &temp);
         mask = 0;
         for(unsigned int i=0; i<temp; i++)
         {
            mask = mask >> 1;
            mask |= 0x80000000;
         }
         fltCfg->localMask = mask;
      }
	  else if (strcmp(pToken, "BLOCK_INBOUND_TCP_PORT") == 0)
	  {
		  // block inbound port assign
		  pToken = strtok(NULL, "\n");
		  if (sscanf(pToken, "%u", &temp) != 1)
		  {
			  ERR("ERROR, invalid config file\n");
			  fclose(pFile);
			  return false;
		  }

		  AddBlockedInboundTcpPort(fltCfg, temp);
	  }
	  else if (strcmp(pToken, "BLOCK_PING_REQ") == 0)
	  {
		  // block ping request code
		  if(!fltCfg->blockInboundEchoReq)
			fltCfg->blockInboundEchoReq = true;
		  else
		  {
			  // supplied more than one time!!!
			  ERR("ERROR, invalid config file\n");
			  fclose(pFile);
			  return false;
		  }
	  }
	  else if (strcmp(pToken, "BLOCK_IP_ADDR") == 0)
	  {
		  // block ip addr code
		  ParseRemainderOfStringForIp(ipAddr);
		  temp = ConvertIpUIntOctetsToUInt(ipAddr);

		  // add IP
		  AddBlockedIpAddress(fltCfg, temp);
	  }
	  else
	  {
		  ERR("ERROR: Unknown argument %s\n", pToken);
		  fclose(pFile);
		  return false;
	  }	  
  }

   // ensure a local net has been set
   if (fltCfg->localIpAddr == 0)
   {
	   ERR("Error, configuration file must set LOCAL_NET\n");
	   fclose(pFile);
	   return false;
   }

   // destory lists and close the file.
   fclose(pFile);

   // donezo
   return true;
}


/// Uses the settings specified by the filter instance to determine
/// if a packet should be allowed or blocked.  The source and
/// destination IP addresses are extracted from each packet and
/// checked using the BlockIpAddress helper function. The IP protocol
/// is extracted from the packet and if it is ICMP or TCP then 
/// additional processing occurs. This processing blocks inbound packets
/// set to blocked TCP destination ports and inbound ICMP echo requests.
/// @param filter The filter configuration to use
/// @param pkt The packet to examine
/// @return True if the packet is allowed by the filter. False if the packet
/// is to be blocked
bool FilterPacket(IpPktFilter filter, unsigned char* pkt)
{
	// cast filter as filter config
	FilterConfig* fltCfg = (FilterConfig*)filter;
	assert(fltCfg != NULL);

	// extract source and dest address from packet and check for block
	unsigned int srcAddr = ExtractSrcAddrFromIpHeader(pkt);
	if (BlockIpAddress(fltCfg, srcAddr))
		return false;

	unsigned int destAddr = ExtractDstAddrFromIpHeader(pkt);
	if (BlockIpAddress(fltCfg, destAddr))
		return false;

	// below only applies to inbound packets
	if (PacketIsInbound(filter, srcAddr, destAddr))
	{
		// get ip protocol
		unsigned int ipProt = ExtractIpProtocol(pkt);
		unsigned int icmpType = 0;
		switch (ipProt)
		{
		case IP_PROTOCOL_ICMP:
			// check to see if echo request
			icmpType = ExtractIcmpType(pkt);
			if (icmpType == ICMP_TYPE_ECHO_REQ &&
				fltCfg->blockInboundEchoReq)
				return false;
			break;
		case IP_PROTOCOL_TCP:
			// check for blocked TCP ports
			if (BlockInboundTcpPort(fltCfg, ExtractTcpDstPort(pkt)))
				return false;
		default:
			break;
		}
	}

	return true;
}


/// Checks if an IP address is listed as blocked by the supplied filter.
/// @param fltCfg The filter configuration to use
/// @param addr The IP address that is to be checked
/// @return True if the IP address is to be blocked
static bool BlockIpAddress(FilterConfig* fltCfg, unsigned int addr)
{
	// get ptr to addr array
	unsigned int* addrArray = fltCfg->blockedIpAddresses;

	// go through IP addresses in for loop to see if we should block it!
	for (unsigned int i = fltCfg->numBlockedIpAddresses; i; --i)
		if (addrArray[i - 1] == addr)
			return true;
	return false;
}


/// Checks if a TCP port is listed as blocked by the supplied filter.
/// @param fltCfg The filter configuration to use
/// @param port The TCP port that is to be checked
/// @return True if the TCP port is to be blocked
static bool BlockInboundTcpPort(FilterConfig* fltCfg, unsigned int port)
{
	// get ptr to tcp array
	unsigned int* tcpArray = fltCfg->blockedInboundTcpPorts;

	// go through IP addresses in for loop to see if we should block it!
	for (unsigned int i = fltCfg->numBlockedInboundTcpPorts; i; --i)
		if (tcpArray[i - 1] == port)
			return true;
	return false;
}

/// Checks if a packet is coming into the network from the external world.
/// Uses
/// the localMask in the supplied filter configuration to compare the
/// srcIpAddr
/// and dstIpAddr to the localIpAddr supplied in the filter configuration. If
/// the
/// dstIpAddr is on the same network as the localIpAddr, and the srcIpAddr
/// is not
/// on the same network as the localIpAddr then the packet is inbound.
/// @param fltCfg The filter configuration to use
/// @param srcIpAddr The source IP address of a packet
/// @param dstIpAddr The destination IP address of a packet
static bool PacketIsInbound(FilterConfig* fltCfg,
	unsigned int srcIpAddr,
	unsigned int dstIpAddr)
{
	// grab mask and ip
	unsigned int mask = fltCfg->localMask;
	unsigned int localIp = fltCfg->localIpAddr & mask;

	// if destIpAddr is on same net as localIpAddr
	// &&
	// srcIpAddr is NOT on the same net as localIpAddr
	// then packet is inbound TRUE
	if (((dstIpAddr & mask) == localIp) &&
		((srcIpAddr & mask) != localIp))
		return true;
		

	// if resulting &s are equal, then same network
	/*
	if ((dstIpAddr & mask) != (srcIpAddr & mask))
		return true;
	*/

	return false;
}


/// Adds the specified IP address to the array of blocked IP addresses in the
/// specified filter configuration. This requires allocating additional memory
/// to extend the length of the array that holds the blocked IP addresses.
/// @param fltCfg The filter configuration to which the IP address is added
/// @param ipAddr The IP address that is to be blocked
static void AddBlockedIpAddress(FilterConfig* fltCfg, unsigned int ipAddr)
{
   unsigned int *pTemp;
   int num = fltCfg->numBlockedIpAddresses;

   if(num == 0)
      pTemp = (unsigned int*)malloc(sizeof(unsigned int));
   else
      pTemp = (unsigned int*)realloc( fltCfg->blockedIpAddresses,
		  sizeof(unsigned int)*(num + 1) );
 
   assert(pTemp != NULL); 
   fltCfg->blockedIpAddresses = pTemp;
   fltCfg->blockedIpAddresses[num] = ipAddr;
   fltCfg->numBlockedIpAddresses++;
}


/// Adds the specified TCP port to the array of blocked TCP ports in the
/// specified filter configuration. This requires allocating additional
/// memory to extend the length of the array that holds the blocked ports.
/// @param fltCfg The filter configuration to which the TCP port is added
/// @param port The TCP port that is to be blocked
static void AddBlockedInboundTcpPort(FilterConfig* fltCfg, unsigned int port)
{
   unsigned int *pTemp;
   int num = fltCfg->numBlockedInboundTcpPorts;

   if(num == 0)
      pTemp = (unsigned int*)malloc(sizeof(unsigned int));
   else
      pTemp = (unsigned int*)realloc( fltCfg->blockedInboundTcpPorts,
		  sizeof(unsigned int)*(num + 1) );
 
   assert(pTemp != NULL); 
   fltCfg->blockedInboundTcpPorts = pTemp;
   fltCfg->blockedInboundTcpPorts[num] = port;
   fltCfg->numBlockedInboundTcpPorts++;
}


/// Parses the remainder of the string last operated on by strtok 
/// and converts each octet of the ASCII string IP address to an
/// unsigned integer value.
/// @param ipAddr The destination into which to store the octets
static void ParseRemainderOfStringForIp(unsigned int* ipAddr)
{
   char* pToken;

   pToken = strtok(NULL, ".");
   sscanf(pToken, "%u", &ipAddr[0]);
   pToken = strtok(NULL, ".");
   sscanf(pToken, "%u", &ipAddr[1]);
   pToken = strtok(NULL, ".");
   sscanf(pToken, "%u", &ipAddr[2]);
   pToken = strtok(NULL, "/");
   sscanf(pToken, "%u", &ipAddr[3]);
}

