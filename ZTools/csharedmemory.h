#ifndef _SHAREDMEMORY_H
#define	_SHAREDMEMORY_H
//Author:Michael Schikora
//       www.schikos.de
//       schiko@schikos.de
//
//How to use:
//
//CSharedMemory m_sm;
//..
//m_sm.Init("MyScharedMemory",256/*bytes*/);
//..
//char* pSharedString = (char*)m_sm.GetData()
//..
//{
// CSharedMemory::Locker locker(m_sm);
// ModifySomthing(pSharedString);
//}

static char cstrSMName[] = "SharedMemory";
class CSharedMemory
{
protected:
	HANDLE	m_hSharedMemoryFile;
// Mu?z.Z. public bleiben,
// Da Dynamark1 und ?? direkt auf m_pwData zugreifen.
public:
	void *	m_pwData;
protected:
	DWORD	dwMaximumSizeHigh; 
	DWORD	dwMaximumSizeLow;
	DWORD	dwNumberOfBytesToMap;
	BOOL	m_bInit;
	BOOL	m_bAlreadyExist;

   CString csMutexName;
   HANDLE m_hMutex;
// Mu?z.Z. public bleiben,
// Da Dynamark1 und ?? direkt auf csName zugreifen.
public:
// Im Sytem eindeutiger Name für gemeinsamen Speicherbereich
   CString csName; 
public:
// Pointer auf gemeinsamen Speicher
	void* GetData()
	{
		if( m_bInit )
			return m_pwData;
		else
			return NULL;
	}
	//Constr ohne Init
	CSharedMemory(){csName = cstrSMName;m_bInit = FALSE;m_bAlreadyExist = FALSE;csMutexName = csName + _T("Mutex");}
	CSharedMemory(LPCSTR cs){csName = cs;m_bInit = FALSE;m_bAlreadyExist = FALSE;csMutexName = csName + _T("Mutex");}
	//Constr mit Init
	CSharedMemory(int size){csName = cstrSMName;m_bAlreadyExist = FALSE;csMutexName = csName + _T("Mutex");Init(size);}
	CSharedMemory(LPCSTR cs, int size){csName = cs;m_bAlreadyExist = FALSE;csMutexName = csName + _T("Mutex"); Init(size);}
	BOOL Init(LPCSTR cs, int size){csName = cs;m_bAlreadyExist = FALSE;csMutexName = csName + _T("Mutex"); return Init(size);}
	BOOL Init(int size ,LPCSTR cs){csName = cs;m_bAlreadyExist = FALSE;csMutexName = csName + _T("Mutex"); return Init(size);}
	BOOL Init( int size )
	{
      m_hMutex = CreateMutex(NULL,FALSE,csMutexName);
		dwNumberOfBytesToMap = size;
		m_hSharedMemoryFile = CreateFileMapping((HANDLE)0xFFFFFFFF,
										NULL,
										PAGE_READWRITE,
										0/*dwMaximumSizeHigh*/,
										dwNumberOfBytesToMap/*dwMaximumSizeLow*/,
										csName.GetBuffer(0));
		if(m_hSharedMemoryFile == NULL)
		{
			m_bAlreadyExist = FALSE;
			m_bInit = FALSE;
			return FALSE;
		}
		else
		{
       if(GetLastError() == ERROR_ALREADY_EXISTS)
          m_bAlreadyExist = TRUE;
		}
		m_pwData = MapViewOfFile(m_hSharedMemoryFile,
								FILE_MAP_WRITE,
								0/*dwFileOffsetHigh*/,
								0/*dwFileOffsetLow*/,
								dwNumberOfBytesToMap);
		if(m_pwData == NULL)
		{
			m_bInit = FALSE;
			CloseHandle(m_hSharedMemoryFile);
         return FALSE;
		}
		else
			m_bInit = TRUE;
      return TRUE;
	}
	~CSharedMemory()
	{
		if (m_bInit)
		{
			UnmapViewOfFile(m_pwData);
			CloseHandle(m_hSharedMemoryFile);
		}
	}
public:
   //return:TRUE if it is not the first Instance
   BOOL AlreadyExist()
   {
      return m_bAlreadyExist;
   }
   //Locks the Mem like CSingelLock locker(TRUE,pSyncObj)
   // Use:
   // {
   //  CSharedMemory::Locker locker(&MySharedMem);
   //  Modify(MySharedMem->GetData() );
   // }
   struct Locker
   {
      Locker(CSharedMemory* sm)
      {
         m_sm = sm;
         m_sm->Lock();
      }
      Locker(CSharedMemory& sm)
      {
         m_sm = &sm;
         m_sm->Lock();
      }
      ~Locker()
      {
         m_sm->Unlock();
      }
      CSharedMemory* m_sm;
   };
   //Locks the Mem 
   // Use:
   // if( MySharedMem.Lock(100) )
   // {
   //    Modify(MySharedMem->GetData() );
   //    MySharedMem.Unlock();
   // }
   BOOL Lock(DWORD dwMilliSec = INFINITE)
   {
      if( WaitForSingleObject(m_hMutex, dwMilliSec) == WAIT_OBJECT_0)
         return TRUE;
      return FALSE;
   }
   BOOL Unlock(/*DWORD dwMilliSec = INFINITE*/)
   {
      return ReleaseMutex(m_hMutex);
   }
   
};
#endif