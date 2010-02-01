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





#ifndef MCCAUDIOROUTER_H
#define MCCAUDIOROUTER_H

// INCLUDES
#include <e32base.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CMMFDevSound;
class CAudioOutput;

// CLASS DECLARATION

/**
*  MCP AudioOutputRouter methods
*
*  MMCCSubthreads.dll
*  @since Series 60 3.0
*/
#ifdef EKA2
NONSHARABLE_CLASS( CMccAudioRouter ) : public CBase
#else
class CMccAudioRouter : public CBase
#endif
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMccAudioRouter* NewL( CMMFDevSound& aDevSound );
          
        /**
        * Destructor.
        */
        ~CMccAudioRouter();
        
      
    public: // New functions
    
        void SetAudioRouteL( TUint32 aRoutingDestination );
        
        void GetAudioRouteL( TUint32& aRoutingDestination );
        
    private: // Private constructors
    
        void ConstructL( CMMFDevSound& aDevSound );
        
        CMccAudioRouter();

    private: // Data
    
        CAudioOutput* iAudioRouter;
        
        TBool iRoutingSupported;     
    
    };

#endif /* MCCAUDIOROUTER_H */

