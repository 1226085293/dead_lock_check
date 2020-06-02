#pragma once
#include <unordered_map>
#include <Windows.h>

namespace debug {
	class dead_lock_check {
		enum class lock_status {
			null,
			have,	//占有
			wait,	//等待
			dead,	//死锁
		};
	private:
		CRITICAL_SECTION											_cri;			//临界区
		std::unordered_map<DWORD, std::vector<CRITICAL_SECTION*>>	_thread_lock;	//线程锁
		std::unordered_map<DWORD, CRITICAL_SECTION*>				_request_lock;	//请求锁

		// 锁状态
		lock_status status(DWORD id_, std::unordered_map<DWORD, std::vector<CRITICAL_SECTION*>>& thread_lock_);
	public:
		dead_lock_check();
		~dead_lock_check();

		// 请求锁
		bool request(CRITICAL_SECTION* cri_);
		// 占有锁
		void have(CRITICAL_SECTION* cri_);
		// 丢弃锁
		void discard(CRITICAL_SECTION* cri_);
	};

#include "source/dead_lock_check.icc"
}