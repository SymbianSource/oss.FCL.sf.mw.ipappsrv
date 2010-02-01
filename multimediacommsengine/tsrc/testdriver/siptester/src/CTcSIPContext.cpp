/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation.
*
*/

#include <sip.h>
#include <siperr.h>
#include <sipconnection.h>
#include <sipheaderbase.h>
#include <sipcseqheader.h>
#include <sipextensionheader.h>
#include <sipresponseelements.h>
#include <sipservertransaction.h>
#include <sipmanagedprofileregistry.h>
#include <sipprofileregistry.h>

#include <sipheaderlookup.h>

#include "TesterConstants.h"
#include "tcuid.h"
#include "TcLog.h"
#include "CTcFileHandler.h"
#include "ErrorHandling.h"

#include "CTcSIPProfileContainer.h"
#include "CTcSIPConnectionContainer.h"
#include "CTcSIPHttpDigestContainer.h"
#include "CTcSIPClientDiscoveryContainer.h"
#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "TTcSIPCommandBase.h"
#include "TTcSIPReceived.h"
#include "CTTcSIPFactory.h"
#include "CTcSIPIMSAuthenticator.h"
#include <sipclientresolverconfigcrkeys.h>

#include "CommandIndex.h"

CTcSIPContext* CTcSIPContext::NewLC( CTcCTRLCodec& aCodec,
									 TInt aIAPId,
                                     TBool aIsMandatory )
	{
	CTcSIPContext* self = new( ELeave ) CTcSIPContext( aCodec );

	CleanupStack::PushL( self );
	self->ConstructL( aIAPId, aIsMandatory );

	return self;
	}

CTcSIPContext::~CTcSIPContext()
	{
	TcLog::Write( _L8("CTcSIPContext::~CTcSIPContext() start\n") );
	
	iSipClientSimulator.Close();
	
	delete iFileHandler;
	
	delete iIMSAuthenticator;
	
	iFreezer->Remove( iTimerEntry );
	delete iFreezer;
	
	// Must be deleted before deleting transactions (in registry)
	delete iSipConnection;

    // We have to delete SIP Profiles before deleting
	// ManagedProfileRegistry instance.
	iRegistry.DestroyAll();

    delete iSipClientDiscovery;
    delete iSipHttpDigest;
    delete iSipProfile;
	SIPHeaderLookup::Close();
	delete iSip;
	delete iIapManager;
	delete iCenRep;
	TcLog::Write( _L8("CTcSIPContext::~CTcSIPContext() end\n") );
	}

CTcSIPContext::CTcSIPContext( CTcCTRLCodec& aCodec )
	: CTcContextBase( aCodec ), iIsMandatory( EFalse ), iSipClientDiscovery( 0 )
	{
	}

void CTcSIPContext::ConstructL( TInt aIAPId, TBool aIsMandatory )
	{
    iIAPId = aIAPId;
    iIsMandatory  = aIsMandatory;

	TcLog::Write( _L8("CTcSIPContext::ConstructL() start\n") );
	iIapManager = CTcIAPManager::NewL();
	
	iFileHandler = CTcFileHandler::NewL();

    iFreezer = CDeltaTimer::NewL( CActive::EPriorityStandard );
    
    if ( !iIsMandatory )
        {
	    TcLog::Write( _L8("CTcSIPContext::ConstructL() initializing SIP client\n") );
	    iSip = CSIP::NewL( TUid::Uid( TCUID ), *this );
	    iSipConnection = CTcSIPConnectionContainer::NewL( *this, IAPNameL( aIAPId ) );
	    SIPHeaderLookup::OpenL();

	    TcLog::Write( _L8("CTcSIPContext::ConstructL() initializing SIP Profiles client\n") );
        iSipProfile = CTcSIPProfileContainer::NewL( *this, *iSip );
        
        TcLog::Write( _L8("CTcSIPContext::ConstructL() initializing SIP Http Digest\n") );
        iSipHttpDigest = CTcSIPHttpDigestContainer::NewL( *this, *iSip );
        }

    User::LeaveIfError( iSipClientSimulator.Connect() );
    
    iCenRep = CRepository::NewL( KCRUidSIPClientResolverConfig );

	TcLog::Write( _L8("CTcSIPContext::ConstructL() end\n") );
	}

// -- CTcContextBase functions ------------------------------------------------

TTcCommandBase* CTcSIPContext::CreateCommandL( TTcIdentifier& aId,
											   MTcTestContext& aContext )
	{
	// Use base class to actually create commands. Just provide the command
	// array and its size.
	return CTcContextBase::CreateCommandL( KTcSIPCommands,
										   sizeof( KTcSIPCommands ),
										   aId, aContext );
	}

// -- MSIPObserver functions --------------------------------------------------

// Note that adding an object by-reference to the registry does NOT transfer
// ownership. Adding by-pointer does transfer ownership, however.
// Most/all SIP objects should be in the registry at this stage anyway,
// just make sure that's the case.

void CTcSIPContext::IncomingRequest( TUint32 aIapId,
									 CSIPServerTransaction* aTransaction )
	{
	TRAPD( err, {
		iRegistry.AddObjectL( aTransaction );
		TTcSIPReceived received;
		received.Set( aIapId );
		received.Set( aTransaction );
		iSipConnection->QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

void CTcSIPContext::TimedOut( CSIPServerTransaction& aSIPServerTransaction )
	{
	TRAPD( err, {
		iRegistry.AddObjectL( aSIPServerTransaction );
		TTcSIPReceived received;
		received.Set( &aSIPServerTransaction );
		received.SetError( KErrTimedOut );
		iSipConnection->QueueReceivedL( received );
		} )
	__ASSERT_ALWAYS( !err, Panic( KSIPErrOOMInNotifier ) );
	}

//
// -- Internal utility functions ----------------------------------------------
//
TBool CTcSIPContext::GetMandatoryValue( void ) const
    {
    return iIsMandatory;
    }

void CTcSIPContext::CreateSipStack( void )
    {
    TcLog::Write( _L8("CTcSIPContext::ConstructL() initializing SIP client\n") );
    iSip = CSIP::NewL( TUid::Uid( TCUID ), *this );
    iSipConnection = CTcSIPConnectionContainer::NewL( *this, IAPNameL( iIAPId ) );
    SIPHeaderLookup::OpenL();

    TcLog::Write( _L8("CTcSIPContext::ConstructL() initializing SIP Profiles client\n") );
    iSipProfile = CTcSIPProfileContainer::NewL( *this, *iSip );
    
    TcLog::Write( _L8("CTcSIPContext::ConstructL() initializing SIP Http Digest\n") );
    iSipHttpDigest = CTcSIPHttpDigestContainer::NewL( *this, *iSip );
    }

CTcSIPConnectionContainer* CTcSIPContext::CreateSipStack( const TDesC8& aName )
    {
    TcLog::Write( _L8("CTcSIPContext::ConstructL() initializing SIP client\n") );
    iSip = CSIP::NewL( TUid::Uid( TCUID ), *this );
    iSipConnection = CTcSIPConnectionContainer::NewL( *this, aName);
    SIPHeaderLookup::OpenL();

    TcLog::Write( _L8("CTcSIPContext::ConstructL() initializing SIP Profiles client\n") );
    iSipProfile = CTcSIPProfileContainer::NewL( *this, *iSip );
    
    TcLog::Write( _L8("CTcSIPContext::ConstructL() initializing SIP Http Digest\n") );
    iSipHttpDigest = CTcSIPHttpDigestContainer::NewL( *this, *iSip );
    
	return iSipConnection;
    }

CTcSIPHttpDigestContainer& CTcSIPContext::DigestContainerL()
    {
    __ASSERT_ALWAYS( iSipHttpDigest, User::Leave( KErrNotFound ) );
    return *iSipHttpDigest;
    }
    
void CTcSIPContext::RemoveDigestContainer()
    {
    delete iSipHttpDigest;
    iSipHttpDigest = NULL;
    }
    
void CTcSIPContext::CreateClientDiscoveryL( TUid aUid )
    {
    __ASSERT_ALWAYS( !iSipClientDiscovery, User::Leave( KErrAlreadyExists ) );
    #ifdef RD_SIP_TESTER
        iSipClientDiscovery = CTcSIPClientDiscoveryContainer::NewL( *this, aUid );
    #else
        aUid.iUid; // Avoid compiler warning
    #endif // RD_SIP_TESTER
    }
		
void CTcSIPContext::RemoveClientDiscovery()
    {
    #ifdef RD_SIP_TESTER
        delete iSipClientDiscovery;
        iSipClientDiscovery = 0;
    #endif // RD_SIP_TESTER
    }
		
CTcSIPClientDiscoveryContainer& CTcSIPContext::ClientDiscoveryL()
    {
    __ASSERT_ALWAYS( iSipClientDiscovery, User::Leave( KErrNotFound ) );
    return *iSipClientDiscovery;
    }
    
void CTcSIPContext::IssueFreezeL( TInt aFreezeTimeSeconds )
    {
    if ( iFreezer->IsActive() )
        {
        User::Leave( KErrInUse );
        }
        
    iFreezeTimeSeconds = aFreezeTimeSeconds;
    TCallBack callback( Freeze, &iFreezeTimeSeconds );
    iTimerEntry.Set( callback );
    iFreezer->Queue( KFreezeAfterTimer * KSecondAsMicros, iTimerEntry );
    }
    
TInt CTcSIPContext::Freeze( TAny* aAny )
    {
    TInt freezeTime = *static_cast< TInt* >( aAny );
    User::After( freezeTime * KSecondAsMicros );
    return KErrNone;
    }
    
CTcSIPIMSAuthenticationParams& CTcSIPContext::AuthenticateL( const TDesC8& aNonce )
    {
    if ( !iIMSAuthenticator )
        {
        iIMSAuthenticator = CTcSIPIMSAuthenticator::NewL();
        }
    return iIMSAuthenticator->AuthenticateL( aNonce );
    }

const TDesC8& CTcSIPContext::SIPErrorToText( TInt aError )
	{
	switch( aError )
		{
		case KErrSIPMalformedMessage:
			{
			return KSIPMalformedMessageErrMsg;
			}
		case KErrSIPInvalidRegistrarResponse:
			{
			return KSIPRegistrarRespondedWithDifferentAORErrMsg;
			}
		case KErrSIPRequestPending:
			{
			return KSIPRequestPendingErrMsg;
			}
		case KErrSIPInvalidTransactionState:
			{
			return KSIPInvalidTransactionStateErrMsg;
			}
		case KErrSIPInvalidDialogState:
			{
			return KSIPInvalidDialogStateErrMsg;
			}
		case KErrSIPInvalidDialogResponse:
			{
			return KSIPInvalidResponseInDialogsErrMsg;
			}
		case KErrSIPInvalidDialogRequest:
			{
			return KSIPInvalidRequestInDialogsErrMsg;
			}
		case KErrSIPTransportFailure:
			{
			return KSIPTransportFailureErrMsg;
			}
		case KErrSIPNoAckReceived:
			{
			return KSIPNoAckReceivedErrMsg;
			}
		case KErrSIPInvalidRegistrationState:
			{
			return KSIPInvalidRegistrationStateErrMsg;
			}
		case KErrSIPInvalidContact:
			{
			return KSIPInvalidContactErrMsg;
			}
		case KErrSIPResourceNotAvailable:
			{
			return KSIPResourceNotAvailableErrMsg;
			}
		case KErrSIPResolvingFailure:
			{
			return KSIPResolvingFailureErrMsg;
			}
		case KErrSIPForbidden:
			{
			return KSIPForbiddenErrMsg;
			}
		case KErrSIPMaxCompartmentsInUse:
			{
			return KSIPMaxCompartmentsInUseErrMsg;
			}
		case KErrSIPTerminatedWithResponse:
			{
			return KSIPTerminatedWithResponseErrMsg;
			}
		case KErrSIPOutboundProxyNotResponding:
			{
			return KSIPOutboundProxyNotRespondingErrMsg;
			}
		case KSIPErrInviteCanceled:
		    {
		    return KSIPInviteCanceledErrMsg;
		    }
		default:
			{
			return KSIPUnrecognizedErrMsg;
			}
		}
	}

RSipClientSimulator& CTcSIPContext::SipClientSimulator()
    {
    return iSipClientSimulator;
    }

CRepository& CTcSIPContext::Repository()
    {
    return *iCenRep;
    }
    
