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




#ifndef TMCEANSWERINGMEDIA_H
#define TMCEANSWERINGMEDIA_H

#include "mcemediastate.h"


/**
*  Represents MM answering media state 
*
*/
class TMceAnsweringMedia : public TMceMediaState
    {
    
public: // Constructors


    static void SetAsCurrentStateL( CMceComSession& aSession );


public: // from TMceMediaState

    inline TMceMMState Id() 
        {
        return KMceAnsweringMedia;
        }


public: // from TMceMediaState


    void UpdateL();

protected:

    /**
    * Consructor
    */
    TMceAnsweringMedia( CMceComSession& aSession );

    };

#endif//TMCEANSWERINGMEDIA_H

// End of File
