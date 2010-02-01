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




#ifndef MCCQOSCONTROLLER_H
#define MCCQOSCONTROLLER_H

//  INCLUDES
#include <e32base.h>
#include "mccrateadaptationdef.h"
#include "mccinternalevents.h"
#include "mmccevents.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MMccRateAdaptationObserver;
class MMccResources;
class TMccEvent;
class CDeltaTimer;
class MDataSink;
class MDataSource;

// CLASS DECLARATION

class TMccQosControllerIcmpError
    {
    public:
    
        TMccQosControllerIcmpError() :
            iLinkId( 0 ),
            iErrorTimeWindowBeginning( 0 ),
            iErrorCount( 0 )
            {
            }
            
    public:
    
        TUint32 iLinkId;
        TTime iErrorTimeWindowBeginning;
        TInt iErrorCount;
    };

/**
*
*/
class CMccQosController : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMccQosController* NewL( MMccRateAdaptationObserver& aObserver, 
                                        MMccResources& aResources );
        
        /**
        * Destructor.
        */
        IMPORT_C ~CMccQosController();

    public: // New functions
    
        /**
        * Processes event if it is qos related.
        * @param aEvent
        * @return ETrue if event should be ignored, otherwise EFalse
        */
        IMPORT_C TBool EventReceived( const TMccEvent& aEvent );       

    public: // Functions from base classes       
        
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * C++ default constructor.
        */
        CMccQosController( MMccRateAdaptationObserver& aObserver,
                           MMccResources& aResources );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        void DoAdaptationCalculations();
        void FillRateAdaptationEvent();
        void ClearRateEventData();
        
        /**
		*Starts timer of type CDeltaTimer, 
		*which callback is AsyncTimerExpired function
		*/
		void StartTimerForAsync();
		
		/**
		* A callback for CDeltaTimer
		*/
		static TInt AsyncTimerExpired(TAny* aPtr);
        
        void GetSinkSources();
        
        void CheckRateChangeResult(TInt aValue);
        
        TBool HandleIcmpError( const TMccEvent& aEvent );
        
        static TBool IcmpErrorMatch( const TMccQosControllerIcmpError& aError1, 
                                     const TMccQosControllerIcmpError& aError2 );
        
        static TBool IcmpErrorCleanup( const TMccQosControllerIcmpError& aError1, 
                                       const TMccQosControllerIcmpError& aError2 );
        
        void DoAdaptation();
        
        TInt GetShortTermComparisonDelay();
        
        TInt CalculateDelayOfShortTermPeriod();

    public:     // Data
    
    protected:  // Data

    private:    // Data
    
        MMccRateAdaptationObserver& iMainObserver;
        MMccResources& 				iResources;
        TInt 						iRoundTripTime;
        TRtpPeerStat 				iStreamStat;
        TMccRateAdaptationEventData iRateEventData;
        TMccEvent					iEvent;
        TMccEvent					iResultEvent;
        TThresholdValues 			iThresholdValues; //rate control threshold values
        CDeltaTimer* 				iDeltaTimer;
        TCallBack 					iDeltaTimerCallBack;
		TDeltaTimerEntry 			iDeltaTimerEntry;
		
       TBool						iBitRateChanged;
       TUint						iEventCounter;
       TBool                        iAdapted;
       TInt                         iRoundTripDelayOfPreviousShortPeriod;
       TInt                         iRoundTripDelays[ KMccShortTermCalculation ];
        RArray<TMccQosControllerIcmpError> iIcmpErrors;
       
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

    #ifdef TEST_EUNIT
    friend class UT_CMccQosController;
    #endif

    };

#endif      // MCCQOSCONTROLLER_H   
            
// End of File
