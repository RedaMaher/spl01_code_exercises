# Manual Test Guide - SPL01 Assignments

This guide translates the GTest checks into manual terminal tests. Replace the executable names shown here (`./pwd`, `./echo`, `./cp`, `./mv`, `./femtoshell`, `./picoshell`, `./nanoshell`, `./microshell`) with the actual executable names you build.

After each command or piped shell session, run `echo $?` to check the exit code.

For shell tasks, `[PROMPT]` means the exact prompt printed by the student's shell. The tests detect this prompt automatically. Expected strings use `\n` to mean a newline.

---

## Task 01 - `pwd`

### Test 1 - `CurrentDirectory`

| Field | Value |
|---|---|
| Setup | Run from any directory. |
| Command/input | `./pwd` |
| Expected output | The absolute path of the current directory, followed by `\n`. It must match the real `pwd`. |
| Expected exit code | `0` |

### Test 2 - `ChangeDirectory`

| Field | Value |
|---|---|
| Setup | `cd /tmp`. Run the program from `/tmp`, or use its absolute path. |
| Command/input | `./pwd` or `/path/to/pwd` |
| Expected output | `/tmp\n` |
| Expected exit code | `0` |

---

## Task 02 - `echo`

| Test | Setup | Command/input | Expected output | Expected exit code |
|---|---|---|---|---|
| `PrintString` | None | `./echo Hello World` | `Hello World\n` | `0` |
| `NoArguments` | None | `./echo` | `\n` | `0` |
| `SpecialCharacters` | None | `./echo Hello 'World!' '@#$%^&*()'` | `Hello World! @#$%^&*()\n` | `0` |
| `MultipleSpaces` | None | `./echo Hello '   ' World` | `Hello     World\n` (5 spaces between words) | `0` |
| `EmptyString` | None | `./echo ''` | `\n` | `0` |
| `NewlineCharacter` | None | `./echo $'Hello\nWorld'` | `Hello\nWorld\n` | `0` |
| `TabCharacter` | None | `./echo $'Hello\tWorld'` | `Hello\tWorld\n` | `0` |
| `LongString` | None | `python3 -c "import subprocess; r=subprocess.run(['./echo','a'*10000], capture_output=True, text=True); print(len(r.stdout)); print(r.returncode)"` | First printed line must be `10001`; second line must be `0`. The real output is 10000 `a` characters plus `\n`. | Python helper exits `0`; student's program exit is printed as `0`. |

---

## Task 03 - `cp`

| Test | Setup | Command/input | Expected output | Expected exit code | Extra verification |
|---|---|---|---|---|---|
| `CopyFile` | `echo -n "This is a test file." > source.txt` | `./cp source.txt destination.txt` | No output checked by GTest. | `0` | `cat destination.txt` prints `This is a test file.` |
| `CopyLargeFile` | `python3 -c "open('large_source.txt','w').write('a'*10000)"` | `./cp large_source.txt large_destination.txt` | No output checked by GTest. | `0` | `wc -c < large_destination.txt` prints `10000`. |
| `CopyToExistingFile` | `echo -n "This is a test file." > source.txt`; `echo -n "Existing content." > destination.txt` | `./cp source.txt destination.txt` | No output checked by GTest. | `0` | `cat destination.txt` prints `This is a test file.` |
| `SourceFileDoesNotExist` | `rm -f nonexistent.txt destination.txt` | `./cp nonexistent.txt destination.txt` | Exact error text is not checked. | Non-zero | `test ! -e destination.txt && echo OK` prints `OK`. |
| `InvalidArguments` | None | `./cp`; then `./cp source.txt` | Exact error text is not checked. | Non-zero for both commands | None |
| `SmallerNewFile` | `echo -n "This is the initial content of the destination file." > destination.txt`; `echo -n "New content." > source.txt` | `./cp source.txt destination.txt` | No output checked by GTest. | `0` | `cat destination.txt` prints exactly `New content.` with no leftover old bytes. |

Cleanup after file tests: `rm -f source.txt destination.txt large_source.txt large_destination.txt`.

---

## Task 04 - `mv`

| Test | Setup | Command/input | Expected output | Expected exit code | Extra verification |
|---|---|---|---|---|---|
| `MoveFile` | `echo -n "This is a test file." > source.txt` | `./mv source.txt destination.txt` | No output checked by GTest. | `0` | `cat destination.txt` prints `This is a test file.`; `test ! -e source.txt && echo OK` prints `OK`. |
| `MoveToExistingFile` | `echo -n "This is a test file." > source.txt`; `echo -n "Existing content." > destination.txt` | `./mv source.txt destination.txt` | No output checked by GTest. | `0` | `cat destination.txt` prints `This is a test file.`; source no longer exists. |
| `SourceFileDoesNotExist` | `rm -f nonexistent.txt destination.txt` | `./mv nonexistent.txt destination.txt` | Exact error text is not checked. | Non-zero | `test ! -e destination.txt && echo OK` prints `OK`. |
| `InvalidArguments` | None | `./mv`; then `./mv source.txt` | Exact error text is not checked. | Non-zero for both commands | None |
| `SmallerNewFile` | `echo -n "This is the initial content of the destination file." > destination.txt`; `echo -n "New content." > source.txt` | `./mv source.txt destination.txt` | No output checked by GTest. | `0` | `cat destination.txt` prints exactly `New content.`; source no longer exists. |
| `MoveLargeFile` | `python3 -c "open('large_source.txt','w').write('a'*1000000)"` | `./mv large_source.txt large_destination.txt` | No output checked by GTest. | `0` | `wc -c < large_destination.txt` prints `1000000`; source no longer exists. |

Cleanup after file tests: `rm -f source.txt destination.txt large_source.txt large_destination.txt`.

---

## Task 05 - `femtoshell`

The GTest helper writes the input to stdin and appends one final newline. The commands below use `printf` to do the same.

| Test | Setup | Command/input | Expected output | Expected exit code |
|---|---|---|---|---|
| `EchoCommand` | None | `printf 'echo Hello, World!\n' \| ./femtoshell` | `[PROMPT]Hello, World!\n[PROMPT]` | `0` |
| `ExitCommand` | None | `printf 'exit\n' \| ./femtoshell` | `[PROMPT]Good Bye\n` | `0` |
| `InvalidCommand` | None | `printf 'ls -l\n' \| ./femtoshell` | `[PROMPT]Invalid command\n[PROMPT]` | Non-zero |
| `PressEnterWithoutCommand` | None | `printf '\n\n\n' \| ./femtoshell` | `[PROMPT][PROMPT][PROMPT][PROMPT]` | `0` |
| `LargeNumberOfConsecutiveCommands` | None | `for i in $(seq 0 99); do echo "echo Command $i"; done \| ./femtoshell` | For each `i = 0..99`: `[PROMPT]Command i\n`, then `[PROMPT][PROMPT]`. | `0` |
| `EchoWithLargeTextWithoutSpaces` | None | `python3 -c "print('echo ' + 'a'*10000)" \| ./femtoshell` | `[PROMPT]` + 10000 `a` characters + `\n[PROMPT]` | `0` |
| `EchoWithLargeNumberOfArguments` | None | `python3 -c "print('echo ' + ' '.join('arg'+str(i) for i in range(1000)))" \| ./femtoshell` | `[PROMPT]arg0 arg1 ... arg999\n[PROMPT]` | `0` |

---

## Task 06 - `picoshell`

`picoshell` includes `echo`, `exit`, `pwd`, `cd`, and external commands.

| Test | Setup | Command/input | Expected output | Expected exit code |
|---|---|---|---|---|
| `PressEnterWithoutCommand` | None | `printf '\n\n\n' \| ./picoshell` | `[PROMPT][PROMPT][PROMPT][PROMPT]` | `0` |
| `HandleMultipleSpaces` | None | `printf 'echo    Hello     World!\n' \| ./picoshell` | `[PROMPT]Hello World!\n[PROMPT]` | `0` |
| `ExitCommand` | None | `printf 'exit\n' \| ./picoshell` | `[PROMPT]Good Bye\n` | `0` |
| `InvalidCommand` | None | `printf 'IamNotACommand_abcdefxyz\n' \| ./picoshell` | `[PROMPT]IamNotACommand_abcdefxyz: command not found\n[PROMPT]` | Non-zero |
| `EchoCommand` | None | `printf 'echo Hello, World!\n' \| ./picoshell` | `[PROMPT]Hello, World!\n[PROMPT]` | `0` |
| `LargeNumberOfConsecutiveCommands` | None | `for i in $(seq 0 99); do echo "echo Command $i"; done \| ./picoshell` | For each `i = 0..99`: `[PROMPT]Command i\n`, then `[PROMPT][PROMPT]`. | `0` |
| `EchoWithLargeTextWithoutSpaces` | None | `python3 -c "print('echo ' + 'a'*10000)" \| ./picoshell` | `[PROMPT]` + 10000 `a` characters + `\n[PROMPT]` | `0` |
| `EchoWithLargeNumberOfArguments` | None | `python3 -c "print('echo ' + ' '.join('arg'+str(i) for i in range(1000)))" \| ./picoshell` | `[PROMPT]arg0 arg1 ... arg999\n[PROMPT]` | `0` |
| `PwdCurrentDirectory` | Run from any directory. | `printf 'pwd\n' \| ./picoshell` | `[PROMPT]<current absolute path>\n[PROMPT]` | `0` |
| `PwdChangeToParentDirectory` | Run `cd ..` before starting the shell. | `printf 'pwd\n' \| /path/to/picoshell` | `[PROMPT]<parent directory absolute path>\n[PROMPT]` | `0` |
| `PwdChangeDirectory` | Run `cd /tmp` before starting the shell. | `printf 'pwd\n' \| /path/to/picoshell` | `[PROMPT]/tmp\n[PROMPT]` | `0` |
| `PwdChangeToNewDirectory` | `mkdir test_dir && cd test_dir` before starting the shell. | `printf 'pwd\n' \| /path/to/picoshell` | `[PROMPT]<absolute path ending in /test_dir>\n[PROMPT]` | `0` |
| `PwdLongPath` | Create and enter `long_path_test/dir_0/dir_1/.../dir_99`. | `printf 'pwd\n' \| /path/to/picoshell` | `[PROMPT]<absolute path ending in /long_path_test/dir_0/.../dir_99>\n[PROMPT]` | `0` |
| `CdCommand` | None | `printf 'cd /tmp\npwd\n' \| ./picoshell` | `[PROMPT][PROMPT]/tmp\n[PROMPT]` | `0` |
| `InvalidCdCommand` | Make sure `/invalid_directory` does not exist. | `printf 'cd /invalid_directory\n' \| ./picoshell` | `[PROMPT]cd: /invalid_directory: No such file or directory\n[PROMPT]` | Non-zero |
| `ExternalCommand` | The GTest helper writes the input itself to `/tmp/input.txt`. Manual setup: `printf 'cat /tmp/input.txt\n' > /tmp/input.txt`. | `printf 'cat /tmp/input.txt\n' \| ./picoshell` | It must run external `cat`; with the manual setup: `[PROMPT]cat /tmp/input.txt\n[PROMPT]`. The GTest only checks that output is not the unexecuted text without newline. | `0` |
| `ExternalCommandWithLargeNumberOfArguments` | Save the same long command line into `/tmp/input.txt`: `cat /tmp/input.txt` plus 50 more ` /tmp/input.txt` args. | Run that same long `cat` command through `./picoshell`. | `[PROMPT]`, then that long command line printed 51 times, each followed by `\n`, then `[PROMPT]`. | `0` |
| `CreateRemoveDir` | `rm -rf test_dir` | `printf 'mkdir test_dir\ncd test_dir\npwd\ncd ..\nrmdir test_dir\n' \| ./picoshell` | `[PROMPT][PROMPT][PROMPT]<current absolute path>/test_dir\n[PROMPT][PROMPT][PROMPT]` | `0` |

---

## Task 07 - `nanoshell`

`nanoshell` includes all `picoshell` behavior plus variables and `export`.

### Tests 1-18 - same behavior as `picoshell`, run on `./nanoshell`

| Test | Setup | Command/input | Expected output | Expected exit code |
|---|---|---|---|---|
| `PressEnterWithoutCommand` | None | `printf '\n\n\n' \| ./nanoshell` | `[PROMPT][PROMPT][PROMPT][PROMPT]` | `0` |
| `HandleMultipleSpaces` | None | `printf 'echo    Hello     World!\n' \| ./nanoshell` | `[PROMPT]Hello World!\n[PROMPT]` | `0` |
| `ExitCommand` | None | `printf 'exit\n' \| ./nanoshell` | `[PROMPT]Good Bye\n` | `0` |
| `InvalidCommand` | None | `printf 'IamNotACommand_abcdefxyz\n' \| ./nanoshell` | `[PROMPT]IamNotACommand_abcdefxyz: command not found\n[PROMPT]` | Non-zero |
| `EchoCommand` | None | `printf 'echo Hello, World!\n' \| ./nanoshell` | `[PROMPT]Hello, World!\n[PROMPT]` | `0` |
| `LargeNumberOfConsecutiveCommands` | None | `for i in $(seq 0 99); do echo "echo Command $i"; done \| ./nanoshell` | For each `i = 0..99`: `[PROMPT]Command i\n`, then `[PROMPT][PROMPT]`. | `0` |
| `EchoWithLargeTextWithoutSpaces` | None | `python3 -c "print('echo ' + 'a'*10000)" \| ./nanoshell` | `[PROMPT]` + 10000 `a` characters + `\n[PROMPT]` | `0` |
| `EchoWithLargeNumberOfArguments` | None | `python3 -c "print('echo ' + ' '.join('arg'+str(i) for i in range(1000)))" \| ./nanoshell` | `[PROMPT]arg0 arg1 ... arg999\n[PROMPT]` | `0` |
| `PwdCurrentDirectory` | Run from any directory. | `printf 'pwd\n' \| ./nanoshell` | `[PROMPT]<current absolute path>\n[PROMPT]` | `0` |
| `PwdChangeToParentDirectory` | Run `cd ..` before starting the shell. | `printf 'pwd\n' \| /path/to/nanoshell` | `[PROMPT]<parent directory absolute path>\n[PROMPT]` | `0` |
| `PwdChangeDirectory` | Run `cd /tmp` before starting the shell. | `printf 'pwd\n' \| /path/to/nanoshell` | `[PROMPT]/tmp\n[PROMPT]` | `0` |
| `PwdChangeToNewDirectory` | `mkdir test_dir && cd test_dir` before starting the shell. | `printf 'pwd\n' \| /path/to/nanoshell` | `[PROMPT]<absolute path ending in /test_dir>\n[PROMPT]` | `0` |
| `PwdLongPath` | Create and enter `long_path_test/dir_0/dir_1/.../dir_99`. | `printf 'pwd\n' \| /path/to/nanoshell` | `[PROMPT]<absolute deep path>\n[PROMPT]` | `0` |
| `CdCommand` | None | `printf 'cd /tmp\npwd\n' \| ./nanoshell` | `[PROMPT][PROMPT]/tmp\n[PROMPT]` | `0` |
| `InvalidCdCommand` | Make sure `/invalid_directory` does not exist. | `printf 'cd /invalid_directory\n' \| ./nanoshell` | `[PROMPT]cd: /invalid_directory: No such file or directory\n[PROMPT]` | Non-zero |
| `ExternalCommand` | Manual setup: `printf 'cat /tmp/input.txt\n' > /tmp/input.txt`. | `printf 'cat /tmp/input.txt\n' \| ./nanoshell` | It must run external `cat`; with setup: `[PROMPT]cat /tmp/input.txt\n[PROMPT]`. | `0` |
| `ExternalCommandWithLargeNumberOfArguments` | Save the same long 51-file `cat` command into `/tmp/input.txt`. | Run the same long command through `./nanoshell`. | `[PROMPT]`, then the command line printed 51 times with newline, then `[PROMPT]`. | `0` |
| `CreateRemoveDir` | `rm -rf test_dir` | `printf 'mkdir test_dir\ncd test_dir\npwd\ncd ..\nrmdir test_dir\n' \| ./nanoshell` | `[PROMPT][PROMPT][PROMPT]<current absolute path>/test_dir\n[PROMPT][PROMPT][PROMPT]` | `0` |

### Tests 19-26 - variables

| Test | Setup | Command/input | Expected output | Expected exit code |
|---|---|---|---|---|
| `VariableAssignment` | None | `printf 'you_are=a_programmer\n' \| ./nanoshell` | `[PROMPT][PROMPT]` | `0` |
| `VariableSubstitution` | None | `printf 'you_are=a_good_programmer\necho $you_are\n' \| ./nanoshell` | `[PROMPT][PROMPT]a_good_programmer\n[PROMPT]` | `0` |
| `VariableSubstitutionInPath` | GTest writes the whole shell input to `/tmp/input.txt`. Manual setup: `printf 'filename=input.txt\ncat /tmp/$filename\n' > /tmp/input.txt`. | `printf 'filename=input.txt\ncat /tmp/$filename\n' \| ./nanoshell` | `[PROMPT][PROMPT]filename=input.txt\ncat /tmp/$filename\n[PROMPT]` | `0` |
| `NonExistingVariableSubstitution` | Do not define `non_existing_var`. | `printf 'echo $non_existing_var\n' \| ./nanoshell` | `[PROMPT]\n[PROMPT]` | `0` |
| `NonExistingVariableSubstitutionInString` | Do not define `non_existing_var`. | `printf 'echo Hello$non_existing_var World!\n' \| ./nanoshell` | `[PROMPT]Hello World!\n[PROMPT]` | `0` |
| `ExportVariable` | None | `printf 'Linux_system_programming=FunForEver\nexport Linux_system_programming\nprintenv\n' \| ./nanoshell` | Output starts with `[PROMPT][PROMPT][PROMPT]`, then `printenv` output. It must contain `Linux_system_programming=FunForEver`. | `0` |
| `CreateRemoveDirWithVars` | `rm -rf test_vars_dir` | `printf 'MKDIR=mkdir\nDIR_NAME=test_vars_dir\n$MKDIR $DIR_NAME\ncd $DIR_NAME\npwd\ncd ..\nrmdir $DIR_NAME\n' \| ./nanoshell` | `[PROMPT][PROMPT][PROMPT][PROMPT][PROMPT]<current absolute path>/test_vars_dir\n[PROMPT][PROMPT][PROMPT]` | `0` |
| `EchoWithLargeNumberOfArgumentsWithVars` | None | `python3 -c "print(''.join(f'var{i}=value{i}\\n' for i in range(1000)) + 'echo ' + ' '.join(f'$var{i}' for i in range(1000)))" \| ./nanoshell` | 1001 prompts total before/around the final echo result; final line is `value0 value1 ... value999\n[PROMPT]`. | `0` |

---

## Task 08 - `microshell`

`microshell` includes all `nanoshell` behavior plus redirection with `>`, `<`, and `2>`.

### Tests 1-26 - same behavior as `nanoshell`, run on `./microshell`

| Test | Setup | Command/input | Expected output | Expected exit code |
|---|---|---|---|---|
| `PressEnterWithoutCommand` | None | `printf '\n\n\n' \| ./microshell` | `[PROMPT][PROMPT][PROMPT][PROMPT]` | `0` |
| `HandleMultipleSpaces` | None | `printf 'echo    Hello     World!\n' \| ./microshell` | `[PROMPT]Hello World!\n[PROMPT]` | `0` |
| `ExitCommand` | None | `printf 'exit\n' \| ./microshell` | `[PROMPT]Good Bye\n` | `0` |
| `InvalidCommand` | None | `printf 'IamNotACommand_abcdefxyz\n' \| ./microshell` | `[PROMPT]IamNotACommand_abcdefxyz: command not found\n[PROMPT]` | Non-zero |
| `EchoCommand` | None | `printf 'echo Hello, World!\n' \| ./microshell` | `[PROMPT]Hello, World!\n[PROMPT]` | `0` |
| `LargeNumberOfConsecutiveCommands` | None | `for i in $(seq 0 99); do echo "echo Command $i"; done \| ./microshell` | For each `i = 0..99`: `[PROMPT]Command i\n`, then `[PROMPT][PROMPT]`. | `0` |
| `EchoWithLargeTextWithoutSpaces` | None | `python3 -c "print('echo ' + 'a'*10000)" \| ./microshell` | `[PROMPT]` + 10000 `a` characters + `\n[PROMPT]` | `0` |
| `EchoWithLargeNumberOfArguments` | None | `python3 -c "print('echo ' + ' '.join('arg'+str(i) for i in range(1000)))" \| ./microshell` | `[PROMPT]arg0 arg1 ... arg999\n[PROMPT]` | `0` |
| `PwdCurrentDirectory` | Run from any directory. | `printf 'pwd\n' \| ./microshell` | `[PROMPT]<current absolute path>\n[PROMPT]` | `0` |
| `PwdChangeToParentDirectory` | Run `cd ..` before starting the shell. | `printf 'pwd\n' \| /path/to/microshell` | `[PROMPT]<parent directory absolute path>\n[PROMPT]` | `0` |
| `PwdChangeDirectory` | Run `cd /tmp` before starting the shell. | `printf 'pwd\n' \| /path/to/microshell` | `[PROMPT]/tmp\n[PROMPT]` | `0` |
| `PwdChangeToNewDirectory` | `mkdir test_dir && cd test_dir` before starting the shell. | `printf 'pwd\n' \| /path/to/microshell` | `[PROMPT]<absolute path ending in /test_dir>\n[PROMPT]` | `0` |
| `PwdLongPath` | Create and enter `long_path_test/dir_0/dir_1/.../dir_99`. | `printf 'pwd\n' \| /path/to/microshell` | `[PROMPT]<absolute deep path>\n[PROMPT]` | `0` |
| `CdCommand` | None | `printf 'cd /tmp\npwd\n' \| ./microshell` | `[PROMPT][PROMPT]/tmp\n[PROMPT]` | `0` |
| `InvalidCdCommand` | Make sure `/invalid_directory` does not exist. | `printf 'cd /invalid_directory\n' \| ./microshell` | `[PROMPT]cd: /invalid_directory: No such file or directory\n[PROMPT]` | Non-zero |
| `ExternalCommand` | Manual setup: `printf 'cat /tmp/input.txt\n' > /tmp/input.txt`. | `printf 'cat /tmp/input.txt\n' \| ./microshell` | It must run external `cat`; with setup: `[PROMPT]cat /tmp/input.txt\n[PROMPT]`. | `0` |
| `ExternalCommandWithLargeNumberOfArguments` | Save the same long 51-file `cat` command into `/tmp/input.txt`. | Run the same long command through `./microshell`. | `[PROMPT]`, then the command line printed 51 times with newline, then `[PROMPT]`. | `0` |
| `CreateRemoveDir` | `rm -rf test_dir` | `printf 'mkdir test_dir\ncd test_dir\npwd\ncd ..\nrmdir test_dir\n' \| ./microshell` | `[PROMPT][PROMPT][PROMPT]<current absolute path>/test_dir\n[PROMPT][PROMPT][PROMPT]` | `0` |

### Tests 19-26 - variables, run on `./microshell`

| Test | Setup | Command/input | Expected output | Expected exit code |
|---|---|---|---|---|
| `VariableAssignment` | None | `printf 'you_are=a_programmer\n' \| ./microshell` | `[PROMPT][PROMPT]` | `0` |
| `VariableSubstitution` | None | `printf 'you_are=a_good_programmer\necho $you_are\n' \| ./microshell` | `[PROMPT][PROMPT]a_good_programmer\n[PROMPT]` | `0` |
| `VariableSubstitutionInPath` | GTest writes the whole shell input to `/tmp/input.txt`. Manual setup: `printf 'filename=input.txt\ncat /tmp/$filename\n' > /tmp/input.txt`. | `printf 'filename=input.txt\ncat /tmp/$filename\n' \| ./microshell` | `[PROMPT][PROMPT]filename=input.txt\ncat /tmp/$filename\n[PROMPT]` | `0` |
| `NonExistingVariableSubstitution` | Do not define `non_existing_var`. | `printf 'echo $non_existing_var\n' \| ./microshell` | `[PROMPT]\n[PROMPT]` | `0` |
| `NonExistingVariableSubstitutionInString` | Do not define `non_existing_var`. | `printf 'echo Hello$non_existing_var World!\n' \| ./microshell` | `[PROMPT]Hello World!\n[PROMPT]` | `0` |
| `ExportVariable` | None | `printf 'Linux_system_programming=FunForEver\nexport Linux_system_programming\nprintenv\n' \| ./microshell` | Output starts with `[PROMPT][PROMPT][PROMPT]`, then `printenv` output. It must contain `Linux_system_programming=FunForEver`. | `0` |
| `CreateRemoveDirWithVars` | `rm -rf test_vars_dir` | `printf 'MKDIR=mkdir\nDIR_NAME=test_vars_dir\n$MKDIR $DIR_NAME\ncd $DIR_NAME\npwd\ncd ..\nrmdir $DIR_NAME\n' \| ./microshell` | `[PROMPT][PROMPT][PROMPT][PROMPT][PROMPT]<current absolute path>/test_vars_dir\n[PROMPT][PROMPT][PROMPT]` | `0` |
| `EchoWithLargeNumberOfArgumentsWithVars` | None | `python3 -c "print(''.join(f'var{i}=value{i}\\n' for i in range(1000)) + 'echo ' + ' '.join(f'$var{i}' for i in range(1000)))" \| ./microshell` | 1001 prompts total before/around the final echo result; final line is `value0 value1 ... value999\n[PROMPT]`. | `0` |

### Tests 27-37 - I/O redirection

| Test | Setup | Command/input | Expected output | Expected exit code | Extra verification |
|---|---|---|---|---|---|
| `RedirectOutputToFile` | `rm -f /tmp/output.txt` | `printf 'echo Hello, World! > /tmp/output.txt\n' \| ./microshell` | `[PROMPT][PROMPT]` | `0` | `cat /tmp/output.txt` prints `Hello, World!\n`. |
| `RedirectInputFromFile` | `echo -n "Hello, World!" > /tmp/input_iored.txt` | `printf 'echo Hello\ncat < /tmp/input_iored.txt\necho World!\n' \| ./microshell` | `[PROMPT]Hello\n[PROMPT]Hello, World![PROMPT]World!\n[PROMPT]` | `0` | None |
| `RedirectErrorToFile` | `rm -f /tmp/error.txt` | `printf 'ls non_existing_file_no_redirect\nls non_existing_file 2> /tmp/error.txt\nls non_existing_file_no_redirect_after\n' \| ./microshell` | `[PROMPT]ls: cannot access 'non_existing_file_no_redirect': No such file or directory\n[PROMPT][PROMPT]ls: cannot access 'non_existing_file_no_redirect_after': No such file or directory\n[PROMPT]` | Non-zero | `/tmp/error.txt` contains `ls: cannot access 'non_existing_file': No such file or directory\n`. |
| `RedirectOutputAndErrorToFile` | `rm -f /tmp/output.txt /tmp/error.txt` | `printf 'ls non_existing_file_o_e > /tmp/output.txt 2> /tmp/error.txt\n' \| ./microshell` | `[PROMPT][PROMPT]` | Non-zero | `/tmp/output.txt` is empty; `/tmp/error.txt` contains `ls: cannot access 'non_existing_file_o_e': No such file or directory\n`. |
| `RedirectOutputWithVariableSubstitution` | `rm -f /tmp/output_var.txt` | `printf 'filename=output_var.txt\necho Hello, World! > /tmp/$filename\n' \| ./microshell` | `[PROMPT][PROMPT][PROMPT]` | `0` | `/tmp/output_var.txt` contains `Hello, World!\n`. |
| `RedirectInputWithVariableSubstitution` | `echo -n "Hello, World!" > /tmp/input_var.txt` | `printf 'filename=input_var.txt\ncat < /tmp/$filename\n' \| ./microshell` | `[PROMPT][PROMPT]Hello, World![PROMPT]` | `0` | None |
| `RedirectErrorWithVariableSubstitution` | `rm -f /tmp/error_var.txt` | `printf 'filename=error_var.txt\nls non_existing_file_var 2> /tmp/$filename\n' \| ./microshell` | `[PROMPT][PROMPT][PROMPT]` | Non-zero | `/tmp/error_var.txt` contains `ls: cannot access 'non_existing_file_var': No such file or directory\n`. |
| `RedirectOutputToNonWritableLocation` | Run as a normal non-root user. | `printf 'echo Hello, World! > /root/output.txt\n' \| ./microshell` | `[PROMPT]/root/output.txt: Permission denied\n[PROMPT]` | Non-zero | None |
| `RedirectInputFromNonExistentFile` | `rm -f /tmp/non_existent_file.txt` | `printf 'cat < /tmp/non_existent_file.txt\n' \| ./microshell` | `[PROMPT]cannot access /tmp/non_existent_file.txt: No such file or directory\n[PROMPT]` | Non-zero | None |
| `RedirectionOrder1` | `rm -f /tmp/output.txt /tmp/err.txt /tmp/non_existent_file.txt` | `printf 'cat < /tmp/non_existent_file.txt > /tmp/output.txt 2> /tmp/err.txt\n' \| ./microshell` | `[PROMPT]cannot access /tmp/non_existent_file.txt: No such file or directory\n[PROMPT]` | Non-zero | `/tmp/output.txt` and `/tmp/err.txt` are empty. |
| `RedirectionOrder2` | `rm -f /tmp/output.txt /tmp/err.txt /tmp/non_existent_file.txt` | `printf 'cat > /tmp/output.txt 2> /tmp/err.txt < /tmp/non_existent_file.txt\n' \| ./microshell` | `[PROMPT][PROMPT]` | Non-zero | `/tmp/output.txt` is empty; `/tmp/err.txt` contains `cannot access /tmp/non_existent_file.txt: No such file or directory\n`. |

---

## Quick Checklist

| Task | All listed manual tests pass? |
|---|---|
| 01 `pwd` | [ ] |
| 02 `echo` | [ ] |
| 03 `cp` | [ ] |
| 04 `mv` | [ ] |
| 05 `femtoshell` | [ ] |
| 06 `picoshell` | [ ] |
| 07 `nanoshell` | [ ] |
| 08 `microshell` | [ ] |
