# STB headers

Some of my own STB headers to help C along.

## Cvars

id Software introduced in-game consoles with CVars that allowed
modders to change game vars at runtime. This is pretty much that.

## Job system

While not as analogous as the co-routine/go-routine, this is a job
system inspired by the Naughty Dog talk. It's synchronous, but
different job systems could be created in separate threads to run
asynchronously. It would be up to the user to determine decent
moments at which to yield for other functions. Purely for
recreational use, will eventually migrate to a proper context-switching
library with the same API.
