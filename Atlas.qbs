import qbs

Project {
    references: [
        "3rdparty/projects/bgfx/bgfx.qbs",
        "3rdparty/projects/entityx/entityx.qbs",
        "3rdparty/projects/fmt/fmt.qbs",
        "3rdparty/projects/imgui/imgui.qbs",
        "Engine/Engine.qbs",
        "Editor/Editor.qbs",
        "Player/Player.qbs",
        "Tests/Tests.qbs",
        "Window/Window.qbs",
    ]
    name: "Atlas"
}

