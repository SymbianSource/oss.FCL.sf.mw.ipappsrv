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
* Description:    implementation
*
*/




#ifndef MNATFWUNSAFMSGASSEMBLERCTX_H
#define MNATFWUNSAFMSGASSEMBLERCTX_H

// INCLUDES
#include "tnatfwunsafmsgstatebase.h"

// FORWARD DECLARATIONS
class MNATFWUNSAFMsgAssemblerObserver;
class CNATFWUNSAFMessageFactory;
class CNATFWUNSAFMessage;

// CLASS DEFINITION
/**
* Defines an interface of CNATFWUNSAFMsgAssembler used by state classes.
*
*  @lib natfwunsafprotocols.lib
*/
class MNATFWUNSAFMsgAssemblerCtx
    {
    public:    // Constructors and destructor

        virtual ~MNATFWUNSAFMsgAssemblerCtx() {};

    public: // Abstract methods

        virtual MNATFWUNSAFMsgAssemblerObserver& MsgObserver() = 0;

        virtual CNATFWUNSAFMessageFactory& MsgFactory() = 0;

        virtual CBufFlat& MsgBuffer() = 0;

        virtual CNATFWUNSAFMessage* Message() = 0;

        virtual void SetMessage( CNATFWUNSAFMessage* aMessage ) = 0;

        virtual void DetachMessage() = 0;

        virtual void SetMessageLength( TInt aMessageLength ) = 0;

        virtual TInt MessageLength() = 0;

        virtual void ChangeState(
            TNATFWUNSAFMsgStateBase::TNATFWUNSAFMsgStateValue aState ) = 0;

        virtual TNATFWUNSAFMsgStateBase& CurrentState() = 0;
    };

#endif // end of MNATFWUNSAFMSGASSEMBLERCTX_H



