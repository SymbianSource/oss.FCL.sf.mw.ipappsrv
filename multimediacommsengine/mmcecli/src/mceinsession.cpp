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




#include "mceinsession.h"
#include "mcemanager.h"
#include "mceclientserver.h"
#include "mceserial.h"

#include "mcecomsession.h"
#include "mceitcsender.h"
#include "mceclilogs.h"

#define _FLAT_DATA static_cast<CMceComSession*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// CONSTANTS
const TUint32 KMceMinAcceptable = 200;
const TUint32 KMceMaxAcceptable = 299;
const TUint32 KMceMinError = 300;
const TUint32 KMceMaxError = 699;
const TUint32 KMceRinging = 180;
_LIT8( KMcePhraseRinging, "Ringing" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceInSession::NewL
// -----------------------------------------------------------------------------
//
CMceInSession* CMceInSession::NewL( RReadStream& aReadStream,
            						CMceManager& aManager,
            						TUint32 aProfileId,
            						TBool aUpdateSession )
    {
    MCECLI_DEBUG("CMceInSession::NewL, Entry");
    
    CMceInSession* self = new (ELeave) CMceInSession( &aManager, aProfileId );    
    CleanupStack::PushL( self );
    //internalize
    //initialize streams to setup parents

    MMceComSerializationContext serCtx( aReadStream );
    CleanupClosePushL( serCtx );
    
    self->InitializeInSessionL( serCtx, aUpdateSession );
    CleanupStack::PopAndDestroy();//serCtx
    
    self->InitializeL();
    CleanupStack::Pop( self );
    
    MCECLI_DEBUG("CMceInSession::NewL, Exit");
    return self;
        
    }

// -----------------------------------------------------------------------------
// CMceInSession::NewL
// -----------------------------------------------------------------------------
//

CMceInSession* CMceInSession::NewL()
    {
    MCECLI_DEBUG("CMceInSession::NewL(empty), Entry");
    
    CMceInSession* self = new (ELeave) CMceInSession( NULL, NULL );
    CleanupStack::PushL( self );
    self->ConstructL( KNullDesC8, KNullDesC8 );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceInSession::NewL(empty), Exit");
    return self;
    }

// -----------------------------------------------------------------------------
// CMceInSession::~CMceInSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMceInSession::~CMceInSession()
    {
    }

// -----------------------------------------------------------------------------
// CMceInSession::StreamInitializeCondition
// -----------------------------------------------------------------------------
//
TBool
CMceInSession::StreamInitializeCondition( CMceMediaStream& aMediaStream ) const
	{
	// In case of pull mode, stream is uninitialized. Initialize it to get the
	// supported codecs.
	return aMediaStream.State() == CMceMediaStream::EUninitialized;
	}

// -----------------------------------------------------------------------------
// CMceInSession::RingL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::RingL( CDesC8Array* aHeaders,
					 		 		HBufC8* aContentType,
					 		 		HBufC8* aContent )
    {
    MCECLI_DEBUG("CMceInSession::RingL, Entry");
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state );
    
    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    
    HBufC8* ringing = KMcePhraseRinging().AllocLC();
    //create reply
	CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( 
													*ringing,
	                                                KMceRinging, 
	                                                *aHeaders, 
	                                                *aContentType );
    CleanupStack::PushL( reply );
        	
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
	
	iSender->SendL( ids, EMceItcRing, *reply, aContent );    
	
    //update the state
	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );
	state = State();
    MCECLI_DEBUG_CLISTATE( "CMceInSession::RingL, after ITC, state", state );

    //cleanup    
    CleanupStack::PopAndDestroy( reply );
    CleanupStack::PopAndDestroy( ringing );
    delete aHeaders;
    delete aContentType;
    MCECLI_DEBUG("CMceInSession::RingL, Exit");
    }
    
    
// -----------------------------------------------------------------------------
// CMceInSession::RespondL  
// -----------------------------------------------------------------------------
//

EXPORT_C void CMceInSession::RespondL(  const TDesC8& aReason,
                       	                 TUint32 aCode,
                    	                 CDesC8Array* ,
                    	                 HBufC8* /*aContentType*/,
                    		     		 HBufC8* /*aContent*/ )
		
    {
    MCECLI_DEBUG("CMceInSession::RespondL, Entry");
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state );
    
    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );
    	//copy reason
    HBufC8* reason = aReason.AllocLC();
    
    
    HBufC8* emptyContentType = KMceEmpty().AllocLC();
    
    CDesC8Array* emptyHdrs = new( ELeave ) CDesC8ArrayFlat( 1 );
    CleanupStack::PushL( emptyHdrs );
    
    //create reply
	CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( *reason, 
	                                                       aCode, 
	                                                       *emptyHdrs, 
	                                                       *emptyContentType );
    CleanupStack::PushL( reply );
        	
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
	
	iSender->SendL( ids, EMceItcCallInQueue , *reply );    
    
    //update the state
	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );
	state = State();
    MCECLI_DEBUG_CLISTATE( "CMceInSession::RespondL, after ITC, state", state );

    //cleanup    
    CleanupStack::PopAndDestroy( reply );
    CleanupStack::PopAndDestroy( emptyHdrs );
    CleanupStack::PopAndDestroy( emptyContentType );
    CleanupStack::PopAndDestroy( reason );
    
    MCECLI_DEBUG("CMceInSession::RespondL, Exit");
    }                    		         
    
// -----------------------------------------------------------------------------
// CMceInSession::AcceptL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::AcceptL()
    {
    MCECLI_DEBUG("CMceInSession::AcceptL, Entry");
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state );
    
    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );
    
    //update crypto 
	TInt cryptoCount = FLAT_DATA( iClientCryptoSuites ).Count();
	if ( cryptoCount != KMceAnswerCryptoCount )
		{
		for (TInt i = cryptoCount; i > KMceAnswerCryptoCount ; i--)
	    	{
	    	FLAT_DATA( iClientCryptoSuites ).Remove(i-1);
	    	}
		}
	SendITCEventL( EMceItcAcceptSession );
	
    MCECLI_DEBUG("CMceInSession::AcceptL, Exit");

    }

// -----------------------------------------------------------------------------
// CMceInSession::AcceptL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::AcceptL( const TDesC8& aReason,
            		                  TUint32 aCode,
            		                  CDesC8Array* /*aHeaders*/,
					                  HBufC8* /*aContentType*/,
					                  HBufC8* /*aContent*/ )            		                  
    {
    MCECLI_DEBUG("CMceInSession::AcceptL, Entry");
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state );
    MCECLI_DEBUG_SVALUE( "reason", aReason );
    MCECLI_DEBUG_DVALUE( "code", aCode );
    
    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );
    User::LeaveIfError( 
        aCode >= KMceMinAcceptable && aCode <= KMceMaxAcceptable ?
        KErrNone : KErrArgument );
	//copy reason
    HBufC8* reason = aReason.AllocLC();
    HBufC8* emptyContentType = KMceEmpty().AllocLC();
    
    CDesC8Array* emptyHdrs = new( ELeave ) CDesC8ArrayFlat(1);
    CleanupStack::PushL( emptyHdrs );
    
    //create reply
	CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( *reason, 
	                                                       aCode, 
	                                                       *emptyHdrs, 
	                                                       *emptyContentType );
    CleanupStack::PushL( reply );
        	
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
	
	iSender->SendL( ids, EMceItcAcceptSession , *reply );    
	
	//update crypto 
	TInt cryptoCount = FLAT_DATA( iClientCryptoSuites ).Count();
	if ( cryptoCount != KMceAnswerCryptoCount )
		{
		for (TInt i = cryptoCount; i > KMceAnswerCryptoCount ; i--)
	    	{
	    	FLAT_DATA( iClientCryptoSuites ).Remove(i-1);
	    	}
		}
		
    //update the state
	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );
	state = State();
    MCECLI_DEBUG_CLISTATE( "CMceInSession::AcceptL, after ITC, state", state );

    //cleanup    
    CleanupStack::PopAndDestroy( reply );
    CleanupStack::PopAndDestroy( emptyHdrs );
    CleanupStack::PopAndDestroy( emptyContentType );
    CleanupStack::PopAndDestroy( reason );
    
    MCECLI_DEBUG("CMceInSession::AcceptL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceInSession::RejectL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::RejectL()
    {
    MCECLI_DEBUG("CMceInSession::RejectL, Entry");
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state );
    
    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );
	SendITCEventL( EMceItcRejectSession );
	
	state = State();
    MCECLI_DEBUG_CLISTATE( "CMceInSession::RejectL, after ITC, state", state );
	
	if ( FLAT_DATA( iState ) == EEstablished )
	    {
        //update the whole session
        TMceIds ids;
        SynchronizeL( ids );
	    }
    
    MCECLI_DEBUG("CMceInSession::RejectL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceInSession::RejectL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::RejectL( const TDesC8& aReason,
            		                  TUint32 aCode,
            		                  CDesC8Array* aHeaders,
					                  HBufC8* aContentType,
					                  HBufC8* aContent )
    {
    MCECLI_DEBUG("CMceInSession::RejectL, Entry");
    TState state = State();
    MCECLI_DEBUG_CLISTATE( "state", state );
    MCECLI_DEBUG_SVALUE( "reason", aReason );
    MCECLI_DEBUG_DVALUE( "code", aCode );
    
    User::LeaveIfError( IsZombie() ? KErrArgument : KErrNone  );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    User::LeaveIfError( aCode >= KMceMinError && aCode <= KMceMaxError ?
        KErrNone : KErrArgument );
	
	//copy reason
    HBufC8* reason = aReason.AllocLC();
    
    //create reply
	CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( *reason, 
	                                                       aCode, 
	                                                       *aHeaders, 
	                                                       *aContentType );
    CleanupStack::PushL( reply );
        	
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
	
	iSender->SendL( ids, EMceItcRejectSession, *reply, aContent );    
	
    //update the state
	FLAT_DATA( iState ) = static_cast<TState>( ids.iState );
	state = State();
    MCECLI_DEBUG_CLISTATE( "CMceInSession::RejectL, after ITC, state", state );
	
	if ( FLAT_DATA( iState ) == EEstablished )
	    {
        //update the whole session
        SynchronizeL( ids );
	    }
	    
    //cleanup    
    CleanupStack::PopAndDestroy( reply );
    CleanupStack::PopAndDestroy( reason );
    delete aHeaders;
    delete aContentType;
    MCECLI_DEBUG("CMceInSession::RejectL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceInSession::CMceInSession
// -----------------------------------------------------------------------------
//
CMceInSession::CMceInSession( CMceManager* aManager, TUint32 aProfileId )
    : CMceSession( aManager, aProfileId )
    {
    }


// -----------------------------------------------------------------------------
// CMceInSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceInSession::ConstructL( const TDesC8& aOriginator, const TDesC8& aRecipient )
    {
    iFlatData = CMceComSession::NewL( CMceComSession::EInSession );
    
    delete FLAT_DATA( iOriginator );
    FLAT_DATA( iOriginator ) = 0;
    FLAT_DATA( iOriginator ) = aOriginator.AllocL();
    delete FLAT_DATA( iRecipient );
    FLAT_DATA( iRecipient ) = 0;
    FLAT_DATA( iRecipient ) = aRecipient.AllocL();
    
    CMceSession::ConstructL();
    
    }

// -----------------------------------------------------------------------------
// CMceInSession::InitializeInSessionL
// -----------------------------------------------------------------------------
//
void CMceInSession::InitializeInSessionL(
	MMceComSerializationContext& aSerCtx,
	TBool aUpdateSession )
    {
    iFlatData = CMceComSession::NewL( CMceComSession::EInSession );
    
    InternalizeL( aSerCtx );
    
    CMceSession::ConstructL( aUpdateSession );    
    }

