/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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






#ifndef MCCPERIODICRUNNER_H
#define MCCPERIODICRUNNER_H
#include <e32base.h>


// CLASS DECLARATION

/**
*  CMccPeriodicRunner class declaration
*/
class CMccPeriodicRunner : public CActive
	{
    public:
    
	    static CMccPeriodicRunner* NewL();
		
	    ~CMccPeriodicRunner(); 
		
	public:
	 
	 	void Start( TTimeIntervalMicroSeconds32 aInterval,
	 	            TCallBack aCallBack );
	 	void Stop();
	 	
	 	TUint32 Interval();
	 	
    public: // From CActive

		void DoCancel();
		void RunL();
		TInt RunError(TInt aError);	

    private:
        CMccPeriodicRunner();
	    void ConstructL(); // second construction phase

    private:
	    
	    //Timer service provider
		RTimer iTimer;
		
		TTimeIntervalMicroSeconds32 iInterval;
		
		TCallBack iCallBack;
    };


#endif