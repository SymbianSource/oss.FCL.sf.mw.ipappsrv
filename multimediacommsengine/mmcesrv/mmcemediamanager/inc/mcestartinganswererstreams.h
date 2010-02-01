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




#ifndef TMCESTARTINGANSWERERSTREAMS_H
#define TMCESTARTINGANSWERERSTREAMS_H


#include "mcepreparingstreams.h"



/**
*  Represents MM state for preparing answerer send stream
*
*/
class TMceStartingAnswererStreams : public TMcePreparingStreams
    {
    
public: // Constructors

    static void SetAsCurrentStateL( CMceComSession& aSession );


public: // from TMediaManagerState

    inline TMceMMState Id() 
        {
        return KMceStartingAnswererStreams;
        }

    void EntryL();
        
protected: // from TMcePreparingStreams
    
    void StreamsPreparedL();
    void StreamsStartedL();

protected:

    /**
    * Consructor
    */
    TMceStartingAnswererStreams( CMceComSession& aSession );

    
    };

#endif//TMCESTARTINGANSWERERSTREAMS_H

// End of File
