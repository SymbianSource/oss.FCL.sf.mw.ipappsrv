/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Interface proxy for speech encoder configuration.
*
*/




// INCLUDE FILES
#include "SpeechEncoderConfigProxy_STUB.h"
//#include "SpeechEncoderConfigMsgs.h"
#include <CustomCommandUtility.h>
#include <CustomInterfaceUtility.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CSpeechEncoderConfigProxy_STUB::CSpeechEncoderConfigProxy_STUB
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSpeechEncoderConfigProxy_STUB::CSpeechEncoderConfigProxy_STUB()
    {
    }


// -----------------------------------------------------------------------------
// CSpeechEncoderConfigProxy_STUB::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSpeechEncoderConfigProxy_STUB::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSpeechEncoderConfigProxy_STUB::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSpeechEncoderConfigProxy_STUB* CSpeechEncoderConfigProxy_STUB::NewL()
    {
    CSpeechEncoderConfigProxy_STUB* self = new(ELeave) CSpeechEncoderConfigProxy_STUB();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// Destructor
EXPORT_C CSpeechEncoderConfigProxy_STUB::~CSpeechEncoderConfigProxy_STUB()
    {
    }

// ---------------------------------------------------------
// CSpeechEncoderConfigProxy_STUB::GetSupportedBitrates
// Calls a  subfunction which sends the appropriate custom command for this
// function to its message handler. A subfunction is used
// to contain multiple leaving functions for a single trap.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CSpeechEncoderConfigProxy_STUB::GetSupportedBitrates(
                                           RArray<TUint>& aSupportedBitrates)
    {
    TInt status;
    TRAP(status, GetSupportedBitratesL(aSupportedBitrates));
    return status;
    }

// ---------------------------------------------------------
// CSpeechEncoderConfigProxy_STUB::GetSupportedBitratesL
// Sends the custom command for this function to its message handler.
// This requires two commands. The first is a request for the number of
// supported bitrates. A buffer is allocated locally to hold this number of
// bitrates that will be returned. A pointer to this buffer is sent with the
// next command which is a request for the bitrates. This buffer will be filled
// with the bitrate values. These bitrates are then copied into the array
// provided to this function and the local buffer is deleted.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSpeechEncoderConfigProxy_STUB::GetSupportedBitratesL(
                                            RArray<TUint>& /*aSupportedBitrates*/)
    {
    }

// ---------------------------------------------------------
// CSpeechEncoderConfigProxy_STUB::SetBitrate
// Sends the custom command for this function to its message handler.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CSpeechEncoderConfigProxy_STUB::SetBitrate(TUint /*aBitrate*/)
	{
	return KErrNone;
	}

// ---------------------------------------------------------
// CSpeechEncoderConfigProxy_STUB::GetBitrate
// Sends the custom command for this function to its message handler.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CSpeechEncoderConfigProxy_STUB::GetBitrate(TUint& /*aBitrate*/)
	{
	return KErrNone;
	}

// ---------------------------------------------------------
// CSpeechEncoderConfigProxy_STUB::SetVadMode
// Sends the custom command for this function to its message handler.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CSpeechEncoderConfigProxy_STUB::SetVadMode(TBool /*aVadMode*/)
	{
	return KErrNone;
	}

// ---------------------------------------------------------
// CSpeechEncoderConfigProxy_STUB::GetVadMode
// Sends the custom command for this function to its message handler.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CSpeechEncoderConfigProxy_STUB::GetVadMode(TBool& /*aVadMode*/)
	{
	return KErrNone;
	}


// End of File
