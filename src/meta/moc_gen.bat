set bin=D:\cdk\qt\5.15.2\msvc2019_64\bin\
del moc_PieMenu.cpp
del moc_ControlWidget.cpp
del moc_GLWidget.cpp
del moc_Dialog.cpp
del moc_ChatWidget.cpp
del moc_ChatHistoryView.cpp
%bin%moc.exe ..\win\PieMenu.h -o moc_PieMenu.cpp
%bin%moc.exe ..\win\ChatHistoryView.h -o moc_ChatHistoryView.cpp
%bin%moc.exe ..\win\ControlWidget.h -o moc_ControlWidget.cpp
%bin%moc.exe ..\win\ChatWidget.h -o moc_ChatWidget.cpp
%bin%moc.exe ..\win\GLWidget.h -o moc_GLWidget.cpp
%bin%moc.exe ..\win\Dialog.h -o moc_Dialog.cpp
pause