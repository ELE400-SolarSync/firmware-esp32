## Documentation project

How to generate the documentation for the project.

## Tutorial

Links to [tutorials](https://medium.com/@aytackahveci93/documenting-c-code-with-sphinx-d6315b338615) on how to generate doc for the project.

## Commands

# Linux

```\bash
  cd docs
  doxygen Doxyfile.in
  sphinx-build -b html -Dbreathe_projects.solarsync=doc_out/xml . doc_out/sphinx/
```

# Windows

```bash
  sphinx-build -b html -D breathe_projects.solarsync=doc_out\\xml . doc_out\\sphinx
```

Documents will be generated in the docs/doc_out/sphinx/index.html.
