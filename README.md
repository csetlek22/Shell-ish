# COMP 304 - Shell-ish Assignment 1
Student:Caner Şetlek
GitHub Repository: https://github.com/csetlek22/Shell-ish.git

To compile the shell, run the following command:
`gcc -o shell-ish shellish-skeleton.c`

To compile the chatroom application:
`gcc -o chatroom chatroom.c`

To run the shell:
`./shell-ish` or `rlwrap ./shell-ish`

Custom Command: `roll`
For the custom command part, I implemented a dice rolling simulator called `roll`. It generates a random number based on the given sides. defaul is 6

Usage:
* `roll` : Rolls a standard 6-sided dice (returns a number between 1 and 6).
* `roll <N>` : Rolls an N-sided dice (e.g., `roll 20` returns a number between 1 and 20).

Example:
$ roll
🎲 6 yuzlu zar atildi. Gelen sayi: 4
$ roll 100
🎲 100 yuzlu zar atildi. Gelen sayi: 87
