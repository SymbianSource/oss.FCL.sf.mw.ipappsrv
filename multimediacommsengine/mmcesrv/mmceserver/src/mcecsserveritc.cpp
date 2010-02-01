/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/




#include "mcecsserveritc.h"
#include "mceserver.pan"
#include "mcelogs.h"


const TInt KInt0 = 0;
const TInt KInt1 = 1;
const TInt KInt2 = 2;
const TInt KInt3 = 3;


// -----------------------------------------------------------------------------
// CMceCsServerITC::NewL
// -----------------------------------------------------------------------------
//
CMceCsServerITC* CMceCsServerITC::NewL ()
	{
    CMceCsServerITC* self = CMceCsServerITC::NewLC ();
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::NewLC
// -----------------------------------------------------------------------------
//
CMceCsServerITC* CMceCsServerITC::NewLC ()
	{
    CMceCsServerITC* self = new (ELeave) CMceCsServerITC;
    CleanupStack::PushL (self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::CMceCsServerITC
// -----------------------------------------------------------------------------
//
CMceCsServerITC::CMceCsServerITC ()
	{
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::~CMceCsServerITC
// -----------------------------------------------------------------------------
//
CMceCsServerITC::~CMceCsServerITC ()
	{
	}
    
// -----------------------------------------------------------------------------
// CMceCsServerITC::WriteL
// -----------------------------------------------------------------------------
//
void CMceCsServerITC::WriteL (const RMessage2& aMessage,
                              const TDesC8& aDes,
                              TMceItcArguments aItcArgIndex) const
    {
    WriteL (aItcArgIndex,aMessage,aDes);
    }

// -----------------------------------------------------------------------------
// CMceCsServerITC::WriteL
// -----------------------------------------------------------------------------
//
void CMceCsServerITC::WriteL (const RMessage2& aMessage,
                              const TMceIds& aIds) const
	{
    TPckgBuf<TMceIds> mceIdsPckg(aIds);
    WriteL (EMceItcArgIds, aMessage, mceIdsPckg);
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::WriteL
// -----------------------------------------------------------------------------
//
void CMceCsServerITC::WriteL (const RMessage2& aMessage,
                              const TMceMessageBufSizes aSizes) const
    {
    TPckgBuf<TMceMessageBufSizes> bufSizesPckg(aSizes);
    WriteL (EMceItcArgBufSizes, aMessage, bufSizesPckg);
    }

// -----------------------------------------------------------------------------
// CMceCsServerITC::ReadMCEIdsL
// -----------------------------------------------------------------------------
//
TMceIds CMceCsServerITC::ReadMCEIdsL (const RMessage2& aMessage) const
	{
    TPckgBuf<TMceIds> mceIdsPckg;
    aMessage.ReadL (EMceItcArgIds, mceIdsPckg);
    return mceIdsPckg();
	}
	
// -----------------------------------------------------------------------------
// CMceCsServerITC::ReadTUint32L
// -----------------------------------------------------------------------------
//
TUint32 CMceCsServerITC::ReadTUint32L (const RMessage2& aMessage,
	TMceItcArguments aItcArgIndex) const
	{
    TPckgBuf<TUint32> mceDataPckg;
    aMessage.ReadL (aItcArgIndex, mceDataPckg);
    return mceDataPckg();
	}
	
// -----------------------------------------------------------------------------
// CMceCsServerITC::ReadIntL
// -----------------------------------------------------------------------------
//
TInt CMceCsServerITC::ReadIntL(const RMessage2& aMessage, 
	TMceItcArguments aItcArgIndex) const
	{
	
    TInt integer = 0;
    switch (static_cast<TInt>(aItcArgIndex))
		{
        case KInt0: 
        	integer = aMessage.Int0();
        	break;
        case KInt1: 
        	integer = aMessage.Int1();
        	break;
        case KInt2: 
        	integer = aMessage.Int2();
        	break;
        case KInt3: 
        	integer = aMessage.Int3();
        	break;
        default: 
        	User::Leave(KErrNotFound);
		}
    return integer; 
	}

	
// -----------------------------------------------------------------------------
// CMceCsServerITC::ReadLC
// -----------------------------------------------------------------------------
//
HBufC8* CMceCsServerITC::ReadLC (const RMessage2&  aMessage,
                                 TMceItcArguments aItcArgIndex) const
	{
    TInt length = aMessage.GetDesLength (aItcArgIndex);
    if (length < 0)
        {
        User::Leave (KErrBadDescriptor);
        }
    HBufC8* buf = HBufC8::NewLC (length);
	if (length > 0)
		{
		TPtr8 bufPtr(buf->Des());
		aMessage.ReadL (aItcArgIndex, bufPtr);
		}
    return buf;
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::Function
// -----------------------------------------------------------------------------
//
TInt CMceCsServerITC::Function (const RMessage2& aMessage) const
	{
    return aMessage.Function();
	}
	    
// -----------------------------------------------------------------------------
// CMceCsServerITC::Complete
// -----------------------------------------------------------------------------
//
void CMceCsServerITC::Complete (const RMessage2& aMessage,
                                TInt aCompletionCode) const
	{
    aMessage.Complete (aCompletionCode);
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::PanicClient
// -----------------------------------------------------------------------------
//
void CMceCsServerITC::PanicClient(const RMessage2& aMessage, TInt aPanic) const
	{
    RThread client;
    aMessage.Client(client);
    client.Panic(KMceServerPanic, aPanic);
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::ITCArgPtr
// -----------------------------------------------------------------------------
//
const TAny* CMceCsServerITC::ITCArgPtr (TMceItcArguments aItcArgIndex,
                                        const RMessage2&  aMessage) const
	{
    const TAny* ptr;
    switch (static_cast<TInt>(aItcArgIndex))
		{
        case KInt0: ptr = aMessage.Ptr0(); break;
        case KInt1: ptr = aMessage.Ptr1(); break;
        case KInt2: ptr = aMessage.Ptr2(); break;
        case KInt3: ptr = aMessage.Ptr3(); break;
        default: ptr = 0; break;
		}
    return ptr; 
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::ITCArgInt
// -----------------------------------------------------------------------------
//
TInt CMceCsServerITC::ITCArgInt (TMceItcArguments aItcArgIndex,
                                 const RMessage2&  aMessage) const
	{
    TInt integer;
    switch (static_cast<TInt>(aItcArgIndex))
		{
        case KInt0: integer = aMessage.Int0(); break;
        case KInt1: integer = aMessage.Int1(); break;
        case KInt2: integer = aMessage.Int2(); break;
        case KInt3: integer = aMessage.Int3(); break;
        default: integer = KErrArgument; break;
		}
    return integer; 
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::WriteL
// -----------------------------------------------------------------------------
//
void CMceCsServerITC::WriteL (TMceItcArguments aItcArgIndex,
							  const RMessage2&  aMessage,
							  const TDesC8&    aDes) const
	{
    TInt length = aMessage.GetDesMaxLength(aItcArgIndex);
    if (length < 0)
        {
        User::Leave (KErrArgument);
        }
    if (length < aDes.Length())
        {
        User::Leave (KErrOverflow);
        }
    aMessage.WriteL (aItcArgIndex, aDes);
	}

// -----------------------------------------------------------------------------
// CMceCsServerITC::ReadL
// -----------------------------------------------------------------------------
//
HBufC8* CMceCsServerITC::ReadL (const RMessage2&  aMessage,
                                TMceItcArguments aItcArgIndex) const
	{
	HBufC8* buf = ReadLC(aMessage,aItcArgIndex);
    CleanupStack::Pop(buf);
	return buf;
	}
