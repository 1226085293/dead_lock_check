# dead_lock_check
死锁检测模块，可为您的项目在debug中检测死锁并处理，在不要求极致性能的情况下甚至可以嵌入到release版本中。死锁发生及时处理

**如果要在release版本中避免死锁，请把safe_critical类的构造函数改为如下代码， 并且取消掉析构函数内的预处理宏判断**
```cpp
inline safe_critical::safe_critical(debug::dead_lock_check& check, CRITICAL_SECTION* cri_) :
#ifdef _DEBUG
	_check(check),
#endif
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


如果对您有帮助请给个星星，谢谢。

If it helps you, please give a star, thank you.
