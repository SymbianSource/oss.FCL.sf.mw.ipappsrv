/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef CNATFWUNSAFMSGASSEMBLER_H
#define CNATFWUNSAFMSGASSEMBLER_H

//  INCLUDES
#include <e32base.h>

#include "mnatfwunsafmsgassemblerctx.h"

// FORWARD DECLARATIONS
class MNATFWUNSAFMsgAssemblerObserver;


// CLASS DECLARATION
/**
* A class for assembling UNSAF messages.
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS( CNATFWUNSAFMsgAssembler ): public CBase, 
    public MNATFWUNSAFMsgAssemblerCtx
    {
    public: // Enums

        enum
            {
            EDefaultBufferSize = 500
            };

    public:    // Constructors and destructor

        static CNATFWUNSAFMsgAssembler* NewL(
            MNATFWUNSAFMsgAssemblerObserver& aObserver );

        static CNATFWUNSAFMsgAssembler* NewLC(
            MNATFWUNSAFMsgAssemblerObserver& aObserver );

        /**
        * Destructor.
        */
        ~CNATFWUNSAFMsgAssembler();


    public: // From MNATFWUNSAFMsgAssemblerCtx

        MNATFWUNSAFMsgAssemblerObserver& MsgObserver();

        CNATFWUNSAFMessageFactory& MsgFactory();

        CBufFlat& MsgBuffer();

        CNATFWUNSAFMessage* Message();

        void SetMessage( CNATFWUNSAFMessage* aMessage );

        void DetachMessage();

        void SetMessageLength( TInt aMessageLength );

        TInt MessageLength();

        void ChangeState(
            TNATFWUNSAFMsgStateBase::TNATFWUNSAFMsgStateValue aState );

        TNATFWUNSAFMsgStateBase& CurrentState();


    public: // New functions

        /**
        * Tries to assemble UNSAF message from inputted data. If there's
        * not enough data for decoding UNSAF message, data is stored
        * and completing data is waited.
        * @param aData ownership is transferred
        * @param aNextLength will be set to reflect amount of missing data
        * @return ETrue if message assembling completed, EFalse otherwise.
        */
        TBool InputL( HBufC8* aData, TUint& aNextLength );


    private: // Constructors

        void ConstructL();

        CNATFWUNSAFMsgAssembler( MNATFWUNSAFMsgAssemblerObserver& aObserver );


    private: // Data

        MNATFWUNSAFMsgAssemblerObserver& iObserver;

        CBufFlat* iMsgBuf;

        CNATFWUNSAFMessageFactory* iMsgFactory;

        CNATFWUNSAFMessage* iMessage;

        TInt iMessageLength;

        CArrayVarFlat< TNATFWUNSAFMsgStateBase > iStates;

        TNATFWUNSAFMsgStateBase::TNATFWUNSAFMsgStateValue iStateValue;


#ifdef TEST_EUNIT
        friend class UT_CNATFWUNSAFTlsReceiver;
#endif

    };

#endif // CNATFWUNSAFMSGASSEMBLER_H


