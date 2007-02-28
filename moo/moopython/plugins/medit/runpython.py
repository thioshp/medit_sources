import sys
import os
import re
import gtk
import moo
from moo.utils import _

if os.name == 'nt':
    PYTHON_COMMAND = '"' + sys.exec_prefix + '\\pythonw.exe" -u'
else:
    PYTHON_COMMAND = 'python -u'

PANE_ID = 'PythonOutput'

class Runner(object):
    def __init__(self, window, python_command=PYTHON_COMMAND, pane_id=PANE_ID, pane_label=None):
        self.window = window
        self.python_command = python_command
        self.pane_id = pane_id
        self.pane_label = pane_label

    def __get_output(self):
        return self.window.get_pane(self.pane_id)
    def __ensure_output(self):
        pane = self.__get_output()
        if pane is None:
            label = self.pane_label or moo.utils.PaneLabel(icon_stock_id=moo.utils.STOCK_EXECUTE,
                                                           label=_("Python Output"))
            output = moo.edit.CmdView()
            output.set_property("highlight-current-line", True)
            output.set_filter(moo.edit.command_filter_create("python"))

            pane = gtk.ScrolledWindow()
            pane.set_shadow_type(gtk.SHADOW_ETCHED_IN)
            pane.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
            pane.add(output)
            pane.show_all()

            pane.output = output
            self.window.add_pane(self.pane_id, pane, label, moo.utils.PANE_POS_BOTTOM)
            self.window.add_stop_client(output)
        return pane

    def run(self, filename=None, args_string=None, working_dir=None):
        pane = self.__get_output()

        if pane is not None and pane.output.running():
            return

        if filename is None:
            doc = self.window.get_active_doc()

            if not doc:
                return
            if not doc.get_filename() or doc.get_status() & moo.edit.EDIT_MODIFIED:
                if not doc.save():
                    return

            filename = doc.get_filename()

        pane = self.__ensure_output()
        pane.output.clear()
        self.window.paned.present_pane(pane)

        if working_dir is None:
            working_dir = os.path.dirname(filename)
        cmd_line = self.python_command + ' "%s"' % os.path.basename(filename)
        if args_string is not None:
            cmd_line += ' %s' % (args_string,)
        pane.output.run_command(cmd_line, working_dir)
