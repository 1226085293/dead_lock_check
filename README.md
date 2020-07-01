# dead_lock_check
死锁检测模块，可为您的项目在debug中检测死锁并处理，在不要求极致性能的情况下可以嵌入到release版本中。代码尽量精简易读

**使用代码，例：**
```cpp
struct test {
	CRITICAL_SECTION cri;
	test() {
		InitializeCriticalSection(&cri);
	}
	~test() {
		DeleteCriticalSection(&cri);
	}
};

int main() {
	debug::dead_lock_check d1;
	test a, b, c;
	// 双死锁
	/*std::thread t1([&] {
		raii::safe_critical r1(d1, &a.cri);
		Sleep(50);
		raii::safe_critical r2(d1, &b.cri);
		printf("线程结束: %ld\n", GetCurrentThreadId());
		}
	);
	std::thread t2([&] {
		raii::safe_critical r1(d1, &b.cri);
		Sleep(50);
		raii::safe_critical r2(d1, &a.cri);
		printf("线程结束: %ld\n", GetCurrentThreadId());
		}
	);/**/
	// 三死锁
	/*std::thread t1([&] {
		raii::safe_critical r1(d1, &a.cri);
		Sleep(50);
		raii::safe_critical r2(d1, &c.cri);
		printf("线程结束: %ld\n", GetCurrentThreadId());
		}
	);
	std::thread t2([&] {
		raii::safe_critical r1(d1, &b.cri);
		Sleep(50);
		raii::safe_critical r2(d1, &a.cri);
		printf("线程结束: %ld\n", GetCurrentThreadId());
		}
	);
	std::thread t3([&] {
		raii::safe_critical r1(d1, &c.cri);
		Sleep(50);
		raii::safe_critical r2(d1, &b.cri);
		printf("线程结束: %ld\n", GetCurrentThreadId());
		}
	);/**/
}
```

**如果要在release版本中避免死锁，请把safe_critical类的构造函数改为如下代码， 并且取消掉析构函数内的预处理宏判断**
```cpp
inline safe_critical::safe_critical(debug::dead_lock_check& check, CRITICAL_SECTION* cri_) :
	_check(check),
	_cri(cri_)
{
#ifdef _DEBUG
	_check.request(_cri);
	EnterCriticalSection(_cri);
	_check.have(_cri);
#else
	if (_check.request(_cri) {
		EnterCriticalSection(_cri);
		_check.have(_cri);
	}
#endif
}

```
