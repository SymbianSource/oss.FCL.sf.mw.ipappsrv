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




// INCLUDE FILES
#include <e32svr.h>

#include "MccCameraHandler.h"
#include "MccScreen.h"
#include "MccControllerLogs.h"

#include "mmccinterfacedef.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCameraHandler::CMccCameraHandler
// -----------------------------------------------------------------------------
//
CMccCameraHandler::CMccCameraHandler( TInt aCameraIndex, TInt aPriority ):
	iState( EInit), iPriority( aPriority ), iCameraIndex( aCameraIndex )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::~CMccCameraHandler
// -----------------------------------------------------------------------------
//
CMccCameraHandler::~CMccCameraHandler()
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::NewL
// -----------------------------------------------------------------------------
//
CMccCameraHandler* CMccCameraHandler::NewL( TInt aCameraIndex, TInt aPriority )
    {    
    CMccCameraHandler* self = new ( ELeave ) CMccCameraHandler( aCameraIndex, aPriority );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ConstructL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::EnableViewFinderL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::EnableViewFinderL( TMccVideoSinkSetting& aSetting )
	{
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::DisableViewFinderL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::DisableViewFinderL()
	{
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetCamInfoL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::GetCamInfoL( TCameraInfo& aInfo, TBool aForceCameraCreation )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetNumOfCamerasL
// -----------------------------------------------------------------------------
//
TInt CMccCameraHandler::GetNumOfCamerasL()
    {
	return 1;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::GetValueL( TUint32 aParam, TInt& aVal )
    {
	switch( aParam )
	    {
	    case KMccCamZoomFactor:
            aVal = 1;
	        break;
	    case KMccCamDigitZoomFactor:
            aVal = 2;
	        break;
	    case KMccCamContrast:
            aVal = 3;
	        break;
	    case KMccCamBrightness:
            aVal = 4;
	        break;
	    case KMccCamWhiteBalance:
            aVal = 5;
	        break;
	    default:
	        User::Leave( KErrNotSupported );    
	    }
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::SetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::SetValueL( TUint32 aParam, TInt aVal )
    {
    RDebug::Print(_L("aParam=%d, and aVal=%d"), aParam, aVal);
	switch( aParam )
	    {
	    case KMccCamZoomFactor:
	        break;
	    case KMccCamDigitZoomFactor:
	        break;
	    case KMccCamContrast:
	        break;
	    case KMccCamBrightness:
	        break;
	    case KMccCamWhiteBalance:
	        break;
	    default:
	        User::Leave( KErrNotSupported );    
	    }
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::GetValueL( TUint32 /*aParam*/, TReal& /*aVal*/ )
    {
	User::Leave( KErrNotSupported );    
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::SetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::SetValueL( TUint32 /*aParam*/, TReal /*aVal*/ )
    {
	User::Leave( KErrNotSupported );    
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ReserveResourcesL
// -----------------------------------------------------------------------------
//   
void CMccCameraHandler::ReserveResourcesL(  TInt aCameraIndex )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ReleaseResources
// -----------------------------------------------------------------------------
//        
void CMccCameraHandler::ReleaseResources()
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::Handle
// -----------------------------------------------------------------------------
//
TUint32 CMccCameraHandler::Handle()
    {
    return 1;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::IsCameraReady
// -----------------------------------------------------------------------------
//        
TBool CMccCameraHandler::IsCameraReady()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::MonitorCameraReservation
// -----------------------------------------------------------------------------
//        
TInt CMccCameraHandler::MonitorCameraReservation( 
    MMccCameraHandlerObserver& /*aObserver*/, 
    TBool /*aStopMonitoring*/ )
    {
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccCameraHandler::ReserveComplete
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ReserveComplete( TInt aError )
	{
	}
	
// -----------------------------------------------------------------------------
// CMccCameraHandler::PowerOnComplete
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::PowerOnComplete( TInt aError )
	{
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::ViewFinderFrameReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ViewFinderFrameReady( CFbsBitmap& aFrame )
	{
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::ImageReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ImageReady( CFbsBitmap* /*aBitmap*/, HBufC8* /*aData*/, TInt /*aError*/ )
	{
	
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::FrameBufferReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::FrameBufferReady( MFrameBuffer* /*aFrameBuffer*/, TInt /*aError*/ )
	{	
	}
	
// -----------------------------------------------------------------------------
// CMccCameraHandler::GetViewFinderSettingsL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::GetViewFinderSettingsL( TMccVideoSinkSetting& /*aSetting*/ )
    {
        
    }