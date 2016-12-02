#pragma once

class ProjFiles
{
    static std::string guidString;
    static std::string tlguidString;
public:
    ProjFiles(void);
    ~ProjFiles(void);
    static void CreateFiles(std::string projname);
    static void CreateProgramCS(std::string projname);
    static void CreateProjDotVCProj(std::string);
    static void CreateSolutionFile(std::string projname);
    static void CreateAssemblyInfoCS(std::string projname);

    static void PrintGuid(std::string& s, GUID guid);
};
