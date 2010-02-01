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




#ifndef CMCCRATEADAPTATIONOBSERVERSTUB_H
#define CMCCRATEADAPTATIONOBSERVERSTUB_H

//  INCLUDES
#include <e32base.h>
#include "mccrateadaptationobserver.h"
// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class TMccEvent;
class CMccResourcePool;
// CLASS DECLARATION

/**
*
*/
class CMMccRateAdaptationObserverStub :
    public CBase, 
    public MMccRateAdaptationObserver
    {

 public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMMccRateAdaptationObserverStub* NewL();
          
        /**
        * Destructor.
        */
        virtual ~CMMccRateAdaptationObserverStub();



    public:
    
	
        TInt RateAdaptationRequest( const TMccEvent& aInputData, 
                                            TMccEvent& aOutputData );
                                            
        TInt RateAdaptationAlert( const TMccEvent& aAdaptationData,
                                          TAlertType aAlertType );
    public: //data
    CMccResourcePool*  iResourcePool;
    
    private:
    
    	CMMccRateAdaptationObserverStub();
    	
    	void ConstructL();

    };

#endif      // CMCCRATEADAPTATIONOBSERVERSTUB_H   
            
// End of File
