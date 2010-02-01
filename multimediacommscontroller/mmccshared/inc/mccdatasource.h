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




#ifndef MCCDATASOURCE_H
#define MCCDATASOURCE_H

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfdatasource.h>
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
class CMccDataSource : 
    public CBase, 
    public MDataSource, 
    public MMccSourceSink,
    public MMccRateAdaptationObserver 
    {
    public:
    
        inline virtual ~CMccDataSource();
        
    public: // From MDataSource
    
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
    
        inline CMccDataSource( TUid aType );
        
    protected:
        
        MMccResources* iMccResources;
         
    };
    
#include "mccdatasource.inl"

#endif      // MCCDATASOURCE_H   
            
// End of File
