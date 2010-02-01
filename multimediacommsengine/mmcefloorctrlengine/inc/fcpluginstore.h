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




#ifndef __MFCPLUGINSTORE_H__
#define __MFCPLUGINSTORE_H__

// INCLUDES
#include <e32base.h>
#include "fcpluginiter.h"

// CLASS DECLARATION

/**
* An interface to get the plubinIterator
* @lib FCPlugInEngine.lib
*/
class MFCPlugInstore
    {
    public:
	    /**
	    * Returns an iteration to the list of supported
	    * Floor Control PlugIn.
	    * @return TFCPlugInIter
	    */
	    virtual TFCPlugInIter& FCPlugInIter() = 0;
    };

#endif