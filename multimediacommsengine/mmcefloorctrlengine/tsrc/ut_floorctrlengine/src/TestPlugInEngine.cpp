/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#include "TestPlugInEngine.h"

#include <e32base.h>


// ----------------------------------------------------------------------------
// CTestPlugInEngine::NewL
// ----------------------------------------------------------------------------
//
CTestPlugInEngine* CTestPlugInEngine::NewL(MFCMessageObserver& aObs)
	{
	CTestPlugInEngine* self = CTestPlugInEngine::NewLC(aObs);
	CleanupStack::Pop( self );
	return self;
	}

// ----------------------------------------------------------------------------
// CTestPlugInEngine::NewLC
// ----------------------------------------------------------------------------
//
CTestPlugInEngine* CTestPlugInEngine::NewLC(MFCMessageObserver& aObs)
	{
	CTestPlugInEngine* self = new (ELeave) CTestPlugInEngine(aObs);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CTestPlugInEngine::~CTestPlugInEngine()
	{

	iFCPlugIns.ResetAndDestroy();
	
	
	REComSession::FinalClose();
	
	
	}

// ----------------------------------------------------------------------------
// CTestPlugInEngine::ConstructL
// ----------------------------------------------------------------------------
//
void CTestPlugInEngine::ConstructL()
	{
	ListMechanismImplementationsL();
	
	}

// ----------------------------------------------------------------------------
// CTestPlugInEngine::ConstructL
// ----------------------------------------------------------------------------
//
CTestPlugInEngine::CTestPlugInEngine(MFCMessageObserver& aObs)
: iFCPlugInIter(iFCPlugIns),
iMsgObserver(aObs)
	{
	}


// ----------------------------------------------------------------------------
// CTestPlugInEngine::FCPlugInIter
// ----------------------------------------------------------------------------
//	
TFCPlugInIter& CTestPlugInEngine::FCPlugInIter()
	{
	return iFCPlugInIter;
	}
// ----------------------------------------------------------------------------
// CTestPlugInEngine::SupportedProtocolL();
// ----------------------------------------------------------------------------
//	

CDesC8Array* CTestPlugInEngine::SupportedProtocolL()
	{
	
	CDesC8ArrayFlat* protocols = new (ELeave) CDesC8ArrayFlat(1);
	CleanupStack::PushL(protocols);
	TFCPlugInIter plugInIter(iFCPlugIns);
	CFCPlugInInfo* plugin = plugInIter.First();	

	while (plugin)
		{
		protocols->AppendL( plugin->Name() );
		plugin = plugInIter.Next();
		}
	
	CleanupStack::Pop(protocols);
	return protocols;
	}
	
// ----------------------------------------------------------------------------
// CTestPlugInEngine:: PlugInByName();
// ----------------------------------------------------------------------------
//	

MFCPlugIn* CTestPlugInEngine::PlugInByNameL(const TDesC8& aProtocolName) 
	{
	TFCPlugInIter plugInIter(iFCPlugIns);
	CFCPlugInInfo* plugin = NULL;
	plugin = plugInIter.First();
	while (plugin)
		{
		if (aProtocolName.CompareF(plugin->Name()) == KErrNone)
			{
		    TestInterfaceInitParams initParam(iMsgObserver);
			// Instantiate plug-in with UID
			return CTestInterface::NewL( plugin->Uid(), initParam );
			}
		plugin = plugInIter.Next();
		}
	return NULL;
	}
	
	

// ----------------------------------------------------------------------------
// CTestPlugInEngine::ListMechanismImplementationsL
// ----------------------------------------------------------------------------
//
void CTestPlugInEngine::ListMechanismImplementationsL()
	{
	// List implementations in ECom registry for our launcher interface
	RImplInfoPtrArray infoArray;		
	CleanupStack::PushL( TCleanupItem( ArrayCleanup, &infoArray ) );	
	REComSession::ListImplementationsL( KTestInterfaceUid,
										infoArray );

	// Go trough the ECom entries and parse them into CSIPClientData objects	
	for( TInt i = 0; i < infoArray.Count(); i++ )
		{
		CFCPlugInInfo* plugin = 
		    CFCPlugInInfo::NewL( infoArray[i]->ImplementationUid(), 
		                         infoArray[i]->DataType() );
		//add to the Plugin Iter
		User::LeaveIfError( iFCPlugIns.Append( plugin ) );
		}

	CleanupStack::PopAndDestroy(); //TCleanupItem
	}


// -----------------------------------------------------------------------------
// CTestPlugInEngine::ArrayCleanup
// -----------------------------------------------------------------------------
//
void CTestPlugInEngine::ArrayCleanup(TAny* aArray)
	{
    RImplInfoPtrArray* array = reinterpret_cast<RImplInfoPtrArray*>(aArray);
    if (array)
        {
        array->ResetAndDestroy();
        }
	}


