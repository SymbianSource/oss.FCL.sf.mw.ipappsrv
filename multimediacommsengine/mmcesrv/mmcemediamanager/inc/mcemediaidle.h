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




#ifndef TMCEMEDIAIDLE_H
#define TMCEMEDIAIDLE_H

#include "mcemediastate.h"


/**
*  Represents MM idle state 
*
*/

class TMceMediaIdle : public TMceMediaState
    {
    
public: // Constructors

    /**
    * Consructor
    */
    TMceMediaIdle( CMceComSession& aSession );

    static void SetAsCurrentStateL( CMceComSession& aSession );
    

public: // from TMceMediaState

    inline TMceMMState Id() 
        {
        return KMceMediaIdle;
        }

public: // from TMceMediaState

    void UpdateL();
    
    TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );
    
private: 

    void OfferL();

    void AnswerL();

    };

#endif//TMCEMEDIAIDLE_H

// End of File
