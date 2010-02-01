/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Observer class.
*
*/




#ifndef MASYNCLINKCREATIONOBSERVER_H
#define MASYNCLINKCREATIONOBSERVER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CMccAsyncLinkCreator;


// CLASS DECLARATION

/**
 *  Observer class for asynchronous link creation process
 *
 *  @lib Mcccontroller.lib
 *  @since S60 v3.1
 */
class MAsyncLinkCreationObserver
    {

public:

    /**
     * Called when CMccAsyncLinkCreator has finished link creation
     * and can be deleted
     * @since Series 60 v3.1
     * @param aClientData Data for the client containing resolved 
     *                    IP addresses and ports
     * @param aCreator    The link creator which has finished
     * @param aError      System wide error code
     */
    virtual void CreationProcessEnd( const TMccCreateLinkPckg& aClientData, 
                                     CMccAsyncLinkCreator* aCreator, 
                                     TInt aError ) = 0;
    };

#endif      // MASYNCLINKCREATIONOBSERVER_H

//  End of File
