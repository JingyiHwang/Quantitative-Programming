// handle.h - Handles to C++ objects
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
//#include <memory>
#include <set>
#include <windows.h>
#include "XLCALL.H"

using HANDLEX = double;

namespace xll {


	// HANDLEX that defaults to NaN
	class handlex {
		HANDLEX h_;
	public:
		handlex()
			: h_(std::numeric_limits<double>::quiet_NaN())
		{ }
		operator HANDLEX()
		{
			return h_;
		}
		HANDLEX operator=(HANDLEX h)
		{
			return h_ = h;
		}
	};

	// Pointers to objects encoded as doubles.
	// Assumes high 32 bits are constant.
	template<class T>
	class handle {
		static std::set<T*>& handles()
		{
			static std::set<T*> handles_;

			return handles_;
		}
		static void gc()
		{
			for (auto& h : handles())
				delete h;
		}
		struct XLOPER12 : public ::XLOPER12
		{
			XLOPER12()
			{
				this->xltype = xltypeNil;
			}
			XLOPER12(const XLOPER12&) = delete;
			XLOPER12& operator=(const XLOPER12&) = delete;
			~XLOPER12()
			{
				Excel12(xlFree, 0, 1, this);
			}
		};
		static bool insert(T* p)
		{
			auto hs{handles()};
			auto coerce = Excel(xlCoerce, Excel(xlfCaller));
			
			if (coerce.xltype == xltypeNum && coerce.val.num != 0)
			{
				handle h(coerce.val.num);
				auto i = hs.find(h.ptr());
				if (i != hs.end()) {
					delete h.ptr();
					hs.erase(i);
				}
			}

			return hs.insert(p).second;
		}
	public:
		static int32_t& i1()
		{
			static int32_t i1_ = 0;
			
			return i1_; // high order bits
		}
		T* pt;
		handle(T* pt)
			: pt(pt)
		{
			union {
				T* p;
				int32_t i[2];
			};

#ifdef _DEBUG
			ensure (i1() == 0 || i1() == i[1]);
#endif
			p = pt;
			i1() = i[1];
			insert(pt);
		}
		handle(HANDLEX h)
		{
			union {
				T* p;
				int32_t i[2];
			};

			i[0] = static_cast<int32_t>(h);
			i[1] = i1();
			pt = p;
		}
		handle(const handle&) = delete;
		handle& operator=(const handle&) = delete;
		~handle()
		{ }
		HANDLEX get() const
		{
			union {
				T* p;
				int32_t i[2];
			};

			p = pt;
			
			return i[0];
		}
		operator HANDLEX()
		{
			return get();
		}
		T& operator*()
		{
			return *pt;
		}
		T* operator->()
		{
			return pt;
		}
		T* ptr()
		{
			return pt;
		}
		/*		operator T*()
		{
			return ptr();
		}
		const operator T*() const
		{
			return ptr();
		}
*/	};
	template<class  T>
	inline HANDLEX p2h(T* pt)
	{
		union {
			T* p;
			int32_t i[2];
		};

		p = pt;
		handle<T>::i1() = i[1];

		return i[0];
	}
	template<class  T>
	inline T* h2p(HANDLEX h)
	{
		union {
			T* p;
			int32_t i[2];
		};

		i[0] = static_cast<int32_t>(h);
		i[1] = handle<T>::i1();

		return p;
	}

} // xll namespace
