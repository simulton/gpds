[![Packaging status](https://repology.org/badge/vertical-allrepos/gpds.svg)](https://repology.org/project/gpds/versions)

GPDS is a **G**eneral **P**urpose **D**ata **S**erializer implemented as a very small C++ library.
It allows to serialize C++ classes to and from XML/YAML files in a generic format
that can be processed by other XML/YAML processing software
(or just for the sake of readability).

Consider the following C++ class:
```cpp
class Color {
public:
	std::string name;
	int red;
	int blue;
	int green;
};
```
Most serializers would produce the following output when serializing to XML:
```xml
<valuelist>
    <value type="string">Black</value>
    <value type="int">0</value>
    <value type="int">0</value>
    <value type="int">0</value>
</valuelist>
```
This is not really practical when we want to process the same XML file with other
software. GPDS on the other hand produces the following output:
```xml
<color name="Black" format="rgb">
    <red depth="32">0</red>
    <green depth="32">0</green>
    <blue depth="32">0</blue>
</color>
```
GPDS not only supports XML, but also supports YAML:
```yaml
- color:
    "-format": rgb
    "-name": Black
    blue:
        "#text": 0
        "-depth": 32
    green:
        "#text": 0
        "-depth": 32
    red:
        "#text": 0
        "-depth": 32
```

For more information, see https://gpds.simulton.com
