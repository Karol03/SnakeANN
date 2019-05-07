Project configuration for developers

Project has been written using QT Creator and it is the simplest and convenient
way to develop application because of *.pro file with configuration for QT environment.

(The instruction was created for windows operational system)
To prepare QT enviroment follow the steps below:
1. Download QT Creator from https://www.qt.io/
2. Install QT Creator
3. Download SFML library for "GCC 7.3.0 MinGW (DW2) - 32-bit" from https://www.sfml-dev.org/download/sfml/2.5.1/
4. Unzip SFML-2.5.1.zip to "C:/" (make sure that directiories "include" and "lib" have path C:/SFML-2.5.1/[directory name])
5. Pull repository
6. In QT Creator click File->Open file or project.. and choose file "Snake.pro" from project directory
7. Click "build" and after binary builds, "start". You should see some of message windows with errors
8. After step (7.) build directiory was created. If you didn't change default properies it should appears 
as "C:/Users/[your_username]/Documents/build-Snake-Desktop_Qt_5_11_1_MinGW_32bit-Debug" or similary.
Copy all *.dll files from C:/SFML-2.5.1/bin/ to this directory and rebuild project.
