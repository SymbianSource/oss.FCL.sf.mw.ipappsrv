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
* Description:    Implementation.
*
*/




// INCLUDES

#include "TMCETestUIEngineCmdSubscribeEvent.h"
#include "cmcetestuiengineoutevent.h"
#include "cmcetestuiengineoutrefer.h"
#include <MCEOutEvent.h>
#include <mceoutrefer.h>



// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSubscribeEvent::TMCETestUIEngineCmdSubscribeEvent
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdSubscribeEvent::TMCETestUIEngineCmdSubscribeEvent(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineOutEvent& aOutEvent )
    : TMCETestUIEngineCmdBase( aEngine ),
    iOutEvent( aOutEvent )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSubscribeEvent::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdSubscribeEvent::ExecuteL()
	{

	// ---------------------------------------
	// Next is to route INVITE to MCETestUI
/*	CDesC8Array* headers = new (ELeave) CDesC8ArrayFlat( 1 );
	CleanupStack::PushL( headers );
	headers->AppendL( KAcceptContactMCETestUI );

	// ---------------------------------------
	// Next is to find a way around MMCE's bug
	
	HBufC8* contentType = KNullDesC8().AllocLC();
	HBufC8* content = KNullDesC8().AllocLC();
*/	
	/// --------------------------------------
	
	iOutEvent.OutEvent().SubscribeL();
/*	CleanupStack::Pop( content );
	CleanupStack::Pop( contentType );
	CleanupStack::Pop( headers );
*/	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdSubscribeEvent::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdSubscribeEvent::Caption() const
	{
	return KCommandCaptionSubscribeEvent;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRefer::TMCETestUIEngineCmdRefer
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdRefer::TMCETestUIEngineCmdRefer(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineOutRefer& aOutRefer )
    : TMCETestUIEngineCmdBase( aEngine ),
    iOutRefer( aOutRefer )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRefer::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdRefer::ExecuteL()
	{
	iEngine.CreateOutEventL(iOutRefer.OutRefer().ReferL());
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRefer::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdRefer::Caption() const
	{
	return KCommandCaptionRefer;
	}


// End of File
