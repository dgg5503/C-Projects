#ifndef __PKT_UTILITY_H__
#define __PKT_UTILITY_H__
/// \file pktUtility.h
/// \brief Provides functionality to extract information from the
/// IP, ICMP, and TCP headers of IP packets
/// Author: Chris Dickens (RIT CS)///
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



/// The identifier for the ICMP protocol as specified by the
/// Internet Assigned Numbers Authority (IANA), see RFC3232
/// for further details
#define IP_PROTOCOL_ICMP 1


/// The identifier for the TCP protocol as specified by the
/// Internet Assigned Numbers Authority (IANA) see RFC3232
/// for further details
#define IP_PROTOCOL_TCP 6


/// The identifier for the UDP protocol as specified by the
/// Internet Assigned Numbers Authority (IANA) see RFC3232
/// for further details
#define IP_PROTOCOL_UDP 17


/// The identifier for the ICMP Echo Request as specified by the
/// Internet Assigned Numbers Authority (IANA) see RFC2780
/// for further details
#define ICMP_TYPE_ECHO_REQ 8


/// The identifier for the ICMP Echo Reply as specified by the
/// Internet Assigned Numbers Authority (IANA) see RFC2780
/// for further details
#define ICMP_TYPE_ECHO_REPLY 0


/// Reads the source IP address from the IP header of the 
/// packet. In the header the source IP address is formatted
/// as 4 bytes in big endian byte order. The bytes are shifted
/// and packed into an unsigned int.
/// @param pkt The packet to examine
/// @return The IP address of the computer that sent the packet
unsigned int ExtractSrcAddrFromIpHeader(unsigned char* pkt);


/// Reads the destination IP address from the IP header of the 
/// packet. In the header the destination IP address is formatted
/// as 4 bytes in big endian byte order. The bytes are shifted
/// and packed into an unsigned int.
/// @param pkt The packet to examine
/// @return The IP address of the computer that the packet
/// is addressed to
unsigned int ExtractDstAddrFromIpHeader(unsigned char* pkt);


/// Reads the protocol number from the IP header
/// @param pkt The packet to examine
/// @return The value stored in the protocol field of the IP header
unsigned int ExtractIpProtocol(unsigned char* pkt);


/// Reads the value of the Type field in the ICMP header
/// of an ICMP message. This function assumes that the ICMP
/// message is contained in an IP packet (with a standard
/// length 20 byte IP header)
/// @param pkt The packet to examine
/// @return The ICMP Type
unsigned char ExtractIcmpType(unsigned char* pkt);


/// Reads the destination port number out of the TCP header
/// of an IP packet containing a TCP protocol data unit. This
/// function assumes that the IP packet starts with a standard
/// length 20 byte IP header.
/// @param pkt The packet to examine
/// @return The destination port of the TCP protocol data unit
unsigned int ExtractTcpDstPort(unsigned char* pkt);


/// Converts an IP address represented as an array of 4 octets
/// into a single unsigned int
/// @details This function expects a pointer to the start of a unsigned
/// char array holding the IP address octets, for example: | C0 | A8 | 01 | 64 |
/// is converted into an unsigned int with value C0A80164 and returned
/// @param ip The IP address represented as an array of 4 octets
/// with each octet stored in an unsigned char
/// @return The IP address packed into an unsigned int
unsigned int ConvertIpUCharOctetsToUInt(unsigned char* ip);


/// Converts an IP address represented as an array of 4 octets
/// into a single unsigned int
/// @details This function expects a pointer to the start of a unsigned
/// int array holding the IP address octets, for example: | 000000C0 |
/// 000000A8 | 00000001 | 00000064 | is converted into an unsigned int 
/// with value C0A80164 and returned
/// @param ip The IP address represented as an array of 4 octets
/// with each octet stored in an unsigned int
/// @return The IP address packed into an unsigned int
unsigned int ConvertIpUIntOctetsToUInt(unsigned int* ip);

#endif


