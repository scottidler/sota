sota
====
````
sota = (State of the Art)
    inspired by =
        | python | yaml
        | c# | c | c++
        | perl | ruby
        | f# | haskel
        | bash | Javascript
````

Sota aims to satisfy all of the annoyances in various programming
languages I have experienced over the last 10 years.  I also plan
to beg, borrow and steal the best ideas I have seen implemented in
other languages.  There is no shame in using what works and new
languages are most successful when their core is recognizable to
would be users.  Sota's syntax borrows heavily from python and yaml.
It doubles down on the context of newlines and indentations.  It also
borrows heavily from functional programming concepts; closures,
lambdas, etc.  In short I hope this to be the sum of everything I
think works well while minimizing the annoyances and overly verbose
syntax.

The requisite hello world:
````
#!/usr/bin/sota

main = ()
    print 'hello world!'
````
After I get the core language implemented I plan to tackle
meta-programming, concurrency | threading and asynchronous support.  I
could use help implementing the language, and later libraries as
well as design what next features will make it truly the State of the
Art.  If you're interested, please contact me.

Features & Origins
==================

Most if not all of Sota's feature set is lifted from other programming
languages.  Here is a list of those features and where they were taken
from.
````
lifted =
    | regex: modified but from Perl
    | piping: from f# and bash
    | match: from f#
    | enums: from f#
    | list, dicts, sets oh my: from python
    | comprehensions: python
    | lambda syntax: mix of c# and python
    | return the last expression in a function: f#
    | inline syntax: mix of c-like langs and yaml
    | namespaces: from c++ without the keyword or the braces
    | string interpolation: mix of python, c# and ruby
    | ranges: lifted straight from ruby

invented =
    | path concat: too lazy to write os.path.join (Python)
    | definition via assignment: done before? maybe I get some credit; nope js!
    | public vs private capitalization: laziness and removing keywords
    | inverting dicts: I have wanted this forever!
    | accessing dicts via expressions other than keys: just makes sense
    | this keyword simplified to a single, leading dot: all keywords must die!
    
````
Capitalization
==============

Sota is primarily written and intended for use in Linux (Windows
later if I have to).  As such lowercase is preferred if at all possible.
Capitalization is allowed and used in Sota, but sparingly.

Keywords
========

It annoys me that languages consume a lot of good identifier names
with keywords.  Sota aims to reduce the list of reserved words.
````
removed =
    | return
    | class | struct (type is used instead for both)
    | func | function | def
    | for (foreach replaces all uses)
    | switch | case
    | namespace
    | static
    | void
    | goto
    | public | protected | private
    | virtual | override
    | interface

kept =
    | if | then | elif | else
    | do | while
    | foreach
    | continue | break
    | print | debug | trace
````

Regular Expressions
===================

Regex in Sota are first-class and built-in.  No need to import some
library to get access to one of the most important tools for the
programmer!  The regular expressions are largely inspired by Perl.
However, the =~ operator has been removed.  The implementation
should be fast and complete.

Syntax
======

Sota's syntax is inspired by my many years working with python and
yaml.  The use of newlines and indents provide good markers for the
parser and make it easy for the human eye to follow, thereby ideal
for a programming language.  Whatever could be removed to distill
the language just to the bare essentials.  For example python
requires a colon : just before an indent (ie if-then-else and function
def).  Sota removes this and similar unneeded syntax.

Lambda expressions and other functional ideas are becoming essential
for the programmers toolkit.  Therefore why not make the named
function definition as much like the syntax of the lambda function?
Sota does this by making all definitions of function, types, and
enums as assignment expressions.
````
# a lambda expression
(name) -> print name

# a function definition
print_name = (name) -> print name

# enumerations are defined via assignment as well
Fruit =
    | Apple
    | Banana
    
# the same goes for types
Person = Type(object)
    .new = (name, age, sex)
        .name = name
        .age = age
        .sex = sex
````

Although Sota is primarily written in the flow style (like
Python) it also has support for inlining which uses braces and
semi-colons (like c|c++|c#|java) when necessary.
````
# here is an inlined function utilizing braces and semi-colons
# because of the more than one statement
add = (x, y) -> { z = x + y; print '{x} + {y} = {z}' }

# note: if we reduce the statements in the body to 1 we don't
# need the braces and semi-colons
add = (x, y) -> print '{x} + {y} = {x + y}'
````

Finally, Sota style will never dictate line lengths at 80 or some
other ridiculous number.  We are all adults and don't care about
the width of punch-cards in the 60s and 70s!

Switch | Case vs Match
======================

Sota is going to steal as much as possible the f#|ocaml style of
match expressions (not regex).  These are much more powerful version
of the switch statements.  These add a lot of expressive power
for the programmer to do conditionals.
````
# here is a fibonacci algorithm taken from the examples/fibonacci.sota file
fib5 = (n)
    match n
        | 0 | 1 -> n                     # this matches|returns 0 or 1
        | _ -> fib5(n - 1) + fib5(n - 2) # this matches everything else
````
Similar and related are enumerations.  Here again Sota will steal
from f#|ocaml.  In the simple case these behave like simple int
enums.  However, they can also be a union of different types.

Roadmap
=======

Sota is going to be a handwritten recursive descent parser written in
c++11 taking advantage of the powerful llvm backend.  Compiled, jitted
and interpreted options will be explored with possible support for
all three.
````
milestones =
    | v0.1 implement basic language with llvm backend working
    | v0.5 add meta-programming support
    | v0.8 add some core libraries written in sota
    | v1.0 stable workable version
    | v1.1 add concurrency programming support
    | v1.2 add asynchronous programming support
````
Of course this rough outline could change, but I could use your help.

Thanks,
Scott Idler
