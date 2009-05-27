#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import gtk
import gtk.glade

(COL_NUM, COL_TITLE, COL_ARTIST) = range(3)
class MainWindow():
    def __init__(self):

        self.gladefile="gMystar.glade"
        self.xml=gtk.glade.XML(self.gladefile)
        self.window = self.xml.get_widget("gMystar")

        #self.window.set_title("GMBox")
        #self.window.set_default_size(800, 600)
        self.window.connect('destroy', gtk.main_quit)      
        self.window.show_all();

def main():
    win = MainWindow();
    gtk.main()


if __name__ == '__main__':
    main()
