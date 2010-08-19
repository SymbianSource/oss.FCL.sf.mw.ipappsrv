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




#include "mcecomcodec.h"

#include "mceserial.h"
#include "mceevents.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmcccodecinformation.h>
#include "mcesrvstream.h"

#endif//MCE_COMMON_SERVER_SIDE


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComCodec::CMceComCodec
// -----------------------------------------------------------------------------
//
CMceComCodec::CMceComCodec()
  : iID( TMceMediaId( (TUint32)&iID ) ),
    iPayloadType( 0 ),
    iIsEnabled( ETrue ),
    iBitrate( 0 ),
    iAllowedBitrates( 0 ),
    iCodecMode( 0 ),
    iFourCC( 0 ),
    iFrameSize( 0 ),
	iState( CMceCodec::EEnabled ),
	iPriority( 0 ),
	iPriorityPreference( 0 ),
    iKeepAliveTimer( 0 ),
    iKeepAlivePayloadType( 0 ),
    iIsNegotiated( ETrue ),
    iCodecSdpIndex( 0 ),
    iPreference( 0 )
        {
        }

// -----------------------------------------------------------------------------
// CMceComCodec::Id
// -----------------------------------------------------------------------------
//
TMceMediaId CMceComCodec::Id() const
    {
    return iID;
    }

// -----------------------------------------------------------------------------
// CMceComCodec::Stream
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceComCodec::Stream() const
    {
    return iStream;
    }


// -----------------------------------------------------------------------------
// CMceComCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComCodec::ConstructL( CMceComCodec& aCodec )
    {
    iID = aCodec.iID;
    iSdpName = aCodec.iSdpName;
    iFmtpAttr = aCodec.iFmtpAttr ? aCodec.iFmtpAttr->AllocL() : NULL;
	iPayloadType = aCodec.iPayloadType;
	iFourCC = aCodec.iFourCC;
    iBitrate = aCodec.iBitrate;
	iAllowedBitrates = aCodec.iAllowedBitrates;
	iCodecMode = aCodec.iCodecMode;		
	iFrameSize = aCodec.iFrameSize;
	iState = aCodec.iState;
	iPriority = aCodec.iPriority;
	iPriorityPreference = aCodec.iPriorityPreference;
	iKeepAliveTimer = aCodec.iKeepAliveTimer;
	iKeepAlivePayloadType = aCodec.iKeepAlivePayloadType;
	if ( aCodec.iKeepAliveData )
	    {
	    SetKeepAliveDataL( *aCodec.iKeepAliveData );
	    }
    iIsNegotiated = aCodec.iIsNegotiated;
	iCodecSdpIndex = aCodec.iCodecSdpIndex;
    }


// -----------------------------------------------------------------------------
// CMceComCodec::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceComCodec::SerializationId() const
    {
    return iID.Uint64();
    }

// -----------------------------------------------------------------------------
// CMceComCodec::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComCodec::InternalizeFlatL( RReadStream& aReadStream )
    {
    MceSerial::DecodeL( iSdpName, aReadStream );
    MceSerial::DecodeL( iID, aReadStream );
	iPayloadType = aReadStream.ReadUint8L();
	iFourCC = aReadStream.ReadUint32L();
	iFrameSize = aReadStream.ReadUint32L();
	iBitrate = aReadStream.ReadUint32L();
	iAllowedBitrates = aReadStream.ReadUint32L();
	iCodecMode = aReadStream.ReadUint32L();
	iState = aReadStream.ReadInt32L();
	iPriority = aReadStream.ReadInt32L();
	iPriorityPreference = aReadStream.ReadInt32L();
    iKeepAliveTimer = aReadStream.ReadUint32L();
    iKeepAlivePayloadType = aReadStream.ReadUint32L();
    MceSerial::DecodeL( iKeepAliveData, aReadStream );
    }
    
// -----------------------------------------------------------------------------
// CMceComCodec::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComCodec::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    MceSerial::EncodeL( iSdpName, aWriteStream );
    MceSerial::EncodeL( iID, aWriteStream );
    aWriteStream.WriteUint8L( iPayloadType );
    aWriteStream.WriteUint32L( iFourCC );
    aWriteStream.WriteUint32L( iFrameSize );
    aWriteStream.WriteUint32L( iBitrate );
    aWriteStream.WriteUint32L( iAllowedBitrates );
    aWriteStream.WriteUint32L( iCodecMode );
    aWriteStream.WriteInt32L( iState );
    aWriteStream.WriteInt32L( iPriority );
    aWriteStream.WriteInt32L( iPriorityPreference );
    aWriteStream.WriteInt32L( iKeepAliveTimer );
    aWriteStream.WriteInt32L( iKeepAlivePayloadType );
    MceSerial::EncodeL( iKeepAliveData, aWriteStream );
    }

// -----------------------------------------------------------------------------
// CMceComCodec::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceComCodec::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    InternalizeFlatL( aSerCtx.ReadStream() );
    }

// -----------------------------------------------------------------------------
// CMceComCodec::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceComCodec::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    ExternalizeFlatL( aSerCtx.WriteStream() );
    }


// -----------------------------------------------------------------------------
// CMceComCodec::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComCodec::UpdateL( CMceComCodec& aUpdate )
    {
    iSdpName = aUpdate.iSdpName;
    iID = aUpdate.iID;
    iPayloadType = aUpdate.iPayloadType;
    iFourCC = aUpdate.iFourCC;
    iBitrate = aUpdate.iBitrate;
    iAllowedBitrates = aUpdate.iAllowedBitrates;
    iCodecMode = aUpdate.iCodecMode;
	iState = aUpdate.iState;
	iPriority = aUpdate.iPriority;
	iPriorityPreference = aUpdate.iPriorityPreference;
    iKeepAliveTimer = aUpdate.iKeepAliveTimer;
    iKeepAlivePayloadType = aUpdate.iKeepAlivePayloadType;
    if ( aUpdate.iKeepAliveData )
        {
        delete iKeepAliveData;
        iKeepAliveData = aUpdate.iKeepAliveData;
        aUpdate.iKeepAliveData = NULL;
        }
    else
        {
        SetKeepAliveDataL( KNullDesC8 );
        }
    }

// -----------------------------------------------------------------------------
// CMceComCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComCodec::ConstructL( TMceComCodecName aSdpName )
    {
    iSdpName = aSdpName;
    SetKeepAliveDataL( KNullDesC8 );
    SetFmtpAttributeL( KNullDesC8 );
    }

// -----------------------------------------------------------------------------
// CMceComCodec::Decodes
// -----------------------------------------------------------------------------
//
TBool CMceComCodec::Decodes( 
    const TDesC8& aRtpmapValue, 
    const TDesC8& /*aFmtpValue*/,
    TBool /*aIsStrictMatch*/ )
    {
    // find MIME type
    TInt slashPos = aRtpmapValue.Find( _L8( "/" ) );
    if ( slashPos != KErrNotFound )
        {
		return aRtpmapValue.Left(slashPos).CompareF( iSdpName ) == 0;
        }

    return aRtpmapValue.FindF(iSdpName) == 0;
    }
    
// -----------------------------------------------------------------------------
// CMceComCodec::BaseFactory
// -----------------------------------------------------------------------------
//
TMceComFactory CMceComCodec::BaseFactory()
    {
    return TMceComFactory();
    }

// -----------------------------------------------------------------------------
// CMceComCodec::FmtpAttr
// -----------------------------------------------------------------------------
//
TPtrC8 CMceComCodec::FmtpAttr() const
    {
    TPtrC8 existingFmtpVal( KNullDesC8() );
    if ( iFmtpAttr && iFmtpAttr->Length() > 0 )
        {
        existingFmtpVal.Set( *iFmtpAttr );
        }
    else if ( iFmtpProposal )
        {
        existingFmtpVal.Set( *iFmtpProposal );
        }
    else
        {
        // NOP
        }
    return existingFmtpVal;
    }
    
// -----------------------------------------------------------------------------
// CMceComCodec::SetFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceComCodec::SetFmtpAttributeL( const TDesC8& aFmtpAttribute, TBool aReset )
    {
    if ( aReset || !iFmtpAttr )
        {
        HBufC8* fmtpAttr = aFmtpAttribute.AllocL();
        delete iFmtpAttr;
        iFmtpAttr = fmtpAttr;
        }
    else
        {
        iFmtpAttr = iFmtpAttr->ReAllocL( iFmtpAttr->Length() + 
                                         KMceFmtpSeparator().Length() +
                                         aFmtpAttribute.Length() );
        iFmtpAttr->Des().Append( KMceFmtpSeparator );
        iFmtpAttr->Des().Append( aFmtpAttribute );
        }
    }

    
// -----------------------------------------------------------------------------
// CMceComCodec::~CMceComCodec
// -----------------------------------------------------------------------------
//
CMceComCodec::~CMceComCodec()
    {
    delete iFmtpAttr;
    iFmtpAttr = NULL;
    delete iKeepAliveData;
    iKeepAliveData = NULL;
    delete iFmtpProposal;
    iFmtpProposal = NULL;
    }

// -----------------------------------------------------------------------------
// CMceComCodec::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComCodec::EventReceivedL( TMceEvent& aEvent )
    {
    TInt status = KMceEventNotOwner;
    TBool idMatch = aEvent.Id().iCodecID == iID;
    if ( !idMatch )
        {
        return KMceEventNotOwner;
        }
        
    if ( aEvent.Id().IsCodecId() )
        {
        status = KMceEventUpdate;
        }
    else
        {
        status = KMceEventNotConsumed;
        }
    
    return status;
    
    }

// -----------------------------------------------------------------------------
// CMceComCodec::ResolveAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComCodec::ResolveAllowedBitrates( 
    TUint /*aBitrate*/, TUint& /*aAllowedBitrates*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMceComCodec::SetEnabled
// -----------------------------------------------------------------------------
//
void CMceComCodec::SetEnabled( TBool aIsEnabled )
    {
    iIsEnabled = aIsEnabled;
    }

// -----------------------------------------------------------------------------
// CMceComCodec::SendSupported
// -----------------------------------------------------------------------------
//
TBool CMceComCodec::SendSupported()
    {
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CMceComCodec::IsEnabled
// -----------------------------------------------------------------------------
//
TBool CMceComCodec::IsEnabled() const
    {
    return iIsEnabled;
    }
        
// -----------------------------------------------------------------------------
// CMceComCodec::InitializeL
// -----------------------------------------------------------------------------
//
void CMceComCodec::InitializeL( CMceComMediaStream& aParent )
    {
    iStream = &aParent;
    }

// -----------------------------------------------------------------------------
// CMceComCodec::SetKeepAliveDataL
// -----------------------------------------------------------------------------
//
void CMceComCodec::SetKeepAliveDataL( const TDesC8& aKeepAliveData )
    { 
    HBufC8* keepaliveData = aKeepAliveData.AllocL();
    delete iKeepAliveData;
    iKeepAliveData = keepaliveData;
    }

    
// -----------------------------------------------------------------------------
// CMceComCodec::SetBitrate
// -----------------------------------------------------------------------------
//
TInt CMceComCodec::SetBitrate( TUint aBitrate )
    {
    if ( aBitrate )
        {
        iBitrate = aBitrate;
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }
    
// -----------------------------------------------------------------------------
// CMceComCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
TInt CMceComCodec::SetAllowedBitrates( TUint aBitrates )
    {
    iAllowedBitrates = aBitrates;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMceComCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
TInt CMceComCodec::SetCodecMode( TUint /*aCodecMode*/ )
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
// CMceComCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
TInt CMceComCodec::SetPayloadType( TUint8 aPayloadType )
    {
    if ( aPayloadType <= KMaxDynPayloadType )
        {
        iPayloadType = aPayloadType;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CMceComCodec::GetMaxBitRate
// -----------------------------------------------------------------------------
//
TUint CMceComCodec::GetMaxBitRate()
    {
    return iBitrate;
    }


// -----------------------------------------------------------------------------
// CMceComCodec::TIterator::TIterator
// -----------------------------------------------------------------------------
//
CMceComCodec::TIterator::TIterator( const RPointerArray<CMceComCodec>& aCodecs, 
                                    CMceComCodec::TIterator::TFilter aFilter,
                                    CMceComCodec* aCodec )
    : iFilter( aFilter ),
      iCodecs( aCodecs ),
      iCurrentIndex( 0 ),
      iCodec( aCodec )
    {
    }
            

// -----------------------------------------------------------------------------
// CMceComCodec::TIterator::IsEof
// -----------------------------------------------------------------------------
//
TBool CMceComCodec::TIterator::IsEof()
    {
    return iCodecs.Count() == 0 || iCurrentIndex >= iCodecs.Count();
    }
            

// -----------------------------------------------------------------------------
// CMceComCodec::TIterator::Next
// -----------------------------------------------------------------------------
//
TBool CMceComCodec::TIterator::Next( 
    CMceComCodec*& aCandidate,  
    TIterator::TStreamStateMatchType aMatchType )
    {
    CMceComCodec* next = NULL;
    
    while( !next && !IsEof() )
        {
        CMceComCodec* codec = iCodecs[ iCurrentIndex ];
        TBool condition = EFalse;
        switch( iFilter )
            {
            case EFilterIsNegotiated:
                {
                condition = codec->iIsNegotiated;
                break;
                }
            case EFilterAssociatedLocal:
                {
                if ( iCodec && 
                     iCodec->Stream() &&
                     codec->Stream() &&
                     codec->Stream()->iStreamType == CMceComMediaStream::ELocalStream &&
                     ( ( aMatchType == EOtherSdpName && 
                         !codec->Decodes( iCodec->iSdpName, iCodec->FmtpAttr() ) ) || 
                       ( aMatchType == EExactMatch && 
                         codec->Decodes( iCodec->iSdpName, iCodec->FmtpAttr() ) ) ) )
                    {
                    CMceComMediaSource* source1 = iCodec->Stream()->Source();
                    CMceComMediaSource* source2 = codec->Stream()->Source();
                    
                    condition = ( source1 != NULL && source1 == source2 );
                    }
                break;
                }
            default:
                {
                condition = ETrue;
                break;
                }
            }

        next = condition ? codec : NULL;
               
        iCurrentIndex++;
        }

    aCandidate = next;
    return aCandidate ? ETrue : EFalse;
    
    }
    
    
// -----------------------------------------------------------------------------
// CMceComCodec::TIterator::Reset
// -----------------------------------------------------------------------------
//
void CMceComCodec::TIterator::Reset()
    {
    iCurrentIndex = 0;
    }
            
// -----------------------------------------------------------------------------
// CMceComCodec::TIterator::SetFilter
// -----------------------------------------------------------------------------
//
void CMceComCodec::TIterator::SetFilter( 
    CMceComCodec::TIterator::TFilter aFilter,
    CMceComCodec* aCodec )
    {
    iFilter = aFilter;
    iCodec = aCodec;
    }
            
// -----------------------------------------------------------------------------
// CMceComCodec::TIterator::Count
// -----------------------------------------------------------------------------
//
TInt CMceComCodec::TIterator::Count()
    {
    TInt count = iCodecs.Count();
    if ( iFilter )
        {
        TInt currentIndex = iCurrentIndex;
        Reset();
        CMceComCodec* codec = NULL;
        count = 0;
        while( Next( codec ) )
            {
            count++;
            }
        iCurrentIndex = currentIndex;
        }

    return count;
    }
    
    
#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComCodec::SynchronizeL
// -----------------------------------------------------------------------------
//
void CMceComCodec::SynchronizeL( CMceComCodec& aCodec, TBool aRoleAnswerer )
    {
    if ( aRoleAnswerer )
        {
        SetFmtpFromIncomingSdpL( *aCodec.iFmtpAttr, aRoleAnswerer );
    	iPayloadType = aCodec.iPayloadType;
        }
    iCodecSdpIndex = aCodec.iCodecSdpIndex;
	iIsNegotiated = aCodec.iIsNegotiated;
    }
    
    
// -----------------------------------------------------------------------------
// CMceComCodec::MccPopulateL
// -----------------------------------------------------------------------------
//
void CMceComCodec::MccPopulateL( CMccCodecInformation& aMccCodec,
                                 CMceSrvStream& aStream,
                                 TMceNegotiationRole aRole )
    {
    MccEncodeL( aMccCodec, aStream );
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
void CMceComCodec::UpdateL( CMccCodecInformation& aMccCodec )
    {
	SetFmtpAttributeL( aMccCodec.GetFmtpL() );
	iSdpName = aMccCodec.SdpName();
    }


// -----------------------------------------------------------------------------
// CMceComCodec::MccEncodeL
// -----------------------------------------------------------------------------
//
void CMceComCodec::MccEncodeL( CMccCodecInformation& aMccCodec, 
                               CMceSrvStream& aStream )
    {	    
    // Encode common 
    EncodeCodecCommonL( aMccCodec );
	       
    // Encode codec specific stuff
    DoMccEncodeL( aMccCodec, aStream );
    }

// -----------------------------------------------------------------------------
// CMceComCodec::MccDecodeL
// -----------------------------------------------------------------------------
//
void CMceComCodec::MccDecodeL( CMccCodecInformation& aMccCodec )
    {
    TBool fmtpReady = iFmtpAttr && iFmtpAttr->Length();
    if ( fmtpReady )
        {
        // Parse fmtp attribute
        aMccCodec.SetFmtpAttrL( *iFmtpAttr );
        }
        
    // Copy common values from mcc codec
    DecodeCodecCommonL( aMccCodec );
    
    // Copy codec specific values from mcc codec
    DoMccDecodeL( aMccCodec );
    
    if ( !fmtpReady )
        {
        aMccCodec.CreateFmtpAttrListL();
        }
        
	SetFmtpAttributeL( aMccCodec.GetFmtpL() );
    }
    
// -----------------------------------------------------------------------------
// CMceComCodec::MccValidateL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceComCodec::MccValidateL( CMccCodecInformation& aMccCodec,
                                          CMceSrvStream& aStream,
                                          TMceNegotiationRole aRole )
    {
    CMceComCodec* bidirectional = NULL;
    
    aMccCodec.SetFmtpAttrL( *iFmtpAttr );
    
    // Validation not needed for disabled streams
    if ( aStream.Data().IsEnabled() )
        {
        // Do common validation
       	if( (TInt)aMccCodec.Bitrate() < 0 )
    		{
    		// Bitrate in the answer was out of range or 
    		User::Leave( KErrNotSupported );
    		}
    		
        // Do codec specific validation
        bidirectional = DoMccValidateL( aMccCodec, aStream, aRole );
        }

    //Validation ok => when answerer update mcc codec with codec values
    //                 when offerer update codec with mcc codec values
    if ( aRole == EMceRoleAnswerer )
        {
        // Encode
        MccEncodeL( aMccCodec, aStream );
    
        //update fmtp
        UpdateL( aMccCodec );
    	
        }
    else
        {
        // Copy common values from mcc codec
        DecodeCodecCommonL( aMccCodec );
        
        // Copy codec specific values from mcc codec
        DoMccDecodeL( aMccCodec );
        }
     
    return bidirectional;  
    }

// -----------------------------------------------------------------------------
// CMceComCodec::MccAdjustL
// -----------------------------------------------------------------------------
//
void CMceComCodec::MccAdjustL( CMccCodecInformation& aMccCodec,
                               CMceSrvStream& aStream )
    {
    User::LeaveIfError( aMccCodec.SetPayloadType( iPayloadType ) );
	    
    DoMccAdjustL( aMccCodec, aStream );
    
    }


// -----------------------------------------------------------------------------
// CMceComCodec::MccRequireSignalling
// -----------------------------------------------------------------------------
//
TInt CMceComCodec::MccRequireSignalling( 
    const CMceSrvStream& aStream,
    const CMccCodecInformation& aMccCurentCodec, 
    const CMccCodecInformation& aMccUpdateCodec ) const
    {
    TInt action = KMceRequiresSignalling;

    if ( !aMccCurentCodec.RequireSignalling( aMccUpdateCodec ) )
        {
        action = DoMccRequireSignalling( aStream, aMccCurentCodec, aMccUpdateCodec );
        }
        
    return action;                
    }

    
// -----------------------------------------------------------------------------
// CMceComCodec::SetState
// -----------------------------------------------------------------------------
//
void CMceComCodec::SetState( TMceMccComEvent& aEvent )
    {

    iIsEnabled = aEvent.iEvent == KMccStreamPrepared ?
                    ETrue :
                 aEvent.iEvent == KMccStreamStarted ?
                    ETrue :
                 aEvent.iEvent == KMccStreamStopped ?
                    EFalse :
                 aEvent.iEvent == KMccStreamPaused ?
                    EFalse :
                 aEvent.iEvent == KMccStreamResumed ?
                   ETrue : iIsEnabled;
    
    }

// -----------------------------------------------------------------------------
// CMceComCodec::Merge
// -----------------------------------------------------------------------------
//
void CMceComCodec::Merge( CMceComCodec& aMergeWith )
    {
    CMceComMediaStream* stream = Stream();
    if ( IS_RECEIVESTREAM( stream ) )
        {
        iIsEnabled = aMergeWith.iIsEnabled;
        }
    iIsNegotiated = aMergeWith.iIsNegotiated;
    iCodecSdpIndex = aMergeWith.iCodecSdpIndex;
    }

// -----------------------------------------------------------------------------
// CMceComCodec::SetFmtpProposalL
// -----------------------------------------------------------------------------
//
TBool CMceComCodec::SetFmtpProposalL( const TDesC8& aFmtpProposal )
    {
    HBufC8* proposal = aFmtpProposal.AllocL();
    delete iFmtpProposal;
    iFmtpProposal = proposal;
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMceComCodec::SetFmtpFromIncomingSdpL
// -----------------------------------------------------------------------------
//
void CMceComCodec::SetFmtpFromIncomingSdpL( 
    const TDesC8& aFmtpAttribute, 
    TBool /*aRoleAnswerer*/,
    TBool aReset )
    {
    SetFmtpAttributeL( aFmtpAttribute, aReset );
    }

// -----------------------------------------------------------------------------
// CMceComCodec::FtmpForOutgoingSdpLC
// -----------------------------------------------------------------------------
//    
HBufC8* CMceComCodec::FtmpForOutgoingSdpLC()
    {
    HBufC8* fmtp = NULL;
    if ( iFmtpAttr )
        {
        fmtp = iFmtpAttr->AllocLC();
        }
    else
        {
        fmtp = KNullDesC8().AllocLC();
        }
    return fmtp;
    }
        
// -----------------------------------------------------------------------------
// CMceComCodec::EncodeCodecCommonL
// -----------------------------------------------------------------------------
//
void CMceComCodec::EncodeCodecCommonL( CMccCodecInformation& aMccCodec )
    {
    User::LeaveIfError( aMccCodec.SetPayloadType( iPayloadType ) );
	User::LeaveIfError( aMccCodec.SetAllowedBitrates( iAllowedBitrates ) );
	User::LeaveIfError( aMccCodec.SetCodecMode( (TCodecMode)iCodecMode ) );
	User::LeaveIfError( aMccCodec.SetBitrate( iBitrate ) );
	aMccCodec.SetPriority( iPriority );
	aMccCodec.SetPriorityPreference( iPriorityPreference );
	User::LeaveIfError( aMccCodec.SetKeepAliveTimer( iKeepAliveTimer ) );
	User::LeaveIfError( aMccCodec.SetKeepAlivePT( iKeepAlivePayloadType ) );
	if ( iKeepAliveData && iKeepAliveData->Length() > 0 )
	    {
	    User::LeaveIfError( aMccCodec.SetKeepAliveData( *iKeepAliveData ) );
	    }	
	}


// -----------------------------------------------------------------------------
// CMceComCodec::DecodeCodecCommonL
// -----------------------------------------------------------------------------
//
void CMceComCodec::DecodeCodecCommonL( CMccCodecInformation& aMccCodec )
    {  
	iAllowedBitrates = aMccCodec.AllowedBitrates();
	iCodecMode = (TUint32)aMccCodec.CodecMode();
	iFourCC = aMccCodec.FourCC();
	iFrameSize = aMccCodec.FrameSize();
	iBitrate =  aMccCodec.Bitrate();
	iPriority = aMccCodec.Priority();
	iPriorityPreference = aMccCodec.PriorityPreference();
	iKeepAliveTimer = aMccCodec.KeepAliveTimer();
	iKeepAlivePayloadType = aMccCodec.KeepAlivePT();
	SetKeepAliveDataL( aMccCodec.KeepAliveData() );
    }

// -----------------------------------------------------------------------------
// CMceComCodec::SetDefaultFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceComCodec::SetDefaultFmtpAttributeL()
    {
    DoSetDefaultFmtpAttributeL();
    }

// -----------------------------------------------------------------------------
// CMceComCodec::DoMccValidateL
// -----------------------------------------------------------------------------
CMceComCodec* CMceComCodec::DoMccValidateL( CMccCodecInformation& aMccCodec,
		                                    CMceSrvStream& /*aStream*/, 
		                                    TMceNegotiationRole /*aRole*/ )
	{
	if ( aMccCodec.CodecMode() != (TCodecMode)iCodecMode )
		{
		// codec-mode was different, fail.
		User::Leave(KErrNotSupported);
		}
	return NULL;
	}
       
#endif//MCE_COMMON_SERVER_SIDE
    

