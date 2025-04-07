# regex
A regex engine built with C for recreational programming purposes.


# Note
<pre>regex is still under construction, bugs may appear from nowhere</pre>

# Expectations
This project is expected to reach a state where it can operate on regular expressions and test string matching.

# Now
regex is able to lex, parse, compile regular expressions into <a href="https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton">NFAs</a> and use <a href="https://graphviz.org/">graphviz</a> for automaton visualization.

# Examples
```regex: "a+b"```

![a+b](./assets/nfa.regex:a+b.png)

```regex: "a|b"```

![a|b](./assets/nfa.regex:a|b.png)

```regex: ".*"```

![a|b](./assets/nfa.regex:.*.png)
