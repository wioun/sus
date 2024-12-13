# SuS
Execute the command as another user using su.

## What's that?
This is a su-based program for executing commands on behalf of other users. It was created to simplify the use of `su -c` as a lightweight replacement for sudo.
But unlike sudo, it asks for the target user's password instead of the current user's, and does it every time.

## How to use?
For example, use:
```
sus rm -rf /tmp/*
```
The root user is used by default.

To specify a user, use the -u argument:
```
sus -u admin rm -rf /tmp/*
```
The argument can be specified only at the beginning, otherwise it will be interpreted as part of the command to be executed.

## Installation
Any su implementation that has the -c argument must be installed on the system.

Clone the repository:
```
git clone https://github.com/wioun/sus
```

Go to the repository directory:
```
cd sus
```

Compile with any convenient C compiler:
```
cc ./sus.c -o ./sus
```

Install the binary (as root):
```
install -m 755 ./sus /usr/bin
```

If you're using bash-completion, install sus_completion.bash to /etc/bash_completion.d/ (as root):
```
install -m 644 ./sus_completion.bash /etc/bash_completion.d/sus_completion.bash
```
