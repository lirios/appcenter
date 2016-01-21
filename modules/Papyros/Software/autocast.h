#ifndef AUTOCAST_H
#define AUTOCAST_H

#include <utility>

template <typename T> class auto_cast_wrapper
{
public:
    template <typename R> friend auto_cast_wrapper<R> auto_cast(const R &x);

    template <typename U> operator U()
    {
        return static_cast<U>(mX);
    }

private:
    auto_cast_wrapper(const T &x) : mX(x)
    {
    }

    auto_cast_wrapper(const auto_cast_wrapper &other) : mX(other.mX)
    {
    }

    // non-assignable
    auto_cast_wrapper &operator=(const auto_cast_wrapper &);

    const T &mX;
};

template <typename R> auto_cast_wrapper<R> auto_cast(const R &x)
{
    return auto_cast_wrapper<R>(x);
}

template <typename T> class explicit_cast_wrapper
{
public:
    template <typename R> friend explicit_cast_wrapper<R> explicit_cast(R &&x);

    template <typename U> operator U() const
    {
        // doesn't allow downcasts, otherwise acts like static_cast
        // see: http://stackoverflow.com/questions/5693432/making-auto-cast-safe
        return U{std::forward<T>(mX)};
    }

private:
    explicit_cast_wrapper(T &&x) : mX(std::forward<T>(x))
    {
    }

    explicit_cast_wrapper(const explicit_cast_wrapper &other) : mX(std::forward<T>(other.mX))
    {
    }

    explicit_cast_wrapper &operator=(const explicit_cast_wrapper &) = delete;

    T &&mX;
};

template <typename R> explicit_cast_wrapper<R> explicit_cast(R &&x)
{
    return explicit_cast_wrapper<R>(std::forward<R>(x));
}

#endif // AUTOCAST_H
