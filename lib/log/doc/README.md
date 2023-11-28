We will assume that we are doing 1Mo log file max.
We will store up to 1Go of log files in each folder.
If we reach the max size of the sd (-10%), we will delete the oldest log files.
Then we just keep incrementing the log file number.

Command to generate the doc:
```bash
cd docs
doxygen Doxyfile.in
sphinx-build -b html -Dbreathe_projects.solarsync=doc_out/xml . doc_out/sphinx/
```