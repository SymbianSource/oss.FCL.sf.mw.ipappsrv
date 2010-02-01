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
* Description:  Implementation
*
*/


#include <utf.h>
#include <hal.h>
#include "TCmdGetMemoryUsage.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	KParamProcess
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	KParamUsedMemory, KParamFreeMemory
 *   IDs:			-
 */
void TCmdGetMemoryUsage::ExecuteL()
	{
	//This returns the used memory across the whole system, and not for the
	//specified server
	TInt totalMemory = 0;	
	User::LeaveIfError( HAL::Get( HALData::EMemoryRAM, totalMemory ) );
	TInt freeMemory = 0;
	User::LeaveIfError( HAL::Get( HALData::EMemoryRAMFree, freeMemory ) );
	TInt usedMemory = totalMemory - freeMemory;



	//TODO: Obtaining the memory use of target process doesn't work yet
#if 0
	TPtrC8 serverParam = ExtractTextL( KParamProcess );
	HBufC16* server = CnvUtfConverter::ConvertToUnicodeFromUtf8L( serverParam );
	CleanupStack::PushL( server );

	HBufC* matchPattern = HBufC::NewL( server->Size() + 2 );
	TPtr ptr = matchPattern->Des();
	_LIT( KProcessMatchFormat, "*%S*" );
	ptr.AppendFormat( KProcessMatchFormat, server );
	CleanupStack::PopAndDestroy( server );
	CleanupStack::PushL( matchPattern );

#if ( !defined( EKA2 ) && ( defined( __WINS__ ) || defined( __WINSCW__ ) ) )
	// Emulator doesn't have a concept of process (in EKA1)
	RThread process;
	TFindThread processFinder( *matchPattern );
#else
	RProcess process;
	TFindProcess processFinder( *matchPattern );
#endif
	
	// Find the process
	TFullName processName;
	if ( processFinder.Next( processName ) == KErrNone )
		{
		User::InfoPrint( _L( "Found" ) );
		User::LeaveIfError( process.Open( processFinder ) );

		TModuleMemoryInfo memInfo;
		//Currently returns KErrNotSupported
		TInt err = process.GetMemoryInfo( memInfo );
		//What does the value returned by memInfo.GetInitialisedDataSize() mean?


		//User-functions AllocSize, Available, CountAllocCells act on the
		//currently active process
		
		//User::SwitchHeap could be used to set the head of the target process
		//as the default heap, and then use RHeap::AllocSize? But how to get
		//RHeap from RProcess?

		//It is not allowed to use kernel code from user code
		//(Kern::FreeRamInBytes etc)

		process.Close();
		}
	
	CleanupStack::PopAndDestroy( matchPattern );
#endif // #if 0

	// -- Response creation ---------------------------------------------------

	AddIntegerResponseL( KParamUsedMemory, usedMemory );
	AddIntegerResponseL( KParamFreeMemory, freeMemory );	
	}

TBool TCmdGetMemoryUsage::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetMemoryUsage") );
	}

TTcCommandBase* TCmdGetMemoryUsage::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetMemoryUsage( aContext );
	}
