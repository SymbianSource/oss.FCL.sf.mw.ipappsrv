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




#ifndef CMCECSSUBSESSIONRECEIVER_H
#define CMCECSSUBSESSIONRECEIVER_H

//  INCLUDES
#include "mcecsreceiverbase.h"

// CLASS DECLARATION

/**
*  MCE CS subsession receiver.
*
*  @lib 
*  @since
*/
class CMceCsSubSessionReceiver : public CMceCsReceiverBase
	{
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aItc, reference to ITC helper.
    */      
    static CMceCsSubSessionReceiver* NewL(CMceCsServerITC& aItc);

    /**
    * Two-phased constructor.
    * @param aItc, reference to ITC helper.
    */      
    static CMceCsSubSessionReceiver* NewLC(CMceCsServerITC& aItc);

    /**
    * Destructor.
    */      
    ~CMceCsSubSessionReceiver();

private: // Second phase constructors

    CMceCsSubSessionReceiver(CMceCsServerITC& aItc);

	//definitions for unit testing
	MCESRV_UT_DEFINITIONS
	
	};

#endif 

// End of File
