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




#ifndef __TFCINTERFACEINITPARAMS_H__
#define __TFCINTERFACEINITPARAMS_H__

// INCLUDES
#include "fcmessageobserver.h"

// CLASS DECLARATION
/**
* A FC interface initial param needed for the plugIn
*/
class TFCInterfaceInitParams
    {
    public:  // Constructors and destructor
        /**
        * Constructor
	    * @param aObserver MFCMessageObserver
	    */
	    TFCInterfaceInitParams(MFCMessageObserver& aObserver);
	    
    public:
	    //Data
	    MFCMessageObserver& iMsgObserver;
    };

#include "fcinterfaceinitParams.inl"

#endif

//  End of File
