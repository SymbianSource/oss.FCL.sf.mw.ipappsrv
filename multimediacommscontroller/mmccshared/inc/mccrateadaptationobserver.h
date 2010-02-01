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




#ifndef MCCRATEADAPTATIONOBSERVER_H
#define MCCRATEADAPTATIONOBSERVER_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class TMccEvent;

// CLASS DECLARATION

/**
*
*/
class MMccRateAdaptationObserver
    {
    public:
    
        enum TAlertType
            {
            ERateAdaptationNotPossible
            };
        
    public:
    
        virtual TInt RateAdaptationRequest( const TMccEvent& aInputData, 
                                            TMccEvent& aOutputData ) = 0;
                                            
        virtual TInt RateAdaptationAlert( const TMccEvent& aAdaptationData,
                                          TAlertType aAlertType ) = 0;

    };

#endif      // MCCRATEADAPTATIONOBSERVER_H   
            
// End of File
