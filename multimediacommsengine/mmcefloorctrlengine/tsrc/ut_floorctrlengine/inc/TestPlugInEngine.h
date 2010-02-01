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




#ifndef TESTPLUGINENGINE_H
#define TESTPLUGINENGINE_H

// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "TestInterface.h"
#include <ecom/ecom.h>
#include <in_sock.h>
#include "TestInterfaceInitParams.h"
#include "fcmessageobserver.h"
#include "fcplugin.h"
#include "fcpluginstore.h"
#include "fcpluginiter.h"
#include "fcsession.h"
#include "fcplugininfo.h"

// FORWARD DECLARATIONS
class MFCMessageObserver;
class CFCSession;

// CLASS DECLARATION
/**
* Class provides functions for using Floor Control Plugin
*
*  @lib CTestPlugInEngine.dll
*/
class CTestPlugInEngine : public CBase,
			public MFCPlugInstore
	{
	public: // Constructors and destructor

		/**
		* Constructs an object; leaves on failure.
		* @param aMsgObserver an interface for requesting floor control events
		* @return new instance
		*/
		static CTestPlugInEngine* NewL(MFCMessageObserver& aObs);

		/**
		* Constructs an object and adds the pointer to the cleanup stack;
		* leaves on failure.
		* @param aMsgObserver an interface for requesting FC events
		* @return new instance
		*/
		static CTestPlugInEngine* NewLC(MFCMessageObserver& aObs);

		/**
		* Destructor
		*/
		~CTestPlugInEngine();
	
	public: // New functions

		TFCPlugInIter& FCPlugInIter();
		void ListMechanismImplementationsL();
		
		/**
		* Gets all supported Floor contrl PlugIn
		* @return supported Floor control plugins names (e.g. TBCP);
        *         the ownership is transferred
		*/
		CDesC8Array* SupportedProtocolL();
		
		/**
		* Gets the specific Floor contrl PlugIn
		* @return specific Floor control plugins name (e.g. TBCP);
        *         the ownership is transferred
		*/
		
		MFCPlugIn* PlugInByNameL(const TDesC8& aProtocolName) ;
		

        
	private:
		// Constructor
		CTestPlugInEngine(MFCMessageObserver& aObs);
		
		void ConstructL();
		

		static void ArrayCleanup(TAny* aArray);
		
		//data
	
		RPointerArray<CFCPlugInInfo> iFCPlugIns;
		TFCPlugInIter iFCPlugInIter;
		MFCMessageObserver& iMsgObserver;
	
		//TInt32  iLastComparisonResult;
	
	};

#endif // TestPlugInEngine_H

// End of File
