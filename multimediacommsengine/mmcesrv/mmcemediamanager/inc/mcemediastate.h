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




#ifndef TMCEMEDIASTATE_H
#define TMCEMEDIASTATE_H

#include <e32std.h>
#include "mcemediadefs.h"
#include "mcesip.h"

class TMceMccComEvent;
class CMceComSession;
class CSdpDocument;

/**
*  Represents MM state 
*
*/

class TMceMediaState
    {
    
public: // Constructors

    /**
    * Consructor
    */
    TMceMediaState( CMceComSession& aSession );
	virtual ~TMceMediaState();


public: // new functions

    virtual TMceMMState Id() = 0;

    virtual TMceMediaState* CloneL( CMceComSession& aSession ) const;

    virtual void EntryL();

    virtual void Callback();
    
    void Pushed();
    
    TInt StackIndex();
    
    TBool CallbackAllowed();
    
    
public: // from mcc

    virtual void MccLinkCreatedL( TMceMccComEvent& aEvent );

    virtual void MccMediaPreparedL( TMceMccComEvent& aEvent );
    
    virtual void MccMediaStartedL( TMceMccComEvent& aEvent );
        
    virtual void MccMediaPausedL( TMceMccComEvent& aEvent );
    
    virtual void MccMediaResumedL( TMceMccComEvent& aEvent );

    virtual void MccError( TInt aError, TMceMccComEvent& aEvent );

public: //from application/server


    virtual TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );

    virtual CSdpDocument* EncodeL();

    virtual void UpdateL();

    
public: 

    virtual TBool NeedToNegotiate( CSdpDocument* aSdpDocument = NULL );
    
    virtual TBool ConfirmationNeeded();
    
    virtual TBool ReservationNeeded();
    
    virtual TMceReturnStatus ReserveL();
    
    
    void SetRole( TMceNegotiationRole aRole );
    
    TMceNegotiationRole Role() const;

protected://state common functions


protected://data

    CMceComSession& iSession;
    TMceNegotiationRole iRole;
    TInt iStackIndex;
    
    };

#endif //TMceMEDIASTATE_H

// End of File
