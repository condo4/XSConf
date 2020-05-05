# XSConf
eXtensible Simple Configuration file


Very simple Configuration class.

```
# Exemple: /etc/myapp.conf

foo = bar
toto=tutu
array1[] = item1
array1[] = item2
array1[] = item3
array1[] = item4
array1[] = item5
array1[] = item6

[SectionTutu]
foo=CaFe

```

```
# Exemple: /etc/myapp.conf.d/overlay.conf

newvar = 12
foo = toto
```

```cpp
/* C++ Usage */

#include <xsconf.h>
#include <stdio>

int main(int argc, char *argv[])
{
    XSConf conf("myapp");

    std::cout << conf["foo"] << std::endl; // Print toto
    std::cout << conf["SectionTutu/foo"] << std::endl; // Print CaFe

    return 0;
}
```

```python
# Python example

from xsconf import XSConf

XSConf conf("myapp")

print("TOTO: %s"%conf["toto"]) # Print tutu
```
