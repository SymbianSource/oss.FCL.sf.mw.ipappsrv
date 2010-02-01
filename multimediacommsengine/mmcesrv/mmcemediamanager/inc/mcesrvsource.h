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




#ifndef CMCESRVSOURCE_H
#define CMCESRVSOURCE_H

#include <e32base.h>
#include "mcesrvendpoint.h"


class CMceComMediaSource;
class CMceMediaManager;
class CMceSrvStream;


/**
 * 
 *
 * @lib 
 */
class CMceSrvSource: public CMceSrvEndpoint
    {

public: // Constructors & Destructor

    /**
     * C++ default constructor.
     */
    CMceSrvSource( CMceMediaManager& aManager,
                   CMceComMediaSource& aData );
    
    /**
    * Destructor.
    */
     ~CMceSrvSource();

    
public: // Source and sink functions

    /**
    * Gets correct endpoint from stream.
    * @param aStream
    * @return CMceSrvEndpoint
    */
    CMceSrvEndpoint& Endpoint( CMceSrvStream& aStream );
    
    /**
    * Handle secure key expired event
    * @param aEvent MCC event
    * @return void
    */
    void HandleSecureEventReceivedL( ) ;
    
    
private: // owned data
    
    
    };



#endif //CMCESRVSOURCE_H
