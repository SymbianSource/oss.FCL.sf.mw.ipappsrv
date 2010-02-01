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



#ifndef __CMCETESTUIAPPVIEW_H
#define __CMCETESTUIAPPVIEW_H

#include <coecntrl.h>
//#include <coeccntx.h>

//#include <aknlists.h>
#include <aknview.h>


#include "CMCETestUIMainViewContainer.h"

class CMCETestUIAppUi;
class CMCETestUIEngine;

class CMCETestUIAppView : public CAknView 
    {
    
public: // Constructors and destructor

	static CMCETestUIAppView* NewL( CMCETestUIEngine& aEngine );	

	~CMCETestUIAppView();

    //CMCETestUIAppUi& GetAppUi();

public:

    void EngineStateChangedL();
    void ChangeToSessionViewL( TInt aSessionIndex );
 
public:
     CMCETestUIMainViewContainer* iContainer;   
    
private: // from CAknView

    TUid Id() const;
    void HandleCommandL( TInt aCommand );
    
    void DoActivateL(
        const TVwsViewId& aPrevViewId,
        TUid aCustomMessageId,
        const TDesC8& aCustomMessage );
        
    void DoDeactivate();
    

private: // contructors

	CMCETestUIAppView();
	CMCETestUIAppView( CMCETestUIEngine& aEngine );
    void ConstructL();
	

private:
	
	//CAknDoubleStyleListBox* iListBox;
	//CDesCArrayFlat* iMessageList;

    
    TUid iId;
    
    CMCETestUIEngine& iEngine;

   
    };


#endif

