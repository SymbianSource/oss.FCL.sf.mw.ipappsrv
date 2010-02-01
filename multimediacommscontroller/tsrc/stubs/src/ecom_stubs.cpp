/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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



#include <ecom/ecom.h>
#include <ecom/implementationinformation.h>
#include <mmf/server/mmfaudiooutput.h>
#include <mmf/server/mmfaudioinput.h>
#include <networking/qos3gpp_subconparams.h>

#include "mccrtpdatasource.h"
#include "mccrtpdatasink.h"
#include "mccanysource.h"
#include "mccanysink.h"
#include "mccuids.hrh"
#include "mcctestuids.hrh"

// -----------------------------------------------------------------------------
// REComSession::REComSession
// -----------------------------------------------------------------------------
//
REComSession::REComSession()
    {
    }

// -----------------------------------------------------------------------------
// REComSession::DestroyedImplementation
// -----------------------------------------------------------------------------
//
void REComSession::DestroyedImplementation(TUid /*aDtorIDKey*/)
    {
    }

// -----------------------------------------------------------------------------
// REComSession::FinalClose
// -----------------------------------------------------------------------------
//
void REComSession::FinalClose()
    {
    }

// -----------------------------------------------------------------------------
// REComSession::CreateImplementationL
// -----------------------------------------------------------------------------
//
EXPORT_C TAny* REComSession::CreateImplementationL(TUid aImplementationUid, 
													TInt32 /*aKeyOffset*/)
    {
   	if ( aImplementationUid.iUid == KImplUidRtpDataSource )
        {
        return CMccRtpDataSource::NewSourceL( aImplementationUid, KNullDesC8 );
        }
	else if ( aImplementationUid.iUid == KImplUidRtpDataSink )
        {
        return CMccRtpDataSink::NewSinkL( aImplementationUid, KNullDesC8 );
        }
	else if ( aImplementationUid.iUid == KUidMmfAudioOutput.iUid )
        {
        return static_cast<MDataSink*>( CMMFAudioOutput::NewSinkL() );
        }
	else if ( aImplementationUid.iUid == KUidMmfAudioInput.iUid )
        {
        return static_cast<MDataSource*>( CMMFAudioInput::NewSourceL() );
        }
    else if ( aImplementationUid.iUid == KImplUidMccAnySource )
        {
        return CMccAnySource::NewSourceL( aImplementationUid, KNullDesC8 );
        }
    else if ( aImplementationUid.iUid == KImplUidMccAnySink )
        {
        return CMccAnySink::NewSinkL( aImplementationUid, KNullDesC8 );
        }   
    else if ( aImplementationUid.iUid == KSubCon3GPPExtParamsFactoryUid )
        {
        return new (ELeave) CSubConQosR99ParamSet();
        }   
	else 
	    {
	    User::Leave( KErrNotFound );
	    }
	    
    return NULL;
    }
