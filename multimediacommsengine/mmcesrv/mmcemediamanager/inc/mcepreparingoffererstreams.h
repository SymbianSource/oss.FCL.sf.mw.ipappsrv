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




#ifndef TMCEPREPARINGOFFERERSTREAMS_H
#define TMCEPREPARINGOFFERERSTREAMS_H

#include "mcepreparingstreams.h"



/**
*  Represents MM state for preparing offerer receive stream
*
*/
class TMcePreparingOffererStreams : public TMcePreparingStreams
    {
    
    
public: // Constructors

    static void SetAsCurrentStateL( CMceComSession& aSession );


public: // from TMediaManagerState

    inline TMceMMState Id() 
        {
        return KMcePreparingOffererStreams;
        }

    void EntryL();
    
protected: // from TMcePreparingStreams
    
    void StreamsPreparedL();
    void StreamsStartedL();
    

protected:

    /**
    * Consructor
    */
    TMcePreparingOffererStreams( CMceComSession& aSession );
    
    };

#endif//TMCEPREPARINGOFFERERSTREAMS_H

// End of File
