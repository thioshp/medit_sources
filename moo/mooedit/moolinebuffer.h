/*
 *   moolinebuffer.h
 *
 *   Copyright (C) 2004-2006 by Yevgen Muntyan <muntyan@math.tamu.edu>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   See COPYING file that comes with this distribution.
 */

#ifndef MOOEDIT_COMPILATION
#error "This file may not be included"
#endif

#ifndef __MOO_LINE_BUFFER_H__
#define __MOO_LINE_BUFFER_H__

#include <gtk/gtktextbuffer.h>
#include "mooedit/mootextbtree.h"
#include "mooedit/mootextbuffer.h"

G_BEGIN_DECLS


typedef struct _LineBuffer LineBuffer;
typedef struct _BTData Line;

typedef struct {
    int first;
    int last;
} Interval;

struct _LineBuffer {
    BTree *tree;
};


LineBuffer *_moo_line_buffer_new        (void);
void     _moo_line_buffer_free          (LineBuffer     *line_buf);

Line    *_moo_line_buffer_get_line      (LineBuffer     *line_buf,
                                         int             index);

Line    *_moo_line_buffer_insert        (LineBuffer     *line_buf,
                                         int             index);

guint    _moo_line_buffer_get_stamp     (LineBuffer     *line_buf);
int      _moo_line_buffer_get_line_index (LineBuffer    *line_buf,
                                         Line           *line);

void     _moo_line_buffer_add_mark      (LineBuffer     *line_buf,
                                         MooLineMark    *mark,
                                         int             line);
void     _moo_line_buffer_remove_mark   (LineBuffer     *line_buf,
                                         MooLineMark    *mark);
void     _moo_line_buffer_move_mark     (LineBuffer     *line_buf,
                                         MooLineMark    *mark,
                                         int             line);
GSList  *_moo_line_buffer_get_marks_in_range (LineBuffer *line_buf,
                                         int             first_line,
                                         int             last_line);

void     _moo_line_buffer_split_line    (LineBuffer     *line_buf,
                                         int             line,
                                         int             num_new_lines);
void     _moo_line_buffer_delete        (LineBuffer     *line_buf,
                                         int             first,
                                         int             num,
                                         int             move_to,
                                         GSList        **moved_marks,
                                         GSList        **deleted_marks);


G_END_DECLS

#endif /* __MOO_LINE_BUFFER_H__ */
