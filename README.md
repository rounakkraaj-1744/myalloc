# High Level Architecture

```plaintext
                    +-------------------+
                    |     myalloc()     |
                    +-------------------+
                               |
             +----------------------------------+
             |                                  |
      +-------------+                  +----------------+
      | Size Class  |                  | Large Requests |
      | Selection   |                  | Handling       |
      +-------------+                  +----------------+
             |
      +----------------------+
      | Segregated Free List |
      +----------------------+
             |
      +----------------------+
      | Block Split / Merge  |
      +----------------------+
             |
      +----------------------+
      | Memory Pool Manager  |
      +----------------------+
```

# Memory Layout
```plaintext
+------------------------------------------------+
| HEADER | USER DATA | HEADER | USER DATA | ... |
+------------------------------------------------+
```

# Each Node Contains:
```plaintext
[Block Metadata][Actual User Memory]
```