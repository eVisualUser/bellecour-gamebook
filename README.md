# Bellecour GameBook

Game book for bellecour school

## Warning
The program have no certificate so all anti-virus will detect the executable as a virus.
And the os may block the file reading (run it as admin to avoid it).

## Files

### Page Render Format

```toml
[config]
title = "Page Title"
type = "classic"

[content]
line1 = "demo_local" # Use localization (see below)

[choices]
choice_a = ["choice_name", "default_cond", "default_act"]

```

### Variables

```toml

[variables]
life = 100 # Default value

```

### Choice Conditions

```toml

[conditions]
# True if the variable life is greater than 0
default_cond = ["life > 0", "life < 100"]

```

### Choice Actions

```toml

[actions]
# True if the variable life is greater than 0
default_act = ["life + 1", "$other.toml"]

```

### Localization

```toml

[english]
demo_local = "Some Text"

```

### CMake Compilation Command

```bat
cmake -S . -G "Unix Makefiles" -B cmake
```
