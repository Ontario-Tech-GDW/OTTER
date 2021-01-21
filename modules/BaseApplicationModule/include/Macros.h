#pragma once

#include <memory>

#define SMART_MEMORY_MANAGED(ClassName) public: \
	typedef std::shared_ptr<ClassName> sptr; \
	typedef std::shared_ptr<ClassName> uptr; \
	template <typename ... TArgs> \
	static ClassName::sptr Create(TArgs&&... args) { \
		return std::make_shared<ClassName>(std::forward<TArgs>(args)...); \
	} \
	ClassName(const ClassName& other) = delete; \
	ClassName(ClassName&& other) = delete; \
	ClassName& operator=(const ClassName& other) = delete; \
	ClassName& operator=(ClassName&& other) = delete;