/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef AUTOCAST_H
#define AUTOCAST_H

#include <utility>

template <typename T>
class auto_cast_wrapper
{
public:
    template <typename R>
    friend auto_cast_wrapper<R> auto_cast(const R &x);

    template <typename U>
    operator U()
    {
        return static_cast<U>(mX);
    }

private:
    auto_cast_wrapper(const T &x) : mX(x) {}

    auto_cast_wrapper(const auto_cast_wrapper &other) : mX(other.mX) {}

    // non-assignable
    auto_cast_wrapper &operator=(const auto_cast_wrapper &);

    const T &mX;
};

template <typename R>
auto_cast_wrapper<R> auto_cast(const R &x)
{
    return auto_cast_wrapper<R>(x);
}

template <typename T>
class explicit_cast_wrapper
{
public:
    template <typename R>
    friend explicit_cast_wrapper<R> explicit_cast(R &&x);

    template <typename U>
    operator U() const
    {
        // doesn't allow downcasts, otherwise acts like static_cast
        // see: http://stackoverflow.com/questions/5693432/making-auto-cast-safe
        return U{std::forward<T>(mX)};
    }

private:
    explicit_cast_wrapper(T &&x) : mX(std::forward<T>(x)) {}

    explicit_cast_wrapper(const explicit_cast_wrapper &other) : mX(std::forward<T>(other.mX)) {}

    explicit_cast_wrapper &operator=(const explicit_cast_wrapper &) = delete;

    T &&mX;
};

template <typename R>
explicit_cast_wrapper<R> explicit_cast(R &&x)
{
    return explicit_cast_wrapper<R>(std::forward<R>(x));
}

#endif // AUTOCAST_H
