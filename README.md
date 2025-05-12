Here is your full `README.md` file content, ready to copy and paste into your GitHub repository:

---

```markdown
# ParseGPX

This project is an implementation of a **GPX file parser** in C, developed as part of a CS223 assignment. It focuses on extracting structured GPS data from XML-based GPX files using a **state machine**, without dynamic memory allocation.

## 🚀 Objectives

- Practice using C control flow and I/O
- Implement a finite state machine
- Process structured XML-like data with strict memory constraints
- Extract and clean GPS coordinate data from GPX files

## 📄 Description

GPX (GPS Exchange Format) files store tracks of location data collected by GPS devices. This program reads a GPX file from **standard input** and extracts a clean list of **track points** (`trkpt` elements), each including:

- `latitude` (`lat` attribute)
- `longitude` (`lon` attribute)
- `elevation` (`ele` element text)
- `timestamp` (`time` element text)

The output is printed to **standard output** in CSV format:

```

latitude,longitude,elevation,time

```

One line per track point. Commas within data are escaped as `&comma;`.

### ✅ Example Output

```

41.3078680,-72.9342120,20.0,2018-08-24T13:49:45Z
41.3078680,-72.9342120,20.0,2018-08-24T13:49:46Z
41.3078810,72.9342590W,20.0,2018-08-24T13:49:49Z

````

## ⚙️ Compilation

Use the provided `Makefile` to compile the program:

```bash
make
````

This will generate an executable named `ParseGPX`.

## 💡 Constraints

* No dynamic memory allocation allowed
* Only constant-size memory usage
* Case-insensitive parsing for XML tags
* Input is assumed to be well-formed GPX unless stated otherwise
* Program must never crash or hang on malformed input

## 🧪 Usage

To run the parser:

```bash
./ParseGPX < input.gpx
```

---
