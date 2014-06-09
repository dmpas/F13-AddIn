
#include "stdafx.h"

#include <stdio.h>
#include <wchar.h>
#include "F13-AddIn.h"
#include <string>
#include <fstream>

static const wchar_t g_kClassNames[] = L"CAddInF13"; //"|OtherClass1|OtherClass2";
static IAddInDefBase *pAsyncEvent = NULL;

uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len = 0);
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len = 0);
uint32_t getLenShortWcharStr(const WCHAR_T* Source);
char* ConvToUtf8(const WCHAR_T *W);


//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
    if(!*pInterface)
    {
        *pInterface= new CAddInF13;
        return (long)*pInterface;
    }
    return 0;
}
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pIntf)
{
   if(!*pIntf)
      return -1;

   delete *pIntf;
   *pIntf = 0;
   return 0;
}
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
    static WCHAR_T* names = 0;
    if (!names)
        ::convToShortWchar(&names, g_kClassNames);
    return names;
}
//---------------------------------------------------------------------------//

// CAddInF13
//---------------------------------------------------------------------------//
CAddInF13::CAddInF13()
{
    m_iMemory = 0;
    m_iConnect = 0;
	hPrinter = 0;
}
//---------------------------------------------------------------------------//
CAddInF13::~CAddInF13()
{

}
//---------------------------------------------------------------------------//
HHOOK KeyBoardHook;
IAddInDefBase      *m_iConnectGlobal = NULL;
//---------------------------------------------------------------------------//
LRESULT CALLBACK LowLevelKeyboardProc(
  _In_  int nCode,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
)
{
	if (m_iConnectGlobal) {
		if (wParam = WM_KEYDOWN) {

			LPKBDLLHOOKSTRUCT data = (LPKBDLLHOOKSTRUCT)lParam;
			if (data->vkCode >= VK_F13 && data->vkCode <= VK_F24) {

				wchar_t wdata[10];
				wsprintf(wdata, L"F%d", (data->vkCode - VK_F1 + 1));

				m_iConnectGlobal->ExternalEvent(L"F13", L"FunctionalKeyDown", wdata);

				return 1;
			}
		}
	}

	return CallNextHookEx(KeyBoardHook, nCode, wParam, lParam);
}
//---------------------------------------------------------------------------//
void CAddInF13::StartHook()
{
	KeyBoardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
}
//---------------------------------------------------------------------------//
void CAddInF13::StopHook()
{
	UnhookWindowsHookEx(KeyBoardHook);
}
//---------------------------------------------------------------------------//
bool CAddInF13::Init(void* pConnection)
{ 
    m_iConnect = (IAddInDefBase*)pConnection;
	m_iConnectGlobal = m_iConnect;

	StartHook();

    return m_iConnect != NULL;
}
//---------------------------------------------------------------------------//
long CAddInF13::GetInfo()
{ 
    // Component should put supported component technology version 
    // This component supports 2.0 version
    return 2000; 
}
//---------------------------------------------------------------------------//
void CAddInF13::Done()
{
	StopHook();
}
/////////////////////////////////////////////////////////////////////////////
// ILanguageExtenderBase
//---------------------------------------------------------------------------//
bool CAddInF13::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{ 
    wchar_t *wsExtension = L"F13";
    int iActualSize = ::wcslen(wsExtension) + 1;
    WCHAR_T* dest = 0;

    if (m_iMemory)
    {
        if(m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
            ::convToShortWchar(wsExtensionName, wsExtension, iActualSize);
        return true;
    }

    return false; 
}
//---------------------------------------------------------------------------//
long CAddInF13::GetNProps()
{ 
    // You may delete next lines and add your own implementation code here
    return eProp_Last;
}
//---------------------------------------------------------------------------//
long CAddInF13::FindProp(const WCHAR_T* wsPropName)
{ 
    return -1;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInF13::GetPropName(long lPropNum, long lPropAlias)
{ 
	return NULL;
}
//---------------------------------------------------------------------------//
bool CAddInF13::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{ 
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInF13::SetPropVal(const long lPropNum, tVariant *varPropVal)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool CAddInF13::IsPropReadable(const long lPropNum)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool CAddInF13::IsPropWritable(const long lPropNum)
{
    return false;
}
//---------------------------------------------------------------------------//
long CAddInF13::GetNMethods()
{ 
    return eMeth_Last;
}
//---------------------------------------------------------------------------//
long CAddInF13::FindMethod(const WCHAR_T* wsMethodName)
{ 
    return -1;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInF13::GetMethodName(const long lMethodNum, const long lMethodAlias)
{ 
	return NULL;
}
//---------------------------------------------------------------------------//
long CAddInF13::GetNParams(const long lMethodNum)
{ 
    return 0;
}
//---------------------------------------------------------------------------//
bool CAddInF13::GetParamDefValue(const long lMethodNum, const long lParamNum,
                          tVariant *pvarParamDefValue)
{ 
    TV_VT(pvarParamDefValue)= VTYPE_EMPTY;
    return false;
} 
//---------------------------------------------------------------------------//
bool CAddInF13::HasRetVal(const long lMethodNum)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool CAddInF13::CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray)
{ 
        return false;
}
//---------------------------------------------------------------------------//
bool CAddInF13::CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{ 
	return false;
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
void CAddInF13::SetLocale(const WCHAR_T* loc)
{
#ifndef __linux__
    _wsetlocale(LC_ALL, loc);
#else
    //We convert in char* char_locale
    //also we establish locale
    //setlocale(LC_ALL, char_locale);
#endif
}
/////////////////////////////////////////////////////////////////////////////
// LocaleBase
//---------------------------------------------------------------------------//
bool CAddInF13::setMemManager(void* mem)
{
    m_iMemory = (IMemoryManager*)mem;
    return m_iMemory != 0;
}
//---------------------------------------------------------------------------//
void CAddInF13::addError(uint32_t wcode, const wchar_t* source, 
                        const wchar_t* descriptor, long code)
{
    if (m_iConnect)
    {
        WCHAR_T *err = 0;
        WCHAR_T *descr = 0;
        
        ::convToShortWchar(&err, source);
        ::convToShortWchar(&descr, descriptor);

        m_iConnect->AddError(wcode, err, descr, code);
        delete[] err;
        delete[] descr;
    }
}
//---------------------------------------------------------------------------//
long CAddInF13::findName(wchar_t* names[], const wchar_t* name, 
                         const uint32_t size) const
{
    long ret = -1;
    for (uint32_t i = 0; i < size; i++)
    {
        if (!wcscmp(names[i], name))
        {
            ret = i;
            break;
        }
    }
    return ret;
}
//---------------------------------------------------------------------------//
uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len)
{
    if (!len)
        len = ::wcslen(Source)+1;

    if (!*Dest)
        *Dest = new WCHAR_T[len];

    WCHAR_T* tmpShort = *Dest;
    wchar_t* tmpWChar = (wchar_t*) Source;
    uint32_t res = 0;

    ::memset(*Dest, 0, len*sizeof(WCHAR_T));
    do
    {
        *tmpShort++ = (WCHAR_T)*tmpWChar++;
        ++res;
    }
    while (len-- && *tmpWChar);

    return res;
}
//---------------------------------------------------------------------------//
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len)
{
    if (!len)
        len = getLenShortWcharStr(Source)+1;

    if (!*Dest)
        *Dest = new wchar_t[len];

    wchar_t* tmpWChar = *Dest;
    WCHAR_T* tmpShort = (WCHAR_T*)Source;
    uint32_t res = 0;

    ::memset(*Dest, 0, len*sizeof(wchar_t));
    do
    {
        *tmpWChar++ = (wchar_t)*tmpShort++;
        ++res;
    }
    while (len-- && *tmpShort);

    return res;
}
//---------------------------------------------------------------------------//
uint32_t getLenShortWcharStr(const WCHAR_T* Source)
{
    uint32_t res = 0;
    WCHAR_T *tmpShort = (WCHAR_T*)Source;

    while (*tmpShort++)
        ++res;

    return res;
}
