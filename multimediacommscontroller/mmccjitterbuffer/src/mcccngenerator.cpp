/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Comfort noise generator of MCC jitterbuffer
*
*/




// INCLUDE FILES
#include <ErrorConcealmentIntfc.h>
#include "mmcccodecinformation.h"
#include "mcccngenerator.h"
#include "mccjitterbufferlogs.h"
#include "mccinternaldef.h"

// CONSTANTS

// When the remote end sends us SID/CN frames, then we can update the SID/CN
// parameters programmatically. Otherwise when we have not received any SID/CN
// frames we should not try to "emulate" SID/CN frames. This can mess up the 
// decoder which could lead into a undesired artefacts in audio quality.
//
// Basically this means that if we have not aquired SID/CN frames
// programmatically we will send NULL/NO_DATA data into the decoder.

// AMR-NB SID FT value is 8 and is located in the upper 4 bits
// See 3GPP TS 26.101
const TUint8 KAmrSidMode = 8;

// AMR-WB SID FT value is 9 and is located in the upper 4 bits
// See 3GPP TS 26.101
const TUint8 KAmrWbSidMode = 9;

// AMR-NB mode mask
const TUint8 KAmrModeMask = 0x78;

// AMR-NB SID Update frame interval
const TInt KAmrNbSidUpdateInterval = 8;

// Minimum required data length for DTX decision
const TInt KMinDataLenForDtx = 1;

// AMR NO_DATA frame needs one byte
// See 3GPP TS 26.101 and RFC 2367
//  ________________
// |0|1|1|1|1|1|0|0|
// |P|  FT   |Q|P|P|
//
//  P = PADDING
//  FT = 15 = NO_DATA
//  Q = QUALITY
const TUint8 KAmrNoDataFrame = 0x7C;

// Length of the header and AMR NO_DATA frames
const TInt KNoDataLength = 1;

// AMR mode solving needs 3 bit shift
const TInt KModeShiftBits = 3;

#if ( defined __WINSCW__ ) || ( defined __WINS__ ) || ( defined TEST_EUNIT )
#define MCC_CN_GEN_DISABLE_CONCEALMENT
#endif   



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCnGenerator::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccCnGenerator* CMccCnGenerator::NewL( const TFourCC& aAudioType, 
        CMMFDevSound& aDevSound )
    {
    __CN_GEN( "CMccCnGenerator::NewL IN" )

    CMccCnGenerator* self = new ( ELeave ) CMccCnGenerator( aAudioType, 
                                                            aDevSound );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    __CN_GEN( "CMccCnGenerator::NewL OUT" )

    return self;
    }

// -----------------------------------------------------------------------------
// CMccCnGenerator::CMccCnGenerator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccCnGenerator::CMccCnGenerator( const TFourCC& aAudioType, 
    CMMFDevSound& aDevSound ) : iAudioType( aAudioType ), iDevSound( aDevSound )
    {

    }

// -----------------------------------------------------------------------------
// CMccCnGenerator::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccCnGenerator::ConstructL()
    {
    __CN_GEN( "CMccCnGenerator::ConstructL" )
    
    #ifdef MCC_CN_GEN_DISABLE_CONCEALMENT
        __CN_GEN( "CMccCnGenerator::ConstructL Please integrate MMF Cust IF's to emulator" )
    #else
        iErrorConcealer = CErrorConcealmentIntfc::NewL( iDevSound );
    #endif

    // Check the codec
    switch ( iAudioType.FourCC() )
        {
        case KMccFourCCIdAMRNB:
        case KMccFourCCIdAMRWB:
        case KMccFourCCIdG711:
        case KMccFourCCIdILBC:
        case KMccFourCCIdG729:
            break;
        default:
            __CN_GEN( "CMccCnGenerator::ConstructL UNKNOWN CODEC" )
            
            User::Leave( KErrNotSupported );
            break;
        }
    }

// -----------------------------------------------------------------------------
// CMccCnGenerator::~CMccCnGenerator
// Destructor
// -----------------------------------------------------------------------------
//
CMccCnGenerator::~CMccCnGenerator()
    {
    delete iErrorConcealer;
    }
    
// -----------------------------------------------------------------------------
// CMccCnGenerator::GenerateSidPacketL
// Generate Sid Packet
// -----------------------------------------------------------------------------
//
void CMccCnGenerator::GenerateSidPacket( TDes8& aPayload, TInt aRequestSize )
    {
    if ( aRequestSize > aPayload.MaxLength() )
        {
        __CN_GEN( "CMccCnGenerator::GenerateSidPacket, adjust request size" )
        aRequestSize = aPayload.MaxLength();
        }
        
    iGeneratedCnFrames++;
    
    __CN_GEN_INT1( "CMccCnGenerator::GenerateSidPacket iGeneratedCnFrames: ",
        iGeneratedCnFrames )
    
    #ifdef MCC_CN_GEN_DISABLE_CONCEALMENT
    
    // Fill with zeroes in emulator
    aPayload.SetMax();
    aPayload.FillZ();
    // WINSCW compile...
    aRequestSize = aRequestSize;
    
    #else // Real HW
    
    if ( KMccFourCCIdAMRNB == iAudioType.FourCC() )
        {
        this->GenerateAmrNoDataPacket( aPayload );
        if( KAmrNbSidUpdateInterval == iGeneratedCnFrames )
            {
            // We've not received an SID_UPDATE for 8th frame
            // this means that it has been lost, so this means packet loss
            iDtxPeriodStarted = EFalse;
            }
        }
    else if ( KMccFourCCIdAMRWB == iAudioType.FourCC() )
        {
        // Using temporarily pre-generated silent data 
        // 04,10,21,00,39,1d,37,d4,91,74,7c,c2,78,e8,e0,88,e2,e0
        // as no data frame causes decoder crash
        //
        const TInt KSilenceDataLength = 18;
        if ( aRequestSize >= KSilenceDataLength )
            {
        	__CN_GEN( "CMccCnGenerator::GenerateSidPacket, amr wb, do silence frame" )
        	const TUint8 KAmrSilenceFrameByte1 = 0x04;
        	const TUint8 KAmrSilenceFrameByte2 = 0x10;
        	const TUint8 KAmrSilenceFrameByte3 = 0x21;
        	const TUint8 KAmrSilenceFrameByte4 = 0x00;
        	const TUint8 KAmrSilenceFrameByte5 = 0x39;
        	const TUint8 KAmrSilenceFrameByte6 = 0x1d;
        	const TUint8 KAmrSilenceFrameByte7 = 0x37;
        	const TUint8 KAmrSilenceFrameByte8 = 0xd4;
        	const TUint8 KAmrSilenceFrameByte9 = 0x91;
        	const TUint8 KAmrSilenceFrameByte10 = 0x74;
        	const TUint8 KAmrSilenceFrameByte11 = 0x7c;
        	const TUint8 KAmrSilenceFrameByte12 = 0xc2;
        	const TUint8 KAmrSilenceFrameByte13 = 0x78;
        	const TUint8 KAmrSilenceFrameByte14 = 0xe8;
        	const TUint8 KAmrSilenceFrameByte15 = 0xe0;
        	const TUint8 KAmrSilenceFrameByte16 = 0x88;
        	const TUint8 KAmrSilenceFrameByte17 = 0xe2;
        	const TUint8 KAmrSilenceFrameByte18 = 0xe0;
        	aPayload.Copy( &KAmrSilenceFrameByte1, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte2, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte3, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte4, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte5, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte6, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte7, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte8, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte9, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte10, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte11, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte12, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte13, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte14, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte15, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte16, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte17, KNoDataLength );
        	aPayload.Append( &KAmrSilenceFrameByte18, KNoDataLength );
            }
        else
            {
        	__CN_GEN( "CMccCnGenerator::GenerateSidPacket, amr wb, do dummy frame" )
        	aPayload.SetLength( aRequestSize );
            aPayload.FillZ();
        	}
        
        if ( KAmrNbSidUpdateInterval == iGeneratedCnFrames )
            {
            // We've not received an SID_UPDATE for 8th frame
            // this means that it has been lost, so this means packet loss
            iDtxPeriodStarted = EFalse;
            }
        }
    else
        {
        this->GenerateVoIPNoDataPacket( aPayload, aRequestSize );
        }
        
    #endif
    
    __CN_GEN( "CMccCnGenerator::GenerateSidPacket, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCnGenerator::GenerateAmrNoDataPacket
// Generates a AMR SID packet
// -----------------------------------------------------------------------------
//
void CMccCnGenerator::GenerateAmrNoDataPacket( TDes8& aPayload ) const
    {
    __CN_GEN( "CMccCnGenerator::GenerateAmrNoDataPacket" )
    
    if ( aPayload.MaxLength() >= KNoDataLength )
        {
        aPayload.Copy( &KAmrNoDataFrame, KNoDataLength );
        }
    };

// -----------------------------------------------------------------------------
// CMccCnGenerator::GenerateVoIPNoDataPacketL
// Generates a VoIP NO_DATA packet
// -----------------------------------------------------------------------------
//
void CMccCnGenerator::GenerateVoIPNoDataPacket( TDes8& aPayload,
    TInt aRequestSize ) const
    {
    __CN_GEN( "CMccCnGenerator::GenerateVoIPNoDataPacket" )
    
    aPayload.FillZ( aRequestSize );
    
    if ( !iDtxPeriodStarted )
        {
        ConcealErrorForNextFrame();
        }
    };

// -----------------------------------------------------------------------------
// CMccCnGenerator::ConcealErrorForNextFrame
// Conceals an error for next frame
// -----------------------------------------------------------------------------
//
void CMccCnGenerator::ConcealErrorForNextFrame() const
    {
    #ifdef MCC_CN_GEN_DISABLE_CONCEALMENT
    
    __CN_GEN( "CMccCnGenerator::ConcealErrorForNextFrame EMULATOR BUILD" )
    
    #else
    
    // If EC instance is available, otherwise ignore silently
    if( iErrorConcealer )
        {
        __CN_GEN( "CMccCnGenerator::ConcealErrorForNextFrame" )
        
        // Ignore return code, just keep going
        iErrorConcealer->ConcealErrorForNextBuffer();
        }
        
    #endif
    }

// -----------------------------------------------------------------------------
// CMccCnGenerator::DoDtxDecision()
// Do the DTX decision based on used codec and buffer data
// -----------------------------------------------------------------------------
//
void CMccCnGenerator::DoDtxDecision( const TDesC8& aData )
    {       
    if ( KMinDataLenForDtx < aData.Length() )
        {
        if ( this->IsSidBuffer( aData ) )
            {
            // DTX period started or updated
            __CN_GEN( "CMccCnGenerator::DoDtxDecision DTX_START" )
            
            iDtxPeriodStarted = ETrue;
            iGeneratedCnFrames = 0;
            }
        else
            {
            // DTX period has ended
            iDtxPeriodStarted = EFalse;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccCnGenerator::IsSidBufferL()
// Is the given buffer a SID UPDATE buffer
// -----------------------------------------------------------------------------
//    
TBool CMccCnGenerator::IsSidBuffer( const TDesC8& aData ) const
    {
    if ( iAudioType == KMccFourCCIdAMRNB )
        {
        // Get AMR mode masking the bits in the first byte and shifting
        const TUint8 mode( ( aData[0] & KAmrModeMask ) >> KModeShiftBits );
        if( KAmrSidMode == mode )
            {
            __CN_GEN( "CMccCnGenerator::IsSidBufferL AMR-NB ETrue" )
            return ETrue; 
            }
        else
            {
            __CN_GEN( "CMccCnGenerator::IsSidBufferL AMR-NB EFalse" )
            return EFalse;
            }
        }
    else if ( iAudioType == KMccFourCCIdAMRWB )
        {
        // Get AMR mode masking the bits in the first byte and shifting
        const TUint8 mode( ( aData[0] & KAmrModeMask ) >> KModeShiftBits );
        if( KAmrWbSidMode == mode )
            {
            __CN_GEN( "CMccCnGenerator::IsSidBufferL AMR-WB ETrue" )
            return ETrue; 
            }
        else
            {
            __CN_GEN( "CMccCnGenerator::IsSidBufferL AMR-WB EFalse" )
            return EFalse;
            }
        }
    else if( iAudioType == KMccFourCCIdG711 || iAudioType == KMccFourCCIdILBC
            || iAudioType == KMccFourCCIdG729 )
        {
        // Check the frame type
        if ( KVoIPCNFrame == aData[0] )
            {
            __CN_GEN( "CMccCnGenerator::IsSidBufferL VOIP ETrue" )
            
            return ETrue;
            }
        else
            {
            __CN_GEN( "CMccCnGenerator::IsSidBufferL VOIP EFalse" )
            
            return EFalse;
            }
        }
    else
        {
        __CN_GEN( "CMccCnGenerator::IsSidBufferL UNKNOWN" )
        
        return EFalse;
        }
    }
        
//  End of File  
