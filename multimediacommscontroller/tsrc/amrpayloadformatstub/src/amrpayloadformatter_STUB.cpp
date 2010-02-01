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
* Description: 
*
*/

// INCLUDE FILES
#include "amrpayloadformatter.h"
#include "amrcommonutil.h"

// CONSTANTS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

 
// -----------------------------------------------------------------------------
// CAmrPayloadFormatter::CAmrPayloadFormatter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAmrPayloadFormatter::CAmrPayloadFormatter( TBool aIsNb )
  : iIsNb( aIsNb ), 
    iChannelCount( 1 ),
    iFrameBlockCount( 1 ),
    iFrameCount( iChannelCount * iFrameBlockCount ),
    iModeRequest( EAmrModeReqNone ),
    iPayloadPtr( 0, 0, 0 )
    {}
    
// Destructor
CAmrPayloadFormatter::~CAmrPayloadFormatter( )
    {
    iPayload = NULL;
    iFrameDatas.ResetAndDestroy();
    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadFormatter::Initialize
// Initialize formatter. MUST be done always when starting decoding/encoding
// possibly after pause/stop. Redundant frames collected so far are destroyed as
// well as frames received for redundancy detection.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatter::Initialize()
    {
    iFrameDatas.ResetAndDestroy();
    DoInitialize();
    }
    
// -----------------------------------------------------------------------------
// CAmrPayloadFormatter::SetPayloadBuffer
// Set payload buffer.
// -----------------------------------------------------------------------------
//
void CAmrPayloadFormatter::SetPayloadBuffer( TDes8& aBuffer )
    {
    iPayload = const_cast<TUint8*>( aBuffer.Ptr( ) );
    iPayloadSize = aBuffer.Length( );
    iPayloadPtr.Set( iPayload, aBuffer.Length( ), aBuffer.MaxLength( ) );
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatter::SetChannelCount
// Set number of audio channels.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatter::SetChannelCount( TInt aChannelCount )
    {
    if ( KMaxChannelCount < aChannelCount || aChannelCount < 1 )
        {
        return KErrArgument;
        }

    iChannelCount = aChannelCount;
    iFrameCount = iChannelCount * iFrameBlockCount;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatter::SetFramesBlockCount
// Set number of AMR frame blocks included in one RTP packet.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatter::SetFrameBlockCount( TInt aFrameblockCount )
    {
    if ( KMaxFrameBlocksPerPacket < aFrameblockCount || aFrameblockCount < 1 )
        {
        return KErrArgument;
        }

    iFrameBlockCount = aFrameblockCount;
    iFrameCount = iChannelCount * iFrameBlockCount;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CAmrPayloadFormatter::SpeechBitCount
// Get number of speech bits in an AMR frame.
// -----------------------------------------------------------------------------
//
TInt CAmrPayloadFormatter::SpeechBitCount( enum TAmrFrameType aFrameType ) const
    {
    if ( iIsNb )
    	{
	    if ( aFrameType > EAmrFrameSid)
	        {
	        return 0;
	        }
		else        
	        {
		    return KAmrNbTotalSpeechBits[TInt( aFrameType )];
	        }
    	}
    else
    	{
	    if ( aFrameType > EAmrWbFrameSid)
	        {
	        return 0;
	        }
		else        
	        {
		    return KAmrWbTotalSpeechBits[TInt( aFrameType )];
	        }
    	}
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
