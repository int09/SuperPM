#ifndef ModuleToo_h__
#define ModuleToo_h__
#include <TLHELP32.H>


class CModuleTool
{
public:
	CModuleTool();
	virtual ~CModuleTool(); 

	BOOL CreateSnapshotThread(DWORD dwPID);

	BOOL Module32First(LPMODULEENTRY32 ModuleData);
	BOOL Module32Next(LPMODULEENTRY32 ModuleData);
private:
	HANDLE m_hSnapshot;
};

#endif // ModuleToo_h__