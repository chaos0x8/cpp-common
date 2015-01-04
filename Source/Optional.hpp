#pragma once
#include <string>
#include <functional>
#include <Exceptions/UninitializedOptionalError.hpp>

namespace Common
{
namespace Detail
{

class None
{
};

}

static const Detail::None none;

template <typename T>
class Optional
{
public:
    Optional()
        : _initialized(false),
          _value(T())
    {
    }
    Optional(const Detail::None&)
        : _initialized(false),
            _value(T())
    {
    }
    Optional(const Optional<T>& other)
    {
        operator = (other);
    }
    Optional(const T& _value)
    {
        operator = (_value);
    }
    Optional(Optional&& other)
    {
        operator = (std::move(other));
    }

    Optional<T>& operator = (const Detail::None&)
    {
        this->_initialized = false;
        this->_value = T();
        return *this;
    }
    Optional<T>& operator = (const Optional<T>& other)
    {
        this->_initialized = other._initialized;
        this->_value = other._value;
        return *this;
    }
    Optional<T>& operator = (const T& _value)
    {
        this->_initialized = true;
        this->_value = _value;
        return *this;
    }
    Optional<T>& operator = (Optional<T>&& other)
    {
        this->_initialized = std::move(other._initialized);
        this->_value = std::move(other._value);
        return *this;
    }

    T* operator -> ()
    {
        if (!_initialized)
            throw Exceptions::UninitializedOptionalError();
        return &_value;
    }
    const T* operator -> () const
    {
        if (!_initialized)
            throw Exceptions::UninitializedOptionalError();
        return &_value;
    }

    T& operator * ()
    {
        if (!_initialized)
            throw Exceptions::UninitializedOptionalError();
        return _value;
    }
    const T& operator * () const
    {
        if (!_initialized)
            throw Exceptions::UninitializedOptionalError();
        return _value;
    }

    T value() const
    {
        if (!_initialized)
            throw Exceptions::UninitializedOptionalError();
        return _value;
    }

    T value_or(const T& _default) const
    {
        if (!_initialized)
            return _default;
        return _value;
    }

    bool initialized() const
    {
        return _initialized;
    }

    bool operator == (const Optional<T>& other) const
    {
        if (!this->_initialized)
            return this->_initialized == other._initialized;
        return this->_initialized == other._initialized && this->_value == other._value;
    }
    bool operator == (const T& _value) const
    {
        if (!this->_initialized)
            return false;
        return this->_value == _value;
    }

private:
    bool _initialized;
    T _value;
};

template <typename T>
Optional<T> makeOptional(const T& _value)
{
    return Optional<T>(_value);
}

Optional<std::string> makeOptional(const char* _value)
{
    return Optional<std::string>(_value);
}

}
