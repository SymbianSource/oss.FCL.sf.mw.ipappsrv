// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Header for class specific to unit tests
// 
//

/**
 @file
 @internalComponent
*/

#if (!defined TE_RTP_WRAPPERCORESERVER_H__)
#define TE_RTP_WRAPPERCORESERVER_H__
#include <test/testexecuteserverbase.h>

#if (!defined __DATA_CAGING__)
IMPORT_C TInt NewServer();
#endif


//#include "rtpsession.h"

class CTe_rtpwrapperCoreStep;

class CTe_rtpwrapperCoreServer : public CTestServer
	{
public:
	static CTe_rtpwrapperCoreServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	~CTe_rtpwrapperCoreServer();

	// Members used by the steps
	TBool iOom;
	CTe_rtpwrapperCoreStep* iCurrentStep;
	TBool iStopping;
	TInt iStopCount;

	CActiveScheduler* iScheduler;

 private:
	CTe_rtpwrapperCoreServer();
	};


#endif
