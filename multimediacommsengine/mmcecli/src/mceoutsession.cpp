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




#include <sipprofile.h>
#include "mceoutsession.h"
#include "mcemanager.h"
#include "mceevent.h"
#include "mcecomsession.h"
#include "mceitcsender.h"
#include "mceserial.h"
#include "mceclilogs.h"


#define _FLAT_DATA static_cast<CMceComSession*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession* CMceOutSession::NewL(
                        CMceManager& aManager,
                        CSIPProfile& aProfile,
                        const TDesC8& aRecipient,
						HBufC8* aOriginator )
    {
    MCECLI_DEBUG("CMceOutSession::NewL, Entry");
    
    TUint32 profileId(0);
    if ( aProfile.GetParameter( KSIPProfileId, profileId ) != KErrNone )
        {
        User::Leave(KErrNotFound);
        }

    CMceOutSession* self = new (ELeave) CMceOutSession( &aManager, profileId );
    CleanupStack::PushL( self );
    self->ConstructL( aRecipient, aOriginator );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutSession::NewL, Exit");
    
    return self;
    }

// -----------------------------------------------------------------------------
// CMceOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession* CMceOutSession::NewL( CMceManager& aManager,
				  TUint32 aIAPId,
				  const TDesC8& aOriginator,
				  const TDesC8& aRecipient,
				  CDesC8Array* aContactParameters )
	{
    CMceOutSession* self = new (ELeave) CMceOutSession( &aManager, 0 );
    CleanupStack::PushL( self );
    self->ConstructL( aRecipient, aOriginator, aIAPId, aContactParameters );
    CleanupStack::Pop( self );
    return self;	
	}

// -----------------------------------------------------------------------------
// CMceOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession* CMceOutSession::NewL( CMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceOutSession::NewL(event), Entry");
    CMceOutSession* self = new (ELeave) CMceOutSession( &aEvent.Manager(),
                                                        aEvent.ProfileId() );
    CleanupStack::PushL( self );
    self->ConstructL( aEvent.Recipient(), 
                      aEvent.Originator(), 
                      aEvent.DialogId() );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutSession::NewL(event), Exit");
    return self;
    }

// -----------------------------------------------------------------------------
// CMceOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession* CMceOutSession::NewL( CMceRefer& aRefer )
    {
    MCECLI_DEBUG("CMceOutSession::NewL(refer), Entry");
    CMceOutSession* self = new (ELeave) CMceOutSession( &aRefer.Manager(),
                                                        aRefer.ProfileId() );
    CleanupStack::PushL( self );
    self->ConstructL( aRefer.Recipient(), 
                      aRefer.Originator(),
                      aRefer.DialogId() );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutSession::NewL(refer), Exit");
    return self;
    }


// -----------------------------------------------------------------------------
// CMceOutSession::~CMceOutSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession::~CMceOutSession()
    {
    }


// -----------------------------------------------------------------------------
// CMceOutSession::EstablishL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceOutSession::EstablishL(
                        TUint32 aTimeout,
                        CDesC8Array* aHeaders, 
                        HBufC8* aContentType,
                        HBufC8* aContent,
                        CDesC8Array* aContentHeaders ) 
    {
    MCECLI_DEBUG("CMceOutSession::EstablishL, Entry");
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state );
    
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || 
       !( aContentType || aContent || aContentHeaders ) ? 
        KErrNone : KErrArgument );
    
    //set sip params
    SetSIPParams( aHeaders, aContentType, aContentHeaders );
    TCleanupItem sipParamsCleanup( SIPParamsCleanup, this );
    CleanupStack::PushL( sipParamsCleanup );
    
    FLAT_DATA( iTimeout ) = aTimeout;

    //create com session of this
	CMceMsgObject<CMceSession>* msg = 
	    new (ELeave) CMceMsgObject<CMceSession>( *this, EMceItcMsgTypeSession );
    CleanupStack::PushL( msg );
    
    //initialize streams, discard unused codecs
    InitializeL( EFalse, ETrue );
    
    //start establishing the session
	TMceIds ids;
	PrepareForITC( ids );
	
	iSender->SendL( ids, EMceItcEstablishSession , *msg, aContent );
	    
    //update the state
	FLAT_DATA( iState ) = static_cast<CMceSession::TState>( ids.iState );
	state = State();
    MCECLI_DEBUG_CLISTATE( "CMceSession::EstablishL, after ITC, state", state );
    
    //cleanup
    CleanupStack::PopAndDestroy( msg );
    
    CleanupStack::Pop(); // sipParamsCleanup
    
    SetSIPParams( NULL, NULL, NULL );

    MCECLI_DEBUG("CMceOutSession::EstablishL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceOutSession::AddStreamL
// -----------------------------------------------------------------------------
//
void CMceOutSession::AddStreamL( CMceMediaStream* aMediaStream )
    {
    __ASSERT_ALWAYS( aMediaStream != NULL, User::Leave( KErrArgument ) );

    // Just call the base class implementation.
    // Don't remove CMceOutSession::AddStreamL keep BC.
    CMceSession::AddStreamL( aMediaStream );
    }

// -----------------------------------------------------------------------------
// CMceOutSession::StreamInitializeCondition
// -----------------------------------------------------------------------------
//
TBool CMceOutSession::StreamInitializeCondition(
	CMceMediaStream& /*aMediaStream*/ ) const
	{
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMceOutSession::CancelL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceOutSession::CancelL()
    {
    MCECLI_DEBUG("CMceOutSession::CancelL, Entry");
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state );
    
    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );
    User::LeaveIfError( State() != EIdle ? KErrNone : KErrNotReady );
    
    //send cancel
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
    iSender->SendL( ids, EMceItcCancel );
    
    //update the state
	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );
	state = State();
    MCECLI_DEBUG_CLISTATE( "CMceSession::CancelL, after ITC, state", state );
    MCECLI_DEBUG("CMceOutSession::CancelL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceOutSession::NewL
// -----------------------------------------------------------------------------
//
CMceOutSession* CMceOutSession::NewL()
    {
    MCECLI_DEBUG("CMceOutSession::NewL(empty), Entry");
    
    CMceOutSession* self = new (ELeave) CMceOutSession( NULL, NULL );
    CleanupStack::PushL( self );
    self->ConstructL( KNullDesC8, NULL );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutSession::NewL(empty), Exit");
    return self;
    }

// -----------------------------------------------------------------------------
// CMceOutSession::CMceOutSession
// -----------------------------------------------------------------------------
//
CMceOutSession::CMceOutSession( CMceManager* aManager, TUint32 aProfileId ) 
    : CMceSession( aManager, aProfileId )
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// CMceOutSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceOutSession::ConstructL( const TDesC8& aRecipient, HBufC8* aOriginator )
    {
    iFlatData = CMceComSession::NewL( CMceComSession::EOutSession );

    delete FLAT_DATA( iRecipient );
    FLAT_DATA( iRecipient ) = 0;
    FLAT_DATA( iRecipient ) = aRecipient.AllocL();

    if ( aOriginator )
        {
        delete FLAT_DATA( iOriginator );
        FLAT_DATA( iOriginator ) = 0;
        FLAT_DATA( iOriginator ) = (*aOriginator).AllocL();
        }
        
    CMceSession::ConstructL();
    
    delete aOriginator;    
    }

// -----------------------------------------------------------------------------
// CMceOutSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceOutSession::ConstructL( const TDesC8& aRecipient, 
                                 const TDesC8& aOriginator,
                                 TUint32 aDialogId )
    {
    User::LeaveIfError(
        aDialogId != KMceNotAssigned ? KErrNone : KErrArgument );
    
    iFlatData = CMceComSession::NewL( CMceComSession::EOutSession );

    FLAT_DATA( iDialogId ) = aDialogId;
    
    delete FLAT_DATA( iRecipient );
    FLAT_DATA( iRecipient ) = 0;
    FLAT_DATA( iRecipient ) = aRecipient.AllocL();

    delete FLAT_DATA( iOriginator );
    FLAT_DATA( iOriginator ) = 0;
    FLAT_DATA( iOriginator ) = aOriginator.AllocL();
        
    CMceSession::ConstructL();
    }

// -----------------------------------------------------------------------------
// CMceOutSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceOutSession::ConstructL( const TDesC8& aRecipient, 
                                 const TDesC8& aOriginator,
                                 TUint32 aIAPId,
                                 CDesC8Array* aContactParameters )
    {    
    iFlatData = CMceComSession::NewL( CMceComSession::EOutSession );
    
    delete FLAT_DATA( iRecipient );
    FLAT_DATA( iRecipient ) = 0;
    FLAT_DATA( iRecipient ) = aRecipient.AllocL();

    delete FLAT_DATA( iOriginator );
    FLAT_DATA( iOriginator ) = 0;
    FLAT_DATA( iOriginator ) = aOriginator.AllocL();
        
    FLAT_DATA( iIapId ) = aIAPId;

    CMceSession::ConstructL();
    
    delete aContactParameters;
    }
