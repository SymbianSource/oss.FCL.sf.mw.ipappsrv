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
* Description:    Implementation
*
*/



#include "MCEConstants.h"
#include "TCmdCreateRefer.h"
#include "CTcMCEContext.h"

#include <MCEEvent.h>
#include <MceOutRefer.h>
#include <MCESession.h>
#include <MceOutSession.h>


void TCmdCreateRefer::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get profile
	CSIPProfile* profile = 
		reinterpret_cast<CSIPProfile*>(GetObjectForIdL(KProfileObj, EFalse));
		
	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, EFalse));	
		
	// Get event
	CMceEvent* event = 
	    reinterpret_cast<CMceEvent*>(GetObjectForIdL(KEventId, EFalse));			

	// Get refer
	CMceRefer* refer = 
	    reinterpret_cast<CMceRefer*>(GetObjectForIdL(KReferId, EFalse));	

	// Get ReferTo
	TPtrC8 referTo = ExtractTextL(KParamReferTo, ETrue);
	//If suppressed=0 then it is NoSuppression, it will receive Notify by outEventRefer
	// outEventRefer returned by calling sendRefer
	TInt suppressed =0; 
	CMceRefer::TType referType ;
	suppressed = ExtractIntegerL(KParamNotifyReceiver,0, EFalse);
	referType = CMceRefer::ENoSuppression;
	if ( suppressed==1)
		{
		referType = CMceRefer::ESuppressed;	
		}
	if (suppressed==2)
		{
		referType = CMceRefer::EMultipleRefer;	
		}
	
    // Get event, which is going to receive notifications. 
    // If not present, notifications are suppressed
    //command out by Chuan, the following will be later removed due to API change
    /*CMceOutEvent* outEvent = NULL;
	TPtrC8 outEventName = ExtractTextL( KParamNotifyReceiverEventId, EFalse );
    if ( outEventName != KNullDesC8 )
        {
        outEvent = reinterpret_cast<CMceOutEvent*>
            (iContext.Registry().ObjectPtrL( outEventName ));
        }
	*/		
	// ---------- Execution ----------------------------------------------------

	CMceOutRefer* outRefer = NULL;

	if ( profile ) 
		{
		TPtrC8 recipient = ExtractTextL( KParamRecipient, ETrue );
		
        HBufC8* originator = HBufCParameterL(ExtractTextL( KParamOriginator, 
                                                           EFalse ));
	    CleanupStack::PushL( originator );
	    
		outRefer = CMceOutRefer::NewL( iContext.MCEManager(), 
		                               *profile,
		                               recipient,
		                               referTo,
		                               referType, 
		                               originator );
		                               
		CleanupStack::Pop( originator );
		}
	else if ( session ) 
		{
		outRefer = CMceOutRefer::NewL( *session, referTo, referType );
		}
	else if ( event ) 
		{
		outRefer = CMceOutRefer::NewL( *event, referTo, referType );
		}	
	else if ( refer ) 
		{
		outRefer = CMceOutRefer::NewL( *refer, referTo, referType );
		}					
    else
    	{
		User::Leave( KErrNotFound );
		}

	// ---------- Response creation --------------------------------------------
 

    AddIdResponseL( KReferId, outRefer );
    
    // If outEvent exists, it is placed in object registry, so it can be added
    // as parameter. 
    //Due to API's change, command out by Chuan and it might be removed later
    /*AddIntegerResponseL( KParamNotifyReceiverEventId, 
                         reinterpret_cast<TInt>(outEvent) );*/

	}
	
TBool TCmdCreateRefer::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("CreateRefer") );
	}

TTcCommandBase* TCmdCreateRefer::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdCreateRefer( aContext );
	}
