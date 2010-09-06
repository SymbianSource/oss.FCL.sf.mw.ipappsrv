/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mcecommessagecodec.h"
#include "mcemsrpcodec.h"  

#include "mceserial.h"
#include "mceevents.h" 

#include "mcecomrtpsource.h"
#include "mcertpsource.h"
#include "mcecommsrpsource.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <MmccCodecInformation.h>
#include "mcesrvstream.h"
#include "mcesrvsource.h"
#include "mcemsrpsource.h"
#include "mcecommsrpsource.h"
#include "mcemsrpsink.h"
#include "mcecommsrpsink.h"

#endif//MCE_COMMON_SERVER_SIDE


// ============================ MEMBER FUNCTIONS ===============================

    
// -----------------------------------------------------------------------------
// CMceComMessageCodec::NewL
// -----------------------------------------------------------------------------
//
CMceComMessageCodec* CMceComMessageCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComMessageCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceComMessageCodec::NewLC
// -----------------------------------------------------------------------------
//
CMceComMessageCodec* CMceComMessageCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComMessageCodec* self = new (ELeave) CMceComMessageCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComMessageCodec::~CMceComMessageCodec
// -----------------------------------------------------------------------------
//
CMceComMessageCodec::~CMceComMessageCodec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceComMessageCodec::CMceComMessageCodec
// -----------------------------------------------------------------------------
//
CMceComMessageCodec::CMceComMessageCodec()
  : CMceComCodec(),
    iEnableVAD( EFalse ),
    iSamplingFreq( 0 ),
    iPTime( 0 ),
    iMaxPTime( 0 )
    {
    }


// -----------------------------------------------------------------------------
// CMceComMessageCodec::ConstructL
// -----------------------------------------------------------------------------
//
CMceComMessageCodec* CMceComMessageCodec::CloneL()
    {
    CMceComMessageCodec* copy = new (ELeave) CMceComMessageCodec();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComMessageCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceComCodec::ConstructL( aSdpName );
    }

// -----------------------------------------------------------------------------
// CMceComMessageCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::ConstructL( CMceComMessageCodec& aCodec )
    {
    CMceComCodec::ConstructL( aCodec );
    
    iEnableVAD = aCodec.iEnableVAD;
	iSamplingFreq = aCodec.iSamplingFreq;	
	iPTime = aCodec.iPTime;
	iMaxPTime = aCodec.iMaxPTime;
    
    }



// -----------------------------------------------------------------------------
// CMceComMessageCodec::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComCodec::InternalizeFlatL( aReadStream );
    
    iEnableVAD = static_cast<TBool>( aReadStream.ReadUint8L() );
	iSamplingFreq = aReadStream.ReadUint16L();
	iPTime = aReadStream.ReadUint16L();
	iMaxPTime = aReadStream.ReadUint16L();
    }
    
// -----------------------------------------------------------------------------
// CMceComMessageCodec::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComCodec::ExternalizeFlatL( aWriteStream );
    
    aWriteStream.WriteUint8L( iEnableVAD );
    aWriteStream.WriteUint16L( iSamplingFreq );
    aWriteStream.WriteUint16L( iPTime );
    aWriteStream.WriteUint16L( iMaxPTime );  
    }



// -----------------------------------------------------------------------------
// CMceComMessageCodec::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::UpdateL( CMceComCodec& aUpdate )
    {
    CMceComCodec::UpdateL( aUpdate );

    CMceComMessageCodec& update = static_cast<CMceComMessageCodec&>( aUpdate );

    iEnableVAD = update.iEnableVAD;
    iSamplingFreq = update.iSamplingFreq;
    
    
    if ( IS_RECEIVESTREAM( iStream ) )
        {
        iPTime = update.iPTime;
        iMaxPTime = update.iMaxPTime;
        }
    }


// -----------------------------------------------------------------------------
// CMceComMessageCodec::Factory
// -----------------------------------------------------------------------------
//
TMceComMessageCodecFactory CMceComMessageCodec::Factory()
    {
    return TMceComMessageCodecFactory();
    }


// -----------------------------------------------------------------------------
// CMceComMessageCodec::CompareSdpIndex
// -----------------------------------------------------------------------------
//
TInt CMceComMessageCodec::CompareSdpIndex( 
    const CMceComMessageCodec& aIndex1, const CMceComMessageCodec& aIndex2 )
    {
    // NOTE: if zero (equals) is returned from here, order is strangely anyway
    // changed. Returning positive value if indexes are equal is for avoiding
    // this quirk.

    if ( aIndex1.iCodecSdpIndex >= aIndex2.iCodecSdpIndex )
        {
        return (1);
        }
    else if ( aIndex1.iCodecSdpIndex < aIndex2.iCodecSdpIndex )
        {
        return (-1);
        }
    else
        {
        return (0);
        }
    } 
    
#ifdef MCE_COMMON_SERVER_SIDE


// -----------------------------------------------------------------------------
// CMceComMessageCodec::operator=
// -----------------------------------------------------------------------------
//
CMceComCodec& CMceComMessageCodec::operator=( CMccCodecInformation& aMccCodec )
    {

	iPayloadType = aMccCodec.PayloadType();
	iBitrate = aMccCodec.Bitrate();
	iAllowedBitrates = aMccCodec.AllowedBitrates();
	iCodecMode = aMccCodec.CodecMode();
	iFourCC = aMccCodec.FourCC();
	iFrameSize = aMccCodec.FrameSize();

    iEnableVAD = aMccCodec.VAD();
	iSamplingFreq = aMccCodec.SamplingFreq();
	iPTime = aMccCodec.PTime();
	iMaxPTime = aMccCodec.MaxPTime();

    return *this;
    
    }
    
// -----------------------------------------------------------------------------
// CMceComMessageCodec::DoMccDecodeL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::DoMccDecodeL( CMccCodecInformation& aMccCodec )
    {
    //copy values, which are not explicitely in SDP, from mcc codec
	iSamplingFreq = aMccCodec.SamplingFreq();
	
	iBitrate = aMccCodec.Bitrate();
    }

// -----------------------------------------------------------------------------
// CMceComMessageCodec::DoMccEncodeL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::DoMccEncodeL( CMccCodecInformation& aMccCodec,
                                      CMceSrvStream& /*aStream*/ )
    {
	User::LeaveIfError( aMccCodec.SetSamplingFreq( iSamplingFreq ) );
	User::LeaveIfError( aMccCodec.SetBitrate( iBitrate ) );
    }


// -----------------------------------------------------------------------------
// CMceComMessageCodec::DoMccValidateL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComMessageCodec::DoMccValidateL( CMccCodecInformation& aMccCodec,
                                                 CMceSrvStream& /*aStream*/,
                                                 TMceNegotiationRole aRole )
    {
    //if answerer mcc codec represents the received offer and codec the answer
    //if offerer codec represents the sent offer and mcc codec the answer
    TBool notValid = aRole == EMceRoleAnswerer ?
                  ( iAllowedBitrates && aMccCodec.AllowedBitrates() == 0 ||
	                iAllowedBitrates > aMccCodec.AllowedBitrates() ) :
                  ( iAllowedBitrates && aMccCodec.AllowedBitrates() == 0 ||
	                iAllowedBitrates < aMccCodec.AllowedBitrates() );
	
	User::LeaveIfError( notValid ? KErrNotSupported : KErrNone );
	
	return NULL;  
    }
    
// -----------------------------------------------------------------------------
// CMceComMessageCodec::DoMccRequireSignalling
// -----------------------------------------------------------------------------
//
TInt CMceComMessageCodec::DoMccRequireSignalling( 
    const CMceSrvStream& aStream,
    const CMccCodecInformation& aMccCurentCodec, 
    const CMccCodecInformation& aMccUpdateCodec ) const
    {
    TInt action = KMceNoSignalling;

    if ( aStream.StreamType() == CMceComMediaStream::EReceiveStream ||
         aStream.StreamType() == CMceComMediaStream::EReceiveOnlyStream )
        {
        if ( aMccCurentCodec.PTime() != aMccUpdateCodec.PTime() ||
             aMccCurentCodec.MaxPTime() != aMccUpdateCodec.MaxPTime() )
            {
            action = KMceRequiresSignalling;
            }
        }
        
    return action;
    }


// -----------------------------------------------------------------------------
// CMceComMessageCodec::DoMccRequireSignalling
// -----------------------------------------------------------------------------
//
TInt CMceComMessageCodec::DoMccRequireSignalling( 
    const CMceSrvStream& aOldStream,
    const CMceSrvStream& aCurrentStream,
    const CMccCodecInformation& /*aMccCurentCodec*/, 
    const CMccCodecInformation& /*aMccUpdateCodec*/ ) const
    {
   TInt action = KMceNoSignalling;
    
    // Check for MSRP capabilities
    
    // Checking for Sink properties
    RPointerArray<CMceComMediaSink> Oldsinks = aOldStream.Data().Sinks();
    TBuf8<50> aOldAWtype;
    TBuf8<50> aOldAcceptTypestemp;
    for ( TInt i = 0; i < Oldsinks.Count(); i++ )
        {
        if(Oldsinks[i]->Type() == KMceMSRPSink)
            {
            aOldAWtype = static_cast<CMceComMsrpSink*>(Oldsinks[i])->iAcceptWrappedTypes;//Oldsinks[i]->iAcceptWrappedTypes;
            TInt aPos = static_cast<CMceComMsrpSink*>(Oldsinks[i])->iAcceptTypes.MdcaCount();
            if(aPos)
                {
                aOldAcceptTypestemp.Append(static_cast<CMceComMsrpSink*>(Oldsinks[i])->iAcceptTypes.MdcaPoint(aPos));
                }
            }
        } 
    
    RPointerArray<CMceComMediaSink> Newsinks = aCurrentStream.Data().Sinks();
    TBuf8<50> aNewAWtype;
    TBuf8<50> aNewAcceptTypestemp;
    for ( TInt i = 0; i < Newsinks.Count(); i++ )
        {
        if(Newsinks[i]->Type() == KMceMSRPSink)
            {
            aNewAWtype = static_cast<CMceComMsrpSink*>(Newsinks[i])->iAcceptWrappedTypes;
            TInt aPos = static_cast<CMceComMsrpSink*>(Newsinks[i])->iAcceptTypes.MdcaCount();
            if(aPos)
                {
                aNewAcceptTypestemp.Append(static_cast<CMceComMsrpSink*>(Newsinks[i])->iAcceptTypes.MdcaPoint(aPos));
                }
            
            if (aOldAWtype.FindC(aNewAWtype) == KErrNotFound )
                {
                // Msrp/AcceptwrappedTypes change detected. requires signalling
                action = KMceRequiresSipSignallingOnly;
                }
            if  (aOldAcceptTypestemp.FindC(aNewAcceptTypestemp) == KErrNotFound )
                {
                //Msrp/AcceptTypes change detected. requires signalling
                action = KMceRequiresSipSignallingOnly;
                }
            }
        }
    
    aOldAWtype.Zero();
    aNewAWtype.Zero();
    aOldAcceptTypestemp.Zero();
    aNewAcceptTypestemp.Zero();
   
    // Checking for Source properties
    CMceComMsrpSource* msrpOldSource = static_cast<CMceComMsrpSource*>(aOldStream.Data().Source());

    if(msrpOldSource->Type() == KMceMSRPSource)
        {
        aOldAWtype = msrpOldSource->iAcceptWrappedTypes; //static_cast<CMceComMsrpSink*>( msrpOldSource->iFlatData )->iAcceptWrappedTypes;
        TInt aPos = msrpOldSource->iAcceptTypes.MdcaCount();
        for(TInt j=0; j<aPos; j++)
            {
            aOldAcceptTypestemp.Append(msrpOldSource->iAcceptTypes.MdcaPoint(j));
            }
        }
    
    CMceComMsrpSource* msrpNewSource = static_cast<CMceComMsrpSource*> (aCurrentStream.Data().Source());
    if(msrpNewSource->Type() == KMceMSRPSource)
        {
    
        aNewAWtype = msrpNewSource->iAcceptWrappedTypes;
        TInt aPos = msrpNewSource->iAcceptTypes.MdcaCount();
        for(TInt j=0; j<aPos; j++)
            {
            aNewAcceptTypestemp.Append(msrpNewSource->iAcceptTypes.MdcaPoint(j));
            }
        
        if (aOldAWtype.FindC(aNewAWtype) == KErrNotFound )
            {
            // Msrp/AcceptwrappedTypes change detected. requires signalling
            action = KMceRequiresSipSignallingOnly;
            }
        
        if  (aOldAcceptTypestemp.FindC(aNewAcceptTypestemp) == KErrNotFound )
            {
            //Msrp/AcceptTypes change detected. requires signalling
            action = KMceRequiresSipSignallingOnly;
            }  
        }

    return action; 
    }


// -----------------------------------------------------------------------------
// CMceComMessageCodec::MccPopulateL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::MccPopulateL( CMccCodecInformation& aMccCodec,
                                 CMceSrvStream& /*aStream*/,
                                 TMceNegotiationRole aRole )
    {
    //MccEncodeL( aMccCodec, aStream );
    if ( aRole == EMceRoleOfferer )
        {
        // Create fmtp
        aMccCodec.CreateFmtpAttrListL();
        }

    UpdateL( aMccCodec );
    
    }

// -----------------------------------------------------------------------------
// CMceComCodec::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::UpdateL( CMccCodecInformation& aMccCodec )
    {
    //SetFmtpAttributeL( aMccCodec.GetFmtpL() );
    iSdpName = aMccCodec.SdpName();
    }
    
// -----------------------------------------------------------------------------
// CMceComMessageCodec::DoMccAdjustL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::DoMccAdjustL( CMccCodecInformation& /*aMccCodec*/,
                                      CMceSrvStream& /*aStream*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CMceComMessageCodec::DoSetDefaultFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceComMessageCodec::DoSetDefaultFmtpAttributeL()
    {
    // NOP
    }
    
       
#endif// MCE_COMMON_SERVER_SIDE

