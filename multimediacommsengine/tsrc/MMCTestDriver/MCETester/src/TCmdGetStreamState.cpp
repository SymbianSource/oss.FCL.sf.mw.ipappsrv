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
#include "CTcMCEMultimediaAPIObserver.h"

#include "TCmdGetStreamState.h"
#include "CTcMCEContext.h"

#include <MceMediaStream.h>
#include <MceMediaSink.h>
#include <MceMediaSource.h>

void TCmdGetStreamState::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------
	
	// Get an item off the receive queue (waits until timeout if none is present)

	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, 
									EFalse );
	CTcMCEReceived& item = iContext.ReceivedStreamItemL( timeout );	

 	CMceMediaStream* expectedStream = reinterpret_cast<CMceMediaStream*>
 		(GetObjectForIdL(KStreamId, EFalse));
/*
    if ( expectedStream )
        {
        if ( item.MediaStream() != expectedStream )
            {
            // If the first item's containing stream is not the expected
            User::Leave( KErrNotFound );
            }
        }
 */
 
 	CTcMCEMultimediaAPIObserver &mObserver = iContext.MultimediaObserver();
 
 	CMceMediaStream& aStream = *expectedStream;
 	
 	mObserver.StreamStateChanged(aStream);
 	
 	CMceMediaStream::TState state = aStream.State();
      
	// ---------- Execution ----------------------------------------------------

	// It is not quaranteed that MediaStream object is not necessarily valid
	// when the event will be fetched from the queue. Only the data
	// stored when the event was being received can be used.

	// ---------- Response creation --------------------------------------------

	AddIdResponseL( KStreamId, aStream );
    

//	AddIdResponseL( KSinkId, aStream.Sinks() );

	AddIdResponseL( KSourceId, *(aStream.Source()) );
	 
	AddIntegerResponseL(KResponseMediaStreamState, state);
	AddTextualMediaStreamStateL(state); 
	
	}
	
TBool TCmdGetStreamState::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetStreamState") );
	}

TTcCommandBase* TCmdGetStreamState::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetStreamState( aContext );
	}
