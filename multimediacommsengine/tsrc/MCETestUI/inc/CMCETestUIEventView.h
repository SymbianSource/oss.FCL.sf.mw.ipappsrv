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



#ifndef __CMCETESTUIEVENTVIEW_H
#define __CMCETESTUIEVENTVIEW_H

//#include <coecntrl.h>
//#include <coeccntx.h>

//#include <aknlists.h>
#include <aknview.h>

#include "CMCETestUIMainViewContainer.h"
#include "CMCETestUIEventViewContainer.h"

class CMCETestUIAppUi;
class CMCETestUIEngine;
class CMCETestUIEngineEvent;
class CAknNavigationDecorator;

class CMCETestUIEventView : public CAknView
    {
    
public: // Constructors and destructor

	static CMCETestUIEventView * NewL( CMCETestUIEngine& aEngine );	

	~CMCETestUIEventView ();

    //CMCETestUIAppUi& GetAppUi();

public:

    void EngineStateChangedL();
    
    void ChangeToMainViewL();
    
    
    
private: // from CAknView

    TUid Id() const;
    void HandleCommandL( TInt aCommand );
    
    void DoActivateL(
        const TVwsViewId& aPrevViewId,
        TUid aCustomMessageId,
        const TDesC8& aCustomMessage );
        
    void DoDeactivate();

private: // new functions

    void ConstructStatusPaneL();

private: // contructors

	CMCETestUIEventView ();
	CMCETestUIEventView ( CMCETestUIEngine& aEngine );
    void ConstructL();


private:
	
	//CAknDoubleStyleListBox* iListBox;
	//CDesCArrayFlat* iMessageList;

    CMCETestUIEventViewContainer* iContainer;
    TUid iId;
    
    CMCETestUIEngine& iEngine;

    // Pointer to navigation label. Owned.
    CAknNavigationDecorator* iNaviLabel;
    
    // Pointer to shown session. Not owned.
    CMCETestUIEngineEvent* iEvent;
    
  

    // Navigation pane text container
   
    };


#endif

