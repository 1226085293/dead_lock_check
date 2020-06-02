#pragma once
#include <Windows.h>
#include "other/debug/dead_lock_check.h"

namespace raii {
	// 临界区
	class critical {
	private:
		CRITICAL_SECTION* _crit;
	public:
		critical(CRITICAL_SECTION* crit);
		critical(const critical&) = delete;
		critical& operator =(const critical&) = delete;
		~critical();
	};

	// 安全临界区
	class safe_critical {
	private:
		CRITICAL_SECTION* _cri;
		debug::dead_lock_check& _check;
	public:
		safe_critical(debug::dead_lock_check& check, CRITICAL_SECTION* cri_);
		safe_critical(const critical&) = delete;
		safe_critical& operator =(const critical&) = delete;
		~safe_critical();
	};

	class safe_critical1 {
	private:
		CRITICAL_SECTION* _cri;
		debug::dead_lock_check* _check;
	};

#include "source/critical.icc"
}