#pragma once
#include <unordered_map>
#include <Windows.h>

namespace debug {
	class dead_lock_check {
		enum class lock_status {
			null,
			have,	//ռ��
			wait,	//�ȴ�
			dead,	//����
		};
	private:
		CRITICAL_SECTION											_cri;			//�ٽ���
		std::unordered_map<DWORD, std::vector<CRITICAL_SECTION*>>	_thread_lock;	//�߳���
		std::unordered_map<DWORD, CRITICAL_SECTION*>				_request_lock;	//������

		// ��״̬
		lock_status status(DWORD id_, std::unordered_map<DWORD, std::vector<CRITICAL_SECTION*>>& thread_lock_);
	public:
		dead_lock_check();
		~dead_lock_check();

		// ������
		bool request(CRITICAL_SECTION* cri_);
		// ռ����
		void have(CRITICAL_SECTION* cri_);
		// ������
		void discard(CRITICAL_SECTION* cri_);
	};

#include "source/dead_lock_check.icc"
}