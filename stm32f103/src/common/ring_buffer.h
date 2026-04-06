
//==================================================================================================
//
// 	File Name		ring_buffer.h
//	Customer
//	Coding			V.Vu
//	History			Ver.0.01	2025.02.05	V.Vu New
//	Outline			Ring Buffer header file
//
//==================================================================================================
//==================================================================================================
//	Local Compile Option
//==================================================================================================
#ifndef RING_BUFFER_H
#    define RING_BUFFER_H

#    include "Define.h"
//==================================================================================================
//	define
//==================================================================================================
typedef struct
{
    U2 u2_Head;       /* Index of the next write position in the ring buffer */
    U2 u2_Tail;       /* Index of the current read position in the ring buffer */
    U2 u2_SizeFilled; /* Current number of bytes stored in the buffer */
    U2 u2_BufferSize; /* Total capacity of the buffer (in bytes) */
    U1* pu1_Buffer;   /* Pointer to the memory area used as the ring buffer */
} ST_RING_BUFFER;

//==================================================================================================
//	Function Prototype
//==================================================================================================
void RingBuffer_Init(ST_RING_BUFFER* apst_RingBuffer, U1* apu1_Buffer, U2 au2_Len);
// Initialize ring buffer with given memory and buffer length

void RingBuffer_Put_Data(ST_RING_BUFFER* apst_RingBuffer, U1 au1_Data);
// Write one byte of data into the ring buffer

U1 u1_RingBuffer_Get_Data(ST_RING_BUFFER* apst_RingBuffer, U1* apu1_Data);
// Read one byte from the ring buffer

U1 u1_RingBufferIsFull(ST_RING_BUFFER* apst_RingBuffer);
// Check if the ring buffer is full

U1 u1_RingBufferIsEmpty(ST_RING_BUFFER* apst_RingBuffer);
// Check if the ring buffer is empty

#endif /* RING_BUFFER_H */
/* ************************************* End of File ******************************************** */
