# ZumoSegway (WIP)

ZumoSegway is a project that contains the entire design flow, down to
implementation, of Segway behavior on a *zumo32u4* board.

## Documentation

The documentation is available at *TBD*

### Build the documentation

The documentation of this project was developed using [Sphinx](). To build the
documentation we recommend to create a ```Python``` virtual environment
first by running;

```
cd <project-root>
virtualenv venv
```

Install all the dependencies for building the documentation by runningl

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

Finallay you can open the ```index.html``` file at
**<project-root>/doc/build/html/index.html** to navigate the documentation.

## Developer Guide

The project's documentation also contains development guides.


# Build and Deploy

To develop and test the code we used [PlatformIO](http://platformio.org/). We
recommend you to use so because the building system is highly integrated with
PlatformIO interface.

Further tools to develop and deploy the code are going to be provided later on.

# Design Workflow

The idea of this project is to have a model developed on Scilab/XCos running on
the *zumo32u4* board. For achieving this some code generation will be used.

Once the code is generated we used **PlatformIO** to deploy the code in the
board.

# Project Management

To manage the development of this project we use (Taiga)[http://taiga.io/].
Click (here)[] to see our project in Taiga.

# Issues tracking

Issues of this project are tracked using both Github's and Taiga's Issues
tracking systems.

# TODO

We are just starting so this doesn't make sense, yet!
