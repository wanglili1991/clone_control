#ifndef __TENNISBASE_HELPER_H__
#define __TENNISBASE_HELPER_H__

#include "I_TennisBase.h"

class TennisBaseHelper
{
public:	
	static I_TennisBase *CreateTennisBase(const char* szFileName);
	static void DestroyTennisBase(I_TennisBase *pTennisBase);

private:
	static void *s_pModule;
	static CREATE_TENNISBASE_INTERFACE s_pCreateTennisBaseInterface;
};

#endif