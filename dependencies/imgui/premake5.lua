project "ImGui"
    kind "StaticLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imconfig.h",
        "imgui.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_widgets.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imgui_demo.cpp",
        "imgui_impl_glfw.cpp",
        "imgui_impl_glfw.h",
        "imgui_impl_opengl3.h",
        "imgui_impl_opengl3.cpp",
        "TextEditor.h",
        "TextEditor.cpp"
    }

    includedirs {
        "%{wks.location}\\dependencies\\glad\\include",
        "%{wks.location}\\dependencies\\glfw3\\include"
    }

    links { 
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    disablewarnings {
        "26812"
    }
    
    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter { "system:windows", "configurations:Debug" }
        buildoptions "/MTd"
        
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"
