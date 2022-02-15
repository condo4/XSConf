# GlobalSettings
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

#include <globalsettings.h>
#include <iostream>

int main(int argc, char *argv[])
{
    GlobalSettings conf;

    std::cout << std::get<std::string>(conf.get("foo")) << std::endl; // Print toto
    std::cout << std::get<std::string>(conf.get("foo", "SectionTutu")) << std::endl; // Print CaFe

    conf.set("foo", "baz");

    return 0;
}
```

```python
# Python example

from pyglobalsettings import GlobalSettings

conf = GlobalSettings()

print("TOTO: %s" % conf.get("toto")) # Print tutu
print("FOO: %s" % conf.get("foo", "SectionTutu", "default")) # Print CaFe
```
