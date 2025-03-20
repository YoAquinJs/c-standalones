# c-standalones

Recopilation of multiple C programs I have found interesting, from sources like
book problems, leetcodes and challenges, replicas from other programs, etc.

Multiple utilities are shared in `utils/`.

### creating new project

Create a copy of `base-project/` with make build utils:

```sh
./new-project.sh project-name`
```

The project is created in `sta/project-name/`

### sync projects after changing base project

For syncing the changes to base project execute:

```sh
./sync-base-config.sh
```

note that project files like `src/`, `tests/` and `.gitignore` are not synced

### regenerating clangd compile commands db

```sh
./gen-clangd-compile-commands.sh project-name
```
