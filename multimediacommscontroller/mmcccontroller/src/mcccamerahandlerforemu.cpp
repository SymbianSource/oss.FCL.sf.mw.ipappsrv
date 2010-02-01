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

#include "mcccamerahandler.h"
#include "mccscreen.h"
#include "mcccontrollerlogs.h"

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
CMccCameraHandler::CMccCameraHandler( 
    MMccResources& aResources, TInt aCameraIndex, TInt aPriority ):
    iResources( aResources ),
	iState( EInit ), 
	iPriority( aPriority ), 
	iCameraIndex( aCameraIndex )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::~CMccCameraHandler
// -----------------------------------------------------------------------------
//
CMccCameraHandler::~CMccCameraHandler()
    {
	__CONTROLLER( "CMccCameraHandler::~CMccCameraHandler" )
	
    iObservers.Reset();
    iObservers.Close();
	
    if( iScreen )
    	{
    	iScreen->Stop();    	
    	}
    	
    delete iScreen;	

	__CONTROLLER( "CMccCameraHandler::~CMccCameraHandler, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::NewL
// -----------------------------------------------------------------------------
//
CMccCameraHandler* CMccCameraHandler::NewL( 
    MMccResources& aResources, TInt aCameraIndex, TInt aPriority )
    {    
    CMccCameraHandler* self = 
        new ( ELeave ) CMccCameraHandler( aResources, aCameraIndex, aPriority );
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
	__CONTROLLER( "CMccCameraHandler::ConstructL" )
	
	__CONTROLLER( "CMccCameraHandler::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::EnableViewFinderL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::EnableViewFinderL( TMccVideoSinkSetting& aSetting )
	{
	__CONTROLLER( "CMccCameraHandler::EnableViewFinderL" )
	
	iViewFinderSettings = aSetting;
	
	iViewFinderEnabled = ETrue;

	__CONTROLLER( "CMccCameraHandler::EnableViewFinderL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::DisableViewFinderL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::DisableViewFinderL()
	{
	__CONTROLLER( "CMccCameraHandler::DisableViewFinderL" )
	
	iViewFinderEnabled = EFalse;
	
	__CONTROLLER( "CMccCameraHandler::DisableViewFinderL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::IsViewFinderEnabled
// -----------------------------------------------------------------------------
//  
TBool CMccCameraHandler::IsViewFinderEnabled() const
    {
    return iViewFinderEnabled;
    }


// -----------------------------------------------------------------------------
// CMccCameraHandler::GetViewFinderSettingsL
// -----------------------------------------------------------------------------
//	
void CMccCameraHandler::GetViewFinderSettingsL( TMccVideoSinkSetting& aSetting )
    {
    __CONTROLLER( "CMccCameraHandler::GetViewFinderSettingsL" )

    aSetting = iViewFinderSettings;
	
	__CONTROLLER( "CMccCameraHandler::GetViewFinderSettingsL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetCamInfoL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::GetCamInfoL( TCameraInfo& aInfo, TBool /*aForceCameraCreation*/ )
    {
	__CONTROLLER( "CMccCameraHandler::GetCamInfoL" )   
	
	aInfo.iMinZoom = -100;
	aInfo.iMaxZoom = 100;
	aInfo.iMaxDigitalZoom = 100;
	aInfo.iOptionsSupported = 2147483647; //4294967295;
	__CONTROLLER( "CMccCameraHandler::GetCamInfoL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetNumOfCamerasL
// -----------------------------------------------------------------------------
//
TInt CMccCameraHandler::GetNumOfCamerasL()
    {
	__CONTROLLER( "CMccCameraHandler::GetNumOfCamerasL" )
    
	__CONTROLLER( "CMccCameraHandler::GetNumOfCamerasL, exit" )
	return 1;
    }
// -----------------------------------------------------------------------------
// CMccCameraHandler::GetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::GetValueL( TUint32 aParam, TInt& aVal )
    {
	__CONTROLLER( "CMccCameraHandler::GetValue" )
	RDebug::Print(_L("CMccCameraHandler::GetValue, aParam=%d"), aParam );
	aVal = 5;
	__CONTROLLER( "CMccCameraHandler::GetValue, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::SetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::SetValueL( TUint32 aParam, TInt aVal )
    {
	__CONTROLLER( "CMccCameraHandler::SetValue" )
	RDebug::Print(_L("CMccCameraHandler::GetValue, aParam=%d, aVal=%d"), aParam, aVal );

	__CONTROLLER( "CMccCameraHandler::SetValue, exit" )
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
void CMccCameraHandler::ReserveResourcesL( 
    TInt aCameraIndex, TBool /*aForceCameraCreation*/ )
    {
    __CONTROLLER_INT1( "CMccCameraHandler::ReserveResourcesL, camera index", aCameraIndex )
    
    iCameraIndex = aCameraIndex;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ReleaseResources
// -----------------------------------------------------------------------------
//       
void CMccCameraHandler::ReleaseResources()
    {
    __CONTROLLER( "CMccCameraHandler::ReleaseResources" )
    }
    
// -----------------------------------------------------------------------------
// CMccCameraHandler::Handle
// -----------------------------------------------------------------------------
//
TUint32 CMccCameraHandler::Handle()
	{
	return 0;
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
    TBool /*aForceCameraReserve*/ )
    {
    __CONTROLLER( "CMccCameraHandler::MonitorCameraReservation" )
    
    return KErrNone;
    }
 
// -----------------------------------------------------------------------------
// CMccCameraHandler::StopCameraMonitoring
// -----------------------------------------------------------------------------
//  
TInt CMccCameraHandler::StopCameraMonitoring( 
    MMccCameraHandlerObserver& /*aObserver*/,
    TBool /*aForceCameraRelease*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::NotifyObservers
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::NotifyObservers( TInt /*aError*/ )
	{	
	}
	           
// -----------------------------------------------------------------------------
// CMccCameraHandler::ReserveComplete
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ReserveComplete( TInt /*aError*/ )
	{
	__CONTROLLER( "CMccCameraHandler::ReserveComplete" )
	}
	
// -----------------------------------------------------------------------------
// CMccCameraHandler::PowerOnComplete
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::PowerOnComplete( TInt /*aError*/ )
	{
	__CONTROLLER( "CMccCameraHandler::PowerOnComplete" )
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::ViewFinderFrameReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ViewFinderFrameReady( CFbsBitmap& aFrame )
	{
	if ( iScreen )
		{
		iScreen->Draw( aFrame );			
		}
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
// CMccCameraHandler::HandleEvent
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::HandleEvent( const TECAMEvent& /*aEvent*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ViewFinderReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ViewFinderReady( 
    MCameraBuffer& /*aCameraBuffer*/, TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ImageBufferReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ImageBufferReady( 
    MCameraBuffer& /*aCameraBuffer*/,TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::VideoBufferReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::VideoBufferReady( 
    MCameraBuffer& /*aCameraBuffer*/,TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::TimerExpiredL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::TimerExpiredL( TMccTimerId /*aTimerId*/, TAny* /*aTimerParam*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccCameraHandler::SettingsChanged
// -----------------------------------------------------------------------------
//	
TBool CMccCameraHandler::SettingsChanged( const TMccVideoSinkSetting& /*aSetting*/ ) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::UpdateCameraSettingsL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::UpdateCameraSettingsL()
    {
    }
    
// -----------------------------------------------------------------------------
// CMccCameraHandler::ResetCameraHandler
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ResetCameraHandler()
    {
    
    }
// End of file

