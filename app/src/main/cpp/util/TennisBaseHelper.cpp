#include "TennisBaseHelper.h"

#include "ALoxHelper.h"

#include <dlfcn.h>

void *TennisBaseHelper::s_pModule = NULL;
CREATE_TENNISBASE_INTERFACE TennisBaseHelper::s_pCreateTennisBaseInterface = NULL;

I_TennisBase *TennisBaseHelper::CreateTennisBase(const char *szFileName) {
    if (s_pModule == NULL) {
		s_pModule = dlopen(szFileName, RTLD_LAZY);
		if (s_pModule == NULL) {
			Log_Error("Failed to call dlopen(), dlerror = 0x%08x", dlerror());
			return NULL;
		}
	}

    if (s_pCreateTennisBaseInterface == NULL) {
		*(void **)(&s_pCreateTennisBaseInterface) = dlsym(s_pModule, "createTennisBaseInterface");
		if (s_pCreateTennisBaseInterface == NULL) {
			Log_Error("Failed to call dlsym(), dlerror = 0x%08x", dlerror());
			return NULL;
		}
	}

	I_TennisBase *pTennisBase = s_pCreateTennisBaseInterface();
	if (pTennisBase == NULL) {
		Log_Error("Failed to call createTennisBaseInterface().");
		return NULL;
	}

	pTennisBase->addRef();

	return pTennisBase;
}

void TennisBaseHelper::DestroyTennisBase(I_TennisBase *pTennisBase) {
	unsigned int refCount = pTennisBase->releaseRef();
    if (refCount == 0) {
		s_pCreateTennisBaseInterface = NULL;

		dlclose(s_pModule);
		s_pModule = NULL;
	}
}