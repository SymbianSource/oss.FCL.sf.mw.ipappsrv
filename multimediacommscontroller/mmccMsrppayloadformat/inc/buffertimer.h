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



#include <e32base.h>

class MBufferTimerNotify
	{
	public:
		virtual TInt TimerExpired() = 0;
		
};
 
 
	class CBufferTimer: public CActive
		{
	public:
		
		static CBufferTimer* NewL(MBufferTimerNotify& aNotify);
		~CBufferTimer();
		
	public:
		
		void After(TTimeIntervalMicroSeconds32 aInterval);

	protected:
		
		void RunL();
		void DoCancel();
		
	private:
		
		CBufferTimer(MBufferTimerNotify& aNotify);
		void ConstructL(void);
		void Call(void);
		
	private:
		RTimer					iTimer;
		MBufferTimerNotify&	iNotify;
		TTimeIntervalMicroSeconds32 iInterval;
		
		};
