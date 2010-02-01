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


////////////////////////////////////////////////////////////////////////
//
// CExampleDocument
//
////////////////////////////////////////////////////////////////////////

#ifndef __MCETESTUIDOC_H
#define __MCETESTUIDOC_H

#include <akndoc.h>
#include <badesca.h>

// Forward declarations
class CMCETestUIAppUi;
class CAknApplication;
class CMCETestUIEngine;

class CMCETestUIDoc : public CAknDocument
	{
public:

	static CMCETestUIDoc* NewL(CEikApplication& aApp);
	static CMCETestUIDoc* NewLC(CEikApplication& aApp);
	~CMCETestUIDoc();
	

	CMCETestUIDoc(CEikApplication& aApp);

	void ConstructL();
private: 
	           // Inherited from CEikDocument
	CEikAppUi* CreateAppUiL();


private: // data

	//CMCETestUIAppUi* iMCETestUIAppUi;
	CMCETestUIEngine* iEngine;

	};


#endif
