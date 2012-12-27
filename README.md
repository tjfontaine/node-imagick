imagick is a binary binding to the following imagemagick commands

 * mogrify
 * convert
 * composite
 * identify
 * compare
 * conjure
 * stream
 * import
 * display
 * animate
 * montage

```javascript
var imagick = require('imagick');

imagick.identify(['-format', '%m:%f %wx%h', '/path/to/file.png'], function (err, metadata) {
  console.log(metadata);
});

// when run against the http://nodejs.org logo, prints
// PNG:logo.png 245x66
```

You simply pass in an array of arguments you would pass to the command line application
and get the response. If there is an error the exception will be in error.

*NOTE* Not all commands generate metadata such that your callback will have the results.
Some commands will merely continue to spew on your console unless you tweak the
command line parameters not to.

Why use this instead of node-imagemagick
========================================

The main reason for this library to exist is such that you're not spawn'ing a new
process for every command you need to run. You'll only need to instantiate the
imagemagick memory once and then continue to run the commands you need over and
over.

For comparison a [quick benchmark](https://gist.github.com/4391835) that serially
identifies the same image 1000 times in a row:

 * imagick was able to do 1000 identifies in 0.323 seconds
 * imagemagick did 1000 in 5.366 seconds.

This test is fast because of the builtin caching that ImageMagick does,
if you spawn a new process for each command you eliminate the usefulness of
that cache.

Why not to use this
===================

Using node-imagemagick spawns a new process per command, which means the operating system
is handling the scheduling, and it's much more likely you'll spread your work
load over more cores. To achieve the same with imagick you would need to spawn
worker child_process and distribute workload.

Another reason is, using imagick will mean that ImageMagick memory will be shared
with your existing node process. This may mean your node process will appear to
use more memory, and that your node application may be adversely affected by
a memory leak in ImageMagick or imagick. Using node-imagemagick means that when
the processing is done the memory associated with that processing is also released.

And finally the last reason command piping, with imagick there is no way to pipe
multiple commands together in a processing chain, this is something that is
easily achieved with child_process and exec'ing the actual ImageMagick utilities.
Or optionally with a more involved MagickWand interface.

