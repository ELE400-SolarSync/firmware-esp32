## Documentation project

How to generate the documentation for the project.

## Commands
```\bash
  cd docs
  doxygen Doxyfile.in
  sphinx-build -b html -Dbreathe_projects.solarsync=doc_out/xml . doc_out/sphinx/
```

Documents will be generated in the build/docs/sphinx folder.