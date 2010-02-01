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
* Description:    The class provides the FC PlugIn interface
*
*/




#ifndef __CFCINTERFACE__
#define __CFCINTERFACE__

//  INCLUDES
#include <e32base.h>
#include <ecom/ecom.h>
#include <badesca.h>
#include "fcinterfaceinitparams.h"
#include "fcplugin.h"

// CONSTANTS
// UID of this interface
const TUid KCFCInterfaceUid = {0x102010E2};

// Default port
const TUint32 KFCDefaultPort = 0;

// CLASS DECLARATION
/**
* A FC interface used for the ecom
*/
class CFCInterface : public CBase,
		             public MFCPlugIn
	{
    public:
	    /** 
	    * The interface for passing initialisation parameters
	    * @param aMatchString aMatch data for ecom
	    * @param specific param for loading the specific plugin
	    * @return the required plugIn Instance
	    */
	    static CFCInterface* NewL( const TUid& aImplUid, 
	                               TFCInterfaceInitParams& aParams );

	    /**
        * Destructor.               
        */
	    virtual ~CFCInterface();

    protected:
	    /** ecom interface */
	    inline CFCInterface();

    private:
	    /** Unique instance identifier key */ 
	    TUid iDtor_ID_Key;
	};

#include "fcinterface.inl"

#endif 

//  End of File
