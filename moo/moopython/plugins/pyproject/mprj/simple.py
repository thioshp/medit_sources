#
#  mprj/simple.py
#
#  Copyright (C) 2004-2010 by Yevgen Muntyan <emuntyan@users.sourceforge.net>
#
#  This file is part of medit.  medit is free software; you can
#  redistribute it and/or modify it under the terms of the
#  GNU Lesser General Public License as published by the
#  Free Software Foundation; either version 2.1 of the License,
#  or (at your option) any later version.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with medit.  If not, see <http://www.gnu.org/licenses/>.
#

if __name__ == '__main__':
    import sys
    import os.path
    dir = os.path.dirname(__file__)
    sys.path.insert(0, os.path.join(dir, '../..'))
    sys.path.insert(0, os.path.join(dir, '..'))

import gobject
import os.path
import tempfile

import moo
from moo import _

from mprj.project import Project
from mprj.config import Config, Dict, Group
from mprj.config.view import *
from mprj.settings import Filename
from mprj.utils import print_error
from mprj.session import Session
import mprj.optdialog
import mprj.utils


class GeneralSettings(Group):
    __items__ = {
        'vars' : Dict(str, name=_('Variables'), xml_elm_name='var'),
    }
    __item_name__ = _('General settings')

class SimpleConfig(Config):
    __items__ = {
        'general': GeneralSettings
    }


class SimpleProject(Project):

    __config__ = SimpleConfig

    def __init__(self, *args, **kwargs):
        Project.__init__(self, *args, **kwargs)
        self.__file_selector_dir = None
        self.filesel = None
        self.filesel_merge_id = 0
        self.__filesel_cb_id = 0
        self.__filesel_destroy_cb_id = 0

    def __filesel_cb(self, filesel, *whatever):
        self.__file_selector_dir = filesel.get_property('current-directory')
    def __filesel_destroy_cb(self, filesel):
        self.filesel = None
        self.__filesel_cb_id = None
        self.__filesel_destroy_cb_id = None
    def __setup_file_selector(self):
        plugin = moo.plugin_lookup('FileSelector')
        if plugin:
            try:
                self.filesel = plugin.call_method('get-widget', self.window)
                if self.filesel:
                    last_dir = self.__file_selector_dir
                    if last_dir and not os.path.isdir(last_dir):
                        last_dir = None
                    if not last_dir:
                        last_dir = self.topdir
                    self.filesel.chdir(last_dir)
                    self.__filesel_cb_id = self.filesel.connect('notify::current-directory', self.__filesel_cb)
                    self.__filesel_destroy_cb_id = self.filesel.connect('destroy', self.__filesel_destroy_cb)
            except:
                print_error()

    def init_ui(self):
        Project.init_ui(self)

    def deinit_ui(self):
        if self.__filesel_cb_id:
            self.filesel.disconnect(self.__filesel_cb_id)
            self.filesel.disconnect(self.__filesel_destroy_cb_id)
            self.__filesel_cb_id = 0
            self.__filesel_destroy_cb_id = 0
        if self.filesel and self.filesel_merge_id:
            xml = self.filesel.get_ui_xml()
            xml.remove_ui(self.filesel_merge_id)
        self.filesel = None
        self.filesel_merge_id = 0
        Project.deinit_ui(self)

    def load(self):
        Project.load(self)
        self.load_session()

    def close(self):
        self.save_session()
        self.save_config()
        return self.window.close_all()

    def get_session_file(self):
        return os.path.join(self.topdir, '.' + \
                            os.path.basename(self.filename) + '.session')

    def load_session(self):
        try:
            file = self.get_session_file()
            if os.path.exists(file):
                session = Session(file)
                session.attach(self.window)
                self.__file_selector_dir = session.get_file_selector_dir()
            else:
                self.__file_selector_dir = self.topdir
            self.__setup_file_selector()
        except:
            print_error()

    def save_session(self):
        try:
            file = self.get_session_file()
            session = Session(self.window)
            session.set_file_selector_dir(self.__file_selector_dir)
            session.save(file)
        except Exception, e:
            moo.error_dialog(self.window, 'Could not save session',
                             'Could not save file %s: %s' % (file, str(e)))

    def save_config(self):
        content = self.config.format()
        try:
            mprj.utils.save_file(self.filename, content)
        except Exception, e:
            moo.error_dialog(self.window, 'Could not save project file',
                             'Could not save file %s: %s' % (self.filename, str(e)))

    def options_dialog(self, window):
        dialog = self.create_options_dialog()
        dialog.run(window)

    def create_options_dialog(self):
        return None


class ConfigPage(mprj.optdialog.ConfigPage):
    __label__ = _("General")
    __types__ = {'vars': DictView, 'name': Entry}

    def __init__(self, config):
        mprj.optdialog.ConfigPage.__init__(self, "page", config,
                                           os.path.join(os.path.dirname(__file__), "simple.glade"))

    def do_init(self):
        mprj.optdialog.ConfigPage.do_init(self)
        self.xml.w_vars.set_dict(self.config.general.vars)

    def do_apply(self):
        mprj.optdialog.ConfigPage.do_apply(self)


gobject.type_register(ConfigPage)


if __name__ == '__main__':
    from mprj.config import File

    s1 = """
    <medit-project name="moo" type="Simple" version="2.0">
      <general>
        <vars>
          <foo>bar</foo>
          <blah>bom</blah>
        </vars>
      </general>
    </medit-project>
    """

    c = SimpleConfig(File(s1))
    s2 = str(c.get_xml())

    print s2

    c = SimpleConfig(File(s2))
    s3 = str(c.get_xml())

    assert s2 == s3
