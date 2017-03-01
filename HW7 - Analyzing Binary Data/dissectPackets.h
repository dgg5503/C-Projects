// File: dissectPackets.h
// header file containing function prototypes for dissectPackets
// @author Douglas Gliner <dgg5503>
// @date 04/22/2016
//
// git repository: ~{/home/stu13/s10/dgg5503/Courses/CS243/Homeworks/hw7}
//
// // // // // // // // // // // // // // // // // // // // // // // 

#ifndef DISSECTPACKETS_H
#define DISSECTPACKETS_H

// Includes
#include "utils.h"

/// Converts 2 unsigned characters into a single short in the form
/// of an unsigned int.
///
/// @param bytes  a pointer to the start of the 2 byte array.
/// @returns finalValue  an unsigned integer with equivalent magnitude of
///						 the provided bytes.
///
unsigned int uchar2UShort(unsigned char bytes[2]);

#endif

// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
