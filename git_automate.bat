@echo off
setlocal EnableDelayedExpansion

:: Path to Git Bash executable (update if different)
set GIT_BASH="E:\Program Files\Git\bin\bash.exe"

:: Check if Git Bash exists
if not exist %GIT_BASH% (
    echo Error: Git Bash not found at %GIT_BASH%
    pause
    exit /b 1
)

:: Check if inside a Git repository
git rev-parse --is-inside-work-tree >nul 2>&1
if errorlevel 1 (
    echo Initializing new Git repository...
    %GIT_BASH% -c "git init"
    %GIT_BASH% -c "git branch -m main"
    echo Repository initialized.
) else (
    echo Existing Git repository found.
)

:: Add all changes
echo Adding all changes...
%GIT_BASH% -c "git add ."

:: Prompt for commit message
set /p COMMIT_MSG="Enter commit message (default: 'Update project'): "
if "!COMMIT_MSG!"=="" set COMMIT_MSG=Update project

:: Commit changes
echo Committing changes with message: "!COMMIT_MSG!"
%GIT_BASH% -c "git commit -m '!COMMIT_MSG!'"

:: Check if remote exists, if not prompt for remote URL
%GIT_BASH% -c "git remote get-url origin >nul 2>&1"
if errorlevel 1 (
    set /p REMOTE_URL="Enter remote repository URL (e.g., https://github.com/user/repo.git): "
    if "!REMOTE_URL!"=="" (
        echo No remote URL provided. Skipping push.
        pause
        exit /b 0
    )
    %GIT_BASH% -c "git remote add origin '!REMOTE_URL!'"
)

:: Push to remote
echo Pushing to remote repository...
%GIT_BASH% -c "git push -u origin main"

:: Check push success
if errorlevel 1 (
    echo Error: Push failed. Check your remote configuration or credentials.
) else (
    echo Push successful!
)

pause
endlocal