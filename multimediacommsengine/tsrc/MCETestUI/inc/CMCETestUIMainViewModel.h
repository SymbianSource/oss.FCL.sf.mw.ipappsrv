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

#ifndef __CMCETESTUIMAINVIEWMODEL_H
#define __CMCETESTUIMAINVIEWMODEL_H

// INCLUDES

#include <bamdesca.h>
#include <badesca.h>

#include "MMCETestUIEngineCmdProvider.h"

// FORWARD DECLARATIONS

class CMCETestUIEngine;


// CLASS DECLARATION

/**
*  CMCETestUIMainViewModel
*  
*/
class CMCETestUIMainViewModel : public MDesCArray
    {
public: // Constructors and destructor
    
    static CMCETestUIMainViewModel* NewL( CMCETestUIEngine& aEngine );
    
    /**
    * Destructor.
    */
    ~CMCETestUIMainViewModel();
    
public: // from MDesCArray

    TInt MdcaCount() const;
    TPtrC MdcaPoint( TInt aIndex ) const; 
    
public: // New functions

    TBool IsManagerSelected( TInt aIndex ) const;
    TInt SelectedProfileIndex( TInt aIndex ) const;
    TInt SelectedSessionIndex( TInt aIndex ) const;
    TInt SelectedEventIndex( TInt aIndex ) const;
    TInt SelectedReferIndex( TInt aIndex ) const;
    MMCETestUIEngineCmdProvider& CmdProvider( TInt aIndex );
    
private: // Constructors 

    CMCETestUIMainViewModel( CMCETestUIEngine& aEngine  );
    CMCETestUIMainViewModel();
    
    void ConstructL();
  

private: // convenience functions

    void PopulateMainViewManagerL();
    void PopulateMainViewProfilesL();
    void PopulateMainViewOutSessionsL();
    void PopulateMainViewInSessionsL();
    void PopulateMainViewOutEventsL();
    void PopulateMainViewInEventsL();
    void PopulateMainViewOutRefersL();
    void PopulateMainViewInRefersL();
    
    
private: //data

    CDesCArray* iArray; 
    CMCETestUIEngine& iEngine;
    
    TBool managerSuppressed;
    TBool profilesSuppressed;
    };
    
#endif
    
// End of File
