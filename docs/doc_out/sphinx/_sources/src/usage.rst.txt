=====
Usage
=====

How to use the serial communication
===================================

The serial communication is used to communicate with the ESP32.
The ESP32 is connected to the computer with a USB cable.

Recommended tools
-----------------

To use the serial communication you will need to install a serial communication tool like :
    * `CoolTerm <https://freeware.the-meiers.org/>`_
    * `Realterm <https://sourceforge.net/projects/realterm/>`_
    * `Arduino IDE <https://www.arduino.cc/en/software>`_

Available commands
------------------

* ``help`` : Display the list of available commands.
* ``log -l <level>`` : Set the log level.
* ``log -o <output>`` : Select the output of the log (SD or terminal or both).
* ``data -e`` : Enable the data printing in the terminal.
* ``data -d`` : Disable the data printing in the terminal.
* ``wifi -s <ssid>`` : Set the ssid of the wifi.
* ``wifi -p <password>`` : Set the password of the wifi.
* ``wifi -c`` : Connect to the wifi.
* ``stop`` : Stop the state machine.
* ``start`` : Start the state machine.

How to write documentation
==========================

The documentation is written in `Restructured Text <http://docutils.sourceforge.net/rst.html>`_ and is built using `Sphinx <http://sphinx-doc.org/>`_ and `Doxygen <https://www.doxygen.nl/>`_.

File Structure
--------------

::

    firmware-esp32
    ├── docs        
    │   ├── doc_out
    |   |   ├── html
    |   |   ├── latex
    |   |   ├── sphinx
    |   |   └── xml
    │   ├── ressouce
    │   |   src
    |   |   ├── installation.rst
    |   |   ├── module.rst
    |   |   └── usage.rst
    │   ├── conf.py
    │   ├── Doxyfile.in
    │   ├── index.rst
    |   └── requirements.txt
    ├── include
    ├── lib         
    ├── src
    └── test

Folder explanation :
    * The folder doc_out is the output of the generated documentation. To read the documentation you will need to openthe file in docs/doc_out/sphinx/index.html.
    * The folder ressource contains the images.
    * The src folder contains the pages of the documentation.
    * The file conf.py is the configuration file for sphinx.
    * The file Doxyfile.in is the configuration file for doxygen.
    * The file index.rst is the main page of the documentation.
    * The file requirements.txt is the file that contains the requirements in order to build the doculentation for the documentation.

How to write in rst
-------------------

The rst syntax is very simple. You can find a tutorial `here <https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html>`_.

Here are some examples of rst syntax :
    * To write a title you need to write the title and then underline it with a line of equal sign.
    * To write a subtitle you need to write the title and then underline it with a line of dash.
    * To write a paragraph you just need to write the text.
    * To write a list you need to start the line with a star.

How to write in doxygen
-----------------------

First of all you will need to add docstring in the code.
If you are on VSCode you can download the extension `Doxygen Documentation Generator <https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen>`_.
Then you can add the docstring by typing the following command : ``/**`` and then press enter.
This will automatically add the docstring in the code.
Lastly you can type the description of the function or the class and, if they are, the parameters and the return value.

If you want to add a doxy class in the documentation you need to add the following line in the rst file :

::

    ..doxyclass:: class_name
        ::members

This will automatically add the docstring of the class in the documentation.

How to build the documentation
------------------------------

To build the documentation you will need to install sphinx and doxygen.
The version for this project can be find in the file requirements.txt.
To install the requirements you can type the following command :

.. code-block:: bash

    pip install -r requirements.txt

Then you will need to go in the docs folder and type the following command :

.. code-block:: bash

    cd docs
    doxygen Doxyfile.in
    sphinx-build -b html -Dbreathe_projects.solarsync=doc_out/xml . doc_out/sphinx/