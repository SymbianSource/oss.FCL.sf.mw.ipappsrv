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




#ifndef MCCDATASINK_H
#define MCCDATASINK_H

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfdatasink.h>
#include "mccsourcesink.h"
#include "mccrateadaptationobserver.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*
*/
class CMccDataSink : 
    public CBase, 
    public MDataSink, 
    public MMccSourceSink,
    public MMccRateAdaptationObserver 
    {
    public:
    
        inline virtual ~CMccDataSink();
        
    public: // From MDataSink
    
    public: // From MMccSourceSink
    
        inline virtual TInt AddUser( MAsyncEventHandler* aUser );
        
        inline virtual void RemoveUser( MAsyncEventHandler* aUser );
        
        inline virtual void SetCurrentUser( MAsyncEventHandler* aUser );
    
        inline virtual void GetParameterL( TUint32 aParam, TDes8& aVal );
        
        inline virtual void SetParameterL( TUint32 aParam, const TDesC8& aVal );
        
        inline virtual void UpdateL( const TDesC8& aVal );
        
        inline void SetResources( MMccResources* aResources );
    
    public: // From MMccRateAdaptationObserver
    
        inline virtual TInt RateAdaptationRequest( const TMccEvent& aInputData, 
                                                   TMccEvent& aOutputData );
                                            
        inline virtual TInt RateAdaptationAlert( const TMccEvent& aAdaptationData,
                                                 TAlertType aAlertType );

    
    protected:
    
        inline CMccDataSink( TUid aType );
   
    protected:
        
        MMccResources* iMccResources;
        
    };
    
#include "mccdatasink.inl"

#endif      // MCCDATASINK_H   
            
// End of File
