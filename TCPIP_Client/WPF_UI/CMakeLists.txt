project(WPF_UI VERSION 0.1.0 LANGUAGES CSharp)
include(CSharpUtilities)

add_executable(WPF_UI
    App.config
    App.xaml
    App.xaml.cs
    MainWindow.xaml
    MainWindow.xaml.cs
    UserInfoWindow.xaml
    UserInfoWindow.xaml.cs

    AutoScroll.cs
    WatermarkedTextBox.cs

    Properties/AssemblyInfo.cs
    Properties/Resources.Designer.cs
    Properties/Resources.resx
    Properties/Settings.Designer.cs
    Properties/Settings.settings
)

csharp_set_designer_cs_properties(
    Properties/AssemblyInfo.cs
    Properties/Resources.Designer.cs
    Properties/Resources.resx
    Properties/Settings.Designer.cs
    Properties/Settings.settings
)

csharp_set_xaml_cs_properties(
    App.xaml
    App.xaml.cs
    MainWindow.xaml
    MainWindow.xaml.cs
    UserInfoWindow.xaml
    UserInfoWindow.xaml.cs   
)

set_property(SOURCE App.xaml PROPERTY VS_XAML_TYPE "ApplicationDefinition")

set_property(TARGET WPF_UI PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
set_property(TARGET WPF_UI PROPERTY WIN32_EXECUTABLE TRUE)

set_property(TARGET WPF_UI PROPERTY VS_GLOBAL_ProjectTypeGuids "{60dc8134-eba5-43b8-bcc9-bb4bc16c2548};{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}" )


set_property(TARGET WPF_UI PROPERTY VS_DOTNET_REFERENCES
    "Microsoft.CSharp"
    "PresentationCore"
    "PresentationFramework"
    "System"
    "System.Core"
    "System.Data"
    "System.Data.DataSetExtensions"
    "System.Drawing"
    "System.Net.Http"
    "System.Xaml"
    "System.Xml"
    "System.Xml.Linq"
    "WindowsBase"
)

target_link_libraries (WPF_UI LINK_PUBLIC TCPIP_CLIENT_DLL)