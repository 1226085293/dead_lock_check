#include "../dead_lock_check.h"

namespace debug {
	dead_lock_check::lock_status dead_lock_check::status(DWORD id_, std::unordered_map<DWORD, std::vector<CRITICAL_SECTION*>>& thread_lock_) {
		// ɾ�������߳���Ϣ, �����ظ�����
		thread_lock_.erase(id_);
		for (auto& i : thread_lock_) {
			// �ҵ������̵߳���ռ����
			for (auto j : i.second) {
				// �ѱ������߳�ռ��
				if (j == _request_lock[id_]) {
					// ռ���߳���������, ��ǰ�̵߳ȴ���
					if (_request_lock.end() == _request_lock.find(i.first)) {
						return lock_status::wait;
					}
					// ռ���߳������� = ��ǰ�߳�ռ����, �γɻػ�, �������
					for (auto& w : _thread_lock[GetCurrentThreadId()]) {
						if (w == _request_lock[i.first]) {
							return lock_status::dead;
						}
					}
					// ռ���߳���������, ��������
					return status(i.first, thread_lock_);
				}
			}
		}
		return lock_status::have;
	}

	bool dead_lock_check::request(CRITICAL_SECTION* cri_) {
		EnterCriticalSection(&_cri);
		auto id = GetCurrentThreadId();
		// ������Ϣ
		_request_lock[id] = cri_;
		// �������
		auto thread_lock = _thread_lock;
		auto type = status(id, thread_lock);
		LeaveCriticalSection(&_cri);
		switch (type) {
		case lock_status::have: {
			return true;
		}
		case lock_status::wait: {
			//printf("��ǰ�߳�: %ld, ռ���� %I64d ʧ��, �ȴ�\n", GetCurrentThreadId(), reinterpret_cast<uintptr_t>(cri_));
			return true;
		} break;
		case lock_status::dead: {
			printf("��ǰ�߳�: %ld, ռ���� %I64d ʧ��, ����\n", GetCurrentThreadId(), reinterpret_cast<uintptr_t>(cri_));
			return false;
		} break;
		default: {
		} break;
		}
		return true;
	}
}