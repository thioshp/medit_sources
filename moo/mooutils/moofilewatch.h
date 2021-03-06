/*
 *   moofilewatch.h
 *
 *   Copyright (C) 2004-2016 by Yevgen Muntyan <emuntyan@users.sourceforge.net>
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

/* Files and directory monitor. Uses stat().
   On win32 does FindFirstChangeNotification and ReadDirectoryChangesW. */

#pragma once

#ifndef __cplusplus
#error "This is a C++ header"
#endif // __cplusplus

#include <glib-object.h>
#include <moocpp/moocpp.h>
#include <memory>

enum MooFileEventCode {
    MOO_FILE_EVENT_CHANGED,
    MOO_FILE_EVENT_CREATED,
    MOO_FILE_EVENT_DELETED,
    MOO_FILE_EVENT_ERROR
};

struct MooFileEvent {
    MooFileEventCode code;
    guint            monitor_id;
    char            *filename;
    GError          *error;
};

class MooFileWatch;

typedef void (*MooFileWatchCallback) (MooFileWatch& watch,
                                      MooFileEvent* event,
                                      gpointer      user_data);

using MooFileWatchPtr = moo::gref_ptr<MooFileWatch>;

class MooFileWatch
{
public:
    ~MooFileWatch();

    static MooFileWatchPtr      create          (GError        **error);

    bool                        close           (GError        **error);

    guint                       create_monitor  (const char     *filename,
                                                 MooFileWatchCallback callback,
                                                 gpointer        data,
                                                 GDestroyNotify  notify,
                                                 GError        **error);

    void                        cancel_monitor  (guint           monitor_id);

    struct Impl;
    Impl&                       impl            () { return *m_impl; }

    void                        ref             ();
    void                        unref           ();

    MooFileWatch(const MooFileWatch&) = delete;
    MooFileWatch& operator=(const MooFileWatch&) = delete;

private:
    MooFileWatch();

    int m_ref;
    std::unique_ptr<Impl> m_impl;
};

namespace moo {

template<>
class obj_ref_unref<MooFileWatch>
{
public:
    static void ref(MooFileWatch* w) { w->ref(); }
    static void unref(MooFileWatch* w) { w->unref(); }
};

} // namespace moo
