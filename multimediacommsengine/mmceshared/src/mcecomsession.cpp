/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mcecomsession.h"
#include "mceserial.h"
#include "mceevents.h"
#include "cleanupresetanddestroy.h"

#ifdef MCE_COMMON_SERVER_SIDE
    
#include "mcesrvstream.h"
#include "mcemediaidle.h"
#include "mcemedianegotiated.h"
#include "mcemediamanager.h"
#include "mcesrvstreamiterator.h"
#include "mceadoptedsrvstream.h"
#include "mcesdpsession.h"

#else

#define KMceNegotiationAnswerTypeNotDefined 0
#define KMceCodecSelectionOff 0

#endif//MCE_COMMON_SERVER_SIDE



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComSession::NewL
// -----------------------------------------------------------------------------
//
CMceComSession* CMceComSession::NewL()
    {
    return NewL( EInSession );
    }

// -----------------------------------------------------------------------------
// CMceComSession::NewL
// -----------------------------------------------------------------------------
//
CMceComSession* CMceComSession::NewL( CMceComSession::TType aType )
    {
    CMceComSession* self = NewLC( aType );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComSession::NewLC
// -----------------------------------------------------------------------------
//
CMceComSession* CMceComSession::NewLC( CMceComSession::TType aType )
    {
    CMceComSession* self = new (ELeave) CMceComSession( aType );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComSession::NewL
// -----------------------------------------------------------------------------
//
CMceComSession* CMceComSession::NewL( MMceComSerializationContext& aSerCtx )
    {
    CMceComSession* self = NewLC( aSerCtx );
    CleanupStack::Pop( self );
    return self;
    } 

// -----------------------------------------------------------------------------
// CMceComSession::NewLC
// -----------------------------------------------------------------------------
//
CMceComSession* CMceComSession::NewLC( MMceComSerializationContext& aSerCtx )
    {
    CMceComSession* self = new (ELeave) CMceComSession();
    CleanupStack::PushL( self );
    self->InternalizeL( aSerCtx );
    self->InitializeL();
    return self;
    } 

// -----------------------------------------------------------------------------
// CMceComSession::~CMceComSession
// -----------------------------------------------------------------------------
//
CMceComSession::~CMceComSession()
    {    
    delete iRecipient;
    delete iOriginator;
    delete iSIPHeaders;
    delete iSIPContentType;
    delete iSIPContentHeaders;
    delete iLocalSessionSDPLines;
    delete iRemoteSessionSDPLines;
    delete iSIPContent;
    iMediaStreams.ResetAndDestroy();
    iBundles.ResetAndDestroy();
    iClientCryptoSuites.Close();	
    
#ifdef MCE_COMMON_SERVER_SIDE

    DeleteSecureSession();
    if ( iSdpSession )
        {
        if( iSdpSession->DetachMedia( *this ) )
        	{
            iSdpSession = NULL;
        	}
        }

    iMccStreams.ResetAndDestroy();
    iMccAdoptedStreams.ResetAndDestroy();
    delete iNegotiationState;
#endif // MCE_COMMON_SERVER_SIDE
    }

// -----------------------------------------------------------------------------
// CMceComSession::BaseFactory
// -----------------------------------------------------------------------------
//
TMceComFactory CMceComSession::BaseFactory()
    {
    return TMceComFactory();
    }

// -----------------------------------------------------------------------------
// CMceComSession::Factory
// -----------------------------------------------------------------------------
//
TMceComFactory CMceComSession::Factory()
    {
    return TMceComFactory();
    }

// -----------------------------------------------------------------------------
// CMceComSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComSession::ConstructL()
    {
    
    iRecipient = KNullDesC8().AllocL();
    iOriginator = KNullDesC8().AllocL();
    iSIPHeaders = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    iSIPContentType = KNullDesC8().AllocL();
    iSIPContentHeaders = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    iLocalSessionSDPLines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    iRemoteSessionSDPLines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    iIsSecureSession = EFalse;
    }


// -----------------------------------------------------------------------------
// CMceComSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComSession::ConstructL( CMceComSession& aSession, TBool aPullModeUpdate )
    {

    
    iID = aSession.iID;
    iRecipient = aSession.iRecipient ? 
        aSession.iRecipient->AllocL() : KNullDesC8().AllocL();
    iOriginator = aSession.iOriginator ? 
        aSession.iOriginator->AllocL() : KNullDesC8().AllocL();
    iSIPContentType = aSession.iSIPContentType ? 
        aSession.iSIPContentType->AllocL() : KNullDesC8().AllocL();
    iState = aSession.iState;
    iIsConnectionActive = aSession.iIsConnectionActive;
    iDialogId = aSession.iDialogId;
    iIapId = aSession.iIapId;
    iLocalIpAddress = aSession.iLocalIpAddress;
    iRemoteIpAddress = aSession.iRemoteIpAddress;
    iMediaObserver = aSession.iMediaObserver;
    iTimeout = aSession.iTimeout;
    iMinSE = aSession.iMinSE;
    iRefresh = aSession.iRefresh;
    iIsSecureSession = aSession.iIsSecureSession;
    iAnswerType = aSession.iAnswerType;
    
    iPullModeUpdate = aPullModeUpdate;
    
    iClientCryptoSuites.Reset();
    for (TInt i=0; i<aSession.iClientCryptoSuites.Count(); i++)
    	{
    	iClientCryptoSuites.InsertL( aSession.iClientCryptoSuites[i], i);
    	}
    
    iSIPHeaders = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    
    for( TInt i = 0;i<aSession.iSIPHeaders->MdcaCount();i++ )
        {
        iSIPHeaders->AppendL( aSession.iSIPHeaders->MdcaPoint( i ) );
        }
    
    iSIPContentHeaders = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    for( TInt i = 0;i<aSession.iSIPContentHeaders->MdcaCount();i++ )
        {
        iSIPContentHeaders->AppendL( aSession.iSIPContentHeaders->MdcaPoint( i ) );
        }
    
    iLocalSessionSDPLines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    for( TInt i = 0; i < aSession.iLocalSessionSDPLines->MdcaCount(); i++ )
        {
        iLocalSessionSDPLines->AppendL( aSession.iLocalSessionSDPLines->MdcaPoint( i ) );
        }
    
    iRemoteSessionSDPLines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    for( TInt i = 0; i < aSession.iRemoteSessionSDPLines->MdcaCount(); i++ )
        {
        iRemoteSessionSDPLines->AppendL( aSession.iRemoteSessionSDPLines->MdcaPoint( i ) );
        }

    for( TInt i = 0; i < KMceMaxSessionModifiers; i++ )
        {
        iSessionModifiers[ i ] = aSession.iSessionModifiers[ i ];
        }
        
    if ( !aPullModeUpdate )
        {
        CloneStreamsL( aSession );
        CloneBudlesL( aSession );     
        } 
        
    iServiceType = aSession.iServiceType;
    iSipContactAddrSecure = aSession.iSipContactAddrSecure;
    
#ifdef MCE_COMMON_SERVER_SIDE
    iSdpSession = aSession.iSdpSession;
	if (iClientCryptoSuites.Count())
		{
		CloneSecureSessionL(aSession);
		}
    
#endif//MCE_COMMON_SERVER_SIDE    
    }
    

// -----------------------------------------------------------------------------
// CMceComSession::CloneStreamsL
// -----------------------------------------------------------------------------
//
void CMceComSession::CloneStreamsL( CMceComSession& aSession )
    {
    for( TInt i = 0; i<aSession.Streams().Count(); i++ )
        {
        CMceComMediaStream* stream = aSession.Streams()[i]->CloneL();
        CleanupStack::PushL( stream );
        AddStreamL( stream );
        CleanupStack::Pop( stream );
        }
    }

// -----------------------------------------------------------------------------
// CMceComSession::CloneBudlesL
// -----------------------------------------------------------------------------
//
void CMceComSession::CloneBudlesL( CMceComSession& aSession )
    {
    for( TInt i = 0; i<aSession.Bundles().Count(); i++ )
        {
        CMceComStreamBundle* bundle = aSession.Bundles()[i]->CloneL( Streams() );
        CleanupStack::PushL( bundle );
        AddBundleL( bundle );
        CleanupStack::Pop( bundle );
        }
    }
    
// -----------------------------------------------------------------------------
// CMceComSession::CloneL
// -----------------------------------------------------------------------------
//
CMceComSession* CMceComSession::CloneL( TBool aPullModeUpdate )
    {
    CMceComSession* copy = new (ELeave) CMceComSession( iType );
    CleanupStack::PushL( copy );
    copy->ConstructL( *this, aPullModeUpdate );
    copy->InitializeL();
    CleanupStack::Pop( copy );
    copy->iCloneOrigin = this;
    
    return copy;
    }
    

// -----------------------------------------------------------------------------
// CMceComSession::AddStreamL
// -----------------------------------------------------------------------------
//
void CMceComSession::AddStreamL( CMceComMediaStream* aMediaStream )
    {
    iMediaStreams.AppendL( aMediaStream );
    }


// -----------------------------------------------------------------------------
// CMceComSession::Streams
// -----------------------------------------------------------------------------
//
RPointerArray<CMceComMediaStream>& CMceComSession::Streams()
    {
    return iMediaStreams;
    }


// -----------------------------------------------------------------------------
// CMceComSession::AddBundleL
// -----------------------------------------------------------------------------
//
void CMceComSession::AddBundleL( CMceComStreamBundle* aBundle )
    {
    aBundle->AddedL();
    iBundles.AppendL( aBundle );
    }

// -----------------------------------------------------------------------------
// CMceComSession::RemoveBundleL
// -----------------------------------------------------------------------------
//
void CMceComSession::RemoveBundleL( CMceComStreamBundle& aBundle )
    {
    TInt index = iBundles.Find( &aBundle );
    User::LeaveIfError( index );
    iBundles.Remove( index );
    }

// -----------------------------------------------------------------------------
// CMceComSession::Bundles
// -----------------------------------------------------------------------------
//
const RPointerArray< CMceComStreamBundle >& CMceComSession::Bundles() const
    {
    return iBundles;
    }

// -----------------------------------------------------------------------------
// CMceComSession::SetObserver
// -----------------------------------------------------------------------------
//
void CMceComSession::SetObserver( MMceMediaSessionObserver& aMediaObserver )
    {
    iMediaObserver = &aMediaObserver;
    }

// -----------------------------------------------------------------------------
// CMceComSession::SetObserver
// -----------------------------------------------------------------------------
//
MMceMediaSessionObserver& CMceComSession::MediaObserver()
    {
    return *iMediaObserver;
    }
// -----------------------------------------------------------------------------
// CMceComSession::SetOriginatorL
// -----------------------------------------------------------------------------
//
void CMceComSession::SetOriginatorL( const TDesC8& aOriginator )
    {
    delete iOriginator;
    iOriginator = NULL;
    iOriginator = aOriginator.AllocL();
    }
// -----------------------------------------------------------------------------
// CMceComSession::SetRecipientL
// -----------------------------------------------------------------------------
//
void CMceComSession::SetRecipientL( const TDesC8& aRecipient )
    {
    delete iRecipient;
    iRecipient = NULL;
    iRecipient = aRecipient.AllocL();
    }


// -----------------------------------------------------------------------------
// CMceComSession::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceComSession::SerializationId() const
    {
    return _MAKE_TUINT64_ZX( iID );
    }
	    
// -----------------------------------------------------------------------------
// CMceComSession::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComSession::InternalizeFlatL( RReadStream& aReadStream )
    {

    
    //iType
    iType = static_cast<TType>( aReadStream.ReadUint8L() );

    //iID
    iID = aReadStream.ReadUint32L();
    
    //iState
    iState = static_cast<CMceSession::TState>( aReadStream.ReadUint8L() );
    
    //iRecipient
    MceSerial::DecodeL( iRecipient, aReadStream ); 
    
    //iOriginator
    MceSerial::DecodeL( iOriginator, aReadStream ); 
    
    //iIsConnectionActive
    iIsConnectionActive = static_cast<TBool>( aReadStream.ReadUint8L() );

    //iDialogId
    iDialogId = aReadStream.ReadUint32L();
    
    //iTimeout
    iTimeout = aReadStream.ReadUint32L();
    
    // iMinSE
    iMinSE = aReadStream.ReadUint32L();
    
    //iSIPHeaders
    MceSerial::DecodeL( iSIPHeaders, aReadStream ); 
    
    //iSIPContentType
    MceSerial::DecodeL( iSIPContentType, aReadStream ); 
    
    //iSIPContentHeaders
    MceSerial::DecodeL( iSIPContentHeaders, aReadStream ); 
    
    //iLocalSessionSDPLines
    MceSerial::DecodeL( iLocalSessionSDPLines, aReadStream ); 
    
    //iRemoteSessionSDPLines
    MceSerial::DecodeL( iRemoteSessionSDPLines, aReadStream ); 
    
    //iSessionModifiers
    TPckgBuf<TMceComSessionModifiers> modifiers;
    MceSerial::DecodeL( modifiers, aReadStream );
    iSessionModifiers = modifiers();
    
    //check RArray
    
    MceSerial::DecodeL(iClientCryptoSuites, aReadStream);
   
   	iServiceType = aReadStream.ReadUint32L();
   	iSipContactAddrSecure = 
   		static_cast<CMceSession::TControlPathSecurityLevel>( aReadStream.ReadUint8L() );
    }



// -----------------------------------------------------------------------------
// CMceComSession::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComSession::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    aWriteStream.WriteUint8L( iType );
    aWriteStream.WriteUint32L( iID );
    aWriteStream.WriteUint8L( iState );
    MceSerial::EncodeL( iRecipient, aWriteStream );
    MceSerial::EncodeL( iOriginator, aWriteStream );
	aWriteStream.WriteUint8L( iIsConnectionActive );
    aWriteStream.WriteUint32L( iDialogId );
    aWriteStream.WriteUint32L( iTimeout );
    aWriteStream.WriteUint32L( iMinSE );
    MceSerial::EncodeL( iSIPHeaders, aWriteStream );
    MceSerial::EncodeL( iSIPContentType, aWriteStream );
    MceSerial::EncodeL( iSIPContentHeaders, aWriteStream );
    MceSerial::EncodeL( iLocalSessionSDPLines, aWriteStream );
    MceSerial::EncodeL( iRemoteSessionSDPLines, aWriteStream );
    TPckgBuf<TMceComSessionModifiers> modifiers( iSessionModifiers );
    MceSerial::EncodeL( modifiers, aWriteStream );
    MceSerial::EncodeL(iClientCryptoSuites, aWriteStream);
    aWriteStream.WriteUint32L( iServiceType );
    aWriteStream.WriteUint8L( iSipContactAddrSecure );
    }


// -----------------------------------------------------------------------------
// CMceComSession::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceComSession::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    TMceSessionSerializer<CMceComSession> serial( *this );
    serial.InternalizeL( aSerCtx );
    }
    
// -----------------------------------------------------------------------------
// CMceComSession::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceComSession::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    TMceSessionSerializer<CMceComSession> serial( *this );
    serial.ExternalizeL( aSerCtx );
    }
  

// -----------------------------------------------------------------------------
// CMceComSession::CMceComSession
// -----------------------------------------------------------------------------
//
CMceComSession::CMceComSession()
 : iID( KMceNotAssigned ),
   iType( EInSession ),
   iState( CMceSession::EIncoming ),
   iRecipient( NULL ),
   iOriginator( NULL ),
   iIsConnectionActive( ETrue ),
   iMccID( KMceNotAssigned ),
   iStructureChanged( EFalse ),
   iUseLocalPreconditions( EFalse ),
   iUseRemotePreconditions( EFalse ),
   iFcSignallingRequired( EFalse ),
   iRequireSignalling( EFalse ),
   iServiceType( KMceDefaultTypeOfServiceValue ),
   iAnswerType( KMceNegotiationAnswerTypeNotDefined ),
   iCodecSelection( KMceCodecSelectionOff ),
   iRemoteSecPreconditionsRequired( EFalse ),
   iTimeout( 0 ),
   iMinSE( 0 )
   {
    for( TInt i = 0;i<KMceMaxSessionModifiers;i++ )
        {
        iSessionModifiers[ i ] = KMaxTUint16;
        }
    Modifier( KMcePreconditions ) = KMcePreconditionsSupported;
    Modifier( KMce100Rel ) = KMce100RelSupported;
    Modifier( KMceMediaDirection ) = KMceMediaDirectionOnly;
    }

// -----------------------------------------------------------------------------
// CMceComSession::CMceComSession
// -----------------------------------------------------------------------------
//
CMceComSession::CMceComSession( CMceComSession::TType aType )
 : iID( KMceNotAssigned ),
   iType( aType ),
   iState( CMceSession::EIdle ),
   iRecipient( NULL ),
   iOriginator( NULL ),
   iIsConnectionActive( ETrue ),
   iMccID( KMceNotAssigned ),
   iServiceType( KMceDefaultTypeOfServiceValue ),
   iTimeout( 0 ),
   iMinSE( 0 )
    {
    if ( iType == EInSession )
        {
        iState = CMceSession::EIncoming;
        }
    for( TInt i = 0;i<KMceMaxSessionModifiers;i++ )
        {
        iSessionModifiers[ i ] = KMaxTUint16;
        }
    Modifier( KMcePreconditions ) = KMcePreconditionsSupported;        
    Modifier( KMce100Rel ) = KMce100RelSupported;
    Modifier( KMceMediaDirection ) = KMceMediaDirectionOnly;
    }

// -----------------------------------------------------------------------------
// CMceComSession::Id
// -----------------------------------------------------------------------------
//
TUint32 CMceComSession::Id() const
    {
    return iID;
    }


// -----------------------------------------------------------------------------
// CMceComSession::IsStructureChanged
// -----------------------------------------------------------------------------
//
TBool& CMceComSession::IsStructureChanged()
    {
    return iStructureChanged;
    }


// -----------------------------------------------------------------------------
// CMceComSession::EventReceivedL
// -----------------------------------------------------------------------------
//
void CMceComSession::EventReceivedL( TMceComEvent& aEvent )
    {

    __ASSERT_ALWAYS( aEvent.Id().iSessionID == iID, User::Leave( KErrGeneral ) );

    if ( aEvent.Id().IsSessionId() )
        {
        if ( aEvent.Action() == EMceItcUpdate )
            {
            CMceComSession* updateSession = static_cast<CMceComSession*>( aEvent.Message() );
            UpdateL( *updateSession );
            }
        else
            {
            User::Leave( KErrGeneral );
            }
        }
    else
        {
        TInt j = 0;
        TInt status = KMceEventNotConsumed;
        while ( status != KMceEventConsumed && j < iMediaStreams.Count() )
            {
            CMceComMediaStream* stream = iMediaStreams[j]; 
            status = stream->EventReceivedL( aEvent );
            j++;                
            }
        
        }
    
    
    }



// -----------------------------------------------------------------------------
// CMceComSession::InitializeL
// -----------------------------------------------------------------------------
//

void CMceComSession::InitializeL()
    {
    for( int i = 0; i < Streams().Count(); i++ )
        {
        Streams()[i]->InitializeL( *this );
        }
    for( int i = 0; i < Bundles().Count(); i++ )
        {
        Bundles()[i]->InitializeL( *this );
        }
        
    }
    
// -----------------------------------------------------------------------------
// CMceComSession::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComSession::UpdateL( CMceComSession& aSession )
    {

    IsStructureChanged() = EFalse;
    
    UpdateFlatL( aSession );

    delete iSIPHeaders;
    iSIPHeaders = aSession.iSIPHeaders;
    aSession.iSIPHeaders = NULL;
    
    delete iSIPContentType;
    iSIPContentType = aSession.iSIPContentType;
    aSession.iSIPContentType = NULL;
    
    delete iSIPContentHeaders;
    iSIPContentHeaders = aSession.iSIPContentHeaders;
    aSession.iSIPContentHeaders = NULL;

    delete iLocalSessionSDPLines;
    iLocalSessionSDPLines = aSession.iLocalSessionSDPLines;
    aSession.iLocalSessionSDPLines = NULL;
    
    UpdateStreamsL( aSession );
    InitializeL();
    
    }


// ---------------------------------------------------------
// CMceComSession::UpdateFlatL
// ---------------------------------------------------------
void CMceComSession::UpdateFlatL( CMceComSession& aSession )
    {
    
    iType = aSession.iType;
    iID = aSession.iID;
    iState = aSession.iState;
    
    delete iRecipient;
    iRecipient = aSession.iRecipient;
    aSession.iRecipient = NULL;

    delete iOriginator;
    iOriginator = aSession.iOriginator;
    aSession.iOriginator = NULL;

    iIsConnectionActive = aSession.iIsConnectionActive;
    iTimeout = aSession.iTimeout;
    iMinSE = aSession.iMinSE;
    
    delete iRemoteSessionSDPLines;
    iRemoteSessionSDPLines = aSession.iRemoteSessionSDPLines;
    aSession.iRemoteSessionSDPLines = NULL;
    
    iSessionModifiers = aSession.iSessionModifiers;
    
    iIsSecureSession=aSession.iIsSecureSession;
    iClientCryptoSuites.Reset();
    for (TInt i=0; i<aSession.iClientCryptoSuites.Count(); i++)
    	{
    	iClientCryptoSuites.InsertL( aSession.iClientCryptoSuites[i], i);
    	}
    iServiceType = aSession.iServiceType;
    iSipContactAddrSecure = aSession.iSipContactAddrSecure;
    }

// ---------------------------------------------------------
// CMceComSession::UpdateStreamsL
// ---------------------------------------------------------
void CMceComSession::UpdateStreamsL( CMceComSession& aSession )
    {
    RPointerArray<CMceComMediaStream> updated;
    MceCleanupResetAndDestroyPushL( updated );
    TInt push = 0;
    
    TInt i = 0;
    for( i = 0; i < aSession.Streams().Count(); i++ )
        {
        CMceComMediaStream* update = aSession.Streams()[i];
        
        TBool isUpdated = EFalse;
        TInt j = 0;
        while( !isUpdated && j < Streams().Count() )
            {
            CMceComMediaStream* stream = Streams()[j];
            
            if ( stream->Id() == update->Id() )
                {
                stream->UpdateL( *update );
                aSession.Streams().Remove( i );
                CleanupStack::PushL( update );
                push++;
                updated.AppendL( stream );
                Streams().Remove( j );
                i--;
                isUpdated = ETrue;
                }
            j++;                
            }
        }
        
    if ( Streams().Count() > 0 )//streams have been removed
        {
        IsStructureChanged() = ETrue;
        }
    
    UpdateBundlesL( aSession, updated );
    
    if ( push )
        {
        CleanupStack::PopAndDestroy( push );
        }
    
    iMediaStreams.ResetAndDestroy();
    while( updated.Count() > 0 )
        {
        AddStreamL( updated[0] );
        updated.Remove( 0 );
        }
        
    CleanupStack::PopAndDestroy();//updated
    
    if ( aSession.Streams().Count() > 0 )//streams have been added
        {
        IsStructureChanged() = ETrue;
            
        while( aSession.Streams().Count() > 0 )
            {
            IsStructureChanged() = ETrue;
            CMceComMediaStream* add = aSession.Streams()[0];
            AddStreamL( add );
            aSession.Streams().Remove( 0 );
            }
        }
        
    }

// ---------------------------------------------------------
// CMceComSession::UpdateBundlesL
// ---------------------------------------------------------
void CMceComSession::UpdateBundlesL( CMceComSession& aSession,
                                     const RPointerArray<CMceComMediaStream>& aStreams )
    {

    iBundles.ResetAndDestroy();
    
    TInt i = 0;
    for( i = 0; i < aSession.Bundles().Count(); i++ )
        {
        CMceComStreamBundle* update = aSession.Bundles()[i];
        CMceComStreamBundle* bundle = update->CloneL( aStreams );
        CleanupStack::PushL( bundle );
        bundle->UpdateL( *update );
        AddBundleL( bundle );
        CleanupStack::Pop( bundle );
        }
    }


// ---------------------------------------------------------
// CMceComSession::Modifier
// ---------------------------------------------------------
//
TUint& CMceComSession::Modifier( TMceSessionModifier aModifier )
    {
    return iSessionModifiers[ (TInt) aModifier ];
    }

// ---------------------------------------------------------
// CMceComSession::UseRtcp
// ---------------------------------------------------------
//
TBool CMceComSession::UseRtcp() const
    {
    TBool useRtcp( ETrue );
    for( TInt i=0; i < iMediaStreams.Count() && useRtcp; i++ )
        {
        useRtcp = iMediaStreams[i]->UseRtcp();
        }
    return useRtcp;    
    }

#ifdef MCE_COMMON_SERVER_SIDE

// ---------------------------------------------------------
// CMceComSession::Delete
// ---------------------------------------------------------
//
void CMceComSession::Delete( CMceComSession*& aThis, 
                             CMceMediaManager& aManager )
    {
    if( aThis )
        {
        aManager.CloseSession( *aThis );
        
        aThis->iMccStreams.ResetAndDestroy();
        aThis->iMccAdoptedStreams.ResetAndDestroy();
        delete aThis->iNegotiationState;
        aThis->iNegotiationState = NULL;
        
        delete aThis;
        aThis = NULL;
        }
    }

    
// ---------------------------------------------------------
// CMceComSession::SdpSession
// ---------------------------------------------------------
CMceSdpSession& CMceComSession::SdpSession() const
    {
    return *iSdpSession;
    }


// -----------------------------------------------------------------------------
// CMceComSession::Backup
// -----------------------------------------------------------------------------
//
CMceComSession* CMceComSession::Backup()
    {
    CMceComSession* backup = NULL;
    
    if ( iSdpSession )
        {
        backup = SdpSession().Backup();
        }
        
    return backup;
    
    }


// -----------------------------------------------------------------------------
// CMceComSession::IsBackup
// -----------------------------------------------------------------------------
//
TBool CMceComSession::IsBackup()
    {
    CMceComSession* backup = Backup();
    
    return this == backup;
    
    }


// -----------------------------------------------------------------------------
// CMceComSession::IsMerged
// -----------------------------------------------------------------------------
//
TBool CMceComSession::IsMerged()
    {
    TBool isMerged = EFalse;
    
    if ( Backup() )
        {
        isMerged = SdpSession().MediaSession()->iMccID ==
                   SdpSession().Backup()->iMccID;
        }
    
    return isMerged;
    
    }



// ---------------------------------------------------------
// CMceComSession::AnswerType
// ---------------------------------------------------------
TMceNegotiationAnswerType& CMceComSession::AnswerType()
    {
    return iAnswerType;
    }



// ---------------------------------------------------------
// CMceComSession::AttachSDPSessionL
// ---------------------------------------------------------
void CMceComSession::AttachSDPSessionL( CMceSdpSession& aSdpSession )
    {
	TMceMediaState* state = NULL;

    iSdpSession = &aSdpSession;
    
    if ( iCloneOrigin && !iNegotiationState ) 
        {
        __ASSERT_ALWAYS( 
            iCloneOrigin->NegotiationState().Id() == KMceMediaNegotiated, 
            User::Leave( KErrArgument ) );
            
        __ASSERT_ALWAYS( iCloneOrigin == aSdpSession.MediaSession(),
            User::Leave( KErrArgument ) );
        
    	//set negotiated state
    	state = new (ELeave) TMceMediaNegotiated( *this );
        SetNegotiationState( state );
        
        aSdpSession.SetBackup( iCloneOrigin );
        iCloneOrigin = NULL;
        
        }
    else if ( !iNegotiationState )
        {
    	//set idle state
    	state = new (ELeave) TMceMediaIdle( *this );
        SetNegotiationState( state );
        }
    else
        {
        //NOP
        }
        
    aSdpSession.SetMediaSession( this );
    
    }

// ---------------------------------------------------------
// CMceComSession::DetachSDPSession
// ---------------------------------------------------------
void CMceComSession::DetachSDPSession()
    {
    iSdpSession = NULL;
    }


// ---------------------------------------------------------
// CMceComSession::NegotiationState
// ---------------------------------------------------------
TMceMediaState& CMceComSession::NegotiationState() const
    {
    return *iNegotiationState;
    }


// ---------------------------------------------------------
// CMceComSession::SetNegotiationStateL
// ---------------------------------------------------------
TMceMMState CMceComSession::SetNegotiationStateL( TMceMediaState* aNewState )
    {
    
    __ASSERT_ALWAYS( aNewState != NULL, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aNewState != iNegotiationState, User::Leave( KErrArgument ) );

    TBool callbackAllowed = EFalse;    
    if ( iNegotiationState )
        {
        iNegotiationState->Pushed();
        TInt oldIndex = iNegotiationState->StackIndex();
        callbackAllowed = aNewState->CallbackAllowed();
        aNewState->EntryL();
        TInt newIndex = iNegotiationState->StackIndex();
        if ( oldIndex == newIndex ) //EntryL didn't push
            {
            SetNegotiationState( aNewState );
            }
        else if ( iNegotiationState->StackIndex() == 0 ) //new state is transitional
            {
            delete aNewState;
            aNewState = NULL;
            }

        if ( callbackAllowed && iNegotiationState == aNewState )
            {
            iNegotiationState->Callback();
            }
            
        }
    else
        {
        SetNegotiationState( aNewState );
        }
    
    
    return iNegotiationState->Id();
    
    }


// -----------------------------------------------------------------------------
// CMceComSession::SetNegotiationState
// -----------------------------------------------------------------------------
//
void CMceComSession::SetNegotiationState( TMceMediaState* aNewState )
    {
    delete iNegotiationState;
    iNegotiationState = aNewState;
    }


// -----------------------------------------------------------------------------
// CMceComSession::CloneAndMergeL
// -----------------------------------------------------------------------------
//
CMceComSession* CMceComSession::CloneAndMergeLC( CMceComSession& aSession )
    {

    CMceComSession* copy = new (ELeave) CMceComSession( iType );
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    copy->InitializeL();
    
    TMceMediaState* state = 
        aSession.NegotiationState().CloneL( *copy );
    copy->SetNegotiationState( state );
    
    copy->iUseLocalPreconditions = aSession.iUseLocalPreconditions;
    copy->iUseRemotePreconditions = aSession.iUseRemotePreconditions;
    copy->iFcSignallingRequired = aSession.iFcSignallingRequired;
    copy->iRequireSignalling = aSession.iRequireSignalling;
    copy->iServiceType = aSession.iServiceType;
    copy->iAnswerType = aSession.iAnswerType;
    copy->iRemoteSecPreconditionsRequired = aSession.iRemoteSecPreconditionsRequired;
	if( copy->iSecureMediaSession )
	    {
        copy->iSecureMediaSession->iLSReadyToBind = EFalse;
		}

    TInt index = 0;
    for( index = 0; index < copy->Streams().Count(); index++ )
        {
        TBool dges = aSession.Streams()[ index ]->DowngradedEndpoints();
        copy->Streams()[ index ]->DowngradedEndpoints() = dges;
        }
        
    copy->PrepareL( aSession.SdpSession().Manager() );
    
    
    
    return copy;
    
    }
    

// -----------------------------------------------------------------------------
// CMceComSession::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComSession::PrepareL( CMceMediaManager& aManager )
                               
    {
    TInt i = 0;
	MccStreams().ResetAndDestroy();
	for ( i = 0; i < iMediaStreams.Count(); i++)
		{
		CMceSrvStream::DecodeL( MccStreams(), *iMediaStreams[i], aManager );
		}
    }


// -----------------------------------------------------------------------------
// CMceComSession::AdoptStreamL
// -----------------------------------------------------------------------------
//
void CMceComSession::AdoptStreamL( CMceSrvStream& aStream )
    {
    
    CMceSrvStream* adopted = CMceAdoptedSrvStream::NewL( aStream );
    CleanupStack::PushL( adopted );
    iMccAdoptedStreams.AppendL( adopted );
    CleanupStack::Pop( adopted );
    
    }
    

// -----------------------------------------------------------------------------
// CMceComSession::RemoveAdoptedStream
// -----------------------------------------------------------------------------
//
CMceSrvStream* CMceComSession::RemoveAdoptedStream( CMceSrvStream& aStream ) 
    {
    
    TMceSrvStreamIterator adopts( AdoptedMccStreams() );
    CMceSrvStream* adopted = NULL;
    
    if ( adopts.Next( adopted, aStream ) )
        {
        AdoptedMccStreams().Remove( adopts.Current() );
        }
    return adopted;
    
    }



// -----------------------------------------------------------------------------
// CMceComSession::Merge
// -----------------------------------------------------------------------------
//
void CMceComSession::Merge( CMceComSession& aSession, TBool aDeepMerge )
    {
    
    iMccID = aSession.iMccID;
    
    TMceSrvStreamIterator mergeStreams( MccStreams() );
    TMceSrvStreamIterator mergeWithStreams( aSession.MccStreams() );
    
    CMceSrvStream* merge = NULL;
    CMceSrvStream* mergeWith = NULL;
    
    while( mergeStreams.NextIntersect( merge, mergeWithStreams, mergeWith ) )
        {
        merge->Merge( *mergeWith, aDeepMerge );
        }

    TInt index = 0;
    for( index = 0; index < Streams().Count(); index++ )
        {
        Streams()[ index ]->DowngradedEndpoints() = EFalse;
        }
    for( index = 0; index < aSession.Streams().Count(); index++ )
        {
        aSession.Streams()[ index ]->DowngradedEndpoints() = EFalse;
        }

    }

// -----------------------------------------------------------------------------
// CMceComSession::UnMerge
// -----------------------------------------------------------------------------
//
void CMceComSession::UnMerge()
    {
    
    TMceSrvStreamIterator streams( MccStreams() );
    CMceSrvStream* stream = NULL;
    
    while( streams.Next( stream ) )
        {
        stream->UnMerge();
        }
    
    }

// -----------------------------------------------------------------------------
// CMceComSession::UnMergeL
// -----------------------------------------------------------------------------
//
void CMceComSession::UnMergeL( CMceComSession& aSession )
    {
    TMceSrvStreamIterator unmergeStreams( MccStreams() );
    TMceSrvStreamIterator unmergeFromStreams( aSession.MccStreams() );
    
    CMceSrvStream* stream = NULL;
    CMceSrvStream* unmergeFrom = NULL;
    
    while( unmergeStreams.NextIntersect( stream, unmergeFromStreams, unmergeFrom ) )
        {
        stream->UnMergeL( *unmergeFrom );
        }
    
    }


// -----------------------------------------------------------------------------
// CMceComSession::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComSession::PrepareL()
    {
    TInt i = 0;
	for ( i = 0; i < iMediaStreams.Count(); i++)
		{
        iMediaStreams[i]->PrepareL();
		}
    }


// -----------------------------------------------------------------------------
// CMceComSession::MccStreams
// -----------------------------------------------------------------------------
//
RPointerArray<CMceSrvStream>& CMceComSession::MccStreams()
    {
    return iMccStreams;
    }

// -----------------------------------------------------------------------------
// CMceComSession::AdoptedMccStreams
// -----------------------------------------------------------------------------
//
RPointerArray<CMceSrvStream>& CMceComSession::AdoptedMccStreams()
    {
    return iMccAdoptedStreams;
    }


// ------------------------------------------------------------------------------
// CMceComSession::SecureSessionL
// ------------------------------------------------------------------------------    
CMceSecureMediaSession& CMceComSession::SecureSessionL()
    {
    if ( !SecureSession() )
        {
        iSecureMediaSession = CMceSecureMediaSession::NewL( *this,
                                         SdpSession().iManager,
                                         *(SdpSession().iManager.iMccSecureInterface ) );
        }
    return *SecureSession();
    }


// ------------------------------------------------------------------------------
// CMceComSession::SecureSession()
// ------------------------------------------------------------------------------    
CMceSecureMediaSession* CMceComSession::SecureSession()
    {
    return iSecureMediaSession;
    }

// --------------------------------------------------------------------------------------
// CMceComSession::CloneSecureSessionL()
// --------------------------------------------------------------------------------------    
void CMceComSession::CloneSecureSessionL(CMceComSession& aSession)
	{
	CMceSecureMediaSession* secureMediaSession = CMceSecureMediaSession::NewL(*this,
							aSession.SdpSession().iManager,
							*(aSession.SdpSession().iManager.iMccSecureInterface));
    CleanupStack::PushL(secureMediaSession);
    
    secureMediaSession->CopyStreamsL( *aSession.SecureSession());
    CleanupStack::Pop(secureMediaSession);
    
    delete iSecureMediaSession;
    
    iSecureMediaSession = secureMediaSession;
	}

// --------------------------------------------------------------------------------------
// CMceComSession::DeleteSecureSession()
// --------------------------------------------------------------------------------------    
void CMceComSession::DeleteSecureSession()
	{
	delete iSecureMediaSession;
    iSecureMediaSession = NULL;
	}

// --------------------------------------------------------------------------------------
// CMceComSession::SdpCleanup()
// --------------------------------------------------------------------------------------   
void CMceComSession::SdpCleanup( CSdpDocument* aPrevious, CSdpDocument* aReplacement )
    {
    if ( iSdpSession )
        {
        iSdpSession->SdpCleanup( aPrevious, aReplacement );
        }
        
    if ( Backup() && &Backup()->SdpSession() )
        {
        Backup()->SdpSession().SdpCleanup( aPrevious, aReplacement );
        }
    }
    

#endif//MCE_COMMON_SERVER_SIDE
