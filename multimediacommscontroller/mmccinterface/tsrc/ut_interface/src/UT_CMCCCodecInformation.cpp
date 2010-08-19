/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




//  CLASS HEADER
#include "UT_CMccCodecInformation.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>


//  INTERNAL INCLUDES
#include "MmccCodecInformation.h"
#include "MmccCodecAMR.h"
#include "mmcccodecinformationfactory.h"
#include "mccinternalcodecs.h"

// CONSTRUCTION
UT_CMccCodecInformation* UT_CMccCodecInformation::NewL()
    {
    UT_CMccCodecInformation* self = UT_CMccCodecInformation::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccCodecInformation* UT_CMccCodecInformation::NewLC()
    {
    UT_CMccCodecInformation* self = new( ELeave ) UT_CMccCodecInformation();
    CleanupStack::PushL( self );

    self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccCodecInformation::~UT_CMccCodecInformation()
    {
    }

// Default constructor
UT_CMccCodecInformation::UT_CMccCodecInformation()
    {
    }

// Second phase construct
void UT_CMccCodecInformation::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//Callback functions (not used)
void UT_CMccCodecInformation::MccEventReceived( const TMccEvent& /*aEvent*/ ){}
void UT_CMccCodecInformation::MccMediaStarted( TUint32 /*aSessionId*/,
                            TUint32 /*aLinkId*/,     
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ ){}
                            
void UT_CMccCodecInformation::MccMediaStopped( TUint32 /*aSessionId*/,
                            TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ ){}
                            
void UT_CMccCodecInformation::MccMediaPaused( TUint32 /*aSessionId*/,
                           TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ ){}
                           
void UT_CMccCodecInformation::MccMediaResumed( TUint32 /*aSessionId*/,
                            TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ ){}
                            
void UT_CMccCodecInformation::MccMediaPrepared( TUint32 /*aSessionId*/,
                             TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ ){}
                             
void UT_CMccCodecInformation::MccMediaInactive( TUint32 /*aSessionId*/,
                             TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ ){}
                             
void UT_CMccCodecInformation::MccMediaActive( TUint32 /*aSessionId*/,
                             TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ ){}
                             
void UT_CMccCodecInformation::MccCtrlError( TInt /*aError*/ ){}

void UT_CMccCodecInformation::MccCtrlError( TInt /*aError*/,
                           TUint32 /*aSessionId*/,
                           TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ ){}

//  METHODS
void UT_CMccCodecInformation::SetupL()
    {
    iInterface = CMccInterface::NewL( *this );    
    GetCapabilitiesL( iCodecArray );
    iCount = iCodecArray.Count();
    } 

void UT_CMccCodecInformation::Teardown()
    {
 
    if( iInterface )
        {
        delete iInterface;
        iInterface = NULL;
        }
        
    iCodecArray.ResetAndDestroy();
    iCodecArray.Close();    
    }


void UT_CMccCodecInformation::UT_CMccCodecInformation_CloneDefaultsLL()
    {
    RPointerArray< CMccCodecInformation > codecArray1;
    RPointerArray< CMccCodecInformation > codecArray2;
    CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray1 );
    CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray2 );
   
    GetCapabilitiesL( codecArray1 );
    
    TInt count = codecArray1.Count();
    CMccCodecInformation* temp = NULL;
    
    EUNIT_ASSERT( count > 0 );

    for( TInt k = 0; k < count; k++ )
        {
        //Clone defaults
        temp = codecArray1[k]->CloneDefaultsL();
        CleanupStack::PushL( temp );
        User::LeaveIfError( codecArray2.Append( temp ) );
        CleanupStack::Pop( temp );
        temp = NULL;
        
        EUNIT_ASSERT( codecArray2[k] != NULL );
        }
    
    CleanupStack::Pop(2); // array1 & array2        
    codecArray1.ResetAndDestroy();
    codecArray1.Close();
    
    codecArray2.ResetAndDestroy();
    codecArray2.Close();
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_CloneDetailedLL()
    {
    RPointerArray<CMccCodecInformation> codecArray1;
    RPointerArray<CMccCodecInformation> codecArray2;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray1 );
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray2 );
    
    GetCapabilitiesL( codecArray1 );
     
    TInt count = codecArray1.Count();
    CMccCodecInformation* temp = NULL;
    
    //Check that the arrary is not empty
    EUNIT_ASSERT( count > 0 );

    for( TInt k = 0; k < count; k++ )
        {
        //Clone detailed
        temp = codecArray1[k]->CloneDetailedL();
        CleanupStack::PushL( temp );
        User::LeaveIfError( codecArray2.Append( temp ) );
        CleanupStack::Pop( temp );
        temp = NULL;
        
        EUNIT_ASSERT( codecArray2[k] != NULL );
        }
    
    CleanupStack::Pop(2); // array1 & array2
    codecArray1.ResetAndDestroy();
    codecArray1.Close();
    
    codecArray2.ResetAndDestroy();
    codecArray2.Close();
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_RequireSignallingL()
    {        
    // Test base class
    EUNIT_ASSERT( !iCodecArray[ 0 ]->CMccCodecInformation::RequireSignalling( *iCodecArray[ 0 ] ) );
    
    // Test not changing
    for ( TInt i = 0; i < iCodecArray.Count(); i++ )
        {
        EUNIT_ASSERT( !iCodecArray[ i ]->RequireSignalling( *iCodecArray[ i ] ) );
        }
        
    // Test changing
    for ( TInt j = 0; j < iCodecArray.Count(); j++ )
        {
        CMccCodecInformation* codec = iCodecArray[ j ]->CloneDetailedL();
        CleanupStack::PushL( codec );
        
        codec->SetPTime( 100 );
        codec->SetMaxPTime( 100 );
        codec->SetPayloadType( 99 );
        codec->SetCodecMode( EOctetAligned );
        codec->SetAllowedBitrates( 0 );
        codec->SetMaxBitrate( 63000 );
        codec->SetFramerate( 13 );
        codec->SetFrameHeight( 100 );
        codec->SetFrameWidth( 100 );
        
        EUNIT_ASSERT( iCodecArray[ j ]->RequireSignalling( *codec ) );
        
        CleanupStack::PopAndDestroy( codec );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetValuesL()
    {
    for( TInt k = 1; k < iCount; k++ )
        {      
        TMccCodecInfo codecInfo;
        iCodecArray[k]->SetValues(codecInfo);
        }    
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_GetValuesL()
    {
    TMccCodecInfo codecinfo;

    for( TInt k = 1; k < iCount; k++ )
        {      
        TMccCodecInfo codecinfo;

        iCodecArray[k]->GetValues(codecinfo);
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_TypeL(  )
    {
    TMccCodecInfo codecinfo;

    for( TInt k = 1; k < iCount; k++ )
        {      
        TMccCodecInfo codecinfo;
        TUid uidi = iCodecArray[k]->Type();
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_EnableVADL()
    {  
    //Confirm that the arrary is not empty
    EUNIT_ASSERT( iCount > 0 );
    
    TInt error( KErrNone );
     
    for( TInt k = 0; k < iCount; k++ )
        {
        //If AMR
        if ( iCodecArray[k]->SdpName().Compare( KAMRSdpName ) == 0 )
            {
            error = iCodecArray[k]->EnableVAD( EFalse );
            EUNIT_ASSERT_EQUALS( error, KErrNone );
        
            error = iCodecArray[k]->EnableVAD( ETrue );
            EUNIT_ASSERT_EQUALS( error, KErrNone );
            }
        
        else
            {
            error = iCodecArray[k]->EnableVAD( EFalse );
            //EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
        
            error = iCodecArray[k]->EnableVAD( ETrue );
            //EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
            }
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetBitrateL()
    {  
    //Confirm that the arrary is not empty
    EUNIT_ASSERT( iCount > 0 );
    
    CMccCodecInformation* codecAMR = NULL;
    
    TInt bitrate( 0 );
    
    // Find the AMR codec
    for ( TInt i = 0; i < iCount; i++ )
        {
        //If amr
        if ( iCodecArray[i]->SdpName().Compare( KAMRSdpName ) == 0 )
            {
            EUNIT_ASSERT_EQUALS( KErrNotSupported, iCodecArray[i]->SetBitrate( bitrate ) );
            }
        else
            {
            iCodecArray[i]->SetBitrate( bitrate );
            }
        }  
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetSamplingFreqL()
    {    
    //Confirm that the arrary is not empty
    EUNIT_ASSERT( iCount > 0 );
    
    TInt error( KErrNone );
    
    //Call parent class method
    //Currently no other sampling frequencies than 8000Hz are supported
    error = iCodecArray[0]->SetSamplingFreq( 5 );
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    
    // Real codecs may support also other freqs
    for( TInt k = 0; k < iCount; k++ )
        {
        error = iCodecArray[k]->SetSamplingFreq( 0 );
        //EUNIT_ASSERT_EQUALS( error, KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetSdpNameL(  )
    {
    // test implemented in inhereted classes.
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetPayloadTypeL(  )
    {
    // test implemented in inhereted classes.
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetCodecModeL(  )
    {
    // test implemented in inhereted classes.
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetAllowedBitratesL(  )
    {        
    TInt error( KErrNone );
    TUint bitrateMask;
    TInt bitrate;    
    HBufC* fmtp16 = NULL;
    
    for( TInt k = 0; k < iCount; k++ )
        {
        /*
        const TUint KMccAllowedAmrNbBitrate475 = 0x0001;
        const TUint KMccAllowedAmrNbBitrate515 = 0x0002;
        const TUint KMccAllowedAmrNbBitrate590 = 0x0004;
        const TUint KMccAllowedAmrNbBitrate670 = 0x0008;
        const TUint KMccAllowedAmrNbBitrate740 = 0x0010;
        const TUint KMccAllowedAmrNbBitrate795 = 0x0020;
        const TUint KMccAllowedAmrNbBitrate102 = 0x0040;
        const TUint KMccAllowedAmrNbBitrate122 = 0x0080;
        const TUint KMccAllowedAmrNbBitrateAll = 0x00FF;
        */
        
        //If AMR
        if ( iCodecArray[k]->SdpName().Compare( KAMRSdpName ) == 0 )
            {
            _LIT( KTxtFmtp2, "TestSetAllowedBitratesL, Fmtp : %s" );
            
            TUint bitrateNotAllowed = 0xFFFF;
            bitrateMask = KMccAllowedAmrNbBitrateAll;
            
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 12200 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 12200 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 12200 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
            
            EUNIT_ASSERT_NO_LEAVE( iCodecArray[k]->CMccCodecInformation::SetAllowedBitrates( 5 ) );
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ) );
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            
            //////////////// new test
            bitrateMask = KMccAllowedAmrNbBitrate475;
            
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 4750 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 4750 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 4750 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
                
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ));
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            
            //////////////// new test
            bitrateMask = KMccAllowedAmrNbBitrate515;
            
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 5150 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 5150 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 5150 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
                
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ) );
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            
            //////////////// new test
            bitrateMask = KMccAllowedAmrNbBitrate590;
            
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 5900 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 5900 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 5900 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
            
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ) );
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            
            //////////////// new test
            bitrateMask = KMccAllowedAmrNbBitrate670;
            
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 6700 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 6700 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 6700 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
                
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ) );
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            
            //////////////// new test
            bitrateMask = KMccAllowedAmrNbBitrate740;
            
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 7400 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 7400 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 7400 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
                
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ) );
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            
            //////////////// new test
            bitrateMask = KMccAllowedAmrNbBitrate795;
            
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 7950 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 7950 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 7950 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
                
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ) );
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            
             //////////////// new test
            bitrateMask = KMccAllowedAmrNbBitrate102;
            
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 10200 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 10200 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 10200 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
                
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ) );
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            
             //////////////// new test
            bitrateMask = KMccAllowedAmrNbBitrate122;
            
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 12200 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 12200 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 12200 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
                
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ) );
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            
             //////////////// new test
            bitrateMask = KMccAllowedAmrNbBitrate740 | KMccAllowedAmrNbBitrate102;
            
            User::LeaveIfError( iCodecArray[k]->SetAllowedBitrates( bitrateMask ) );
            bitrate = iCodecArray[k]->Bitrate();
            
            if( bitrate == 10200 )
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - CORRECT - bitrate 10200 == %d"), bitrate );
                }
            else
                {
                RDebug::Print( _L("TestSetAllowedBitratesL - ERROR - bitrate 10200 == %d"), bitrate );
                User::Leave( KErrGeneral  );
                }
            
            User::LeaveIfError( iCodecArray[k]->SetCodecMode( EOctetAligned ) );
           
            fmtp16 = HBufC::NewLC( iCodecArray[k]->GetFmtpL().Length() + 1 );
            fmtp16->Des().Copy( iCodecArray[k]->GetFmtpL() );
            
            RDebug::Print( KTxtFmtp2, fmtp16->Des().PtrZ() );
            CleanupStack::PopAndDestroy( fmtp16 );
            }
        
        else
            {
            }
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetPTimeL(  )
    {
    // test implemented in inhereted classes.
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetMaxPTimeL(  )
    {
    // test implemented in inhereted classes.
    EUNIT_ASSERT( ETrue );
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetJitterBufBufferLengthL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetJitterBufBufferLength( 4 ), KErrUnderflow );
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetJitterBufBufferLength( 5 ), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetJitterBufThresholdL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetJitterBufThreshold( 5 ), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetJitterBufInactivityTimeOutL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetJitterBufInactivityTimeOut( 5 ), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetMaxBitrateL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetMaxBitrate( 128000 ), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetAverageBitrateL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetAverageBitrate( 12200 ), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetFramerateL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetFramerate( 100 ), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetFrameHeightL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetFrameHeight( 100 ), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetFrameWidthL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetFrameWidth( 100 ), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetAlgoL()
    {
    TInt error( KErrNone );
    
    //Confirm that the arrary is not empty
    EUNIT_ASSERT( iCount > 0 );
    
    //Call parent class method
    TAlgoUsed algo;
    
    error = iCodecArray[0]->CMccCodecInformation::SetAlgo( ENoAlgoUsed );
    EUNIT_ASSERT_EQUALS( KErrNone, error );
    
    algo = iCodecArray[0]->CMccCodecInformation::Algo();
    EUNIT_ASSERT_EQUALS( ENoAlgoUsed, algo );
    
    for( TInt k = 0; k < iCount; k++ )
        {
        error = iCodecArray[k]->SetAlgo( ENoAlgoUsed );
        EUNIT_ASSERT_EQUALS( KErrNone, error );
        
        algo = iCodecArray[k]->Algo();
        EUNIT_ASSERT_EQUALS( ENoAlgoUsed, algo );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetRedCountL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        if (k != 0 )// other codecs
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRedCount( 0 ), KErrNone );
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRedCount( 1 ), KErrNone );
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRedCount( 2 ), KErrArgument );
            }
        else // AMR
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRedCount( 0 ), KErrNone );
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRedCount( 1 ), KErrNone );
            iCodecArray[k]->iMaxRed = 1;
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRedCount( 0 ), KErrNone );
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRedCount( 1 ), KErrNone );
            }
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetRedundancyPTL()
    {    
    EUNIT_ASSERT( iCount > 0 );
    TInt error( KErrNone );
    TInt redundancy( 0 );

     //Call parent class method
    error = iCodecArray[0]->CMccCodecInformation::SetRedundancyPT( redundancy );
    EUNIT_ASSERT_EQUALS( KErrNone, error );
    
    redundancy = iCodecArray[0]->CMccCodecInformation::RedundancyPT();
    EUNIT_ASSERT_EQUALS( 0, redundancy );
    
    for( TInt k = 0; k < iCount; k++ )
        {
        error = iCodecArray[k]->SetRedundancyPT( 0 );
        EUNIT_ASSERT_EQUALS( KErrNone, error );
        
        redundancy = iCodecArray[k]->RedundancyPT();
        EUNIT_ASSERT_EQUALS( 0, redundancy );
        } 
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetPriorityL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        iCodecArray[k]->SetPriority( 1 );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetPriorityPreferenceL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        iCodecArray[k]->SetPriorityPreference( 1 );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetKeepAliveTimerL(  )
    {
    const TInt KNum25 = 25;
    const TInt KSecsToMicroSecs = 1000000;
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetKeepAliveTimer( KNum25 ), KErrNone );
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->KeepAliveTimerMicroSecs(), KNum25 * KSecsToMicroSecs );
        }
    }
    
void UT_CMccCodecInformation::UT_CMccCodecInformation_SetKeepAliveTimerMicroSecsL(  )
    {
    const TInt KNum2000000 = 2000000;
    const TInt KSecsToMicroSecs = 1000000;
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetKeepAliveTimerMicroSecs( 2000000 ), KErrNone );
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->KeepAliveTimer(), KNum2000000 / KSecsToMicroSecs );
        }
    }    

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetKeepAlivePTL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetKeepAlivePT( iCodecArray[k]->iPayloadType ), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetKeepAliveDataL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        const TUint8 KG711KAPayloadSize = 160;
        const TUint8 KG711KeepAlivePayload[KG711KAPayloadSize] = 
            { 
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };
        TBuf8<KG711KAPayloadSize> payload;
        payload.Append( KG711KeepAlivePayload[KG711KAPayloadSize] ); 
        TPtrC8 ptr( payload ); 
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetKeepAliveData( ptr ), KErrNone );
        /*TBuf8<320> payload2;
        payload2.Append( KG711KeepAlivePayload[KG711KAPayloadSize] );
        payload2.Append( KG711KeepAlivePayload[KG711KAPayloadSize] );
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetKeepAliveData( payload2 ), KErrArgument );*/
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetComfortNoiseGenerationL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        if ( iCodecArray[k]->SdpName() == KPCMUSdpName || 
             iCodecArray[k]->SdpName() == KPCMASdpName || 
             iCodecArray[k]->SdpName() == KILBCSdpName )
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetComfortNoiseGeneration( 13 ), KErrNone );
            }
        else
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetComfortNoiseGeneration( 13 ), KErrNotSupported );
            }        
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetChannelsL()
    {
#if 0 // Removed temporarily because code does not work
    //TBuf8<5> noChannels;
    //noChannels.Format( KNullDesC8 );
    
    TBuf8<5> someChannels;
    someChannels.Format( _L8( "1 2" ) );

    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetChannels( KNullDesC8 ), KErrNone );
        RDebug::Print( _L( "UT_CMccCodecInformation_SetChannelsL NULL %d OK" ), k );
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetChannels( someChannels ), KErrNone );
        RDebug::Print( _L( "UT_CMccCodecInformation_SetChannelsL %d OK" ), k );
        }
#endif
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetMaxRedL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        if ( !iCodecArray[k]->SdpName().CompareF( KAMRSdpName ) ||
             !iCodecArray[k]->SdpName().CompareF( KAMRWbSdpName ) )
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetMaxRed( 40 ), KErrNone );
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetMaxRed( 3 ), KErrArgument );
            }
        else
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetMaxRed( 5 ), KErrNotSupported );
            }        
        }
    }
    

void UT_CMccCodecInformation::UT_CMccCodecInformation_VADL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->VAD(), iCodecArray[k]->iEnableVAD ) ;
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_BitrateL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->Bitrate(), iCodecArray[k]->iBitrate );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_AllowedBitratesL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->AllowedBitrates(), iCodecArray[k]->iBitrateMask );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SamplingFreqL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SamplingFreq(), iCodecArray[k]->iSamplingFreq );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SdpNameL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SdpName(), iCodecArray[k]->iSdpName );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_PayloadTypeL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->PayloadType(), iCodecArray[k]->iPayloadType );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_CodecModeL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->CodecMode(), iCodecArray[k]->iCodecMode );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_PTimeL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->PTime(), iCodecArray[k]->iPTime );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_MaxPTimeL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->MaxPTime(), iCodecArray[k]->iMaxPTime );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_GetFmtpLL()
    {
    TDesC8* fmtp;

    for( TInt k = 0; k < iCount; k++ )
        {
        fmtp = &( iCodecArray[k]->GetFmtpL() );
        RDebug::Print( _L( "UT_CMccCodecInformation_GetFmtpLL %d OK" ), k );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_FourCCL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->FourCC(), iCodecArray[k]->iFourCC );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_FrameTimeL()
    {
    TInt error( KErrNone );
    
    //Confirm that the arrary is not empty
    EUNIT_ASSERT( iCount > 0 );
    
    //Call parent class method
    TInt frameTime( 0 );
    frameTime = iCodecArray[0]->CMccCodecInformation::FrameTime();
    EUNIT_ASSERT( frameTime >= 0 );
    
    for( TInt k = 0; k < iCount; k++ )
        {
        frameTime = iCodecArray[k]->FrameTime();
        EUNIT_ASSERT( frameTime >= 0 );
        }    
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_FrameSizeL()
    {
    TInt error( KErrNone );
    
    //Confirm that the arrary is not empty
    EUNIT_ASSERT( iCount > 0 );
    
    //Call parent class method
    TInt frameSize( 0 );
    frameSize = iCodecArray[0]->CMccCodecInformation::FrameSize();
    EUNIT_ASSERT( frameSize >= 0 );
    
    for( TInt k = 0; k < iCount; k++ )
        {
        frameSize = iCodecArray[k]->FrameSize();
        EUNIT_ASSERT( frameSize >= 0 );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_AlgoL()
    {   
    //Confirm that the arrary is not empty
    EUNIT_ASSERT( iCount > 0 );
    
    TInt error( KErrNone );
    TUint redCount( 0 );
    
    //Call parent class method
    error = iCodecArray[0]->CMccCodecInformation::SetRedCount( redCount );
    EUNIT_ASSERT_EQUALS( KErrNone, error );
    
    redCount = iCodecArray[0]->CMccCodecInformation::RedCount();
    EUNIT_ASSERT_EQUALS( 0, redCount );
    
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->RedCount(), KErrNone );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_RedCountL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->RedCount(), iCodecArray[k]->iRedCount );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_RedundancyPTL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->RedundancyPT(), iCodecArray[k]->iRedundancyPT );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_JitterBufInactivityTimeOutL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->JitterBufInactivityTimeOut(), 
                             iCodecArray[k]->iJitterBufInactivityTimeOut );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_JitterBufThresholdL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->JitterBufThreshold(), 
                             iCodecArray[k]->iJitterBufThreshold );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_JitterBufBufferLengthL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->JitterBufBufferLength(), 
                             iCodecArray[k]->iJitterBufBufferLength );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_MaxBitrateL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->MaxBitrate(), 
                             iCodecArray[k]->iMaxBitrate );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_AverageBitrateL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->AverageBitrate(), 
                             iCodecArray[k]->iAverageBitrate );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_FramerateL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->Framerate(), 
                             iCodecArray[k]->iFramerate );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_FrameHeightL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->FrameHeight(), 
                             iCodecArray[k]->iVideoHeight );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetModeChangePeriodL()
    {    
    for( TInt k = 1; k < iCount; k++ )// AMR codec tested in it's own test class    
        {        
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetModeChangePeriod( 0 ), KErrNotSupported );
        RDebug::Print( _L( "UT_CMccCodecInformation_SetModeChangePeriodL %d OK" ), k );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_ModeChangePeriodL()
    {
    for( TInt k = 1; k < iCount; k++ )// AMR codec tested in it's own test class
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->ModeChangePeriod(), KErrNotSupported );
        RDebug::Print( _L( "UT_CMccCodecInformation_ModeChangePeriodL %d OK" ), k );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetModeChangeNeighborL()
    {
    const TBool neighbor( EFalse );

    for( TInt k = 1; k < iCount; k++ )// AMR codec tested in it's own test class
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetModeChangeNeighbor( neighbor ), KErrNotSupported );
        RDebug::Print( _L( "UT_CMccCodecInformation_SetModeChangeNeighborL %d OK" ), k );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_ModeChangeNeighborL()
    {
    for( TInt k = 1; k < iCount; k++ )// AMR codec tested in it's own test class
        {
        // Always true
        EUNIT_ASSERT( iCodecArray[k]->ModeChangeNeighbor() );
        RDebug::Print( _L( "UT_CMccCodecInformation_ModeChangeNeighborL %d OK" ), k );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_FrameWidthL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->FrameWidth(), 
                             iCodecArray[k]->iVideoWidth );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_PriorityL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->Priority(), 
                             iCodecArray[k]->iPriority );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_PriorityPreferenceL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->PriorityPreference(), 
                             iCodecArray[k]->iPriorityPreference );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_KeepAliveTimerL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->KeepAliveTimer(), 
                             iCodecArray[k]->iKeepAliveTimer );
        }
    }
    
void UT_CMccCodecInformation::UT_CMccCodecInformation_KeepAliveTimerMicroSecsL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->KeepAliveTimerMicroSecs(), 
                             iCodecArray[k]->iKeepAliveTimerMicroSecs );
        }
    }    

void UT_CMccCodecInformation::UT_CMccCodecInformation_KeepAlivePTL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->KeepAlivePT(), 
                             iCodecArray[k]->iKeepAlivePT );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_KeepAliveDataL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->KeepAliveData(), 
                             iCodecArray[k]->iKeepAliveData );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_ComfortNoiseGenerationL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->ComfortNoiseGeneration(), 
                             iCodecArray[k]->iComfortNoiseGenerationPt );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_GetChannelsL()
    {
#if 0 // Removed temporarily because code does not work
    HBufC8* buf = HBufC8::NewL( 150 );
    TPtr8 bufRef = buf->Des();

    for( TInt k = 0; k < 1/*iCount*/; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->GetChannels( bufRef ), KErrNone );
        RDebug::Print( _L( "UT_CMccCodecInformation_GetChannelsL %d OK" ), k );
        }
#endif
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_MaxRedL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->MaxRed(), 
                             iCodecArray[k]->iMaxRed );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetFmtpAttrLL()
    {    
    TBuf8<5> buf;
    buf.Format( _L8( "foo42" ) );

    for( TInt k = 0; k < iCount; k++ )
        {
        iCodecArray[k]->SetFmtpAttrL( buf, EFalse );
        }   
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_ParseFmtpAttrLL()
    {    
    //Confirm that the arrary is not empty
    EUNIT_ASSERT( iCount > 0 );
    
    _LIT8( KAMRName, "AMR" );
    CMccCodecInformation* codecAMR = NULL;

    // Find the AMR codec
    for ( TInt i = 0; i < iCount; i++ )
        {
        if ( iCodecArray[i]->SdpName().Compare( KAMRName ) == 0 )
            {
            TInt error( KErrNone );
    
            codecAMR = iCodecArray[i];
            
            EUNIT_ASSERT_EQUALS( KErrNotSupported, codecAMR->SetCodecMode( EPCMU ) );
            EUNIT_ASSERT_EQUALS( KErrNone , codecAMR->SetCodecMode( EBandwidthEfficient ) );
            
            //Create fmtp string
            _LIT8( KTxtOctetAlign1ModeSet, "octet-align=1; mode-set=0,1,3,6," );
            
            //Create a buffer to hold the fmtp string
            HBufC8* fmtpBuf = HBufC8::NewLC( KTxtOctetAlign1ModeSet().Length() );
            fmtpBuf->Des().Copy( KTxtOctetAlign1ModeSet() );
            TPtr8 bufPtr( fmtpBuf->Des() );
            
            //Array for bitrates
            RArray<TUint> bitrates;
            CleanupClosePushL( bitrates );
            
            //codecAMR->CMccCodecInformation::CreateFmtpModeSet( bufPtr, bitrates );
            //codecAMR->CreateFmtpModeSet( bufPtr, bitrates );
            codecAMR->CMccCodecInformation::CreateFmtpAttrListL();
            codecAMR->CreateFmtpAttrListL( bitrates );
            codecAMR->ParseFmtpAttrL( fmtpBuf->Des() );
            
            CleanupStack::PopAndDestroy(); //pop and close bitrates
            CleanupStack::PopAndDestroy( fmtpBuf ); //pop and destroy fmtpBuf
            
            break; //jump out of the loop
            }
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_CreateFmtpAttrListLL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        iCodecArray[k]->CreateFmtpAttrListL();    
        }   
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_CreateFmtpAttrListL_1L(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        iCodecArray[k]->CreateFmtpAttrListL();
        } 
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetCrcL()
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        if ( !iCodecArray[k]->SdpName().CompareF( KAMRSdpName ) ||
             !iCodecArray[k]->SdpName().CompareF( KAMRWbSdpName ) )
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetCrc( EFalse ), KErrNone );
            }
        else
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetCrc( EFalse ), KErrNotSupported );
            }        
        }

    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetCrc( ETrue ), KErrNotSupported );
        }

    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetCrc( -1 ), KErrNotSupported );
        }
 
     for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetCrc( 2 ), KErrNotSupported );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_GetCrcL()
    {
    for( TInt k = 1; k < iCount; k++ )
        {
        // Always false
        EUNIT_ASSERT( !iCodecArray[k]->GetCrc() );
        RDebug::Print( _L( "UT_CMccCodecInformation_GetCrcL %d OK" ), k );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetRobustSortingL()
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        if ( !iCodecArray[k]->SdpName().CompareF( KAMRSdpName ) ||
             !iCodecArray[k]->SdpName().CompareF( KAMRWbSdpName ) )
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRobustSorting( EFalse ), KErrNone );
            }
        else
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRobustSorting( EFalse ), KErrNotSupported );
            }        
        }

    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRobustSorting( ETrue ), KErrNotSupported );
        }

    for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRobustSorting( -1 ), KErrNotSupported );
        }
 
     for( TInt k = 0; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetRobustSorting( 2 ), KErrNotSupported );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_GetRobustSortingL()
    {
    for( TInt k = 1; k < iCount; k++ )
        {
        // Always false
        EUNIT_ASSERT( !iCodecArray[k]->GetRobustSorting() );
        RDebug::Print( _L( "UT_CMccCodecInformation_GetRobustSortingL %d OK" ), k );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetInterleavingL()
    {
    TInt frameBlockCount( 0 );

    for( TInt k = 1; k < iCount; k++ )
        {
        EUNIT_ASSERT_EQUALS( iCodecArray[k]->SetInterleaving( frameBlockCount ), KErrNotSupported );
        RDebug::Print( _L( "UT_CMccCodecInformation_SetInterleavingL %d OK" ), k );
        }
    }

void UT_CMccCodecInformation::UT_CMccCodecInformation_GetInterleavingL()
    {
    for( TInt k = 1; k < iCount; k++ )
        {
        if ( !iCodecArray[k]->SdpName().CompareF( KAMRSdpName ) ||
             !iCodecArray[k]->SdpName().CompareF( KAMRWbSdpName ) )
            {
            EUNIT_ASSERT_EQUALS( 
                iCodecArray[k]->GetInterleaving(), iCodecArray[k]->iFrameBlockCount );
            }
        else
            {
            EUNIT_ASSERT_EQUALS( iCodecArray[k]->GetInterleaving(), KErrNotSupported );
            }
        RDebug::Print( _L( "UT_CMccCodecInformation_GetInterleavingL %d OK" ), k );
        }
    }  

void UT_CMccCodecInformation::UT_CMccCodecInformation_SetConfigKeyL()
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        if ( !iCodecArray[k]->SdpName().CompareF( KAMRSdpName ) )
            {
            EUNIT_ASSERT_SPECIFIC_LEAVE( iCodecArray[k]->SetConfigKeyL( KNullDesC8 ), KErrNotSupported );
            }
        }
    }
    
void UT_CMccCodecInformation::UT_CMccCodecInformation_SetPreferredEncodingDecodingDeviceL(  )
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        if ( iCodecArray[k]->SdpName().CompareF( KAVCSdpName ) == 0 )
            {      
            iCodecArray[k]->SetPreferredEncodingDecodingDevice( TUid::Uid( 0x20001c13 ));
            EUNIT_ASSERT(iCodecArray[k]->PreferredEncodingDecodingDevice() == TUid::Uid( 0x20001c13 ));
            }
        else
            {
            iCodecArray[k]->SetPreferredEncodingDecodingDevice( TUid::Uid( 0x20001c13 ) );
            EUNIT_ASSERT(iCodecArray[k]->PreferredEncodingDecodingDevice() == KNullUid );
            }        
        }
    }        
    
void UT_CMccCodecInformation::UT_CMccCodecInformation_ConfigKeyL()
    {
    for( TInt k = 0; k < iCount; k++ )
        {
        if ( !iCodecArray[k]->SdpName().CompareF( KAMRSdpName ) )
            {
            HBufC8* configKey = iCodecArray[k]->ConfigKeyL();
            EUNIT_ASSERT( configKey == NULL );
            }
        }
    }
    
// HELPERS
void UT_CMccCodecInformation::GetCapabilitiesL( RPointerArray<CMccCodecInformation>& aCodecArray )
    {
    User::LeaveIfError( iInterface->GetCapabilities( aCodecArray ) );
    
    // Append also other codecs which are not yet officially supported!
    
    CMccCodecInformationFactory* codecFactory = CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( codecFactory );
    
    CMccCodecInformation* codec = codecFactory->CreateCodecInformationL( KAVCSdpName );
    CleanupStack::PushL( codec );
    aCodecArray.AppendL( codec );
    CleanupStack::Pop( codec );
    
    CMccCodecInformation* codec2 = codecFactory->CreateCodecInformationL( KRedSdpName );
    CleanupStack::PushL( codec2 );
    aCodecArray.AppendL( codec2 );
    CleanupStack::Pop( codec2 );
    
    CMccCodecInformation* codec3 = codecFactory->CreateCodecInformationL( KTelephoneEvent );
    CleanupStack::PushL( codec3 );
    aCodecArray.AppendL( codec3 );
    CleanupStack::Pop( codec3 );
    
    CMccCodecInformation* codec4 = codecFactory->CreateCodecInformationL( KAMRWbSdpName );
    CleanupStack::PushL( codec4 );
    aCodecArray.AppendL( codec4 );
    CleanupStack::Pop( codec4 );
    
    CleanupStack::PopAndDestroy( codecFactory );
    }

// EUNIT TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccCodecInformation,
    "CMccCodecInformation test",
    "UNIT" )


EUNIT_TEST(
    "CloneDefaultsL - test ",
    "CMccCodecInformation",
    "CloneDefaultsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_CloneDefaultsLL, Teardown)

EUNIT_TEST(
    "CloneDetailedL - test ",
    "CMccCodecInformation",
    "CloneDetailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_CloneDetailedLL, Teardown)

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecInformation",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_RequireSignallingL, Teardown)

EUNIT_TEST(
    "SetValues - test ",
    "CMccCodecInformation",
    "SetValues",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetValuesL, Teardown)

EUNIT_TEST(
    "GetValues - test ",
    "CMccCodecInformation",
    "GetValues",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_GetValuesL, Teardown)

EUNIT_TEST(
    "Type - test ",
    "CMccCodecInformation",
    "Type",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_TypeL, Teardown)

EUNIT_TEST(
    "EnableVAD - test ",
    "CMccCodecInformation",
    "EnableVAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_EnableVADL, Teardown)

EUNIT_TEST(
    "SetBitrate - test ",
    "CMccCodecInformation",
    "SetBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetBitrateL, Teardown)

EUNIT_TEST(
    "SetSamplingFreq - test ",
    "CMccCodecInformation",
    "SetSamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetSamplingFreqL, Teardown)

EUNIT_TEST(
    "SetSdpName - test ",
    "CMccCodecInformation",
    "SetSdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetSdpNameL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccCodecInformation",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "SetCodecMode - test ",
    "CMccCodecInformation",
    "SetCodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetCodecModeL, Teardown)

EUNIT_TEST(
    "SetAllowedBitrates - test ",
    "CMccCodecInformation",
    "SetAllowedBitrates",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetAllowedBitratesL, Teardown)

EUNIT_TEST(
    "SetPTime - test ",
    "CMccCodecInformation",
    "SetPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetPTimeL, Teardown)

EUNIT_TEST(
    "SetMaxPTime - test ",
    "CMccCodecInformation",
    "SetMaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetMaxPTimeL, Teardown)

EUNIT_TEST(
    "SetJitterBufBufferLength - test ",
    "CMccCodecInformation",
    "SetJitterBufBufferLength",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetJitterBufBufferLengthL, Teardown)

EUNIT_TEST(
    "SetJitterBufThreshold - test ",
    "CMccCodecInformation",
    "SetJitterBufThreshold",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetJitterBufThresholdL, Teardown)

EUNIT_TEST(
    "SetJitterBufInactivityTimeOut - test ",
    "CMccCodecInformation",
    "SetJitterBufInactivityTimeOut",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetJitterBufInactivityTimeOutL, Teardown)

EUNIT_TEST(
    "SetMaxBitrate - test ",
    "CMccCodecInformation",
    "SetMaxBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetMaxBitrateL, Teardown)

EUNIT_TEST(
    "SetAverageBitrate - test ",
    "CMccCodecInformation",
    "SetAverageBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetAverageBitrateL, Teardown)

EUNIT_TEST(
    "SetFramerate - test ",
    "CMccCodecInformation",
    "SetFramerate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetFramerateL, Teardown)

EUNIT_TEST(
    "SetFrameHeight - test ",
    "CMccCodecInformation",
    "SetFrameHeight",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetFrameHeightL, Teardown)

EUNIT_TEST(
    "SetFrameWidth - test ",
    "CMccCodecInformation",
    "SetFrameWidth",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetFrameWidthL, Teardown)

EUNIT_TEST(
    "SetAlgo - test ",
    "CMccCodecInformation",
    "SetAlgo",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetAlgoL, Teardown)

EUNIT_TEST(
    "SetRedCount - test ",
    "CMccCodecInformation",
    "SetRedCount",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetRedCountL, Teardown)

EUNIT_TEST(
    "SetRedundancyPT - test ",
    "CMccCodecInformation",
    "SetRedundancyPT",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetRedundancyPTL, Teardown)

EUNIT_TEST(
    "SetPriority - test ",
    "CMccCodecInformation",
    "SetPriority",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetPriorityL, Teardown)

EUNIT_TEST(
    "SetPriorityPreference - test ",
    "CMccCodecInformation",
    "SetPriorityPreference",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetPriorityPreferenceL, Teardown)

EUNIT_TEST(
    "SetKeepAliveTimer - test ",
    "CMccCodecInformation",
    "SetKeepAliveTimer",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetKeepAliveTimerL, Teardown)
    
EUNIT_TEST(
    "SetKeepAliveTimerMicroSecs - test ",
    "CMccCodecInformation",
    "SetKeepAliveTimerMicroSecs",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetKeepAliveTimerMicroSecsL, Teardown)    

EUNIT_TEST(
    "SetKeepAlivePT - test ",
    "CMccCodecInformation",
    "SetKeepAlivePT",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetKeepAlivePTL, Teardown)

EUNIT_TEST(
    "SetKeepAliveData - test ",
    "CMccCodecInformation",
    "SetKeepAliveData",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetKeepAliveDataL, Teardown)

EUNIT_TEST(
    "SetComfortNoiseGeneration - test ",
    "CMccCodecInformation",
    "SetComfortNoiseGeneration",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetComfortNoiseGenerationL, Teardown)

EUNIT_TEST(
    "SetChannels - test ",
    "CMccCodecInformation",
    "SetChannels",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetChannelsL, Teardown)

EUNIT_TEST(
    "SetMaxRed - test ",
    "CMccCodecInformation",
    "SetMaxRed",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetMaxRedL, Teardown)

EUNIT_TEST(
    "VAD - test ",
    "CMccCodecInformation",
    "VAD",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_VADL, Teardown)

EUNIT_TEST(
    "Bitrate - test ",
    "CMccCodecInformation",
    "Bitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_BitrateL, Teardown)

EUNIT_TEST(
    "AllowedBitrates - test ",
    "CMccCodecInformation",
    "AllowedBitrates",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_AllowedBitratesL, Teardown)

EUNIT_TEST(
    "SamplingFreq - test ",
    "CMccCodecInformation",
    "SamplingFreq",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SamplingFreqL, Teardown)

EUNIT_TEST(
    "SdpName - test ",
    "CMccCodecInformation",
    "SdpName",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SdpNameL, Teardown)

EUNIT_TEST(
    "PayloadType - test ",
    "CMccCodecInformation",
    "PayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_PayloadTypeL, Teardown)

EUNIT_TEST(
    "CodecMode - test ",
    "CMccCodecInformation",
    "CodecMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_CodecModeL, Teardown)

EUNIT_TEST(
    "PTime - test ",
    "CMccCodecInformation",
    "PTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_PTimeL, Teardown)

EUNIT_TEST(
    "MaxPTime - test ",
    "CMccCodecInformation",
    "MaxPTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_MaxPTimeL, Teardown)

EUNIT_TEST(
    "GetFmtpL - test ",
    "CMccCodecInformation",
    "GetFmtpL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_GetFmtpLL, Teardown)

EUNIT_TEST(
    "FourCC - test ",
    "CMccCodecInformation",
    "FourCC",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_FourCCL, Teardown)

EUNIT_TEST(
    "FrameTime - test ",
    "CMccCodecInformation",
    "FrameTime",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_FrameTimeL, Teardown)

EUNIT_TEST(
    "FrameSize - test ",
    "CMccCodecInformation",
    "FrameSize",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_FrameSizeL, Teardown)

EUNIT_TEST(
    "Algo - test ",
    "CMccCodecInformation",
    "Algo",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_AlgoL, Teardown)

EUNIT_TEST(
    "RedCount - test ",
    "CMccCodecInformation",
    "RedCount",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_RedCountL, Teardown)

EUNIT_TEST(
    "RedundancyPT - test ",
    "CMccCodecInformation",
    "RedundancyPT",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_RedundancyPTL, Teardown)

EUNIT_TEST(
    "JitterBufInactivityTimeOut - test ",
    "CMccCodecInformation",
    "JitterBufInactivityTimeOut",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_JitterBufInactivityTimeOutL, Teardown)

EUNIT_TEST(
    "JitterBufThreshold - test ",
    "CMccCodecInformation",
    "JitterBufThreshold",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_JitterBufThresholdL, Teardown)

EUNIT_TEST(
    "JitterBufBufferLength - test ",
    "CMccCodecInformation",
    "JitterBufBufferLength",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_JitterBufBufferLengthL, Teardown)

EUNIT_TEST(
    "MaxBitrate - test ",
    "CMccCodecInformation",
    "MaxBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_MaxBitrateL, Teardown)

EUNIT_TEST(
    "AverageBitrate - test ",
    "CMccCodecInformation",
    "AverageBitrate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_AverageBitrateL, Teardown)

EUNIT_TEST(
    "Framerate - test ",
    "CMccCodecInformation",
    "Framerate",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_FramerateL, Teardown)

EUNIT_TEST(
    "FrameHeight - test ",
    "CMccCodecInformation",
    "FrameHeight",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_FrameHeightL, Teardown)

EUNIT_TEST(
    "FrameWidth - test ",
    "CMccCodecInformation",
    "FrameWidth",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_FrameWidthL, Teardown)

EUNIT_TEST(
    "Priority - test ",
    "CMccCodecInformation",
    "Priority",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_PriorityL, Teardown)

EUNIT_TEST(
    "PriorityPreference - test ",
    "CMccCodecInformation",
    "PriorityPreference",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_PriorityPreferenceL, Teardown)

EUNIT_TEST(
    "KeepAliveTimer - test ",
    "CMccCodecInformation",
    "KeepAliveTimer",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_KeepAliveTimerL, Teardown)
    
EUNIT_TEST(
    "KeepAliveTimerMicroSecs - test ",
    "CMccCodecInformation",
    "KeepAliveTimerMicroSecs",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_KeepAliveTimerMicroSecsL, Teardown)    

EUNIT_TEST(
    "KeepAlivePT - test ",
    "CMccCodecInformation",
    "KeepAlivePT",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_KeepAlivePTL, Teardown)

EUNIT_TEST(
    "KeepAliveData - test ",
    "CMccCodecInformation",
    "KeepAliveData",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_KeepAliveDataL, Teardown)

EUNIT_TEST(
    "ComfortNoiseGeneration - test ",
    "CMccCodecInformation",
    "ComfortNoiseGeneration",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_ComfortNoiseGenerationL, Teardown)

EUNIT_TEST(
    "GetChannels - test ",
    "CMccCodecInformation",
    "GetChannels",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_GetChannelsL, Teardown)

EUNIT_TEST(
    "MaxRed - test ",
    "CMccCodecInformation",
    "MaxRed",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_MaxRedL, Teardown)

EUNIT_TEST(
    "SetFmtpAttrL - test ",
    "CMccCodecInformation",
    "SetFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetFmtpAttrLL, Teardown)

EUNIT_TEST(
    "ParseFmtpAttrL - test ",
    "CMccCodecInformation",
    "ParseFmtpAttrL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_ParseFmtpAttrLL, Teardown)

EUNIT_TEST(
    "CreateFmtpAttrListL - test ",
    "CMccCodecInformation",
    "CreateFmtpAttrListL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_CreateFmtpAttrListLL, Teardown)

EUNIT_TEST(
    "CreateFmtpAttrListL - test ",
    "CMccCodecInformation",
    "CreateFmtpAttrListL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_CreateFmtpAttrListL_1L, Teardown)

EUNIT_TEST(
    "SetCrc - test ",
    "CMccCodecInformation",
    "SetCrc",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetCrcL, Teardown)

EUNIT_TEST(
    "GetCrc - test ",
    "CMccCodecInformation",
    "GetCrc",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_GetCrcL, Teardown)

EUNIT_TEST(
    "SetRobustSorting - test ",
    "CMccCodecInformation",
    "SetRobustSorting",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetRobustSortingL, Teardown)

EUNIT_TEST(
    "GetRobustSorting - test ",
    "CMccCodecInformation",
    "GetRobustSorting",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_GetRobustSortingL, Teardown)

EUNIT_TEST(
    "SetInterleaving - test ",
    "CMccCodecInformation",
    "SetInterleaving",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetInterleavingL, Teardown)

EUNIT_TEST(
    "GetInterleaving - test ",
    "CMccCodecInformation",
    "GetInterleaving",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_GetInterleavingL, Teardown)

EUNIT_TEST(
    "SetChannels - test ",
    "CMccCodecInformation",
    "SetChannels",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetChannelsL, Teardown)

EUNIT_TEST(
    "GetChannels - test ",
    "CMccCodecInformation",
    "GetChannels",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_GetChannelsL, Teardown)

EUNIT_TEST(
    "RequireSignalling - test ",
    "CMccCodecInformation",
    "RequireSignalling",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_RequireSignallingL, Teardown)
    
EUNIT_TEST(
    "SetValuesL - test ",
    "CMccCodecInformation",
    "SetValuesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetValuesL, Teardown)
    
EUNIT_TEST(
    "GetValuesL - test ",
    "CMccCodecInformation",
    "GetValuesL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_GetValuesL, Teardown)

EUNIT_TEST(
    "SetConfigKeyL - test ",
    "CMccCodecInformation",
    "SetConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetConfigKeyL, Teardown)
    
EUNIT_TEST(
    "SetPreferredEncodingDecodingDeviceL - test ",
    "CMccCodecInformation",
    "SetPreferredEncodingDecodingDeviceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_SetPreferredEncodingDecodingDeviceL, Teardown)    

EUNIT_TEST(
    "ConfigKeyL - test ",
    "CMccCodecInformation",
    "ConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_CMccCodecInformation_ConfigKeyL, Teardown)
            
EUNIT_END_TEST_TABLE

//  END OF FILE
