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




#ifndef TMCESTARTINGOFFERERRECEIVESTREAMS_H
#define TMCESTARTINGOFFERERRECEIVESTREAMS_H


#include "mcepreparingstreams.h"



/**
*  Represents MM state for preparing offerer send stream
*
*/
class TMceStartingOffererReceiveStreams : public TMcePreparingStreams
    {
    
public: // Constructors

    static void SetAsCurrentStateL( CMceComSession& aSession );


public: // from TMediaManagerState

    inline TMceMMState Id() 
        {
        return KMceStartingOffererReceiveStreams;
        }

    void EntryL();
        
protected: // from TMcePreparingStreams
    
    void StreamsPreparedL();
    void StreamsStartedL();
        
protected:

    /**
    * Consructor
    */
    TMceStartingOffererReceiveStreams( CMceComSession& aSession );

private:

    };

#endif//TMCESTARTINGOFFERERRECEIVESTREAMS_H

// End of File
