[TOC]

# a File Switch Tabbar for Source Insight

## Introduction

TabSiPlus is a PlusIn program for Source Insight. Source Insight is a great tool, lot's of programmer, even some Visual Studio user, using Source Insight as default coding tool. Source Insight has a lot of features to easy the programmers way for viewing and writing code. I like those features, but I think it missing an important one, that is "File Switch Bar". People may wondering and asking "what's the File Switch Tab going on"? Sorry, I really don't known how to name it, many software support this component but give it a very different name. Check the pictures on this url: http://www.codeproject.com/KB/cpp/TabSiPlus/tabsiplus.gif, stuffs which encircle by red line is what I called "File Switch Bar".

People may argue that using "windows" menu item on menu bar can perform file switching function, but I want a fast and direct way, a small file switch table bar is the bulls eye. Unluckily, Source Insight does not support this bar, even the most recently 3.5.x version. I am a completist (but a fake completist), like many lazy programmers, I make tools myself to cater for my laziness, so I made a plus in tools named TabSiPlus. It insert a switch bar to Source Insight window, user can switch files just a single mouse click on it, that's what I want.


## How it works

Adding a switch bar to a MDI(Multi-Document Interface) program is really not a hard work, there are many articles (with example codes) on this web site which help programmers add switch bar to their program, but unfortunately, I don't have source code of Source Insight. The only way is injecting my code to main process of Source Insight, for 3.x version, generally, it is insignt3.exe. Since the emphases of this article is not about how to inject code to remote process, so people who wilder about code injecting can search the article "Three ways to inject your code to remote process" on this site for more detail information.

TabSiPlus for Source Insight has two components: "TabSiHost.exe" and "TabSiPlus.dll". "TabSiHost.exe" is a little stub program, it's duty is spy on the system and find new instance of Source Insight program, than inject the code contain in "TabSiPlus.dll" to insignt3.exe (for Source Insight 3.x). "TabSiPlus.dll" contain the whole function code, it can't run itself, it need TabSiHost.exe to load it to insignt3.exe process context.

## About the code

Code is written in C++, using Visual C++ as compile tool. Project code is now organized with Studio Visual 2008, older version only support Visual C++ 6.0. if you using Visual C++ 6.0, make sure you have SP6 for Visual Studio 6 and Platform SDK installed. 

## Thanks a lot of people

Not all of the code is written by me, **TabSiPlus**, of course, learn a lot from some good open source codes. 

**loadDll** get from open source Task Manager Extension, comments in the code shown that Felix Kasza made the original version, and Zoltan Csizmadia contributed some changes.

**ApiHook** and **SplashWindows** also get from "Task Manager Extension", thanks to Ivo Ivanov, Jeffrey Richter, Sergey Kolomenkin and John Robbins.

**ColourPicker** is a drop-in colour picker control, written by Chris Maunder, and extended by Alexander Bischofberger.

**TabCheckListBox** control is written by Qing Zhang.

The MDI window management method is inspired by **WndTab** addin for Visual C++ 6.0.
