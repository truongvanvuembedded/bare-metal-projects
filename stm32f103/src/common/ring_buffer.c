//==================================================================================================
//
//	File Name	:	ring_buffer.c
//	CPU Type	:
//	IDE			:
//	Customer
//	Version		:	Ver.0.01
//	Coding		:	v.Vu
//	History		:	14/05/2025
//	Outline		:
//
//==================================================================================================
//==================================================================================================
//	#pragma section
//==================================================================================================

//==================================================================================================
//	Local Compile Option
//==================================================================================================

//==================================================================================================
//	Header File
//==================================================================================================
#include "ring_buffer.h"
#include "Define.h"
//==================================================================================================
//	Local define
//==================================================================================================

//==================================================================================================
//	Local define I/O
//==================================================================================================

//==================================================================================================
//	Local Struct Template
//==================================================================================================

//==================================================================================================
//	Local RAM
//==================================================================================================

//==================================================================================================
//	Local ROM
//==================================================================================================

//==================================================================================================
//	Local Function Prototype
//==================================================================================================

//==================================================================================================
//	Source Code
//==================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name		:	RingBuffer_Init
//	Function	:	Initializes the ring buffer structure
//
//	Argument	:	ST_RING_BUFFER *apst_RingBuffer	- pointer to the ring buffer structure
//					U1 *apu1_Buffer					- pointer to the buffer memory
//					U2 au2_Len						- size of the buffer
//	Return		:	void
//	Created		:	14/05/2025
//	Changed		:	-
//	Remarks		:	Sets up initial indices and links the buffer memory to the structure
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void RingBuffer_Init(ST_RING_BUFFER* apst_RingBuffer, U1* apu1_Buffer, U2 au2_Len)
{
    apst_RingBuffer->u2_Head = U2MIN;
    apst_RingBuffer->u2_Tail = U2MIN;
    apst_RingBuffer->u2_BufferSize = au2_Len;
    apst_RingBuffer->pu1_Buffer = apu1_Buffer;
    apst_RingBuffer->u2_SizeFilled = U2MIN;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name		:	RingBuffer_Put_Data
//	Function	:	Writes one byte to the ring buffer
//
//	Argument	:	ST_RING_BUFFER *apst_RingBuffer	- pointer to the ring buffer structure
//					U1 au1_Data						- data byte to be written
//	Return		:	void
//	Created		:	14/05/2025
//	Changed		:	-
//	Remarks		:	If the buffer is full, the oldest data will be overwritten
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void RingBuffer_Put_Data(ST_RING_BUFFER* apst_RingBuffer, U1 au1_Data)
{
    U1 au1_RingBuffer_Full;
    U2 au2_NextHead;
    U2 au2_NextTail;

    apst_RingBuffer->pu1_Buffer[apst_RingBuffer->u2_Head] = au1_Data;
    au2_NextHead = (apst_RingBuffer->u2_Head + (U2)1) % apst_RingBuffer->u2_BufferSize;
    apst_RingBuffer->u2_Head = au2_NextHead;

    au1_RingBuffer_Full = u1_RingBufferIsFull(apst_RingBuffer);
    if (au1_RingBuffer_Full == U1TRUE)
    {
        au2_NextTail = (apst_RingBuffer->u2_Tail + (U2)1) % apst_RingBuffer->u2_BufferSize;
        apst_RingBuffer->u2_Tail = au2_NextTail;
    }
    else
    {
        apst_RingBuffer->u2_SizeFilled++;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name		:	u1_RingBuffer_Get_Data
//	Function	:	Reads one byte from the ring buffer
//
//	Argument	:	ST_RING_BUFFER *apst_RingBuffer	- pointer to the ring buffer structure
//					U1 *apu1_Data					- pointer to store the read byte
//	Return		:	U1								- U1TRUE if successful, U1FALSE if buffer is empty
//	Created		:	14/05/2025
//	Changed		:	-
//	Remarks		:	Decreases size counter after reading
//
////////////////////////////////////////////////////////////////////////////////////////////////////
U1 u1_RingBuffer_Get_Data(ST_RING_BUFFER* apst_RingBuffer, U1* apu1_Data)
{
    U1 au1_RingBuffer_Empty;
    U2 au2_Tail;
    au1_RingBuffer_Empty = u1_RingBufferIsEmpty(apst_RingBuffer);
    if (au1_RingBuffer_Empty == U1TRUE)
    {
        return U1FALSE;
    }
    apst_RingBuffer->u2_SizeFilled--;
    au2_Tail = apst_RingBuffer->u2_Tail;
    *apu1_Data = apst_RingBuffer->pu1_Buffer[au2_Tail];
    apst_RingBuffer->u2_Tail = (apst_RingBuffer->u2_Tail + (U2)1) % apst_RingBuffer->u2_BufferSize;
    return U1TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name		:	u1_RingBufferIsFull
//	Function	:	Checks if the ring buffer is full
//
//	Argument	:	ST_RING_BUFFER *apst_RingBuffer	- pointer to the ring buffer structure
//	Return		:	U1								- U1TRUE if full, otherwise U1FALSE
//	Created		:	14/05/2025
//	Changed		:	-
//	Remarks		:	Used to prevent overwriting data unintentionally
//
////////////////////////////////////////////////////////////////////////////////////////////////////
U1 u1_RingBufferIsFull(ST_RING_BUFFER* apst_RingBuffer)
{
    if (apst_RingBuffer->u2_SizeFilled >= apst_RingBuffer->u2_BufferSize)
    {
        return U1TRUE;
    }
    return U1FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name		:	u1_RingBufferIsEmpty
//	Function	:	Checks if the ring buffer is empty
//
//	Argument	:	ST_RING_BUFFER *apst_RingBuffer	- pointer to the ring buffer structure
//	Return		:	U1								- U1TRUE if empty, otherwise U1FALSE
//	Created		:	14/05/2025
//	Changed		:	-
//	Remarks		:	Should be checked before reading from the buffer
//
////////////////////////////////////////////////////////////////////////////////////////////////////
U1 u1_RingBufferIsEmpty(ST_RING_BUFFER* apst_RingBuffer)
{
    if (apst_RingBuffer->u2_SizeFilled == U2MIN)
    {
        return U1TRUE;
    }
    return U1FALSE;
}