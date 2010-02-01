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




#ifndef MCCSOURCESINK_H
#define MCCSOURCESINK_H

//  INCLUDES
#include <e32base.h>
#include <mmf/common/mmfcontrollerframework.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MMccResources;

// CLASS DECLARATION

/**
*
*/
class MMccSourceSink
    {   
    public:
        
        virtual TInt AddUser( MAsyncEventHandler* aUser ) = 0;
        
        virtual void RemoveUser( MAsyncEventHandler* aUser ) = 0;
        
        virtual void SetCurrentUser( MAsyncEventHandler* aUser ) = 0;
        
        virtual void SetParameterL( TUint32 aParam, const TDesC8& aVal ) = 0;
        
        virtual void UpdateL( const TDesC8& aVal ) = 0;
        
        virtual void SetResources( MMccResources* aResources ) = 0;

    };

#endif      // MCCSOURCESINK_H   
            
// End of File
