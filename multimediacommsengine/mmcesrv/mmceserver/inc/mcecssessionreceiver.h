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




#ifndef CMCECSSESSIONRECEIVER_H
#define CMCECSSESSIONRECEIVER_H

//  INCLUDES
#include "mcecsreceiverbase.h"

// CLASS DECLARATION

/**
*  MCE CS session received.
*
*  @lib 
*  @since
*/
class CMceCsSessionReceiver : public CMceCsReceiverBase
	{
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aType, the type of receiver
	* @param aItc, reference to ITC helper.
	*/   
	static CMceCsSessionReceiver* NewL( TMceManagerType aType, 
	                                    CMceCsServerITC& aItc);

	/**
	* Two-phased constructor.
	* @param aType, the type of receiver
	* @param aItc, reference to ITC helper.
	*/   
	static CMceCsSessionReceiver* NewLC( TMceManagerType aType, 
	                                      CMceCsServerITC& aItc );

	/**
	* Destructor.
	*/   
	~CMceCsSessionReceiver();

	/**
	* Type of receiver
	*/   
    TMceManagerType Type() const;

private:

    
	CMceCsSessionReceiver( TMceManagerType aType, CMceCsServerITC& aItc );


private://data


    TMceManagerType iType;
    
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS

	
	};

#endif

// End of File
