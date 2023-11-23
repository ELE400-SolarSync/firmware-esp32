# Documentation project

How to generate the documentation for the project.

## Tutorial
Links to [tutorials](https://medium.com/@aytackahveci93/documenting-c-code-with-sphinx-d6315b338615) on how to generate doc for the project.
You can find the requirements for the project in the [requirements.txt](requirements.txt) file.

## Documentation
The documentation is generated using [Doxygen](https://www.doxygen.nl/index.html) and [Sphinx](https://www.sphinx-doc.org/en/master/).
The documentation format is [reStructuredText](https://sublime-and-sphinx-guide.readthedocs.io/en/latest/index.html).

## Commands
```\bash
  cd docs
  doxygen Doxyfile.in
  sphinx-build -b html -Dbreathe_projects.solarsync=doc_out/xml . doc_out/sphinx/
```

Documents will be generated in the docs/doc_out/sphinx/index.html.