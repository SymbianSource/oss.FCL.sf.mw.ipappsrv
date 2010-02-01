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
* Description:    The class provides the FC PlugIn interface
*
*/




#ifndef _TESTINTERFACE___H__
#define _TESTINTERFACE___H__

#include <e32base.h>
#include <ecom/ECom.h>
#include <badesca.h>
#include "TestInterfaceInitParams.h"
#include "fcplugin.h"

// CLASS DECLARATION
/**
* 	An FC abstract class being representative of the
*	concrete class which the client wishes to use.
*	It acts as a base, for a real class to provide all the 
*	functionality that a client requires.  
*	It supplies instantiation & destruction by using
*	the ECom framework, and functional services
*	by using the methods of the actual class.
*/
class CTestInterface : public CBase,
		 public MFCPlugIn
	{
public:
	// The interface for passing initialisation parameters
	// to the derived class constructor.
	
	static CTestInterface* NewL(const TUid& aImplUid, TestInterfaceInitParams& aParams);

	// Destructor.
	virtual ~CTestInterface();

	
protected:
	//Default c'tor
	inline CTestInterface();

private:
	// Unique instance identifier key
	TUid iDtor_ID_Key;
	};

#include "TestInterface.inl"

#endif 

