/*
* Copyright (c) 2010Nokia Corporation and/or its subsidiary(-ies).
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


#include "buffertimer.h"
#include "msrppayloadformatdefs.h"

CBufferTimer::CBufferTimer(MBufferTimerNotify& aNotify)
:CActive( EPriorityStandard ),iNotify(aNotify)
{
	CActiveScheduler::Add( this );
}
 
CBufferTimer::~CBufferTimer()
{	
	Cancel();
	iTimer.Close();
}
 
CBufferTimer* CBufferTimer::NewL(MBufferTimerNotify& aNotify)
{
	
		DP_MSRP_WRITE( "CBufferTimer::NewL" );
    CBufferTimer* me = new (ELeave) CBufferTimer(aNotify); 
    CleanupStack::PushL(me);
		me->ConstructL();
		CleanupStack::Pop();
    return me;
}
 
void CBufferTimer::ConstructL(void)
{
	iTimer.CreateLocal();
}
 
void CBufferTimer::After(TTimeIntervalMicroSeconds32 aInterval)
{
	DP_MSRP_WRITE( "CBufferTimer::After()" );
	iInterval = aInterval;
	Call(); 

}

 
void CBufferTimer::DoCancel()
{
	iTimer.Cancel();
}
 
void CBufferTimer::Call()
	{
			DP_MSRP_WRITE( "CBufferTimer::Call()" );
	    TRequestStatus* stat = &iStatus; 
     
      // Cancel the statemachine and set a new state
    	Cancel();
      User::RequestComplete( stat, KErrNone );
      SetActive();
	} 
 
 
void CBufferTimer::RunL()
{
	
	if ( KErrNone == iStatus.Int() )
		{
			DP_MSRP_WRITE( "CBufferTimer::RunL()" );
			iTimer.After(iStatus,iInterval); 
			iNotify.TimerExpired();
		}
	else
		{
			  #ifdef _DEBUG
            RDebug::Print( _L("CBufferTimer::RunL ERROR: %d"), iStatus.Int());
        #endif
        User::Leave( iStatus.Int() );
		}
	
}

