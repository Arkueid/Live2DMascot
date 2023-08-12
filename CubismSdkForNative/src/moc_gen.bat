set bin=D:\cdk\qt\5.15.2\msvc2019_64\bin\
del moc_PieMenu.cpp
del moc_ControlWidget.cpp
del moc_GLWidget.cpp
del moc_Dialog.cpp
del moc_ChatWidget.cpp
del moc_ChatHistoryView.cpp
%bin%moc.exe PieMenu.h -o moc_PieMenu.cpp
%bin%moc.exe ChatHistoryView.h -o moc_ChatHistoryView.cpp
%bin%moc.exe ControlWidget.h -o moc_ControlWidget.cpp
%bin%moc.exe ChatWidget.h -o moc_ChatWidget.cpp
%bin%moc.exe GLWidget.h -o moc_GLWidget.cpp
%bin%moc.exe Dialog.h -o moc_Dialog.cpp
pause