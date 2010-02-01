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

#ifndef __CMCETESTUIEVENTVIEWMODEL_H
#define __CMCETESTUIEVENTVIEWMODEL_H

// INCLUDES

#include <bamdesca.h>
#include <badesca.h>

#include "MMCETestUIEngineCmdProvider.h"

// FORWARD DECLARATIONS

class CMCETestUIEngine;
class CMCETestUIEngineEvent;


// CLASS DECLARATION

/**
*  CMCETestUIEventViewModel
*  
*/
class CMCETestUIEventViewModel : public MDesCArray
    {
public: // Constructors and destructor
    
    static CMCETestUIEventViewModel* NewL( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineEvent& iEvent );
    
    /**
    * Destructor.
    */
    ~CMCETestUIEventViewModel();
    
public: // from MDesCArray

    TInt MdcaCount() const;
    TPtrC MdcaPoint( TInt aIndex ) const; 
    
public: // New functions
    
    MMCETestUIEngineCmdProvider& CmdProvider( TInt aIndex );
    
private: // Constructors 

    CMCETestUIEventViewModel( CMCETestUIEngine& aEngine,
                                CMCETestUIEngineEvent& iEvent );
    CMCETestUIEventViewModel();
    
    void ConstructL();
  

private: // convenience functions

    void PopulateStreamsL();
    
private: //data

    CDesCArray* iArray; 
    CMCETestUIEngine& iEngine;
    CMCETestUIEngineEvent& iEvent; // Not owned
    };
    
#endif
    
// End of File
