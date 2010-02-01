/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CMCECSSERVERCALLER_H__
#define __CMCECSSERVERCALLER_H__

#include <e32base.h>

#include <cppunit/Test.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>

#include <e32std.h>
#include "mceclientserver.h"


// ----------------------------------------------------------------------------------------
// Handles the communication from test client to SIP Server
// ----------------------------------------------------------------------------------------

class CMceCsServerCaller : public RSessionBase
{
public:

	CMceCsServerCaller ();
	~CMceCsServerCaller ();

   	static CMceCsServerCaller* NewL ();
   	
   	
public:

   	void ConnectL();
   	TInt Send(TMceItcFunctions aFunction);
   	
protected:

	void ConstructL();

	};

#endif //__CSIPCSSERVERCALLER_H__
