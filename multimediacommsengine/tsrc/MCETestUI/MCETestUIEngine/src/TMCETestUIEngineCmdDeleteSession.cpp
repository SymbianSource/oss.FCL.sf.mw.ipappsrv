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



#include "TMCETestUIEngineCmdDeleteSession.h"
#include "CMCETestUIEngineSession.h"


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteSession::TMCETestUIEngineCmdDeleteSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDeleteSession::TMCETestUIEngineCmdDeleteSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDeleteSession::ExecuteL()
	{
	iEngine.DeleteSession( iSession );
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDeleteSession::Caption() const
	{
	return KCommandCaptionDeleteSession;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoSession::TMCETestUIEngineCmdEnableVideoSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableVideoSession::TMCETestUIEngineCmdEnableVideoSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableVideoSession::ExecuteL()
	{
	iEngine.EnableVideo(iSession);
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableVideoSession::Caption() const
	{
	return KCommandCaptionSessionEnableVideo;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoSession::TMCETestUIEngineCmdDisableVideoSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableVideoSession::TMCETestUIEngineCmdDisableVideoSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableVideoSession::ExecuteL()
	{
	iEngine.DisableVideo(iSession);
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableVideoSession::Caption() const
	{
	return KCommandCaptionSessionDisableVideo;
	}



// TMCETestUIEngineCmdEnableReceivingVideoSession::TMCETestUIEngineCmdEnableReceivingVideoSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableReceivingVideoSession::TMCETestUIEngineCmdEnableReceivingVideoSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableReceivingVideoSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableReceivingVideoSession::ExecuteL()
	{
	iEngine.EnableReceivingVideo(iSession);
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableReceivingVideoSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableReceivingVideoSession::Caption() const
	{
	return KCommandCaptionSessionEnableVideo;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableReceivingVideoSession::TMCETestUIEngineCmdDisableReceivingVideoSession
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableReceivingVideoSession::TMCETestUIEngineCmdDisableReceivingVideoSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession )
    : TMCETestUIEngineCmdBase( aEngine ),
      iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableReceivingVideoSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableReceivingVideoSession::ExecuteL()
	{
	iEngine.DisableReceivingVideo(iSession);
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableReceivingVideoSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableReceivingVideoSession::Caption() const
	{
	return KCommandCaptionSessionDisableVideo;
	}




// End of File
