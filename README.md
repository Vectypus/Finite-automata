# Finite automata

Operations on Finite Automata and word recognition

## Prerequisities

You will need gcc to compile this program.
Open the terminal in the main directory and execute this command:

```
$ gcc *.c -o fa && ./fa
```

## Use

### Create an automaton

You can use an existing automaton or create your own one with the following syntax:

```
nbAlpha
nbStates
nbInit [list initStates sep=' ']
nbTerm [list termStates sep=' ']
nbTrans
[list trans sep='\n']
```

Then, save the file in /automata with the name `#<automaton_number>.txt`.

### Operations

1. The program reads an automaton from a txt file in /automata. To load an automaton in the program, just write its number when asked "Which automaton do you want to use ?"

2. When loaded, the automaton will be displayed by indicating its initial states, final states and transition table.

3. After that, the automaton will be determinized and completed.

4. The next step is minimization, where states are renamed according to a table of correspondence. The all process is detailed.

5. You can now write words and see if this automaton recognizes them. Write "\*" for the empty word.

6. The last step is complementary language and standardization. You can after these operations, write words again (the language is the complementary of the original automaton).

### Specifications

* The alphabet consists of characters from "a" to "z". For example, an automaton whose alphabet has a size of 3 will use the characters "a", "b" and "c".

* States are labeled by numbers starting from 0. There is no limit on the number of states an automaton can contain.

* Asynchronous automata are supported. An epsilon-transition is represented by the symbol "\*".