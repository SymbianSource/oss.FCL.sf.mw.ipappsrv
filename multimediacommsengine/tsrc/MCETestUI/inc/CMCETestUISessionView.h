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



#ifndef __CMCETESTUISESSIONVIEW_H
#define __CMCETESTUISESSIONVIEW_H

//#include <coecntrl.h>
//#include <coeccntx.h>

//#include <aknlists.h>
#include <aknview.h>

#include "CMCETestUIMainViewContainer.h"
#include "CMCETestUISessionViewContainer.h"

class CMCETestUIAppUi;
class CMCETestUIEngine;
class CMCETestUIEngineSession;
class CAknNavigationDecorator;

class CMCETestUISessionView : public CAknView
    {
    
public: // Constructors and destructor

	static CMCETestUISessionView* NewL( CMCETestUIEngine& aEngine );	

	~CMCETestUISessionView();

    //CMCETestUIAppUi& GetAppUi();

public:

    void EngineStateChangedL();
    
    void ChangeToMainViewL();
    
    
    void ChangeToStreamViewL( TInt aStreamIndex );
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

	CMCETestUISessionView();
	CMCETestUISessionView( CMCETestUIEngine& aEngine );
    void ConstructL();


private:
	
	//CAknDoubleStyleListBox* iListBox;
	//CDesCArrayFlat* iMessageList;

    CMCETestUISessionViewContainer* iContainer;
    TUid iId;
    
    CMCETestUIEngine& iEngine;

    // Pointer to navigation label. Owned.
    CAknNavigationDecorator* iNaviLabel;
    
    // Pointer to shown session. Not owned.
    CMCETestUIEngineSession* iSession;
    
    TBuf<30> iTitlePaneText;

    // Navigation pane text container
   
    };


#endif

