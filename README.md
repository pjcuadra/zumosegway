# ZumoSegway

ZumoSegway is a project that contains the entire design flow, down to
implementation, of Segway on a *zumo32u4* board.

## Documentation

The documentation is available
[here](http://zumosegway.readthedocs.io/en/latest/)

### Build the documentation

The documentation of this project was developed using
[Sphinx](http://www.sphinx-doc.org/en/stable/). To build the documentation we
recommend to create a ```Python``` virtual environment first by running;

```
cd <project-root>
virtualenv venv
```

Install all the dependencies for building the documentation by running;

```
cd <project-root>/doc
pip install -r requirements.txt
```

Activate the virtual environment by running;

```
source venv/bin/activate
```

Now you can build the documentation by running;

```
cd <project-root>/doc
make html
```

Finallay you can open the `index.html` file at
`<project-root>/doc/build/html/index.html` to navigate the documentation.
