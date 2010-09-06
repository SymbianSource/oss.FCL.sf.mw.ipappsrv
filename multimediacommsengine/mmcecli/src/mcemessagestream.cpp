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



#include "mcemessagestream.h"
#include "mcecommessagestream.h"
#include "mcemanager.h"
#include "mcemessagecodec.h"
#include "mcecommessagestream.h"
#include "mceserial.h"
#include "mcemessagecodec.h"
#include "mcecommessagestream.h"
#include "mceclilogs.h"

#define _FLAT_DATA static_cast<CMceComMessageStream*>( iFlatData )
#define FLAT_DATA( data ) (_FLAT_DATA->data)

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceAudioStream::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageStream* CMceMessageStream::NewL()
    {
    CMceMessageStream* self = CMceMessageStream::NewLC();
    CleanupStack::Pop( self );
    return self; 
    }
   
// -----------------------------------------------------------------------------
// CMceAudioStream::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageStream* CMceMessageStream::NewLC()
    {
    CMceMessageStream* self = new (ELeave) CMceMessageStream();
    CleanupStack::PushL( self );
    self->ConstructL();    
    return self;    
    }

// -----------------------------------------------------------------------------
// CMceMessageStream::~CMceMessageStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageStream::~CMceMessageStream()
    {
    iCodecs.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMceMessageStream::Codecs
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceMessageCodec>& CMceMessageStream::Codecs()
    {
    return iCodecs;
    }

// -----------------------------------------------------------------------------
// CMceMessageStream::AddCodecL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMessageStream::AddCodecL(CMceMessageCodec* aCodec)
    {
    __ASSERT_ALWAYS(aCodec, User::Leave(KErrArgument));
    iCodecs.AppendL(aCodec);
    }

// -----------------------------------------------------------------------------
// CMceMessageStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMessageStream::RemoveCodecL(CMceMessageCodec& aCodec)
    {
    RemoveCodecL( aCodec, Binder() );
    }

// -----------------------------------------------------------------------------
// CMceMessageStream::ReplaceCodecsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMessageStream::ReplaceCodecsL(
    RPointerArray<CMceMessageCodec>* /*aCodecs*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceMessageStream::CMceMessageStream
// -----------------------------------------------------------------------------
//
CMceMessageStream::CMceMessageStream()
    :CMceMediaStream()
    {
    }

// -----------------------------------------------------------------------------
// CMceMessageStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMessageStream::ConstructL()
    {    
	CMceComMessageStream *stream = CMceComMessageStream::NewLC();
	CMceMediaStream::ConstructL( stream );
	CleanupStack::Pop(stream);
    }

// -----------------------------------------------------------------------------
// CMceMessageStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMessageStream::InitializeL( CMceSession& aParent )
    {
    
    CMceMediaStream::InitializeL( aParent, 0 );
            
    for( TInt i = 0; i < iCodecs.Count(); i++ )
        {
        iCodecs[i]->InitializeL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CMceMessageStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMessageStream::InitializeL( CMceManager* aManager, CMceSession& aSession )
    {
    
    __ASSERT_ALWAYS( aManager, User::Leave( KErrArgument ) );
    
    CMceMediaStream::InitializeL( aManager, aSession );
    
    if ( iCodecs.Count() == 0 )
        {
        const RPointerArray<const CMceMessageCodec>& supportedCodes = 
                                      aManager->SupportedMessageCodecs();
                                      
        for( TInt i = 0; i < supportedCodes.Count(); i++ )
            {
            CMceMessageCodec* codec = supportedCodes[i]->CloneL();
            CleanupStack::PushL( codec );
            iCodecs.AppendL( codec );
            CleanupStack::Pop( codec );
            MCECLI_DEBUG_SVALUE("CMceMessageStream::InitializeL, \
Added supported codec", codec->SdpName() );
            }
                                      
        }
        
    }
    

void CMceMessageStream::SynchronizeWithFileL( CMceFileSource& /*aFile*/ )
	{
	}
        
CMceCodec* CMceMessageStream::FindCodec( CMceCodec& aCodec )
    {
    const RPointerArray<CMceCodec>& codecs = 
        reinterpret_cast<const RPointerArray<CMceCodec>&>( Codecs() );
    return CMceMediaStream::FindCodec( codecs, aCodec );
    }
        
void CMceMessageStream::ReorderCodecsByPreferenceL( TBool /*aDiscardUnusedCodecs*/ )
	{
	}        
     
const RPointerArray<CMceCodec>& CMceMessageStream::BaseCodecs()
	{
    const RPointerArray<CMceCodec>& codecs = 
        reinterpret_cast<const RPointerArray<CMceCodec>&>( Codecs() );
    return codecs;
	}


// -----------------------------------------------------------------------------
// CMceMessageStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
void CMceMessageStream::RemoveCodecL( CMceMessageCodec& aCodec, TBool aBinderOriginated )
    {
    MCECLI_DEBUG_SVALUE( "CMceMessageStream::RemoveCodecL, sdp name:", aCodec.SdpName() )
    
    TBool removed( EFalse );
    for(int i = 0; i < iCodecs.Count() && !removed; i++ )
        {
        if( iCodecs[i] == &aCodec || 
            iCodecs[i]->SdpName().CompareF( aCodec.SdpName() ) == 0 )
            {
            MCECLI_DEBUG( "CMceMessageStream::RemoveCodecL, removing" )
            
            TBool commandBound = aBinderOriginated ? ( Binder() ) : ( !Binder() );
            if ( BoundStream() && commandBound )
                {
                static_cast<CMceMessageStream*>( 
                    iLinkedStream )->RemoveCodecL( aCodec, aBinderOriginated );
                }
                
            delete iCodecs[i];
            iCodecs.Remove( i );
            removed = ETrue;
            }
        }
        
    MCECLI_DEBUG( "CMceMessageStream::RemoveCodecL, Exit" )
    }
        
// -----------------------------------------------------------------------------
// CMceMessageStream::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceMessageStream::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceMediaStream::InternalizeL( aSerCtx );
    
    TMceMessageStreamSerializer<CMceMessageStream> serial( *this );
    serial.InternalizeL( aSerCtx );

    }
    
// -----------------------------------------------------------------------------
// CMceMessageStream::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceMessageStream::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    CMceMediaStream::ExternalizeL( aSerCtx );

    TMceMessageStreamSerializer<CMceMessageStream> serial( *this );
    serial.ExternalizeL( aSerCtx );
    
    }




// -----------------------------------------------------------------------------
// CMceMessageStream::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceMessageStream::EventReceivedL( TMceEvent& aEvent )
    {

    TInt status = CMceMediaStream::EventReceivedL( aEvent );
    if ( status != KMceEventNotConsumed )
        {
        return status;
        }

    //try codecs
    if ( aEvent.Id().IsCodecId() )
        {
        TInt codecStatus = status;
        TInt j = 0;
        while ( codecStatus != KMceEventConsumed && j < iCodecs.Count() )
            {
            CMceMessageCodec* codec = iCodecs[j]; 
            codecStatus = codec->EventReceivedL( aEvent );
            if ( codecStatus == KMceEventUpdate )
                {
                MCECLI_DEBUG("CMceMessageStream::EventReceivedL, \
update codec by replacing old version with new one");
                CMceMsgObject<CMceMessageCodec>* updateMsg = 
                    static_cast<CMceMsgObject<CMceMessageCodec>*>( aEvent.Message() );
                CMceMessageCodec* updateCodec = updateMsg->Object();
                CleanupStack::PushL( updateCodec );
                iCodecs.Remove( j );
                delete codec;
                updateCodec->InitializeL( *this );
                iCodecs.AppendL( updateCodec );
                CleanupStack::Pop( updateCodec );
                updateCodec->Updated();
                codecStatus = KMceEventConsumed;
                }
            j++;                
            }
        status = codecStatus == KMceEventNotOwner ? KMceEventNotConsumed : codecStatus;
        }

    return status;

    }

// -----------------------------------------------------------------------------
// CMceMessageStream::ConnectionSetUpL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMessageStream::ConnectionSetUpL(TConnSetup  aSetup)
    {
    // create media level setup SDP a-line and add to localmediasdp lines
    
    CDesC8Array* LocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CDesC8Array* CurrMediaSdpLines = FlatData()->iLocalMediaSDPLines;
    CleanupStack::PushL(LocalMediaSDPLines);
    // prepare setup adp attribute line
    _LIT8(KSetupAttr, "a=setup:");
    // Retain the current media level attributes and keep adding to these
    for(TInt i=0; i<CurrMediaSdpLines->MdcaCount(); i++)
        {
        if (CurrMediaSdpLines->MdcaPoint(i).Find(KSetupAttr) != KErrNotFound)
            {
            User::Leave(KErrAlreadyExists);
            }
        LocalMediaSDPLines->AppendL(CurrMediaSdpLines->MdcaPoint(i));
        }   
    CleanupStack::Pop(LocalMediaSDPLines);

    TBuf8<25> setupAttr(KSetupAttr);
    switch(aSetup)
        {
        case CMceMessageStream::EActive:
            setupAttr.Append(_L8("active\r\n"));
            break;
        case CMceMessageStream::EActpass:
            setupAttr.Append(_L8("actpass\r\n"));
            break;
        case CMceMessageStream::EPassive:
            setupAttr.Append(_L8("passive\r\n"));
            break;
        default: User::Leave(KErrNotSupported);
        }
     // Append the accepttype attribute line to Media level attribute list
    LocalMediaSDPLines->AppendL(setupAttr);
    SetMediaAttributeLinesL(LocalMediaSDPLines );
    FLAT_DATA(iConnSetup) = aSetup;
    }


// -----------------------------------------------------------------------------
// CMceMessageStream::GetConnectionSetup
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageStream::TConnSetup CMceMessageStream::GetConnectionSetup()
    {
    return FLAT_DATA(iConnSetup);
    }


// -----------------------------------------------------------------------------
// CMceMessageStream::ReuseMsrpConnection
// -----------------------------------------------------------------------------
//
void CMceMessageStream::ReuseMsrpConnectionL(TBool aReUse)
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    FLAT_DATA(iMsrpConnUsage) = aReUse;
    }
