#include "../dead_lock_check.h"

namespace debug {
	dead_lock_check::lock_status dead_lock_check::status(DWORD id_, std::unordered_map<DWORD, std::vector<CRITICAL_SECTION*>>& thread_lock_) {
		// 删除遍历线程信息, 避免重复遍历
		thread_lock_.erase(id_);
		for (auto& i : thread_lock_) {
			// 找到其他线程的已占有锁
			for (auto j : i.second) {
				// 已被其他线程占有
				if (j == _request_lock[id_]) {
					// 占有线程无请求锁, 当前线程等待锁
					if (_request_lock.end() == _request_lock.find(i.first)) {
						return lock_status::wait;
					}
					// 占有线程请求锁 = 当前线程占有锁, 形成回环, 造成死锁
					for (auto& w : _thread_lock[GetCurrentThreadId()]) {
						if (w == _request_lock[i.first]) {
							return lock_status::dead;
						}
					}
					// 占有线程有请求锁, 继续遍历
					return status(i.first, thread_lock_);
				}
			}
		}
		return lock_status::have;
	}

	bool dead_lock_check::request(CRITICAL_SECTION* cri_) {
		EnterCriticalSection(&_cri);
		auto id = GetCurrentThreadId();
		// 更新信息
		_request_lock[id] = cri_;
		// 检测死锁
		auto thread_lock = _thread_lock;
		auto type = status(id, thread_lock);
		LeaveCriticalSection(&_cri);
		switch (type) {
		case lock_status::have: {
			return true;
		}
		case lock_status::wait: {
			//printf("当前线程: %ld, 占有锁 %I64d 失败, 等待\n", GetCurrentThreadId(), reinterpret_cast<uintptr_t>(cri_));
			return true;
		} break;
		case lock_status::dead: {
			printf("当前线程: %ld, 占有锁 %I64d 失败, 死锁\n", GetCurrentThreadId(), reinterpret_cast<uintptr_t>(cri_));
			return false;
		} break;
		default: {
		} break;
		}
		return true;
	}
}