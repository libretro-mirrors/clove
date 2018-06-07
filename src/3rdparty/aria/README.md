# aria
A *tiny*, easily embeddable lisp-shaped language implemented in C89.

```lisp
(do

  (= fib (function (n)
    (if (>= n 2)
        (+ (fib (- n 1)) (fib (- n 2)))
        n)))

  (print (fib 20))) ; prints 6765
```


## Overview
* Supports numbers (floats), symbols, strings, pairs, vectors, lambdas, macros
* Stack tracebacks with filename and line numbers on error
* Exception-like error handling
* Simple implementation and garbage collector
* Easy-to-use C API
* FFI
* Cross-platform (Linux,MacOS and Windows)

## Contribuitors
1. Murii.
2. rxi.
3. BitPuffin.
4. MasterGeek_

## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.
