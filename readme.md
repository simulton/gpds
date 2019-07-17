GPDS is a **G**eneral **P**urpose **D**ata **S**erializer implemented as a very small C++ library.
It allows to serialize C++ classes to an from XML files in a generic format that can be processed
by other XML processing software.

Consider the following C++ class:
```
class Color {
public:
	std::string name;
	int red;
	int blue;
	int green;
};
```
Most serializers would produce the following output when serializing to XML:
```
<valuelist>
    <value type="string">Black</value>
    <value type="int">0</value>
    <value type="int">0</value>
    <value type="int">0</value>
</valuelist>
```
This is not really practical when we want to process the same XML file with other software. GPDS 
on the other hand produces the following output:
```
<color name="Black" format="rgb">
    <red depth="32">0</red>
    <green depth="32">0</green>
    <blue depth="32">0</blue>
</color>
```

For more information, see https://gpds.simulton.com
