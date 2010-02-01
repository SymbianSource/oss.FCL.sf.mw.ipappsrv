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

#ifndef __CMCETESTUISTREAMVIEWMODEL_H
#define __CMCETESTUISTREAMVIEWMODEL_H

// INCLUDES

#include <bamdesca.h>
#include <badesca.h>

#include "MMCETestUIEngineCmdProvider.h"

// FORWARD DECLARATIONS

class CMCETestUIEngine;
class CMCETestUIEngineSession;
class CMCETestUIEngineAudioStream;
class CMCETestUIEngineVideoStream;

// CLASS DECLARATION

/**
*  CMCETestUISessionViewModel
*  
*/
class CMCETestUIStreamViewModel : public MDesCArray
    {
public: // Constructors and destructor
    
    static CMCETestUIStreamViewModel* NewL( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineAudioStream& aStream );
    
    /**
    * Destructor.
    */
    ~CMCETestUIStreamViewModel ();
    
public: // from MDesCArray

    TInt MdcaCount() const;
    TPtrC MdcaPoint( TInt aIndex ) const; 
    
public: // New functions
    TBool IsAudioSourceSelected( TInt aIndex ) const;
    TInt SelectedAudioSinkIndex( TInt aIndex ) const;
    TInt SelectedAudioCodecIndex( TInt aIndex ) const;
    MMCETestUIEngineCmdProvider& CmdProvider( TInt aIndex );
    
private: // Constructors 

    CMCETestUIStreamViewModel( CMCETestUIEngine& aEngine,
                                CMCETestUIEngineAudioStream& aStream );
    CMCETestUIStreamViewModel();
    
    void ConstructL();
  

private: // convenience functions

   // void PopulateStreamsL();
    void PopulateSourcesL();
    void PopulateSinksL();
    void PopulateCodecsL();
    
private: //data

    CDesCArray* iArray; 
    CMCETestUIEngine& iEngine;
    CMCETestUIEngineAudioStream& iStream; // Not owned
    
    TBool audioSourceSuppressed;
    TBool audioSinksSuppressed;

    };

/**
*  CMCETestUISessionViewModel
*  
*/
class CMCETestUIVideoStreamViewModel : public MDesCArray
    {
public: // Constructors and destructor
    
    static CMCETestUIVideoStreamViewModel* NewL( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineVideoStream& aStream );
    
    /**
    * Destructor.
    */
    ~CMCETestUIVideoStreamViewModel ();
    
public: // from MDesCArray

    TInt MdcaCount() const;
    TPtrC MdcaPoint( TInt aIndex ) const; 
    
public: // New functions
    TBool IsVideoSourceSelected( TInt aIndex ) const;
    TInt SelectedVideoSinkIndex( TInt aIndex ) const;
    TInt SelectedVideoCodecIndex( TInt aIndex ) const;
    MMCETestUIEngineCmdProvider& CmdProvider( TInt aIndex );
    
private: // Constructors 

    CMCETestUIVideoStreamViewModel( CMCETestUIEngine& aEngine,
                                CMCETestUIEngineVideoStream& aStream );
    CMCETestUIVideoStreamViewModel();
    
    void ConstructL();
  

private: // convenience functions

   // void PopulateStreamsL();
    void PopulateVideoSourcesL();
    void PopulateVideoSinksL();
    void PopulateVideoCodecsL();
    
private: //data

    CDesCArray* iArray; 
    CMCETestUIEngine& iEngine;
    CMCETestUIEngineVideoStream& iStream; // Not owned
    
    TBool videoSourceSuppressed;
    TBool videoSinksSuppressed;
    };
    
#endif
    
// End of File
