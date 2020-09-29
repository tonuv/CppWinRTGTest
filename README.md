# CppWinRTGTest
Demonstrates how to use goole test framework with C++ WinRT

This project aims to demonstrate how to:

- Reference a WinRT component from the Google Test project
- Activate Xaml based classes in tests
- Run async code in tests

# Setting up the test project
1. Add a new project, select Google Test, set Project Name
2. In Select Project to Test option select the project you want to test
If you don't do it here or want to add project later you can do so by editing project file and adding:
```
<ItemGroup>
    <ProjectReference Include="..\MyLibrary\MyLibrary.vcxproj">
    </ProjectReference>
</ItemGroup>
```
3. Add the following nuget packages to the google test project
- Microsoft.Windows.CppWinRT
- Microsoft.VCRTForwarders.140

4. Add a property sheet to your google test project to reference the WinRT component output
- Select Add item on the test project and select Property Sheets -> Property Sheet

- Add the following content:
```
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ImportGroup Label="PropertySheets" />
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup />
  <ItemDefinitionGroup />
  <ItemGroup />
  <ItemGroup>
    <Reference Include="$(SolutionDir)bin\MyLibrary\$(Configuration)\$(PlatformShortName)\MyLibrary.winmd">
      <IsWinMDFile>true</IsWinMDFile>
    </Reference>
    <ReferenceCopyLocalPaths Include="$(SolutionDir)bin\MyLibrary\$(Configuration)\$(PlatformShortName)\MyLibrary.dll">
      <IsWinMDFile>false</IsWinMDFile>
    </ReferenceCopyLocalPaths>
  </ItemGroup>
</Project>

```

5. As the default output paths for x86 and other platforms are using different scheme in C++ the easiest way to fix this is to update output directories.
- Select Project Properties (for both projects)
- Set output path to ```$(SolutionDir)\bin\$(ProjectName)\$(Configuration)\$(PlatformShortName)\```

*Note:*
Make sure you have All Configurations and All Platforms selected to apply this for any combination.
This can be a different path as long as the references made in 4. property sheet match the location.

6. It's a good idea to add some header includes in the test project pch.h
```C++
#include <Unknwn.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/MyLibrary.h>
```

At this point you can run tests and reference classes from your WinRT component and from OS. However if the dll name does not match the namespace name or you want to activate xaml components you also need to add a manifest.

7. Add a manifest file
- Add an XML file to test project and call it <test project name>.exe.manifest
  
```xml
<?xml version="1.0" encoding="utf-8"?>
<assembly manifestVersion="1.0" xmlns="urn:schemas-microsoft-com:asm.v1">
  <compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1">
    <application>

      <!-- The following lines are needed to initialize XAML -->
      <maxversiontested Id="10.0.18362.0"/>
      <supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}" />

    </application>
  </compatibility>
  
  <!-- Add similar entries for all the classes you need to activate -->
  <file name="MyLibrary.dll">
    <activatableClass
        name="MyLibrary.Class"
        threadingModel="both"
        xmlns="urn:schemas-microsoft-com:winrt.v1" />
  </file>
  
</assembly>
```
8. Building for x64
The default Google test template adds a preprocessor define "X64" that will break the build if compiled for x64 platform. To fix that
- Open Project Properties
- Got to C/C++ Preprocessor settings
- Select Configuration: All configurations, Platform: x64
- Remove the X64 value from Preprocessor definitions.
