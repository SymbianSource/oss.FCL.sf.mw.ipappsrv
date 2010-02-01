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
* Description:    See class definition below.
*
*/




#ifndef CDELAYEDPROCESS_H_
#define CDELAYEDPROCESS_H_

// INCLUDES
#include <e32base.h>
#include <e32std.h>

class CMceMediaSource;

class CDelayedProcess : public CActive
{
public:
	// Construct / destruct
	static CDelayedProcess* NewL(CMceMediaSource* aDtmfSource);
	~CDelayedProcess();

	// Request
	void StartProcess(TTimeIntervalMicroSeconds32 aDelay);
	
private:
	// Construct / destruct
	CDelayedProcess(CMceMediaSource* aDtmfSource);
	void ConstructL();

	// From CActive
	void RunL();
	void DoCancel();

private:
	RTimer iTimer;
	CMceMediaSource* iDtmfSrc;
};

#endif // CDELAYEDPROCESS_H_

