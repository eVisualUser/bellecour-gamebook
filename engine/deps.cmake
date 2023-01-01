include(FetchContent)
FetchContent_Declare(
    toml
    GIT_REPOSITORY https://github.com/ToruNiina/toml11.git
    GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(toml)
