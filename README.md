# Installation

Run the following command to download pybind11:

```
git clone https://github.com/pybind/pybind11.git
```
    
Then, build the extension module with the following command:
    
```
cmake . && make
```


To test the module, run the following command:
    
```
python3 -c "import cpp_string_lookup; s = cpp_string_lookup.StringHolder(['23234', 'asdfasdf', '234243']); print(s.lookup('23234'))"
```
