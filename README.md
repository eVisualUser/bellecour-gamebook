# Bellecour GameBook

Game book for bellecour school

## Files

### Page Render Format

```toml
[config]
title = "Page Title"
author = "Jean-Michel"
type = "classic"

[content]
line1 = "Some Text"
line2 = "More Text"

[choices]
choice_a = ["choice_name", "default_cond", "default_act"]

```

### Variables

```toml

life = "100" # Default value

```

### Choice Conditions

```toml

# True if the variable life is greater than 0
default_cond = "life > 0"

```

### Choice Actions

```toml

# True if the variable life is greater than 0
default_act = "life + 1 $other.toml"

```
