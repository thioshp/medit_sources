/*
 *   moocpp/strutils.cpp
 *
 *   Copyright (C) 2004-2015 by Yevgen Muntyan <emuntyan@users.sourceforge.net>
 *
 *   This file is part of medit.  medit is free software; you can
 *   redistribute it and/or modify it under the terms of the
 *   GNU Lesser General Public License as published by the
 *   Free Software Foundation; either version 2.1 of the License,
 *   or (at your option) any later version.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with medit.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "moocpp/strutils.h"
#include <string.h>

using namespace moo;

static bool str_equal(const char* s1, const char* s2)
{
    if (!s1 || !*s1)
        return !s2 || !*s2;
    else if (!s2)
        return false;
    else
        return strcmp(s1, s2) == 0;
}

bool moo::operator==(const gstr& s1, const char* s2)
{
    return str_equal(s1, s2);
}

bool moo::operator==(const char* s1, const gstr& s2)
{
    return str_equal(s1, s2);
}

bool moo::operator==(const gstr& s1, const gstr& s2)
{
    return str_equal(s1, s2);
}

bool moo::operator!=(const gstr& s1, const gstr& s2)
{
    return !(s1 == s2);
}

bool moo::operator!=(const gstr& s1, const char* s2)
{
    return !(s1 == s2);
}

bool moo::operator!=(const char* s1, const gstr& s2)
{
    return !(s1 == s2);
}


class StringData
{
public:
    StringData(const char* s)
        : m_p(g_strdup(s))
        , m_ref(1)
    {
    }

    ~StringData()
    {
        g_free(m_p);
    }

    StringData(const StringData&) = delete;
    StringData& operator=(const StringData&) = delete;

    char* get() const
    {
        return m_p;
    }

    char* release()
    {
        char* ret = m_p;
        m_p = nullptr;
        return ret;
    }

    void ref()
    {
        g_atomic_int_inc(&m_ref);
    }

    void unref()
    {
        if (g_atomic_int_dec_and_test(&m_ref))
            delete this;
    }

    int ref_count() const
    {
        return g_atomic_int_get(&m_ref);
    }

private:
    char* m_p;
    int m_ref;
};

gstr::gstr()
    : m_p(nullptr)
    , m_is_inline(true)
    , m_is_const(true)
{
}

gstr::gstr(const char* s, mem_transfer mt)
    : gstr()
{
    if (s == nullptr)
        return;

    if (*s == 0)
    {
        if (mt == mem_transfer::take_ownership)
            g_free(const_cast<char*>(s));

        mt = mem_transfer::borrow;
        s = "";
    }

    switch (mt)
    {
    case mem_transfer::borrow:
        m_is_const = true;
        m_is_inline = true;
        m_p = const_cast<char*>(s);
        break;

    case mem_transfer::make_copy:
        m_is_const = false;
        m_is_inline = true;
        m_p = g_strdup(s);
        break;

    case mem_transfer::take_ownership:
        m_is_const = false;
        m_is_inline = true;
        m_p = const_cast<char*>(s);
        break;
    }
}

gstr::gstr(const gstr& other)
    : gstr()
{
    if (other.m_p == nullptr)
        return;

    if (other.m_is_const)
    {
        moo_assert(other.m_is_inline);
        m_p = other.m_p;
        m_is_const = true;
        m_is_inline = true;
    }
    else
    {
        moo_assert(other.m_p != nullptr);
        moo_assert(!other.m_is_const);

        StringData* d;

        if (other.m_is_inline)
        {
            d = new StringData(static_cast<const char*>(other));
        }
        else
        {
            d = reinterpret_cast<StringData*>(other.m_p);
            d->ref();
        }

        m_p = d;
        m_is_inline = false;
        m_is_const = false;
    }
}

gstr& gstr::operator=(const gstr& other)
{
    if (this != &other)
    {
        gstr tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

gstr::gstr(gstr&& other)
    : gstr()
{
    *this = std::move(other);
}

gstr& gstr::operator=(gstr&& other)
{
    std::swap(m_is_const, other.m_is_const);
    std::swap(m_is_inline, other.m_is_inline);
    std::swap(m_p, other.m_p);
    return *this;
}

gstr::~gstr()
{
    clear();
}

bool gstr::is_null() const
{
    bool ret = (m_p == nullptr);
    moo_assert(!ret || m_is_const == true);
    moo_assert(!ret || m_is_inline == true);
    return ret;
}

gstr::operator const char*() const
{
    if (m_is_inline)
        return reinterpret_cast<char*>(m_p);
    else
        return reinterpret_cast<StringData*>(m_p)->get();
}

void gstr::assign(const char* s, mem_transfer mt)
{
    gstr tmp(s, mt);
    *this = std::move(tmp);
}

void gstr::clear()
{
    if (m_p == nullptr)
        return;

    if (!m_is_const)
    {
        moo_assert(m_p != nullptr);

        if (m_is_inline)
            g_free(m_p);
        else
            reinterpret_cast<StringData*>(m_p)->unref();

        m_is_const = true;
    }

    m_is_inline = true;
    m_p = nullptr;
}

char* gstr::get_mutable()
{
    if (!m_p)
    {
        return nullptr;
    }
    else if (m_is_const)
    {
        char* s = reinterpret_cast<char*>(m_p);

        if (*s != 0)
        {
            set(s);
            moo_assert(!m_is_const);
            moo_assert(m_is_inline);
        }

        return reinterpret_cast<char*>(m_p);
    }
    else if (m_is_inline)
    {
        return reinterpret_cast<char*>(m_p);
    }
    else
    {
        StringData* d = reinterpret_cast<StringData*>(m_p);
        moo_assert(d->get() && *d->get());

        if (d->ref_count() == 1)
        {
            return d->get();
        }
        else
        {
            m_p = g_strdup(d->get());
            m_is_inline = true;
            return reinterpret_cast<char*>(m_p);
        }
    }
}

char* gstr::release_owned()
{
    if (m_p == nullptr)
    {
        return nullptr;
    }
    else if (m_is_const)
    {
        return g_strdup(get());
    }
    else if (m_is_inline)
    {
        m_is_const = true;
        char* p = reinterpret_cast<char*>(m_p);
        m_p = nullptr;
        return p;
    }

    StringData* d = reinterpret_cast<StringData*>(m_p);
    moo_assert(d->get() && *d->get());

    char* p = d->ref_count() == 1 ? d->release() : g_strdup(d->get());

    d->unref();

    m_p = nullptr;
    m_is_const = true;
    m_is_inline = true;
    return p;
}
