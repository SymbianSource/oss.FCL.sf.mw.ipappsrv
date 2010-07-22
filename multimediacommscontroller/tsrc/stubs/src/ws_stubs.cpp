/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include <w32std.h>
#include "ws_stubs_helper.h"

static CGraphicsContext::TDrawMode testDrawMode = CGraphicsContext::EDrawModeAND;
static TInt testNumWindowsCreated = 0;

CGraphicsContext::TDrawMode TWsStubsHelper::CurrentDrawMode()
{
    return testDrawMode;
}

TInt TWsStubsHelper::NumWindowsCreated()
{
    return testNumWindowsCreated;
}

void TWsStubsHelper::Reset()
{
    testDrawMode = CGraphicsContext::EDrawModeAND;
    testNumWindowsCreated = 0;
}

// -----------------------------------------------------------------------------
// CWindowGc
// -----------------------------------------------------------------------------
//
CWindowGc::CWindowGc(CWsScreenDevice *aDevice)
    {
    
    }
CWindowGc::~CWindowGc()
    {
    
    }

void CWindowGc::Activate( RDrawableWindow &aDevice )
    {
    
    }
void CWindowGc::SetBrushStyle( TBrushStyle aBrushStyle )
    {
    
    }
void CWindowGc::SetBrushColor( const TRgb &aColor )
    {
    
    }
void CWindowGc::Clear( const TRect &aRect)
    {
    
    }
void CWindowGc::SetDrawMode( TDrawMode aDrawingMode )
    {
    testDrawMode = aDrawingMode;
    }

void CWindowGc::Deactivate()
    {
    
    }
void CWindowGc::CancelClippingRect()
	{
	
	}
void CWindowGc::DrawRect( const TRect &aRect )
	{
	
	}
/*
TInt CWindowGc::APIExtension(TUid aUid, TAny*& aOutput, TAny* aInput)
    {
    
    }
void CWindowGc::Reserved_CGraphicsContext_2()
    {
    
    }
void CWindowGc::Reserved_CBitmapContext_1()
    {
    
    }
void CWindowGc::Reserved_CBitmapContext_2()
    {
    
    }
void CWindowGc::Reserved_CBitmapContext_3()
    {
    
    }
void CWindowGc::Reserved_CWindowGc_3()
    {
    
    }
void CWindowGc::Reserved_CWindowGc_4()
    {
    
    }
void CWindowGc::Reserved_CWindowGc_5()
    {
    
    }
*/
// -----------------------------------------------------------------------------
// CWsScreenDevice
// -----------------------------------------------------------------------------
//
CWsScreenDevice::CWsScreenDevice(RWsSession &/*aWs*/)
    {
    
    }

CWsScreenDevice::~CWsScreenDevice()
    {
    
    }

TInt CWsScreenDevice::Construct( TInt /*aDefaultScreenNumber*/ )
    {
    return KErrNone;
    }
TDisplayMode CWsScreenDevice::DisplayMode() const
    {
    return EColor16MU;
    }

TInt CWsScreenDevice::CreateContext(CGraphicsContext *&aGc)
    {
    CWindowGc* gc = new CWindowGc(NULL);
    aGc = gc;
    return gc != NULL ? KErrNone : KErrNoMemory;
    }

TInt CWsScreenDevice::CurrentScreenMode() const
    {
    
    }

void CWsScreenDevice::GetScreenModeSizeAndRotation(TInt aMode, TPixelsTwipsAndRotation &aSizeAndRotation) const
{

}
// -----------------------------------------------------------------------------
// RWindow
// -----------------------------------------------------------------------------
//
RWindow::RWindow(RWsSession &aWs)
    {
    
    }
TInt RWindow::Construct(const RWindowTreeNode &parent,TUint32 aHandle)
    {
    testNumWindowsCreated++;
    return KErrNone;
    }
void RWindow::BeginRedraw(const TRect &aRect)
    {
    
    }
void RWindow::EndRedraw()
    {
    
    }
void RWindow::Invalidate(const TRect &aRect)
    {
    
    }
void RWindow::SetSize(const TSize &size)
    {
    
    }


// -----------------------------------------------------------------------------
// RWindowBase
// -----------------------------------------------------------------------------
//
RWindowBase::RWindowBase()
    {
    
    }
void RWindowBase::Activate()
    {
    
    }
void RWindowBase::SetPosition(const TPoint &point)
    {
    
    }
TDisplayMode RWindowBase::DisplayMode() const
    {
    return EColor16MU;
    }

// -----------------------------------------------------------------------------
// RWsSession
// -----------------------------------------------------------------------------
//
RWsSession::RWsSession()
    {
    
    }
TInt RWsSession::Connect()
    {
    return KErrNone;
    }
void RWsSession::Close()
    {
    
    }

TInt RWsSession::GetFocusWindowGroup() const
    {
    return 1;
    }
void RWsSession::Flush()
    {
    
    }
void RWsSession::RedrawReady(TRequestStatus *aStat)
    {
    User::RequestComplete(aStat, KErrNone);
    }
void RWsSession::RedrawReadyCancel()
    {

    }
void RWsSession::GetRedraw(TWsRedrawEvent &aEvent)
    {

    }
 
// -----------------------------------------------------------------------------
// RWindowTreeNode
// -----------------------------------------------------------------------------
//
RWindowTreeNode::RWindowTreeNode()
    {
    
    }
void RWindowTreeNode::SetOrdinalPosition(TInt aPos)
    {

    }
void RWindowTreeNode::SetOrdinalPosition(TInt aPos,TInt aOrdinalPriority)
    {

    }

// -----------------------------------------------------------------------------
// RWindowGroup
// -----------------------------------------------------------------------------
//
RWindowGroup::RWindowGroup()
    {

    }
RWindowGroup::RWindowGroup(RWsSession &aWs)
    {

    }
TInt RWindowGroup::Construct(TUint32 aClientHandle,TBool aIsFocusable)
    {
    return KErrNone;
    }
TInt RWindowGroup::SetName(const TDesC &aName)
    {
    return KErrNone;
    }

void RWindowGroup::AutoForeground(TBool /*aState*/)
    {
        
    }

// -----------------------------------------------------------------------------
// RDrawableWindow
// -----------------------------------------------------------------------------
//
RDrawableWindow::RDrawableWindow()
    {
    
    }

// -----------------------------------------------------------------------------
// MWsClientClass
// -----------------------------------------------------------------------------
//
MWsClientClass::MWsClientClass()
    {
    
    }

// -----------------------------------------------------------------------------
// MGraphicsDeviceMap
// -----------------------------------------------------------------------------
//
MGraphicsDeviceMap::MGraphicsDeviceMap()
    {
    
    }

MGraphicsDeviceMap::~MGraphicsDeviceMap()
    {
    
    }

// -----------------------------------------------------------------------------
// CDirectScreenAccess
// -----------------------------------------------------------------------------
//
CDirectScreenAccess* CDirectScreenAccess::NewL(
        RWsSession& aWs,CWsScreenDevice& aScreenDevice,
        RWindowBase& aWin,MDirectScreenAccess& aAbort)
    {
    CDirectScreenAccess* self = new (ELeave) CDirectScreenAccess(aWs,&aScreenDevice,aWin,aAbort);
    CleanupStack::PushL(self);
    self->ConstructL(aWs,EFalse);
    CleanupStack::Pop(self);
    return self;
    }
CDirectScreenAccess* CDirectScreenAccess::NewL(
        RWsSession& aWs,CWsScreenDevice& aScreenDevice,
        RWindowBase& aWin,MDirectScreenAccess& aAbort,TBool aRegionTrackingOnly)
    {
    CDirectScreenAccess* self = new (ELeave) CDirectScreenAccess(aWs,&aScreenDevice,aWin,aAbort);
    CleanupStack::PushL(self);
    self->ConstructL(aWs,EFalse);
    CleanupStack::Pop(self);
    return self;
    }
/*
CDirectScreenAccess::CDirectScreenAccess(
        RWsSession& aWs,CWsScreenDevice* aScreenDevice,
        RWindowBase& aWindow,MDirectScreenAccess& aAbort) :
        CActive( EPriorityStandard ),
        iWindow(aWindow),
        iAbort(aAbort)
    {
    
    }*/
void CDirectScreenAccess::ConstructL(RWsSession& aWs,TBool aRegionTrackingOnly)
    {
    iDrawingRegion = new (ELeave) RRegion;
    iScreenDevice = CFbsScreenDevice::NewL(_L("aa"), EColor16M );
    iGc = CFbsBitGc::NewL();
    }
CDirectScreenAccess::~CDirectScreenAccess()
    {
    delete iGc;
    delete iScreenDevice;
    delete iDrawingRegion;
    }
void CDirectScreenAccess::StartL()
    {
    
    }

void CDirectScreenAccess::DoCancel()
    {
    
    }
void CDirectScreenAccess::RunL()
    {
    
    }

// -----------------------------------------------------------------------------
// CFbsScreenDevice
// -----------------------------------------------------------------------------
//
CFbsScreenDevice* CFbsScreenDevice::NewL(const TDesC& aLibname,TDisplayMode aDispMode)
    {
    return new (ELeave) CFbsScreenDevice(0);
    }
CFbsScreenDevice::CFbsScreenDevice(TInt aScreenNo)
    {
    
    }
CFbsScreenDevice::~CFbsScreenDevice()
    {
    
    }

void CFbsScreenDevice::Update()
    {
    
    }

// -----------------------------------------------------------------------------
// CFbsDevice
// -----------------------------------------------------------------------------
//
CFbsDevice::CFbsDevice()
    {
    
    }

CFbsDevice::~CFbsDevice()
    {
    
    }
// -----------------------------------------------------------------------------
// CFbsBitGc
// -----------------------------------------------------------------------------
//
CFbsBitGc* CFbsBitGc::NewL()
    {
    return new (ELeave) CFbsBitGc;
    }
CFbsBitGc::CFbsBitGc()
    {
    
    }
CFbsBitGc::~CFbsBitGc()
    {
    
    }
void CFbsBitGc::DrawRect(const TRect& aRect)
    {
    
    }
void CFbsBitGc::SetClippingRegion(const TRegion* aRegion)
    {
    
    }
void CFbsBitGc::SetBrushColor(const TRgb& aColor)
    {
    
    }
void CFbsBitGc::SetBrushOrigin(const TPoint& aOrigin)
    {
    
    }
void CFbsBitGc::SetBrushStyle(TBrushStyle aBrushStyle)
    {
    
    }

// -----------------------------------------------------------------------------
// CFbsBitmap
// -----------------------------------------------------------------------------
//
CFbsBitmap::CFbsBitmap()
    {
    
    }

CFbsBitmap::~CFbsBitmap()
    {
    
    }

TInt CFbsBitmap::Create(const TSize& aSizeInPixels,TDisplayMode aDispMode)
    {
    return KErrNone;
    }
