#include "Backup.h"

CBackup* CBackup::__instance = NULL;

CBackup* CBackup::GetInstance()
{
	if (__instance == NULL) __instance = new CBackup();
	return __instance;
}