@echo off
title Git Automation - %CD%

echo.
echo ======================================================================
echo == Ban dang o: %CD%
echo == CAC LENH GIT THUONG DUNG:
echo ======================================================================
echo.

REM Buoc 1: Add File/Thay doi
echo - Buoc 1: Add File/Thay doi
echo   Nhap them ten file (VD: tenfile.txt) HOAC '.' (de add tat ca)
set /p DUMMY_ADD="git add "
git add %DUMMY_ADD%

echo.
git status
echo.

REM Buoc 2: Commit Thay doi
echo - Buoc 2: Commit Thay doi
echo   Nhap them tin nhan commit (VD: "Them chuc nang login") - PHAI CO DAU NHAY KEP!
set /p DUMMY_COMMIT="git commit -m "
git commit -m %DUMMY_COMMIT%

echo.
echo - Buoc 3: Push Thay doi
echo   Lenh 'git push' da san sang!
pause
git push
echo.

echo ======================================================================
== Quy trinh da HOAN TAT!
echo ======================================================================
pause
exit