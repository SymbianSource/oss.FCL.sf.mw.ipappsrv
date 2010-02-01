/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef TFCPLUGINITER_H
#define TFCPLUGINITER_H

// INCLUDES
#include <e32base.h>

//Forward Class
class CFCPlugInInfo;

/**
* Iterator Implementation to get the plubin
* @lib FCPlugInEngine.lib
*/
class TFCPlugInIter
	{
    public:
    	/**
	    * Constructor for Iterator
	    * @param aMechanisms a Array of plugin mechanisms
	    * @return TFCPlugInIter
	    */	
	    TFCPlugInIter(RPointerArray<CFCPlugInInfo>& aMechanisms);
	    CFCPlugInInfo* Previous();		
	    CFCPlugInInfo* Next();
	    CFCPlugInInfo* First();
	    CFCPlugInInfo* Last();
	    CFCPlugInInfo* Current();
	    TInt Count() const;
	
    private://Data
	    TInt iPos;
	    RPointerArray<CFCPlugInInfo>& iFCPlugIn;
	};

#endif

//  End of File
