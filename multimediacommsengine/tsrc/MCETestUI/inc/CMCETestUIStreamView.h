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



#ifndef __CMCETESTUISTREAMVIEW_H
#define __CMCETESTUISTREAMVIEW_H

#include <aknview.h>

#include "CMCETestUIMainViewContainer.h"
#include "CMCETestUISessionViewContainer.h"
#include "CMCETestUIStreamViewContainer.h"


class CMCETestUIAppUi;
class CMCETestUIEngine;
class CMCETestUIEngineSession;
class CMCETestUIEngineAudioStream;
class CMCETestUIEngineVideoStream;
class CAknNavigationDecorator;
class CMCETestUIStreamViewContainer;

class CMCETestUIStreamView : public CAknView
    {
    
public: // Constructors and destructor

	static CMCETestUIStreamView* NewL( CMCETestUIEngine& aEngine );	

	~CMCETestUIStreamView();

    //CMCETestUIAppUi& GetAppUi();

public:

    void EngineStateChangedL();
    void ChangeToMainViewL();
    void ChangeToSessionViewL();
    
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

	CMCETestUIStreamView();
	CMCETestUIStreamView( CMCETestUIEngine& aEngine );
    void ConstructL();


private:
	
	//CAknDoubleStyleListBox* iListBox;
	//CDesCArrayFlat* iMessageList;

    CMCETestUIStreamViewContainer* iContainer;
    
    CMCETestUIVideoStreamViewContainer* iVideoContainer;
    TUid iId;
    
    CMCETestUIEngine& iEngine;

    // Pointer to navigation label. Owned.
    CAknNavigationDecorator* iNaviLabel;
    
    // Pointer to shown session. Not owned.
    CMCETestUIEngineSession* iSession;

    CMCETestUIEngineAudioStream* iStream;
    
    CMCETestUIEngineVideoStream* iVideoStream;
    // Navigation pane text container
   
    };


#endif

