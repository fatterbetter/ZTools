#pragma once

//#include <mshtml.h>
#include <atlcomcli.h>
#include <atlexcept.h>
#include <dispex.h>

#pragma warning( disable : 4290 )

namespace TI
{
	//struct EVeryBadThing { };

	class CTypeLib
	{
		CComQIPtr<ITypeLib> _tl;
	public:
		operator ITypeLib*() { return _tl.p; }
		ITypeLib* operator->() { return _tl.p; }
		ITypeLib** operator&() { return &_tl; }

		CTypeLib() {}
		CTypeLib(const CTypeLib& other) : _tl(other._tl) {}
		CTypeLib(ITypeLib* tl) : _tl(tl) {}
		CTypeLib(LPCOLESTR file) { LoadTypeLibEx(file, REGKIND_NONE, &_tl); }
	};

	class CTypeInfo
	{
		CComQIPtr<ITypeInfo> _ti;
	public:
		operator ITypeInfo*() { return _ti.p; }
		ITypeInfo* operator->() { return _ti.p; }
		ITypeInfo** operator&() { return &_ti; }

		CTypeInfo() {}
		CTypeInfo(const CTypeInfo& other) : _ti(other._ti) {}
		CTypeInfo(ITypeInfo* ti) : _ti(ti) {}
		CTypeInfo(IDispatch* disp) //throw(EVeryBadThing)
		{
			if (disp)
				HRESULT hr = disp->GetTypeInfo(0,0,&_ti);
			//if(hr) throw EVeryBadThing();
		}
	};

	class CTypeAttr
	{
		TYPEATTR* _typeAttr;
		CTypeInfo _ti;
	public:
		operator TYPEATTR*() { return _typeAttr; }
		TYPEATTR* operator->() { return _typeAttr; }
		explicit CTypeAttr(ITypeInfo* ti) /*throw(EVeryBadThing)*/ : _ti(ti), _typeAttr(0)
		{
			if (_ti)
				HRESULT hr = _ti->GetTypeAttr(&_typeAttr);
			//if(hr) throw EVeryBadThing();
		}
		explicit CTypeAttr(IDispatch* disp) /*throw(EVeryBadThing)*/ : _ti(disp), _typeAttr(0)
		{
			if (_ti)
				HRESULT hr = _ti->GetTypeAttr(&_typeAttr);
			//if(hr) throw EVeryBadThing();
		}
		~CTypeAttr() { if (_typeAttr) _ti->ReleaseTypeAttr(_typeAttr); }
	};

	class CFuncDesc
	{
		FUNCDESC* _funcDesc;
		CTypeInfo _ti;
	public:
		operator FUNCDESC*() { return _funcDesc; }
		FUNCDESC* operator->() { return _funcDesc; }
		CFuncDesc(ITypeInfo* ti, int index) /*throw(EVeryBadThing)*/ : _ti(ti), _funcDesc(0)
		{
			if (_ti)
				HRESULT hr = _ti->GetFuncDesc(index, &_funcDesc);
			//if(hr) throw EVeryBadThing();
		}
		~CFuncDesc() { if (_funcDesc) _ti->ReleaseFuncDesc(_funcDesc); }
	};

	class CVarDesc
	{
		VARDESC* _varDesc;
		CTypeInfo _ti;
	public:
		operator VARDESC*() { return _varDesc; }
		VARDESC* operator->() { return _varDesc; }
		CVarDesc(ITypeInfo* ti, int index) /*throw(EVeryBadThing)*/ : _ti(ti), _varDesc(0)
		{
			if (_ti)
				HRESULT hr = _ti->GetVarDesc(index, &_varDesc);
			//if(hr) throw EVeryBadThing();
		}
		~CVarDesc() { if (_varDesc) _ti->ReleaseVarDesc(_varDesc); }
	};

	class CLibAttr
	{
		TLIBATTR* _libAttr;
		CTypeLib _tl;
	public:
		operator TLIBATTR*() { return _libAttr; }
		TLIBATTR* operator->() { return _libAttr; }
		explicit CLibAttr(ITypeLib* tlb) /*throw(EVeryBadThing)*/ : _tl(tlb), _libAttr(0)
		{
			if (_tl)
				HRESULT hr(_tl->GetLibAttr(&_libAttr));
			//if(hr) throw EVeryBadThing();
		}
		~CLibAttr() { if (_libAttr) _tl->ReleaseTLibAttr(_libAttr); }
	};

	static inline LPCOLESTR get_dispatch_type(IDispatch* disp)
	{
		static CComBSTR name;
		if (disp==NULL) return NULL;
		CTypeInfo ti(disp);
		name.Empty();
		if (ti) ti->GetDocumentation(-1, &name, NULL, NULL, NULL);
		return name;
	}

	static inline IID get_dispatch_iid(IDispatch* disp)
	{
		if (disp==NULL) return IID_NULL;
		CTypeAttr attr(disp);
		return attr ? attr->guid : IID_NULL;
	}
}

#pragma warning( default : 4290 )

class VAR2BSTR
{
	CComVariant _var;
public:
	explicit VAR2BSTR(const CComVariant& var)
	{
		_var.ChangeType(VT_BSTR, &var);
	}
	explicit VAR2BSTR(VARIANT* pvar)
	{
		_var.ChangeType(VT_BSTR, pvar);
	}
	operator BSTR() { return _var.vt==VT_BSTR?_var.bstrVal:L"Unknown Data"; }
};

namespace DispUtil
{
	static inline bool is_static_id(IDispatch* disp, DISPID did)
	{
		if (disp==NULL || did==DISPID_UNKNOWN)
			return false;

		TI::CTypeInfo ti(disp);
		if (ti)
		{
			TI::CTypeAttr attr(ti);
			if (attr)
			{
				for (WORD i=0; i<attr->cFuncs; i++)
				{
					TI::CFuncDesc fd(ti, i);
					if (fd->memid == did)
						return true;
				}
			}
		}
		return false;
	}

	//static inline bool is_named_item(IDispatch* disp, LPCOLESTR name)
	//{
	//	if (disp==NULL || name==NULL || name[0]==0)
	//		return false;

	//	CComQIPtr<IHTMLWindow2> win = disp;
	//	if (win == NULL)
	//		return false;

	//	CComVariant para = name;
	//	CComVariant result;
	//	HRESULT hr = win->item(&para, &result);
	//	if (SUCCEEDED(hr) && result.vt==VT_DISPATCH && result.pdispVal)
	//		return true;
	//	return false;

	//	//DISPID did = DISPID_UNKNOWN;
	//	//IID iid = TI::get_dispatch_iid(disp);
	//	//CComBSTR type = TI::get_dispatch_type(disp);
	//	//HRESULT hr = disp->GetIDsOfNames(IID_NULL, (LPOLESTR*)&L"item", 1, LOCALE_USER_DEFAULT, &did);
	//	//if (FAILED(hr) || did==DISPID_UNKNOWN)
	//	//	return false;

	//	//CComVariant para = name;
	//	//DISPPARAMS dp = {&para, NULL, 1, 0};
	//	//CComVariant result;
	//	//hr = disp->Invoke(did, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &result, NULL, NULL);
	//	//if (SUCCEEDED(hr) && result.vt==VT_DISPATCH)
	//	//	return true;
	//	//return false;
	//}
}

class CDispatch : public CComDispatchDriver
{
	HRESULT m_hr;
public:
	static CComVariant& vOptional()
	{
		static CComVariant vOpt;
		return vOpt;
	}

	CDispatch() : m_hr(S_OK) {}
	CDispatch(IDispatch* lp) : CComDispatchDriver(lp), m_hr(S_OK) {}
	CDispatch(const CDispatch& lp) : CComDispatchDriver(lp.p), m_hr(S_OK) {}
	CDispatch(const CComVariant& v) : m_hr(E_UNEXPECTED)
	{
		if (v.vt == VT_DISPATCH && v.pdispVal)
			m_hr = v.pdispVal->QueryInterface(IID_IDispatch, (void**)&p);
		else if (v.vt == VT_UNKNOWN && v.punkVal)
			m_hr = v.punkVal->QueryInterface(IID_IDispatch, (void**)&p);
	}

	HRESULT GetLastHResult() { return m_hr; }

	bool Has(LPCOLESTR lpsz)
	{
		DISPID dispid;
		m_hr=GetIDOfName(lpsz, &dispid);
#ifdef __dispex_h__
		if (FAILED(m_hr))
		{
			CComQIPtr<IDispatchEx> dispex(p);
			if (dispex.p)
			{
				DISPID did = DISPID_UNKNOWN;
				m_hr = dispex.p->GetDispID((BSTR)lpsz, fdexNameCaseSensitive, &did);
			}
		}
#endif // __dispex_h__
		return SUCCEEDED(m_hr);
	}
	CComVariant Get(LPCOLESTR lpsz)
	{
		CComVariant v;
		ATLASSERT(p);
		if (p==NULL) return v;
		m_hr = GetPropertyByName(lpsz, &v);
#ifdef __dispex_h__
		if (FAILED(m_hr))
		{
			CComQIPtr<IDispatchEx> dispex(p);
			if (dispex.p)
			{
				DISPID did = DISPID_UNKNOWN;
				m_hr = dispex.p->GetDispID((BSTR)lpsz, fdexNameCaseSensitive, &did);
				if (SUCCEEDED(m_hr) && did!=DISPID_UNKNOWN)
					m_hr = GetProperty(did, &v);
			}
		}
#endif // __dispex_h__
		ATLASSERT(SUCCEEDED(m_hr));
		//if (FAILED(hr)) AtlThrow(hr);
		return v;
	}
	CComVariant Get(DISPID dispid)
	{
		CComVariant v;
		ATLASSERT(p);
		if (p==NULL) return v;
		m_hr = GetProperty(dispid, &v);
		ATLASSERT(SUCCEEDED(m_hr));
		//if (FAILED(hr)) AtlThrow(hr);
		return v;
	}
	CDispatch GetPtr(LPCOLESTR lpsz)
	{
		return Get(lpsz);
	}
	CDispatch GetPtr(DISPID dispid)
	{
		return Get(dispid);
	}
#ifdef __dispex_h__
	CComVariant GetEx(LPCOLESTR lpsz)
	{
		ATLASSERT(p);
		if (p==NULL) return CComVariant();
		CComVariant v;
		m_hr = GetPropertyByName(lpsz, &v);
		if (SUCCEEDED(m_hr)) return v;

		CComQIPtr<IDispatchEx> dispex(p);
		if (dispex.p)
		{
			DISPID did = DISPID_UNKNOWN;
			m_hr = dispex.p->GetDispID((BSTR)lpsz, fdexNameCaseSensitive|fdexNameEnsure, &did);
		}
		ATLASSERT(SUCCEEDED(m_hr));
		//if (FAILED(hr)) AtlThrow(hr);
		return CComVariant();
	}
	CDispatch GetExPtr(LPCOLESTR lpsz)
	{
		return Get(lpsz);
	}
#endif // __dispex_h__

	void Put(LPCOLESTR lpsz, CComVariant v)
	{
		ATLASSERT(p);
		if (p==NULL) return;
		m_hr = PutPropertyByName(lpsz, &v);
#ifdef __dispex_h__
		if (FAILED(m_hr))
		{
			CComQIPtr<IDispatchEx> dispex(p);
			if (dispex.p)
			{
				DISPID did = DISPID_UNKNOWN;
				m_hr = dispex.p->GetDispID((BSTR)lpsz, fdexNameCaseSensitive|fdexNameEnsure, &did);
				if (SUCCEEDED(m_hr) && did!=DISPID_UNKNOWN)
					m_hr = PutProperty(did, &v);
			}
		}
#endif // __dispex_h__
		ATLASSERT(SUCCEEDED(m_hr));
		//if (FAILED(hr)) AtlThrow(hr);
	}
	void Put(DISPID dispid, CComVariant v)
	{
		ATLASSERT(p);
		if (p==NULL) return;
		m_hr = PutProperty(dispid, &v);
		ATLASSERT(SUCCEEDED(m_hr));
		//if (FAILED(hr)) AtlThrow(hr);
	}

	CComVariant Invoke(LPCOLESTR lpsz, CComVariant param1=vOptional(), CComVariant param2=vOptional(), CComVariant param3=vOptional(),
										CComVariant param4=vOptional(), CComVariant param5=vOptional(), CComVariant param6=vOptional())
	{
		CComVariant v;
		ATLASSERT(p);
		if (p==NULL) return v;
		CComVariant pm[6] = {param6, param5, param4, param3, param2, param1};
		int nOptCnt = 0;
		if (param6.vt == VT_EMPTY) nOptCnt++;
		if (param5.vt == VT_EMPTY) nOptCnt++;
		if (param4.vt == VT_EMPTY) nOptCnt++;
		if (param3.vt == VT_EMPTY) nOptCnt++;
		if (param2.vt == VT_EMPTY) nOptCnt++;
		if (param1.vt == VT_EMPTY) nOptCnt++;
		m_hr = __super::InvokeN(lpsz, nOptCnt==6?NULL:&pm[nOptCnt], 6-nOptCnt, &v);
		ATLASSERT(SUCCEEDED(m_hr));
		//if (FAILED(hr)) AtlThrow(hr);
		return v;
	}
	CComVariant Invoke(DISPID dispid, CComVariant param1=vOptional(), CComVariant param2=vOptional(), CComVariant param3=vOptional(),
										CComVariant param4=vOptional(), CComVariant param5=vOptional(), CComVariant param6=vOptional())
	{
		CComVariant v;
		ATLASSERT(p);
		if (p==NULL) return v;
		CComVariant pm[6] = {param6, param5, param4, param3, param2, param1};
		int nOptCnt = 0;
		if (param6.vt == VT_EMPTY) nOptCnt++;
		if (param5.vt == VT_EMPTY) nOptCnt++;
		if (param4.vt == VT_EMPTY) nOptCnt++;
		if (param3.vt == VT_EMPTY) nOptCnt++;
		if (param2.vt == VT_EMPTY) nOptCnt++;
		if (param1.vt == VT_EMPTY) nOptCnt++;
		m_hr = __super::InvokeN(dispid, nOptCnt==6?NULL:&pm[nOptCnt], 6-nOptCnt, &v);
		ATLASSERT(SUCCEEDED(m_hr));
		//if (FAILED(hr)) AtlThrow(hr);
		return v;
	}
	CDispatch InvokePtr(LPCOLESTR lpsz, CComVariant param1=vOptional(), CComVariant param2=vOptional(), CComVariant param3=vOptional(),
								CComVariant param4=vOptional(), CComVariant param5=vOptional(), CComVariant param6=vOptional())
	{
		return Invoke(lpsz, param1, param2, param3, param4, param5, param6);
	}
	CDispatch InvokePtr(DISPID dispid, CComVariant param1=vOptional(), CComVariant param2=vOptional(), CComVariant param3=vOptional(),
								CComVariant param4=vOptional(), CComVariant param5=vOptional(), CComVariant param6=vOptional())
	{
		return Invoke(dispid, param1, param2, param3, param4, param5, param6);
	}
	CComVariant InvokeCallback(CComVariant param1=vOptional(), CComVariant param2=vOptional(), CComVariant param3=vOptional(),
							CComVariant param4=vOptional(), CComVariant param5=vOptional(), CComVariant param6=vOptional())
	{
		return Invoke((DISPID)DISPID_VALUE, param1, param2, param3, param4, param5, param6);
	}
	CDispatch InvokeCallbackPtr(CComVariant param1=vOptional(), CComVariant param2=vOptional(), CComVariant param3=vOptional(),
							CComVariant param4=vOptional(), CComVariant param5=vOptional(), CComVariant param6=vOptional())
	{
		return InvokeCallback(param1, param2, param3, param4, param5, param6);
	}
	CDispatch Construct(DISPID dispid, CComVariant param1=vOptional(), CComVariant param2=vOptional(), CComVariant param3=vOptional(),
							CComVariant param4=vOptional(), CComVariant param5=vOptional(), CComVariant param6=vOptional())
	{
		CComVariant v;
		ATLASSERT(p);
		if (p==NULL) return v;
		CComVariant pm[6] = {param6, param5, param4, param3, param2, param1};
		int nOptCnt = 0;
		if (param6.vt == VT_EMPTY) nOptCnt++;
		if (param5.vt == VT_EMPTY) nOptCnt++;
		if (param4.vt == VT_EMPTY) nOptCnt++;
		if (param3.vt == VT_EMPTY) nOptCnt++;
		if (param2.vt == VT_EMPTY) nOptCnt++;
		if (param1.vt == VT_EMPTY) nOptCnt++;
		DISPPARAMS dispparams = { nOptCnt==6?NULL:&pm[nOptCnt], NULL, 6-nOptCnt, 0};
		m_hr = p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_CONSTRUCT, &dispparams, &v, NULL, NULL);
		ATLASSERT(SUCCEEDED(m_hr));
		return v;
	}
	CDispatch Construct(LPCOLESTR lpsz, CComVariant param1=vOptional(), CComVariant param2=vOptional(), CComVariant param3=vOptional(),
							CComVariant param4=vOptional(), CComVariant param5=vOptional(), CComVariant param6=vOptional())
	{
		CComVariant v;
		ATLASSERT(p);
		if (p==NULL) return v;
		DISPID dispid;
		if (SUCCEEDED(m_hr=GetIDOfName(lpsz, &dispid)))
			return Construct(dispid, param1, param2, param3, param4, param5, param6);
		return v;
	}
};