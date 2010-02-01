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




#ifndef TMCEANSWERINGMEDIAUPDATE_H
#define TMCEANSWERINGMEDIAUPDATE_H

#include "mceansweringmedia.h"


/**
*  Represents MM answering media state 
*
*/
class TMceAnsweringMediaUpdate : public TMceAnsweringMedia
    {
    
public: // Constructors


    static void SetAsCurrentStateL( CMceComSession& aSession );


public: // from TMceMediaState

    inline TMceMMState Id() 
        {
        return KMceAnsweringMediaUpdate;
        }


    void UpdateL();
    
protected:

    /**
    * Consructor
    */
    TMceAnsweringMediaUpdate( CMceComSession& aSession );

    };

#endif//TMCEANSWERINGMEDIAUPDATE_H

// End of File
