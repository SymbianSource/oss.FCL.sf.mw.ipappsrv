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



#include "TMCETestUIEngineCmdRemoveStream.h"
#include <mcemediastream.h>
#include <mcesession.h>

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRemoveStream::TMCETestUIEngineCmdRemoveStream
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdRemoveStream::TMCETestUIEngineCmdRemoveStream(
    CMCETestUIEngine& aEngine,
    CMceMediaStream& aStream )
    : TMCETestUIEngineCmdBase( aEngine ),
      iStream( aStream )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRemoveStream::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdRemoveStream::ExecuteL()
	{
    __ASSERT_ALWAYS( iStream.Session(), User::Leave( KErrNotReady ) );
	
	iStream.Session()->RemoveStreamL( iStream );
	iEngine.EngineStateChangedL();
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdRemoveStream::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdRemoveStream::Caption() const
	{
	return KCommandCaptionRemoveStream;
	}


// End of File
