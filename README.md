# Xper6tem
A program that takes a set of rules and initial facts an use them to answer queries.

### Usage
make

./Xper6tem <input_file>

### Input file format
Facts and queries are uppercase letters of the alphabet.

The rules are made of a condition, a equivalence sign `=>` or `<=>`, and a result, followed by a new line.

A `=>` means `then` and a `<=>` means `if and only if`.

The condition and the results are made of uppercase letters of the alphabets, `+ | ^ !` operations, and parenthesis `()`.

The + is equivalent to the binary `&`.

The facts line is a `=` followed by uppercase letters that will be known as true from the beginning of the program. 

All the letters that are not initial facts are false until proven true by application of a rule.

The queries line is a `?` followed by uppercase letters, which status we want to know at the end of the program. To do so, the program will use a backward chaining algorithm.

#### Example of an input file
```
P + A => Q
I ^ K => P
O => !P
K + L => O
D + E + M => N
L ^ K => M
I | C => J + !K
G + A => I
F => !H
E => F + (G | H)
D => E ^ C
B => !C
A => B

=AD
?BEFNOPQ
```

#### Expected output
```
P + A => Q
I ^ K => P
O => !P
K + L => O
D + E + M => N
L ^ K => M
I | C => J + !K
G + A => I
F => !H
E => F + (G | H)
D => E ^ C
B => !C
A => B
=AD
?BEFNOPQ
B is true
E is true
F is true
N is not true
O is not true
P is true
Q is true
```
