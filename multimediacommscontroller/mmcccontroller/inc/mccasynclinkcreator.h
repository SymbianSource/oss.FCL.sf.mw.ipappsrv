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
* Description:    MCC Controller Asynchronous Link Creator active object
*
*/




#ifndef MCCASYNCLINKCREATOR_H
#define MCCASYNCLINKCREATOR_H

// INCLUDES
#include <e32base.h>
#include "mccinternaldef.h"

// FORWARD DECLARATIONS
class TMMFMessage;
class CMccUlDlClient;
class MAsyncLinkCreationObserver;
class MMccEventHandler;


// CLASS DECLARATION

/**
 * The main purpose of this class is to create link and RTP session.
 * Class also requests public IP resolving from Media NAT Traversal
 * plug-in via subcontroller.
 *
 * @lib Mcccontroller.lib
 * @since S60 v3.1
 */
NONSHARABLE_CLASS( CMccAsyncLinkCreator ) : public CActive
    {

public:

    static CMccAsyncLinkCreator* NewL( MAsyncLinkCreationObserver& aObserver, 
                                       CMccUlDlClient* aSession );

    static CMccAsyncLinkCreator* NewLC( MAsyncLinkCreationObserver& aObserver,
                                        CMccUlDlClient* aSession );

    virtual ~CMccAsyncLinkCreator();

    /**
     * Starts the connection creation process
     *
     * @since  S60 v3.2
     * @param  aMessage Message containing client data
     * @return void
     */
    void StartLinkCreationL( TMMFMessage& aMessage );
   
    
    /**
     *GetSessionId()
     *@return TUint32 Session ID 
     */
     TUint32 GetSessionId();
     

protected:

// from base class CActive

    /**
     * From CActive
     * Active object event handling
     *
     * @since S60 v3.2
     */
    void RunL();
     
    /**
     * From CActive
     * Active cancellation
     *
     * @since S60 v3.2
     */
    void DoCancel();

    /**
     * From CActive
     * RunL error handling
     *
     * @since   S60 v3.2
     * @param   aError System wide error code
     * @return  KErrNone
     */
    TInt RunError( TInt aError );
    

private:

    CMccAsyncLinkCreator( MAsyncLinkCreationObserver& aObserver,
        CMccUlDlClient* aSession );

    /**
     * Completes the client request with given error code
     *
     * @since S60 v3.2
     * @param aError System wide error code
     */
    void CompleteRequest( TInt aError );


private: // data

    /**
     * Observer for this link creator
     */
    MAsyncLinkCreationObserver& iObserver;

    /**
     * Client data read from the message
     */
    TMccCreateLinkPckg iClientData;

    /**
     * Pointer to Mcc Ul/Dl Client
     * Not own.
     */
    CMccUlDlClient* iSession;

    /**
     * State of public IP resolving
     */
    TInt iResolveState;

    /**
     * State of link creation.
     */
    TBool iLinkCreated;

    };

#endif // MCCASYNCLINKCREATOR_H
