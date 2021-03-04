# aplus-jvm
[![Build Status](https://travis-ci.com/kwrx/aplus-jvm.svg?branch=main)](https://travis-ci.com/kwrx/aplus-jvm)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](/LICENSE)

Simple Java Virtual Machine developed for [aplus](https://github.com/kwrx/aplus)

### How to run?
```sh
# Clone repository
$> git clone https://github.com/kwrx/aplus-jvm
$> cd aplus-jvm

# Configure & Build
$> ./configure --prefix=DESTDIR
$> make
$> make install
```

**NOTE:** Replace ```DESTDIR``` with your destionation path, default: ```/usr/local```  
**NOTE:** See ```./configure --help``` for more infos


### Example

```java
//
// Calc.java
//                         
public class Calc {                            
 public static int Sum(int a, int b) {         
     return a + b;                             
 }                                             
}                                             
```

```cpp
//
// main.c
//
int main(int argc, char** argv) {

    /* Initialize VM */
    avm_init();

    /* Open Calc.class */
    if(avm_open("Calc.class") == J_ERR)
        { perror("Calc.class"); abort(); }


    /* Initialize context & resolve assemblies */
    avm_begin();


    j_value a, b;
    
    a.i32 = 10;
    b.i32 = 15;

    /* c = Calc.Sum(a, b); */
    j_value c = avm_call("Calc", "Sum", 2, a, b);

    /* Destroy context & all resources */
    avm_end();

}
```


## License

Copyright (c) Antonino Natale. All rights reserved.

Licensed under the [MIT](/LICENSE) license.