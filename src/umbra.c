#include "umbra.h"

static UCoreContext* u_lib_instance_ = NULL;

UCoreContextRef U_InitContext()
{
	if (u_lib_instance_ == NULL) {
		u_lib_instance_ = malloc(sizeof(UCoreContext) + 1);
	}

	return u_lib_instance_;
};

UMBRA_API UCoreContextRef U_GetInstance()
{
	return u_lib_instance_;
};

void U_DestroyContext()
{
	free(u_lib_instance_);
}