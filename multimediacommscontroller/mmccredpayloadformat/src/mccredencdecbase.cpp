/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Base class for redundancy payload encoder and decoder
*
*/





// INCLUDE FILES
#include    <mmf/server/mmfdatabuffer.h>
#include    "mccredencdecbase.h"
#include    "mccredpayloadformatdefs.h"

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRedEncDecBase::CMccRedEncDecBase
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRedEncDecBase::CMccRedEncDecBase()
    :
    iRedBlockCount( KDefRedCount )
    {
    DP_RED_ENCODE( "CMccRedEncDecBase::CMccRedEncDecBase" )
    }

// Destructor
CMccRedEncDecBase::~CMccRedEncDecBase()
    {
    DP_RED_ENCODE( "CMccRedEncDecBase::~CMccRedEncDecBase" )
    iEncodings.DeleteAll();
    delete iRTPPayload;
    }

// -----------------------------------------------------------------------------
// CMccRedEncDecBase::SetEncodingBlockL
// Set encoding, which is wanted to encode/decode.
// SetMaxPayloadSizeL() MUST be called before coming here.
// -----------------------------------------------------------------------------
//
void CMccRedEncDecBase::SetEncodingBlockL( TMccRedEncoding aEncoding,
                                           TDesC8& aEncodingBuf )
    {
    DP_RED_ENCODE2( "CMccRedEncDecBase::SetEncodingBlock() - \
        PARAMBUF SIZE: %d", aEncodingBuf.Size() )
    
    if ( EMccRTPPayload == aEncoding )
        {
        __ASSERT_ALWAYS( iRTPPayload, User::Leave( KErrNotReady ) );
        
        DP_RED_ENCODE2( "MAX RTP Payload SIZE to ACCEPT: %d", \
            iRTPPayload->Data().MaxSize() )
        
        TDes8& rtpPayload = iRTPPayload->Data();
        rtpPayload.Copy( aEncodingBuf );
        }
    else
        {
        __ASSERT_ALWAYS( aEncoding < iEncodings.Count(), 
            User::Leave( KErrArgument ) );
        __ASSERT_ALWAYS( iEncodings[ aEncoding ], User::Leave( KErrNotReady ) );
        
        TDes8& encoding = iEncodings[ aEncoding ]->Data();
        DP_RED_ENCODE2( "MAX Encoding SIZE to ACCEPT: %d", \
            encoding.MaxSize() )
        
        encoding.Copy( aEncodingBuf );
        }
    }
  
// -----------------------------------------------------------------------------
// CMccRedEncDecBase::GetEncodingBlockL
// Get encoding specified with parameter.
// -----------------------------------------------------------------------------
//                                   
void CMccRedEncDecBase::GetEncodingBlockL( TMccRedEncoding aEncoding, 
                                           TDes8& aToBuffer )
    {
    DP_RED_ENCODE2( "CMccRedEncDecBase::GetEncodingBlock() - \
        PARAMBUFSIZE: %d", aToBuffer.MaxSize() )
    
    if ( EMccRTPPayload == aEncoding )
        {
        __ASSERT_ALWAYS( iRTPPayload, User::Leave( KErrNotReady ) );
        DP_RED_ENCODE2( "RTP Payload SIZE: %d", iRTPPayload->Data().Size() )
        aToBuffer.Copy( iRTPPayload->Data() );
        }
    else
        {
        __ASSERT_ALWAYS( aEncoding < iEncodings.Count(), 
            User::Leave( KErrArgument ) );
        
        DP_RED_ENCODE2( "Encoding SIZE: %d", \
            iEncodings[ aEncoding ]->Data().Size() )
        
        aToBuffer.Copy( iEncodings[ aEncoding ]->Data() );
        }        
    }
        

// -----------------------------------------------------------------------------
// CMccRedEncDecBase::SetPayloadTypes
// Set payload types to wait for. Issue payload not defined values for unused
// encodings.
// -----------------------------------------------------------------------------
//        
TInt CMccRedEncDecBase::SetPayloadTypes( RArray<TUint>& aPayloadTypes )
    {
    TInt ptCount( aPayloadTypes.Count() );
    
    DP_RED_ENCODE2( "CMccRedEncDecBase::SetPayloadTypes() - PTs: %d",
        ptCount )
        
    TInt maxNumOfPTs( iPayloadTypes.Count() );
    
    if ( ptCount && ptCount <= maxNumOfPTs )
        {
        TInt payloadInd( 0 );
        
        for ( ; payloadInd < ptCount; payloadInd++ )
            {
            iPayloadTypes[payloadInd] = 
                static_cast<TUint8>( aPayloadTypes[payloadInd] );
            }

        for ( ; payloadInd < maxNumOfPTs; payloadInd++ )
            {
            iPayloadTypes[payloadInd] = KPayloadNotDefined;
            }
        
        return KErrNone;
        }
    else
        {
        return KErrArgument;    
        }
    }


// -----------------------------------------------------------------------------
// CMccRedEncDecBase::InitializeL
// Reserve space for buffers and reset variables.
// -----------------------------------------------------------------------------
//  
void CMccRedEncDecBase::InitializeL( TInt aRedBlockCount, TInt aMaxPayloadSize,
    TInt aNumOfEncodings )
    {
    DP_RED_ENCODE4( "CMccRedEncDecBase::InitializeL - RedBC: %d, MaxPS: %d, \
    NumOfEncs: %d", aRedBlockCount, aMaxPayloadSize, aNumOfEncodings )
    __ASSERT_ALWAYS( aNumOfEncodings <= iEncodings.Count(), 
        User::Leave( KErrArgument ) );
        
    TInt redHeadersSize( aRedBlockCount * KRedHeaderSize + KFinalHeaderSize );
    iMaxPayloadSize 
        = ( aRedBlockCount + 1 ) * ( aMaxPayloadSize ) + redHeadersSize;
            
    // Reserve space for RTP payload, which includes redundancy
    delete iRTPPayload;
    iRTPPayload = NULL;
    iRTPPayload = CMMFDataBuffer::NewL( iMaxPayloadSize );
    DP_RED_ENCODE2( "Redundancy payload MAX size: %d", iMaxPayloadSize )
    
    // Reserve memory for payload for number of encodings used
    TUint encBufSize( aMaxPayloadSize * ( aRedBlockCount + 1 ) );
    for ( TInt i = 0; i < aNumOfEncodings; i++ )
        {
        if ( iEncodings[i] )
            {
            if ( iEncodings[i]->BufferSize() < encBufSize )
                {
                delete iEncodings[i];
                iEncodings[i] = NULL;
                iEncodings[i] = CMMFDataBuffer::NewL( encBufSize );
                }
            }
        else
            {
            iEncodings[i] = CMMFDataBuffer::NewL( encBufSize );
            }            
        }

    return DoInitializeL( aRedBlockCount, aMaxPayloadSize, aNumOfEncodings );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================


//  End of File  
