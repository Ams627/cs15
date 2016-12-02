#include "StdAfx.h"
#include ".\projfiles.h"


std::string ProjFiles:: guidString;
std::string ProjFiles:: tlguidString;

ProjFiles::ProjFiles(void)
{
}

ProjFiles::~ProjFiles(void)
{
}

#pragma warning (disable:6031)
void ProjFiles::CreateFiles(std::string projname)
{
    GUID guid;
    // get two guids into static GUID strings:
    CoCreateGuid(&guid);
    PrintGuid(guidString, guid);
    CoCreateGuid(&guid);
    PrintGuid(tlguidString, guid);

    CreateSolutionFile(projname);
    SetCurrentDirectory(projname.c_str());
    CreateProgramCS(projname);
    CreateProjDotVCProj(projname);
    SetCurrentDirectory("Properties");
    CreateAssemblyInfoCS(projname);
    SetCurrentDirectory("c:\\temp");
}


void ProjFiles::CreateProgramCS(std::string projname)
{
	std::string sixteen(16, ' ');
	auto errorStuff = 
		sixteen + "string codeBase = System.Reflection.Assembly.GetExecutingAssembly().CodeBase;\r\n" +
		sixteen + "string progname = Path.GetFileNameWithoutExtension(codeBase);\r\n" +
		sixteen + "Console.Error.WriteLine(progname + \": Error: \" + ex.Message);";


    std::ofstream csfile("program.cs", std::ios::binary);
    // write byte order mark - see http://en.wikipedia.org/wiki/Byte_Order_Mark
    csfile << "\xEF\xBB\xBF";
    csfile << "using System;\r\n";
	csfile << "using System.Collections.Generic;\r\n";
	csfile << "using System.IO;\r\n";
    csfile << "using System.Linq;\r\n";
    csfile << "using System.Text;\r\n";
    csfile << "using System.Threading.Tasks;\r\n";
    csfile << "\r\n";
    csfile << "namespace " << projname << "\r\n";
    csfile << "{\r\n";
    csfile << "    class Program\r\n";
    csfile << "    {\r\n";
    csfile << "        static void Main(string[] args)\r\n";
	csfile << "        {\r\n";
	csfile << "            try\r\n            {\r\n            }\r\n            catch (Exception ex)\r\n            {\r\n" + errorStuff + "\r\n            }\r\n";
    csfile << "        }\r\n";
    csfile << "    }\r\n";
    csfile << "}\r\n";
    csfile.close();

    std::ofstream appconfig("app.config", std::ios::binary);
    // write byte order mark - see http://en.wikipedia.org/wiki/Byte_Order_Mark
    appconfig << "\xEF\xBB\xBF";
    appconfig << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n";
    appconfig << "<configuration>\r\n";
    appconfig << "    <startup> \r\n";
    appconfig << "        <supportedRuntime version=\"v4.0\" sku=\".NETFramework,Version=v4.5.2\" />\r\n";
    appconfig << "    </startup>\r\n";
    appconfig << "</configuration>\r\n";
    appconfig.close();
}


void ProjFiles::CreateProjDotVCProj(std::string projname)
{
    std::string filename = projname + ".csproj";
    std::ofstream f(filename, std::ios::binary);
    // write byte order mark - see http://en.wikipedia.org/wiki/Byte_Order_Mark
    f << "\xEF\xBB\xBF";
    f << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";
    f << "<Project ToolsVersion=\"14.0\" DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n";
    f << "  <Import Project=\"$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props\" Condition=\"Exists('$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props')\" />\r\n";
    f << "  <PropertyGroup>\r\n";
    f << "    <Configuration Condition=\" '$(Configuration)' == '' \">Debug</Configuration>\r\n";
    f << "    <Platform Condition=\" '$(Platform)' == '' \">AnyCPU</Platform>\r\n";
    f << "    <ProjectGuid>{" << guidString << "}</ProjectGuid>\r\n";
    f << "    <OutputType>Exe</OutputType>\r\n";
    f << "    <AppDesignerFolder>Properties</AppDesignerFolder>\r\n";
    f << "    <RootNamespace>" << projname << "</RootNamespace>\r\n";
    f << "    <AssemblyName>" << projname << "</AssemblyName>\r\n";
    f << "    <TargetFrameworkVersion>v4.5.2</TargetFrameworkVersion>\r\n";
    f << "    <FileAlignment>512</FileAlignment>\r\n";
    f << "    <AutoGenerateBindingRedirects>true</AutoGenerateBindingRedirects>\r\n";
    f << "  </PropertyGroup>\r\n";
    f << "  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' \">\r\n";
    f << "    <PlatformTarget>AnyCPU</PlatformTarget>\r\n";
    f << "    <DebugSymbols>true</DebugSymbols>\r\n";
    f << "    <DebugType>full</DebugType>\r\n";
    f << "    <Optimize>false</Optimize>\r\n";
    f << "    <OutputPath>bin\\Debug\\</OutputPath>\r\n";
    f << "    <DefineConstants>DEBUG;TRACE</DefineConstants>\r\n";
    f << "    <ErrorReport>prompt</ErrorReport>\r\n";
    f << "    <WarningLevel>4</WarningLevel>\r\n";
    f << "  </PropertyGroup>\r\n";
    f << "  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Release|AnyCPU' \">\r\n";
    f << "    <PlatformTarget>AnyCPU</PlatformTarget>\r\n";
    f << "    <DebugType>pdbonly</DebugType>\r\n";
    f << "    <Optimize>true</Optimize>\r\n";
    f << "    <OutputPath>bin\\Release\\</OutputPath>\r\n";
    f << "    <DefineConstants>TRACE</DefineConstants>\r\n";
    f << "    <ErrorReport>prompt</ErrorReport>\r\n";
    f << "    <WarningLevel>4</WarningLevel>\r\n";
    f << "  </PropertyGroup>\r\n";
    f << "  <ItemGroup>\r\n";
    f << "    <Reference Include=\"System\" />\r\n";
    f << "    <Reference Include=\"System.Core\" />\r\n";
    f << "    <Reference Include=\"System.Xml.Linq\" />\r\n";
    f << "    <Reference Include=\"System.Data.DataSetExtensions\" />\r\n";
    f << "    <Reference Include=\"Microsoft.CSharp\" />\r\n";
    f << "    <Reference Include=\"System.Data\" />\r\n";
    f << "    <Reference Include=\"System.Net.Http\" />\r\n";
    f << "    <Reference Include=\"System.Xml\" />\r\n";
    f << "  </ItemGroup>\r\n";
    f << "  <ItemGroup>\r\n";
    f << "    <Compile Include=\"Program.cs\" />\r\n";
    f << "    <Compile Include=\"Properties\\AssemblyInfo.cs\" />\r\n";
    f << "  </ItemGroup>\r\n";
    f << "  <ItemGroup>\r\n";
    f << "    <None Include=\"App.config\" />\r\n";
    f << "  </ItemGroup>\r\n";
    f << "  <Import Project=\"$(MSBuildToolsPath)\\Microsoft.CSharp.targets\" />\r\n";
    f << "  <!-- To modify your build process, add your task inside one of the targets below and uncomment it. \r\n";
    f << "       Other similar extension points exist, see Microsoft.Common.targets.\r\n";
    f << "  <Target Name=\"BeforeBuild\">\r\n";
    f << "  </Target>\r\n";
    f << "  <Target Name=\"AfterBuild\">\r\n";
    f << "  </Target>\r\n";
    f << "  -->\r\n";
    f << "</Project>\r\n\r\n";
    f.close();
}

void ProjFiles::CreateAssemblyInfoCS(std::string projname)
{
    std::string filename = "AssemblyInfo.cs";
    std::ofstream f(filename, std::ios::binary);
    // write byte order mark - see http://en.wikipedia.org/wiki/Byte_Order_Mark
    f << "\xEF\xBB\xBF";
    f << "using System.Reflection;\r\n";
    f << "using System.Runtime.CompilerServices;\r\n";
    f << "using System.Runtime.InteropServices;\r\n";
    f << "\r\n";
    f << "// General Information about an assembly is controlled through the following \r\n";
    f << "// set of attributes. Change these attribute values to modify the information\r\n";
    f << "// associated with an assembly.\r\n";
    f << "[assembly: AssemblyTitle(\"" << projname << "\")]\r\n";
    f << "[assembly: AssemblyDescription(\"This program was automatically generated by cs15 - AMS 2015\")]\r\n";
    f << "[assembly: AssemblyConfiguration(\"\")]\r\n";
    f << "[assembly: AssemblyCompany(\"\")]\r\n";
    f << "[assembly: AssemblyProduct(\"" << projname << "\")]\r\n";
    f << "[assembly: AssemblyCopyright(\"Copyright © AMS 2015\")]\r\n";
    f << "[assembly: AssemblyTrademark(\"\")]\r\n";
    f << "[assembly: AssemblyCulture(\"\")]\r\n";
    f << "\r\n";
    f << "// Setting ComVisible to false makes the types in this assembly not visible \r\n";
    f << "// to COM components.  If you need to access a type in this assembly from \r\n";
    f << "// COM, set the ComVisible attribute to true on that type.\r\n";
    f << "[assembly: ComVisible(false)]\r\n";
    f << "\r\n";
    f << "// The following GUID is for the ID of the typelib if this project is exposed to COM\r\n";
    f << "[assembly: Guid(\"" << guidString << "\")]\r\n";
    f << "\r\n";
    f << "// Version information for an assembly consists of the following four values:\r\n";
    f << "//\r\n";
    f << "//      Major Version\r\n";
    f << "//      Minor Version \r\n";
    f << "//      Build Number\r\n";
    f << "//      Revision\r\n";
    f << "//\r\n";
    f << "// You can specify all the values or you can default the Build and Revision Numbers \r\n";
    f << "// by using the '*' as shown below:\r\n";
    f << "// [assembly: AssemblyVersion(\"1.0.*\")]\r\n";
    f << "[assembly: AssemblyVersion(\"1.0.0.0\")]\r\n";
    f << "[assembly: AssemblyFileVersion(\"1.0.0.0\")]\r\n";
    f << "\r\n";
    f.close();
}

void ProjFiles::CreateSolutionFile(std::string projname)
{
    std::string filename = projname + ".sln";
    std::ofstream f(filename, std::ios::binary);
    // write byte order mark - see http://en.wikipedia.org/wiki/Byte_Order_Mark
    f << "\xEF\xBB\xBF";
    f << "Microsoft Visual Studio Solution File, Format Version 12.00\r\n";
    f << "# Visual Studio 14\r\n";
    f << "VisualStudioVersion = 14.0.22823.1\r\n";
    f << "MinimumVisualStudioVersion = 10.0.40219.1\r\n";
    f << "Project(\"{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}\") = \"" << projname << "\", \"" << projname << "\\" << projname << ".csproj\", \"{" << guidString << "}\"\r\n";
    f << "EndProject\r\n";
    f << "Global\r\n";
    f << "	GlobalSection(SolutionConfigurationPlatforms) = preSolution\r\n";
    f << "		Debug|Any CPU = Debug|Any CPU\r\n";
    f << "		Release|Any CPU = Release|Any CPU\r\n";
    f << "	EndGlobalSection\r\n";
    f << "	GlobalSection(ProjectConfigurationPlatforms) = postSolution\r\n";
    f << "		{" << guidString << "}.Debug|Any CPU.ActiveCfg = Debug|Any CPU\r\n";
    f << "		{" << guidString << "}.Debug|Any CPU.Build.0 = Debug|Any CPU\r\n";
    f << "		{" << guidString << "}.Release|Any CPU.ActiveCfg = Release|Any CPU\r\n";
    f << "		{" << guidString << "}.Release|Any CPU.Build.0 = Release|Any CPU\r\n";
    f << "	EndGlobalSection\r\n";
    f << "	GlobalSection(SolutionProperties) = preSolution\r\n";
    f << "		HideSolutionNode = FALSE\r\n";
    f << "	EndGlobalSection\r\n";
    f << "EndGlobal\r\n\r\n";
    f.close();
}

void ProjFiles::PrintGuid(std::string &s, GUID guid)
{
    std::ostringstream ss;
    ss << std::uppercase << std::hex << std::setfill('0') << std::setw(8) << guid.Data1 << '-' <<
        std::setw(4) << guid.Data2 << '-' <<
        std::setw(4) << guid.Data3 << '-';

    for (int i = 0 ; i < 8; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)guid.Data4[i];
        if (i == 1)
        {
            ss << '-';
        }
    }
    s = ss.str();

}
