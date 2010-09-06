/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/


#ifndef MCCMSRPINTERFACE_H
#define MCCMSRPINTERFACE_H

//  INCLUDES
#include <mmf/common/mmfutilities.h>
#include "mccinternalcodecs.h"
#include "mmccevents.h"
#include "mccinternalevents.h"
#include "mccexpirationhandler.h"
#include "mccinternaldef.h"
#include "msrpcallbackmgrobserver.h"

#include <CMSRP.h>
#include <CMSRPSession.h>


// FORWARD DECLARATIONS

class TMccEvent;


// CLASS DECLARATION

/**
* Class to pass the session related parameters to the MSRP source/sink.
* 
* @lib Mccmsrpsourcesink.dll
* @since Series 60 3.0
*/
class TMccMsrpSessionParams
    {
    public:
        
        inline TMccMsrpSessionParams() : iMsrp( NULL ), iMsrpSession( NULL ) 
		{ 
		}

        inline TMccMsrpSessionParams( CMSRP* aMsrp, CMSRPSession* aMsrpSession ) :
            iMsrp( aMsrp ),
            iMsrpSession( aMsrpSession )
            { }
        
        inline TMccMsrpSessionParams( CMSRP* aMsrp, CMSRPSession* aMsrpSession, CMsrpCallbackMgrObserver* aMsrpObserver ) :
            iMsrp( aMsrp ),
            iMsrpSession( aMsrpSession ),
            iMsrpObserver (aMsrpObserver)
            { }
        
        
        virtual void SetSessionParamsL( const TMccMsrpSessionParams& /*aParams*/ )
            {
            }
        

    public: // data

        CMSRP* iMsrp;
		
		CMSRPSession* iMsrpSession;
		
        CMsrpCallbackMgrObserver* iMsrpObserver;
    };




#endif      // MCCMSRPINTERFACE_H
            
// End of File
