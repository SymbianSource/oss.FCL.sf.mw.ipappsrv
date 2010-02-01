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





#ifndef CMCECSRECEIVEQUEUE_H
#define CMCECSRECEIVEQUEUE_H

//  INCLUDES
#include <e32base.h>
#include "mcesrv.h"
#include "mcecsreceiveitem.h"

// CLASS DECLARATION

/**
*  Provides receive queue for the MCE server.
*
*  @lib 
*  @since
*/
class CMceCsReceiveQueue : public CBase
	{
public: // Constructors and destructor

    /**
    * Default constructor
    */
    CMceCsReceiveQueue ();

    /**
    * Destructor
    */
    ~CMceCsReceiveQueue ();

public: // new

    /**
    * Checks if queue is empty
    * @return ETrue if queue is empty
    */
    TBool IsEmpty() const;

    /**
    * Gets first item from the queue, panics
    * if empty.
    * @return first item from the queue
    */
    TMceCSReceiveItem& First ();

    /**
    * Adds item to the end of the queue
    * @param aItem item to be added
    * @return KErrNone if succesfull
    */
    TInt AddLast (const TMceCSReceiveItem& aItem);

    /**
    * Removes first item from the queue.
    */
    void RemoveFirst ();

private: // Data

    RArray<TMceCSReceiveItem> iReceiveItems;

    //definitions for unit testing
    MCESRV_UT_DEFINITIONS
		
	};


#endif

// End of File
