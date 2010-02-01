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
#include "mceoutrefer.h"
#include "mcemanager.h"
#include "mceclientserver.h"
#include "mceitcsender.h"
#include "mceserial.h"
#include "mceclilogs.h"

// CONSTANTS
_LIT8( KReferEventHeader, "refer" );

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceOutRefer::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutRefer* CMceOutRefer::NewL(
                        CMceManager& aManager,
                        CSIPProfile& aProfile,
						const TDesC8& aRecipient,
                    	const TDesC8& aReferTo,
                        TType aType,
					    HBufC8* aOriginator) 
                        
    {
    MCECLI_DEBUG("CMceOutRefer::NewL, Entry");
    MCECLI_DEBUG_SVALUE("recipient", aRecipient );
    MCECLI_DEBUG_SVALUE("refer to", aReferTo );
    
    TUint32 profileId(0);
    if ( aProfile.GetParameter( KSIPProfileId, profileId ) != KErrNone )
        {
        User::Leave(KErrNotFound);
        }

    CMceOutRefer* self = new (ELeave) CMceOutRefer( aManager, 
                                                    profileId,
                                                    KMceNotAssigned,
                                                    aType );
    CleanupStack::PushL( self );
    self->ConstructL( aRecipient, aReferTo, aType, aOriginator);
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutRefer::NewL, Exit");
    return self;  
    }

// -----------------------------------------------------------------------------
// CMceOutRefer::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutRefer* CMceOutRefer::NewL(
			        	CMceEvent& aEvent,
                    	const TDesC8& aReferTo,
			        	TType aType )
    {
    MCECLI_DEBUG("CMceOutRefer::NewL, Entry");
    MCECLI_DEBUG_DVALUE("event", aEvent.Id() );
    MCECLI_DEBUG_SVALUE("refer to", aReferTo );
    
    CMceOutRefer* self = new (ELeave) CMceOutRefer( aEvent.Manager(), 
                                                    aEvent.ProfileId(),
                                                    aEvent.DialogId(),
                                                    aType );
    CleanupStack::PushL( self );
    self->ConstructL( aEvent.Recipient(), 
                      aReferTo, 
                      aType,
                      aEvent.Originator() );
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceOutRefer::NewL, Exit");
    return self;  
    }

// -----------------------------------------------------------------------------
// CMceOutRefer::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutRefer* CMceOutRefer::NewL(
			        	CMceSession& aSession,
                    	const TDesC8& aReferTo,
				        TType aType )
    {
    MCECLI_DEBUG("CMceOutRefer::NewL, Entry");
    MCECLI_DEBUG_SVALUE("session", aSession.Id() );
    MCECLI_DEBUG_SVALUE("refer to", aReferTo );
    
    CMceOutRefer* self = new (ELeave) CMceOutRefer( aSession.Manager(), 
                                                    aSession.Profile(),
                                                    aSession.DialogId(),
                                                    aType );
    CleanupStack::PushL( self );
    self->ConstructL( aSession.Recipient(),
                      aReferTo, 
                      aType,
                      aSession.Originator() );
    CleanupStack::Pop( self );

    MCECLI_DEBUG("CMceOutRefer::NewL, Exit");
    
    return self; 
    }

// -----------------------------------------------------------------------------
// CMceOutRefer::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutRefer* CMceOutRefer::NewL(
			        	CMceRefer& aRefer,
                    	const TDesC8& aReferTo,
				        TType aType )
    {
    MCECLI_DEBUG("CMceOutRefer::NewL, Entry");
    MCECLI_DEBUG_SVALUE("refer", aRefer.Id() );
    MCECLI_DEBUG_SVALUE("refer to", aReferTo );
    
    CMceOutRefer* self = new (ELeave) CMceOutRefer( aRefer.Manager(), 
                                                    aRefer.ProfileId(),
                                                    aRefer.DialogId(),
                                                    aType );
    CleanupStack::PushL( self );
    self->ConstructL( aRefer.Recipient(), 
                      aReferTo,
                      aType, 
                      aRefer.Originator() );
    CleanupStack::Pop( self );
    
    MCECLI_DEBUG("CMceOutRefer::NewL, Exit");
    
    return self; 
    }

// -----------------------------------------------------------------------------
// CMceOutRefer::~CMceOutRefer
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutRefer::~CMceOutRefer()
    {
    }

// -----------------------------------------------------------------------------
// CMceOutRefer::ReferL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutEvent* CMceOutRefer::ReferL(
                    			 CDesC8Array* aHeaders, 
                    			 HBufC8* aContentType,
                    			 HBufC8* aContent ) 
    {   
    MCECLI_DEBUG("CMceOutRefer::ReferL, Entry");
    TState state = State();
    MCECLI_DEBUG_EVENTSTATE( "state", state );
    
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    
	TMceIds ids;
	PrepareForITC( ids );
	ids.iState = State();
    
    CMceMsgSIPEvent* refer = NULL;
    refer = new (ELeave) CMceMsgSIPEvent( Id(),
                                          DialogId(),
                                          EMceItcEventTypeRefer,
                                          iType,
                                          ( TUint32 ) KErrNotFound, 
                                          aHeaders,
                                          aContentType );
    CleanupStack::PushL( refer );
    refer->OwnershipRollbackPushLC();
    
    CMceItcSender& sender = ITCSender();
    
    sender.SendL( ids, EMceItcReferEvent, *refer, aContent );  
    
    CleanupStack::Pop(); // refer->OwnershipRollbackPushLC()
    CleanupStack::PopAndDestroy( refer );
    iState = static_cast<TState>( ids.iState );
    
    state = State();
    MCECLI_DEBUG_EVENTSTATE( "CMceOutRefer::ReferL, after ITC, state", state );
    MCECLI_DEBUG("CMceOutRefer::ReferL, Exit");
    return static_cast< CMceOutEvent* > ( TransferEventOwnership() );
    }

// -----------------------------------------------------------------------------
// CMceOutRefer::CMceOutRefer
// -----------------------------------------------------------------------------
//
CMceOutRefer::CMceOutRefer( CMceManager& aManager, 
                            TUint32 aProfileId,
                            TUint32 aDialogId,
                            TType aType ) 
    : CMceRefer( aManager, aProfileId, aDialogId, aType )
    {
    }
    
// -----------------------------------------------------------------------------
// CMceOutRefer::ConstructL
// -----------------------------------------------------------------------------
//
void CMceOutRefer::ConstructL( 	const TDesC8& aRecipient,
                    	        const TDesC8& aReferTo,
                    	        TType aType,
                    	        HBufC8* aOriginator )
    {
    CDesC8ArrayFlat* params = 
        new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
    params->AppendL( aReferTo );
    
    if ( iDialogId == KMceNotAssigned )
        {
        params->AppendL( aRecipient );
        if ( aOriginator )
            {
            params->AppendL( *aOriginator );
            }
        else
            {
            params->AppendL( KNullDesC8 );
            }
        }
        
    if ( aType == ENoSuppression )
        {
        HBufC8* originator = NULL;
        if ( aOriginator )
            {
            originator = ( *aOriginator ).AllocL();
            }
        CleanupStack::PushL( originator );
        iOwnedEvent = CMceOutEvent::NewL( iManager,
					                  iProfileId,
					                  aRecipient,
							          KReferEventHeader,
		    				          0,
					                  originator,
					                  iDialogId,
			    	                  KMceDlgTypeRefer,
					                  params );
	    CleanupStack::Pop( originator );
	    CleanupStack::Pop( params );
	    }
    
    HBufC8* recipient = aRecipient.AllocLC();
    HBufC8* referTo = aReferTo.AllocLC();
    
    if ( aType == ENoSuppression )
        {
        CMceRefer::ConstructL( recipient, referTo, aOriginator, NULL );    
        CleanupStack::Pop( referTo );
        CleanupStack::Pop( recipient ); 
        }
    else
        {
        CMceRefer::ConstructL( recipient, referTo, aOriginator, params ); 
        CleanupStack::Pop( referTo );
        CleanupStack::Pop( recipient ); 
        CleanupStack::Pop( params );   
        }
    }

// -----------------------------------------------------------------------------
// CMceOutRefer::ConstructL
// -----------------------------------------------------------------------------
//
void CMceOutRefer::ConstructL( 	const TDesC8& aRecipient,
                    	        const TDesC8& aReferTo,
                    	        TType aType,
                    	        const TDesC8& aOriginator )
    {
    HBufC8* originator = aOriginator.AllocLC();
    ConstructL( aRecipient, aReferTo, aType, originator );
    CleanupStack::Pop( originator );
        
    }

// -----------------------------------------------------------------------------
// CMceOutRefer::IsInRefer
// -----------------------------------------------------------------------------
//    
TBool CMceOutRefer::IsInRefer() const
    {
    return EFalse;
    }


