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



#ifndef __TESTINTERFACEINITPARAMS_H__
#define __TESTINTERFACEINITPARAMS_H__

// INCLUDES

#include "fcmessageobserver.h"

class TestInterfaceInitParams
{
public:  // Constructors and destructor
        
    /**
    * Constructor
	* @param aObserver MFCMessageObserver
	* 
    */
	TestInterfaceInitParams(MFCMessageObserver& aObserver);
public:
	//Data
	
	MFCMessageObserver& iMsgObserver;
};

#include "TestInterfaceInitParams.inl"
#endif
//  End of File
