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
* Description:  Controller class implementation
*
*/

#include <sdpdocument.h>
#include <random.h>
#include <centralrepository.h>
#include <unsafprotocolscrkeys.h>
#include <e32math.h>
#include <badesca.h>
#include "natfwconnectivityapi.h"
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "nspsessionobserver.h"
#include "nspcontroller.h"
#include "nspsession.h"
#include "nspevents.h"
#include "nspactive.h"
#include "nspcontentparser.h"
#include "nspdefs.h"

// CONSTANTS
//_LIT8( KStun, "stun" );
//_LIT8( KTurn, "turn" );
_LIT8( KIce, "ice" );

_LIT8( KNSPCharStore,
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/" );
_LIT8( KNSPSingleDigit, "x" );

const TText8* const KComma = _S8( "," );
const TUint KCommaLength = 1;
const TText8* const KDot = _S8( "." );

const TUint KNSPSingleDigitLength = 1;
const TUint KNSPUsernameMinLength = 4;
const TUint KNSPPasswordMinLength = 22;
const TUint KNSPVariationLength = 4;
const TUint KDefaultUpdateSdpTimerValue = 10;

static TDesC8& TrimAll( HBufC8& aDes )
    {
    TPtr8 ptr( aDes.Des() );
    ptr.TrimAll();
    return aDes;
    }


// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPController::CNSPController
// ---------------------------------------------------------------------------
//
CNSPController::CNSPController()
    {    
    }


// ---------------------------------------------------------------------------
// CNSPController::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPController::ConstructL()
    {
    NSPLOG_STR( "CNSPController::ConstructL(), Entry" )
    
    iInterface = CNATFWConnectivityApi::NewL();
    iInterface->RegisterObserverForEventsL(
        *this, MNATFWConnectivityObserver::EAllEvents );
    iContentParser = CNSPContentParser::NewL();
    iBase = Abs( static_cast<TInt>( Math::Random() ) );
    
    NSPLOG_STR( "CNSPController::ConstructL(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPController::NewL
// ---------------------------------------------------------------------------
//
CNSPController* CNSPController::NewL()
    {
    CNSPController* self = CNSPController::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPController::NewLC
// ---------------------------------------------------------------------------
//
CNSPController* CNSPController::NewLC()
    {
    CNSPController* self = new ( ELeave ) CNSPController;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPController::~CNSPController
// ---------------------------------------------------------------------------
//
CNSPController::~CNSPController()
    {
    NSPLOG_STR( "CNSPController::~CNSPController(), Entry" )
    
    iActiveObjects.ResetAndDestroy();
    iActiveObjects.Close();    
    iSessionArray.ResetAndDestroy();
    iSessionArray.Close();
    iClosingSessionArray.ResetAndDestroy();
    iClosingSessionArray.Close();
    delete iContentParser;
    if ( iInterface )
        {
        iInterface->UnregisterObserverForEvents(
            *this, MNATFWConnectivityObserver::EAllEvents );
        }
    delete iInterface;
    
    NSPLOG_STR( "CNSPController::~CNSPController(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPController::EventOccured
// ---------------------------------------------------------------------------
//
void CNSPController::EventOccured( TUint aSessionId, TUint aStreamId,
        TNATFWConnectivityEvent aEvent, TInt aError, TAny* aEventData )
    {
    NSPLOG_STR( "CNSPController::EventOccured(), Entry" )
    
    TInt index = FindSession( aSessionId );
    
    if ( KErrNotFound != index )
        {
        TEventReturnStatus status = iSessionArray[index]->EventOccured(
        		aStreamId, aEvent, aError, aEventData );
        Callback( *iSessionArray[index], status );
        }
    else
        {
        index = FindClosingSession( aSessionId );
        
        if ( KErrNotFound != index )
            {
            TEventReturnStatus status = iClosingSessionArray[index]->EventOccured(
                    aStreamId, aEvent, aError, aEventData );
            
            if ( NSP_DELETE( status.iStatus ) || NSP_ERROR( status.iStatus ) )
                {
                delete iClosingSessionArray[index];
                iClosingSessionArray.Remove(index);
                iClosingSessionArray.Close();
                RemoveActiveObjects( aSessionId );
                }
            }
        else
            {
            NSPLOG_STR( "CNSPController::EventOccured(), KErrNotFound" )
            NSPLOG_INT( "CNSPController::EventOccured(), error:", aError )
            }
        }
    
    NSPLOG_STR( "CNSPController::EventOccured(), Exit" )
    }


// ---------------------------------------------------------------------------
// CNSPController::ContentParser
// ---------------------------------------------------------------------------
//
const CNSPContentParser& CNSPController::ContentParser()
    {
    return (*iContentParser);
    }


// ---------------------------------------------------------------------------
// CNSPController::CreateSessionL
// ---------------------------------------------------------------------------
//
TUint CNSPController::CreateSessionL( TUint32 aIapId, const TDesC8& aDomain )
    {
    return iInterface->CreateSessionL( aIapId, aDomain );
    }


// ---------------------------------------------------------------------------
// CNSPController::LoadPluginL
// ---------------------------------------------------------------------------
//
void CNSPController::LoadPluginL( TUint aSessionId, const CDesC8Array& aPlugins,
        TInt& aPluginIndex )
    {
    iInterface->LoadPluginL( aSessionId, aPlugins, aPluginIndex );
    }


// ---------------------------------------------------------------------------
// CNSPController::CreateStreamL
// ---------------------------------------------------------------------------
//
TUint CNSPController::CreateStreamL( TUint aSessionId, TUint aProtocol, TInt aQoS )
    {
    return iInterface->CreateStreamL( aSessionId, aProtocol, aQoS );
    }


// ---------------------------------------------------------------------------
// CNSPController::CreateWrapperL
// ---------------------------------------------------------------------------
//
MNATFWSocketMediaConnWrapper& CNSPController::CreateWrapperL( 
        TUint aSessionId, TUint aStreamId )
    {
    return iInterface->CreateWrapperL( aSessionId, aStreamId );
    }


// ---------------------------------------------------------------------------
// CNSPController::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CNSPController::FetchCandidateL( TUint aSessionId, TUint aStreamId,
        TUint aAddrFamily )
    {
    iInterface->FetchCandidateL( aSessionId, aStreamId, aAddrFamily );
    }


// ---------------------------------------------------------------------------
// CNSPController::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPController::FetchCandidatesL( TUint aSessionId, TUint aStreamId,
        TUint aCollectionId, TUint aComponentId, TUint aAddrFamily )
    {
    iInterface->FetchCandidatesL( aSessionId, aStreamId,
            aCollectionId, aComponentId, aAddrFamily );
    }


// ---------------------------------------------------------------------------
// CNSPController::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CNSPController::SetReceivingStateL( const CNATFWCandidate& aLocalCand,
        TNATFWStreamingState aState )
    {
    iInterface->SetReceivingStateL( aLocalCand, aState );
    }


// ---------------------------------------------------------------------------
// CNSPController::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CNSPController::SetSendingStateL( const CNATFWCandidate& aLocalCand,
        TNATFWStreamingState aState, const TDesC8& aDestAddr, TUint aPort )
    {
    iInterface->SetSendingStateL( aLocalCand, aState, aDestAddr, aPort );
    }


// ---------------------------------------------------------------------------
// CNSPController::SetRoleL
// ---------------------------------------------------------------------------
//
void CNSPController::SetRoleL( TUint aSessionId, TNATFWIceRole aRole )
    {
    iInterface->SetRoleL( aSessionId, aRole );
    }


// ---------------------------------------------------------------------------
// CNSPController::SetCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPController::SetCredentialsL( TUint aSessionId, TUint aStreamId,
        const CNATFWCredentials& aCredentials )
    {
    CNATFWCandidate* dummyCand = CNATFWCandidate::NewLC();
    dummyCand->SetSessionId( aSessionId );
    dummyCand->SetStreamId( aStreamId );
    iInterface->SetCredentialsL( *dummyCand, aCredentials );
    CleanupStack::PopAndDestroy( dummyCand );
    }


// ---------------------------------------------------------------------------
// CNSPController::PerformCandidateChecksL
// ---------------------------------------------------------------------------
//
void CNSPController::PerformCandidateChecksL( TUint aSessionId,
        RPointerArray<CNATFWCandidate>& aRemoteCandidates )
    {
    iInterface->PerformConnectivityChecksL( aSessionId, aRemoteCandidates );
    }


// ---------------------------------------------------------------------------
// CNSPController::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CNSPController::UpdateIceProcessingL( TUint aSessionId,
        RPointerArray<CNATFWCandidate>& aNewRemoteCands )
    {
    iInterface->UpdateIceProcessingL( aSessionId, aNewRemoteCands );
    }


// ---------------------------------------------------------------------------
// CNSPController::CloseStreamL
// ---------------------------------------------------------------------------
//
void CNSPController::CloseStreamL( TUint aSessionId, TUint aStreamId )
    {
    iInterface->CloseStreamL( aSessionId, aStreamId );
    }


// ---------------------------------------------------------------------------
// CNSPController::CloseSessionL
// ---------------------------------------------------------------------------
//
void CNSPController::CloseSessionL( TUint aSessionId )
    {
    iInterface->CloseSessionL( aSessionId );
    }


// ---------------------------------------------------------------------------
// CNSPController::CreateUniqueId
// ---------------------------------------------------------------------------
//
TUint CNSPController::CreateUniqueId()
    {
    return (iBase++);
    }


// ---------------------------------------------------------------------------
// CNSPController::OrderTimeoutL
// ---------------------------------------------------------------------------
//
TUint CNSPController::OrderTimeoutL( TUint aSessionId, TUint aStreamId,
        MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
        TUint aTimerInMicroSeconds )
    {
    CNSPActive* callback = NULL;
    const TInt index = FindSession( aSessionId );
    const TUint transactionId = CreateUniqueId();
    
    if ( KErrNotFound != index )
        {
        TRAPD( error, callback = CNSPActive::NewL( *this,
                aSessionId, aStreamId, transactionId, aEvent,
                aTimerInMicroSeconds, NULL ) );
        
        User::LeaveIfError( error );
        error = iActiveObjects.Append( callback );
        
        if ( KErrNone != error )
            {
            delete callback;
            User::LeaveIfError( error );
            }
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    return transactionId;
    }


// ---------------------------------------------------------------------------
// CNSPController::OrderUpdateSdpL
// ---------------------------------------------------------------------------
//
TUint CNSPController::OrderUpdateSdpL( TUint aSessionId, CSdpDocument* aDoc )
    {
    CNSPActive* callback = NULL;
    const TInt index = FindSession( aSessionId );
    const TUint transactionId = CreateUniqueId();
    
    if ( KErrNotFound != index )
        {
        CleanupStack::PushL( aDoc );
        callback = CNSPActive::NewL( *this,
                aSessionId, (TUint) KErrNone, transactionId,
                (MNATFWConnectivityObserver::TNATFWConnectivityEvent) KErrNone,
                KDefaultUpdateSdpTimerValue, aDoc );
        CleanupStack::Pop( aDoc );
        CleanupStack::PushL( callback );
        
        iActiveObjects.AppendL( callback );
        CleanupStack::Pop( callback );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    return transactionId;
    }


// ---------------------------------------------------------------------------
// CNSPController::Cancel
// ---------------------------------------------------------------------------
//
void CNSPController::Cancel( TUint aTransactionId )
    {
    const TInt index = FindActiveObject( aTransactionId );
    
    if ( KErrNotFound != index )
        {
        iActiveObjects[index]->Cancel();
        delete (iActiveObjects[index]);
        iActiveObjects.Remove(index);
        iActiveObjects.Compress();
        }
    }


// ---------------------------------------------------------------------------
// CNSPController::GenerateUsernameL
// ---------------------------------------------------------------------------
//
void CNSPController::GenerateUsernameL( TDes8& aUsername )
    {
    aUsername.Zero();
    
    TInt newlength = KNSPUsernameMinLength +
                     RandomByteL() % KNSPVariationLength;
    __ASSERT_ALWAYS( aUsername.MaxLength() >= newlength,
                     User::Leave( KErrArgument ) );
    
    for ( TInt index = 0; index < newlength; index++ )
        {
        aUsername.Append( KNSPCharStore()[
                RandomByteL() % KNSPCharStore().Length() ] );
        }
    }


// ---------------------------------------------------------------------------
// CNSPController::GeneratePasswordL
// ---------------------------------------------------------------------------
//
void CNSPController::GeneratePasswordL( TDes8& aPassword )
    {
    aPassword.Zero();
    
    TInt newlength = KNSPPasswordMinLength +
                     RandomByteL() % KNSPVariationLength;    
    __ASSERT_ALWAYS( aPassword.MaxLength() >= newlength,
                     User::Leave( KErrArgument ) );
    
    for ( TInt index = 0; index < newlength; index++ )
        {
        aPassword.Append( KNSPCharStore()[
                RandomByteL() % KNSPCharStore().Length() ] );
        }
    }


// ---------------------------------------------------------------------------
// CNSPController::NewSessionL
// ---------------------------------------------------------------------------
//
TUint CNSPController::NewSessionL( MNSPSessionObserver& aSessionObserver,
        TUint32 aIapId, const TDesC8& aDomain, TUint aProtocol )
    {
    NSPLOG_STR( "CNSPController::NewSessionL(), Entry" )
    
    CNSPSession* session = CNSPSession::NewLC( *this,
            aSessionObserver, aIapId, aDomain, aProtocol );
    session->SetUseIce( UseIceL( session->Plugins(), aDomain ) );
    User::LeaveIfError( iSessionArray.Append( session ) );
    CleanupStack::Pop( session );
    
    NSPLOG_STR( "CNSPController::NewSessionL(), Exit" )
    return session->SessionId();
    }


// ---------------------------------------------------------------------------
// CNSPController::CloseSessionL
// ---------------------------------------------------------------------------
//
void CNSPController::ClosingSessionL( TUint aSessionId )
    {
    const TInt index = FindSession( aSessionId );
    
    if ( KErrNotFound != index )
        {
        CNSPSession* session = iSessionArray[index];
        iSessionArray.Remove(index);
        iSessionArray.Compress();
        
        CleanupStack::PushL( session );
        TNatReturnStatus status = session->CloseSessionL();
        
        if ( KNatReady == status )
            {
            CleanupStack::Pop( session );
            delete session;
            }
        else
            {
            iClosingSessionArray.AppendL( session );// ownership changed
            CleanupStack::Pop( session );
            }
        }
    else
        {
        NSPLOG_STR( "CNSPController::CloseSessionL(), KErrNotFound" )
        User::Leave( KErrNotFound );
        }
    }


// ---------------------------------------------------------------------------
// CNSPController::FindSessionObject
// ---------------------------------------------------------------------------
//
CNSPSession& CNSPController::FindSessionObjectL( TUint aSessionId )
    {
    const TInt index = FindSession( aSessionId );
    
    if ( KErrNotFound == index )
        {
        NSPLOG_STR( "CNSPController::FindSessionObjectL(), KErrNotFound" )
        User::Leave( KErrNotFound );
        }
    
    return *iSessionArray[index];
    }


// ---------------------------------------------------------------------------
// CNSPController::FindSession
// ---------------------------------------------------------------------------
//
TInt CNSPController::FindSession( TUint aSessionId )
    {
    const TInt sessionCount( iSessionArray.Count() );
    for ( TInt index = 0; index < sessionCount; index++ )
        {
        if ( iSessionArray[index]->SessionId() == aSessionId )
            {
            return index;
            }
        }
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CNSPController::FindClosingSession
// ---------------------------------------------------------------------------
//
TInt CNSPController::FindClosingSession( TUint aSessionId )
    {
    const TInt sessionCount( iClosingSessionArray.Count() );
    for ( TInt index = 0; index < sessionCount; index++ )
        {
        if ( iClosingSessionArray[index]->SessionId() == aSessionId )
            {
            return index;
            }
        }
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CNSPController::FindActiveObject
// ---------------------------------------------------------------------------
//
TInt CNSPController::FindActiveObject( TUint aTransactionId )
    {
    if( aTransactionId != 0 )
        {
        const TInt activeObjectsCount( iActiveObjects.Count() );
        for ( TInt index = 0 ; index < activeObjectsCount; index++ )
            {
            if ( iActiveObjects[index]->TransactionId() == aTransactionId )
                {
                return index;
                }
            }
        }
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CNSPController::RemoveActiveObjects
// ---------------------------------------------------------------------------
//
void CNSPController::RemoveActiveObjects( TUint aSessionId )
    {
    const TInt activeObjectsCount( iActiveObjects.Count() );
    for ( TInt index = 0 ; index < activeObjectsCount; index++ )
        {
        if ( iActiveObjects[index]->SessionId() == aSessionId )
            {
            iActiveObjects[index]->Cancel();
            delete (iActiveObjects[index]);
            iActiveObjects.Remove(index);
            iActiveObjects.Compress();
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPController::RandomByteL
// ---------------------------------------------------------------------------
//
TUint8 CNSPController::RandomByteL() const
    {
    TBuf8< KNSPSingleDigitLength > buffer( KNSPSingleDigit );
    GenerateRandomBytesL( buffer ); // random byte received, as TDes8
    return static_cast< TUint8 >( buffer[0] );
    }


// ---------------------------------------------------------------------------
// CNSPController::UseIceL
// ---------------------------------------------------------------------------
//
TBool CNSPController::UseIceL( CDesC8Array& aArray, const TDesC8& aDomain ) const
    {
    CRepository* cenrep = CRepository::NewLC( KCRUidUNSAFProtocols );
    
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    
    User::LeaveIfError( cenrep->FindEqL( KUNSAFProtocolsDomainMask,
            KUNSAFProtocolsFieldTypeMask, aDomain, keys ) );
    __ASSERT_ALWAYS( keys.Count(), User::Leave( KErrNotFound ) );
    
    TUint32 currentDomainKey = KErrNone;
    currentDomainKey = KUNSAFProtocolsDomainMask^keys[0];
    currentDomainKey |= KUNSAFProtocolsUsedNATProtocolMask;
    
    TBuf8<1> tmp;
    TInt actualLength( 0 );
    cenrep->Get( currentDomainKey, tmp, actualLength ); // ret KErrOverFlow
    
    if ( 0 < actualLength )
        {
        HBufC8* protocol = HBufC8::NewLC( actualLength );
        TPtr8 ptr( protocol->Des() );
        User::LeaveIfError( cenrep->Get( currentDomainKey, ptr ) );
        ReplaceArrayL( aArray, protocol->Des() ); // leave if empty
        CleanupStack::PopAndDestroy( protocol );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    CleanupStack::PopAndDestroy(); // keys
    CleanupStack::PopAndDestroy( cenrep );
    
    return IsIce( aArray.MdcaPoint( 0 ) );
    }


// ---------------------------------------------------------------------------
// CNSPController::ReplaceArrayL
// ---------------------------------------------------------------------------
//
void CNSPController::ReplaceArrayL( CDesC8Array& aDesArray,
                                                const TDesC8& aString ) const
    {
    aDesArray.Reset();
    TPtrC8 ptr( TrimAll( *( aString.AllocLC() ) ) );
    
    while ( ptr.Length() )
        {
        const TInt length = ptr.Length();
        const TInt index = ptr.Find( TPtrC8( KComma ) );
        const TInt left = KErrNotFound != index ? index : length;
        const TInt right = ( length - KCommaLength ) - left;
        aDesArray.AppendL( TrimAll( *( ptr.Left( left ).AllocLC() ) ) );
        CleanupStack::PopAndDestroy();
        ptr.Set( ptr.Right( 0 < right ? right : 0 ) );
        }
    
    CleanupStack::PopAndDestroy();
    User::LeaveIfError( aDesArray.Count() ? KErrNone : KErrArgument );
    }


// ---------------------------------------------------------------------------
// CNSPController::RemoveIce
// ---------------------------------------------------------------------------
//
void CNSPController::RemoveIce( CDesC8Array& aDesArray ) const
    {
    for ( TInt index = 0; index < aDesArray.MdcaCount(); )
        {
        if ( IsIce( aDesArray.MdcaPoint( index ) ) )
            {
            aDesArray.Delete( index );
            aDesArray.Compress();
            }
        else
            {
            index++;
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPController::IsIce
// ---------------------------------------------------------------------------
//
TBool CNSPController::IsIce( const TDesC8& aProtocol ) const
    {
    TInt index = aProtocol.Find( TPtrC8( KDot ) );
    
    if ( KErrNotFound !=  index )
        {
        const TInt right = ( aProtocol.Length() - index ) - KCommaLength;
        TPtrC8 ptr( aProtocol.Right( 0 < right ? right : 0 ) );
        return ( !ptr.CompareF( KIce() ) ? ETrue : EFalse );
        }
    else
        {
        return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// CNSPController::Callback
// ---------------------------------------------------------------------------
//
void CNSPController::Callback( const CNSPSession& aSession,
                                        TEventReturnStatus& aCallback ) const
	{
	switch ( aCallback.iType )
		{
		case TEventReturnStatus::ENone:
			{
			// NOP
			break;
			}
		
		case TEventReturnStatus::EInitialized:
			{
			ExecuteInitialized( aSession );
			break;
			}
		
		case TEventReturnStatus::EOfferReady:
			{
			ExecuteOfferReady( aSession, aCallback.iOffer );
			break;
			}
		
		case TEventReturnStatus::EAnswerReady:
			{
			ExecuteAnswerReady( aSession, aCallback.iAnswer );
			break;
			}
		
		case TEventReturnStatus::EUpdateSdp:
			{
			ExecuteUpdateSdp( aSession, aCallback.iOffer );
			break;
			}
		
		case TEventReturnStatus::EError:
			{
            ExecuteErrorOccurred( aSession, aCallback.iStatus );
			break;
			}
		
		default:
			{
			// NOP
			}
		}
	}


// ---------------------------------------------------------------------------
// CNSPController::ExecuteInitialized
// ---------------------------------------------------------------------------
//
void CNSPController::ExecuteInitialized( const CNSPSession& aSession ) const
	{
	MNSPSessionObserver& observer = aSession.SessionObserver();
	TUint sessionId = aSession.SessionId();
	
	NSP_OUTPUT_INITIALIZED
	observer.Initialized( sessionId );
	}


// ---------------------------------------------------------------------------
// CNSPController::ExecuteOfferReady
// ---------------------------------------------------------------------------
//
void CNSPController::ExecuteOfferReady( const CNSPSession& aSession,
		CSdpDocument* aOffer ) const
	{
	MNSPSessionObserver& observer = aSession.SessionObserver();
	TUint sessionId = aSession.SessionId();
	
	if ( aOffer )
		{
		NSP_OUTPUT_OFFER( KNatReady, *aOffer )
		observer.OfferReady( sessionId, aOffer );
		}
	else
		{
		ExecuteErrorOccurred( aSession, KErrTotalLossOfPrecision );
		}
	}


// ---------------------------------------------------------------------------
// CNSPController::ExecuteAnswerReady
// ---------------------------------------------------------------------------
//
void CNSPController::ExecuteAnswerReady( const CNSPSession& aSession,
		CSdpDocument* aAnswer ) const
	{
	MNSPSessionObserver& observer = aSession.SessionObserver();
	TUint sessionId = aSession.SessionId();
	
	if ( aAnswer )
		{
		NSP_OUTPUT_ANSWER( KNatReady, *aAnswer )
		observer.AnswerReady( sessionId, aAnswer );
		}
	else
		{
		ExecuteErrorOccurred( aSession, KErrTotalLossOfPrecision );
		}
	}


// ---------------------------------------------------------------------------
// CNSPController::ExecuteUpdateSdp
// ---------------------------------------------------------------------------
//
void CNSPController::ExecuteUpdateSdp( const CNSPSession& aSession,
		CSdpDocument* aOffer ) const
	{
	MNSPSessionObserver& observer = aSession.SessionObserver();
	TUint sessionId = aSession.SessionId();
	
	if ( aOffer )
		{
		NSP_OUTPUT_UPDATESDP( KNatReady, *aOffer )
		observer.UpdateSdp( sessionId, aOffer );
		}
	else
		{
		ExecuteErrorOccurred( aSession, KErrTotalLossOfPrecision );
		}
	}


// ---------------------------------------------------------------------------
// CNSPController::ExecuteErrorOccurred
// ---------------------------------------------------------------------------
//
void CNSPController::ExecuteErrorOccurred(
        const CNSPSession& aSession, TInt aError ) const
	{
	MNSPSessionObserver& observer = aSession.SessionObserver();
	TUint sessionId = aSession.SessionId();
	
	NSP_OUTPUT_ERROR( aError )
	
	if ( KErrCommsLineFail == aError )
		{
		observer.IcmpErrorOccurred( sessionId, aError );
		}
	else
		{
		observer.ErrorOccurred( sessionId, aError );
		}
	}

// end of file
