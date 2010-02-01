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




#include <eikapp.h>
#include <akndoc.h>

#include "CMCETestUIEngine.h"

#include "CMCETestUIDoc.h"
#include "CMCETestUIAppUi.h"




CMCETestUIDoc* CMCETestUIDoc::NewL(CEikApplication& aApp)
    {
    CMCETestUIDoc* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }
CMCETestUIDoc* CMCETestUIDoc::NewLC(CEikApplication& aApp)
    {
    CMCETestUIDoc* self = new (ELeave) CMCETestUIDoc(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
void CMCETestUIDoc::ConstructL()
    {
    iEngine = CMCETestUIEngine::NewL();
    }    

CMCETestUIDoc::CMCETestUIDoc(CEikApplication& aApp) 
    : CAknDocument(aApp) 
    {
    // NOP  
    }   


CMCETestUIDoc::~CMCETestUIDoc()
    {
    delete iEngine;
    }

CEikAppUi* CMCETestUIDoc::CreateAppUiL()
    {
    
    CEikAppUi* appUi = new ( ELeave ) CMCETestUIAppUi( *iEngine );
    //iMCETestUIAppUi = ( CMCETestUIAppUi* ) appUi;
    return appUi;
    
    }
    