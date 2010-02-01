/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#ifndef __CMCETESTUISESSIONVIEWMODEL_H
#define __CMCETESTUISESSIONVIEWMODEL_H

// INCLUDES

#include <bamdesca.h>
#include <badesca.h>

#include "MMCETestUIEngineCmdProvider.h"

// FORWARD DECLARATIONS

class CMCETestUIEngine;
class CMCETestUIEngineSession;


// CLASS DECLARATION

/**
*  CMCETestUISessionViewModel
*  
*/
class CMCETestUISessionViewModel : public MDesCArray
    {
public: // Constructors and destructor
    
    static CMCETestUISessionViewModel* NewL( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineSession& iSession );
    
    /**
    * Destructor.
    */
    ~CMCETestUISessionViewModel();
    
public: // from MDesCArray

    TInt MdcaCount() const;
    TPtrC MdcaPoint( TInt aIndex ) const; 
    
public: // New functions
    TInt SelectedAudioStreamIndex( TInt aIndex ) const;
    TInt SelectedVideoStreamIndex( TInt aIndex ) const;
    MMCETestUIEngineCmdProvider& CmdProvider( TInt aIndex );
    
private: // Constructors 

    CMCETestUISessionViewModel( CMCETestUIEngine& aEngine,
                                CMCETestUIEngineSession& iSession );
    CMCETestUISessionViewModel();
    
    void ConstructL();
  

private: // convenience functions

    void PopulateStreamsL();
    
    void PopulateVideoStreamsL();
    
private: //data

    CDesCArray* iArray; 
    CMCETestUIEngine& iEngine;
    CMCETestUIEngineSession& iSession; // Not owned
	
	TBool audioStreamsSuppressed;
    };
    
#endif
    
// End of File
