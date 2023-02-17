# lexdiff

Let's say you want to compare the content of two files, simple right?
Here's two example files:
```
$ cat a.txt
apples,bananas,peaches
dragonfruit,grapefruit
grapes,onions
$ cat b.txt
apples, bananas, peaches
dragonfruit
grapefruit
grapes, onions
```

Classic approach is to use `diff`, so let's try that:

```
$ diff a.txt b.txt
1,3c1,4
< apples,bananas,peaches
< dragonfruit,grapefruit
< grapes,onions
---
> apples, bananas, peaches
> dragonfruit
> grapefruit
> grapes, onions
```

Hmmm... I **think** the first line says the that the `c`hange bewteen files
spans from the `1`st and `3`rd line of the first file and the `1`st and
the `4`th file of the second file. Then `diff` gives the lines
that are exclusive to the first file, then a horizontal divider, then the lines
that are exclusive to the second file. This looks like `diff` is saying that
both files are completely different because `b.txt` has spaces after the commas
and there's a line break bewteen dragonfruit and grapefruit.
Well they don't look **completely** different to **me**! This is not helpful!

So what's going on here? `diff` compares the contents of each file line-by-line,
matching the lines **exactly**. Any byte difference in a line would cause
`diff` to consider the entire line non-matching. This approach succeeds at
finding differences, but it doesn't consider the **importance** of the differences.
To a human, these two files are basically the same, so we need something
that can identity
the kinds of the differences in a file, and know which kinds matter more than
others.

This is the goal of `lexdiff`. Instead of a line-by-line comparison, `lexdiff`
compares the contents of the two files **lexically**, meaning that it groups
characters/bytes into groups called "tokens" and compares the tokens against
each other. The intuition here is that the program is doing what humans
do when reading text: grouping letters into words, and extracting meaning
from a sentence by considering the interaction of the words as atomic units.

How `lexdiff` groups characters into tokens/words, how the user can communicate
to `lexdiff` the significance (or insignificance) of certain tokens, and how
`lexdiff` identifies the differences is covered in detail on
its [dedicated webpage](https://zanderthannhauser.github.io/lexdiff/).

















