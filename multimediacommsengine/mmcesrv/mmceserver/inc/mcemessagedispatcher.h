/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCEMESSAGEDISPATCHER_H
#define MCEMESSAGEDISPATCHER_H

#include "mcesrvlogs.h"


/**
 * message dispatcher
 *
 * @lib 
 */
template <class T>
class TMceMessageDispatcher
    {
    
public:

    /**
    * Constructor 
    * @param aService the service
    */
    inline TMceMessageDispatcher( T& aService,
                                  CMceCsServerITC& aItc );

    inline void DispatchL( TMceIds& aIds, 
                            TMceItcFunctions aFunction, 
					        const RMessage2& aMessage );
					  
    inline CMceMsgBase* DecodeL( TMceItcDataType aType, 
                                 const TDesC8& aMessage );

private:

    inline CMceCsServerITC& ITC() 
        {
        return iItc;
        }
    
private:

    /**
    * service
    */
    T& iService;

    /**
    * ITC
    */
    CMceCsServerITC& iItc;

    };


#include "mcemessagedispatcher.inl"


#endif //MCEMESSAGEDISPATCHER_H