/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation.
*
*/




#include <sipprofile.h>
#include <utf.h>
#include "TMCETestUIEngineCmdDisplayProfileUser.h"
#include "CMCETestUIEngineProfile.h"
#include "MCETestUIEngineConstants.h"

TMCETestUIEngineCmdDisplayProfileUser::TMCETestUIEngineCmdDisplayProfileUser( 
  CMCETestUIEngine& aEngine,
  CMCETestUIEngineProfile& aProfile )
  : TMCETestUIEngineCmdBase( aEngine ), iProfile(aProfile)
  {
  }

void TMCETestUIEngineCmdDisplayProfileUser::ExecuteL()
  {
  CSIPProfile& profile = iProfile.Profile();
  const TDesC8* user( NULL );
  User::LeaveIfError( profile.GetParameter( KSIPContactHeaderUser, user ) ); 
  // ownership of user not transferred, CSIPProfile owns it.  
  HBufC* unicodeUser = CnvUtfConverter::ConvertToUnicodeFromUtf8L(*user);
  CleanupStack::PushL(unicodeUser);
  iEngine.NotifyUserWaitL(*unicodeUser);
  CleanupStack::PopAndDestroy(unicodeUser);
  }
  
const TDesC& TMCETestUIEngineCmdDisplayProfileUser::Caption() const
{
  return KCommandCaptionDisplayProfileUser;
}