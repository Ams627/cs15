#include "stdafx.h"
#include "ProjFiles.h"
#include <ams/errorutils.h>

BOOL CreateDirectoryRec(std::string dir);
void StartProcess(std::string projname);

#define THROWERROR(x) throw(Error(x, __FILE__, __FUNCTION__, __LINE__))
class Error
{
public:
    enum Errors {
        CANTACCESSREGKEY,
        CANTACCESSINSTALLDIR,
    };
    Error(Errors e, char* file, char* function, int line) : error_(e), file_(file), function_(function), line_(line) {}
    void DisplayError()
    {
        if (error_ == CANTACCESSREGKEY)
        {
            std::cerr << "RegOpenKeyEx cannot access the registry key HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\10.0\n";
        }
        else if (error_ == CANTACCESSINSTALLDIR)
        {
            std::cerr << "RegQueryValueEx cannot access registry value InstallDir under key HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\10.0\n";
        }
    }
private:
    Errors error_;
    char* function_;
    char* file_;
    int line_;
};

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        try {
            std::string projname(argv[1]);
            CreateDirectoryRec(projname + "/" + projname);
            CreateDirectoryRec(projname + "/" + projname + "/Properties");

            SetCurrentDirectory(projname.c_str());
            char dirbuf[_MAX_PATH];
            GetCurrentDirectory(_MAX_PATH, dirbuf);
            ProjFiles::CreateFiles(projname);
            SetCurrentDirectory(dirbuf);
            StartProcess(projname);
        }
        catch (Error& e)
        {
            e.DisplayError();
        }
    }
    else
    {
        std::cout << "error - you must supply an argument\n";
    }
    return 0;
}

void StartProcess(std::string projname)
{
    // regkey for devenv location is: HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\14.0 - value InstallDir 

    HKEY key;
    long res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\VisualStudio\\14.0", 0, KEY_QUERY_VALUE, &key);
    if (res != ERROR_SUCCESS)
    {
        THROWERROR(Error::CANTACCESSREGKEY);
    }

    DWORD length = 0;
    res = RegQueryValueEx(key, "InstallDir", NULL, NULL, NULL, &length);
    if (res != ERROR_SUCCESS)
    {
        THROWERROR(Error::CANTACCESSINSTALLDIR);
    }
    std::vector<char> installDir(length);
    res = RegQueryValueEx(key, "InstallDir", NULL, NULL, (LPBYTE)&installDir[0], &length);
    if (res != ERROR_SUCCESS)
    {
        THROWERROR(Error::CANTACCESSINSTALLDIR);
    }

    std::string devenvExecutable = std::string(&installDir[0]);
    if (devenvExecutable[devenvExecutable.length() - 1] != '\\')
    {
        devenvExecutable += '\\';
    }
    devenvExecutable += "devenv.exe";

    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi;
    si.cb = sizeof(STARTUPINFO);
    std::string startname = devenvExecutable + ' ' + projname + ".sln" + " /command OpenCSharpConApp";

    std::cout << startname << std::endl;
    std::vector<char> cstring(startname.length() + 1);
    std::copy(startname.begin(), startname.end(), cstring.begin());
    cstring[startname.length()] = 0;

    try
    {
        RR CreateProcess(
            NULL,               // application name
            &cstring[0],
            NULL,               // Process security attributes:
            NULL,               // Thread security attributes
            FALSE,              // inherit handles
            0,                  // creation flags
            NULL,               // environment
            NULL,               // current directory
            &si,                // startup information
            &pi                 // process information
            );
        HANDLE procHandle = pi.hProcess;
        HANDLE threadHandle = pi.hThread;
        RR CloseHandle(pi.hThread);
        RR CloseHandle(pi.hProcess);
    }
    catch (Reporter &r)
    {
        r.DisplayError(std::cerr);
    }
}


BOOL CreateDirectoryRec(std::string dir)
{
    BOOL createResult = FALSE;

    if (!dir.empty() && dir != "/")
    {

        std::replace(dir.begin(), dir.end(), '\\', '/');
        // Break directory up into parts
        std::vector<std::string> parts;
        size_t findres;
        size_t n = 0;

        do
        {
            findres = dir.find_first_of('/', n + 1);
            std::string partial = dir.substr(n, findres - n);
            n = findres + 1;
            parts.push_back(partial);
        } while (findres != std::string::npos);

        std::string current;

        createResult = TRUE;
        for (std::vector<std::string>::iterator p = parts.begin(); p != parts.end() && createResult; ++p)
        {
            current += *p;
            createResult = CreateDirectory(current.c_str(), NULL);

            // It's not a failure to attempt to create a directory if it already exists:
            DWORD error = GetLastError();
            if (error == ERROR_ALREADY_EXISTS)
            {
                createResult = TRUE;
            }
            current += '/';
        }
    }

    return createResult;
}
