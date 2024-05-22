import tkinter
import os
from tkinter import *
from tkinter.messagebox import *
from tkinter.filedialog import *
from tkinter import font
import subprocess

class AutoResizeFont:
    def __init__(self, root):
        self.root = root
        self.font = font.Font(size=16)
        self.root.bind('<Configure>', self.resize)
        self.pending_resize = False

    def resize(self, event):
        if not self.pending_resize:
            self.pending_resize = True
            self.root.after(200, self.do_resize)

    def do_resize(self):
        # 获取窗口的新大小
        width = self.root.winfo_width()
        height = self.root.winfo_height()

        # 根据窗口大小调整字体大小
        new_size = min(width, height) // 24
        self.font.configure(size=new_size)
        self.pending_resize = False


class Notepad:
    __root = Tk()

    # default window width and height
    __thisWidth = 300
    __thisHeight = 300
    __thisfont = AutoResizeFont(__root)
    __thisTextArea = Text(__root,font=__thisfont.font)
    __thisMenuBar = Menu(__root)

    #__thisLineBar = Frame(__root, width=2, bg='grey')
    __thisLineNumbers = Text(font = __thisfont.font,width=2, bg='grey')
    
    # To add scrollbar
    __thisScrollBar = Scrollbar(__thisTextArea)
    __file = None

    def __init__(self, **kwargs):

        # Set icon
        try:
            self.__root.wm_iconbitmap("Notepad.ico")
        except:
            pass

        try:
            self.__thisWidth = kwargs['width']
        except KeyError:
            pass

        try:
            self.__thisHeight = kwargs['height']
        except KeyError:
            pass

        # Set the window text
        self.__root.title("Untitled - Notepad")

        


        # Center the window
        screenWidth = self.__root.winfo_screenwidth()
        screenHeight = self.__root.winfo_screenheight()

        # For left-alling
        left = (screenWidth / 2) - (self.__thisWidth / 2)

        # For right-allign
        top = (screenHeight / 2) - (self.__thisHeight / 2)

        # For top and bottom
        self.__root.geometry('%dx%d+%d+%d' % (self.__thisWidth,
                                            self.__thisHeight,
                                            left, top))

        # To make the textarea auto resizable
        self.__root.grid_rowconfigure(0, weight=1)
        self.__root.grid_columnconfigure(1, weight=1)

        # Add controls (widget)
        #self.__thisLineBar.grid(row=0, column=0, sticky='ns')
        self.__thisLineNumbers.grid(row=0, column=0, sticky='ns')
        self.__thisTextArea.grid(row=0, column=1, sticky='nsew')

        # To save current file
        self.__thisMenuBar.add_command(label="Save",
                                        command=self.__saveFile)

        self.__thisMenuBar.add_command(label="Exit",
                                        command=self.__quitApplication)
        
        self.__thisMenuBar.add_command(label="Render",
                                        command=self.__render)


        # To create a feature of description of the notepad
        self.__thisMenuBar.add_command(label="About",
                                        command=self.__showAbout)
        Button(self.__root,text='Render',command=self.__render).grid(row=1,column=0)
        Button(self.__root,text='Quit',command=self.__quitApplication).grid(row=1,column=1)
        Button(self.__root,text='Save',command=self.__saveFile).grid(row=1,column=2)
        Button(self.__root,text='About',command=self.__showAbout).grid(row=1,column=3)
        self.__root.config(menu=self.__thisMenuBar)

        self.__thisScrollBar.pack(side=RIGHT, fill=Y)
        self.__thisScrollBar.config(command=self.__onYview)

        # Scrollbar will adjust automatically according to the content
        # self.__thisScrollBar.config(command=self.yviewSync)
        self.__thisLineNumbers.config(yscrollcommand=self.__thisScrollBar.set)
        self.__thisTextArea.config(yscrollcommand=self.__thisScrollBar.set)
        
        self.__thisTextArea.bind('<KeyRelease>', self.__onKey)
        self.__thisTextArea.bind('<MouseWheel>', self.__onScroll)
    
    def __render(self):
        self.__saveFile()
        # cd Lexer && g++ $(CXXFLAGS) -o lexer lexer.cpp && ./lexer
        subprocess.run(['g++', 'Lexer/lexer.cpp','-std=c++17', '-o',  'Lexer/lexer'])
        subprocess.run(['./Lexer/lexer'])
        subprocess.run(['g++', 'Parser/parser.cpp','-std=c++17', '-o',  'Parser/parser'])
        subprocess.run(['./Parser/parser'])
        subprocess.run(['venv/bin/python.exe', 'Common/autogenerate.py'])
        #subprocess.run(['python', 'Common/autogenerate.py'])

    def __quitApplication(self):
        self.__root.destroy()
        # exit()

    def __showAbout(self):
        showinfo("Notepad", "Mrinal Verma")

    def __onYview(self, *args):
        """处理滚动条的滚动事件"""
        self.__thisTextArea.yview(*args)
        self.__thisLineNumbers.yview(*args)

    def __openFile(self):
        with open('Data/raw_code.txt', 'r') as file:
            self.__file = file.read()

    def __saveFile(self):
        with open('Data/raw_code.txt', 'w') as file:
            file.write(self.__thisTextArea.get(1.0, END))

    def __onKey(self, event):
        line, col = self.__thisTextArea.index(END).split('.')
        linenumbers = [str(i) + '\n' for i in range(1, int(line))]
        linenumbers_text = ''.join(linenumbers)
        self.__thisLineNumbers.config(state='normal')
        self.__thisLineNumbers.delete('1.0', END)
        self.__thisLineNumbers.insert('1.0', linenumbers_text)
        self.__thisLineNumbers.config(state='disabled')
        self.__onYview('moveto', self.__thisTextArea.yview()[0])

    def __onScroll(self, event):
        """处理鼠标滚轮滚动，确保不会在文本区底部继续滚动"""
        first, last = self.__thisTextArea.yview()
        if first > 0 and last < 1:

            scroll_step = int(-1*(event.delta/120))
            self.__thisTextArea.yview_scroll(scroll_step, "units")
            self.__thisLineNumbers.yview_scroll(scroll_step, "units")
        elif first == 0 and event.delta > 0:

            self.__thisTextArea.yview_scroll(-1, "units")
            self.__thisLineNumbers.yview_scroll(-1, "units")
        elif last == 1 and event.delta < 0:

            pass

    def __cut(self):
        self.__thisTextArea.event_generate("<<Cut>>")

    def __copy(self):
        self.__thisTextArea.event_generate("<<Copy>>")

    def __paste(self):
        self.__thisTextArea.event_generate("<<Paste>>")
    
    def __select_all(self):
        self.__thisTextArea.tag_add('sel', '1.0', 'end')
        return 'break'

    def run(self):

        # Run main application
        self.__openFile()
        self.__thisTextArea.insert(1.0, self.__file)
        self.__onKey(None)
        #self.__updateLineNumbers()
        self.__root.mainloop()

    # Run main application


notepad = Notepad(width=600, height=400)
notepad.run()
