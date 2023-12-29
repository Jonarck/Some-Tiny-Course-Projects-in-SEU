The distribution of RDFox consists of the following files and directories:

- The "examples" directory contains demonstration programs that show how to call RDFox as a library from Java.
  The "examples/Java/build.xml" Apache Ant script can be used to compile and run the Java demonstration program.

- The 'lib' directory contains the libraries.

  * JRDFox.jar is the Java bridge to the C++ native RDFox engine.
  * RDFox (on Mac OS X and Linux) and RDFox.exe (on Windows) are stand-alone executables
    that can be used to run RDFox on the command line. The system provides a shell that
    can load an RDF file, materialize facts w.r.t. a set of rules, and answer SPARQL queries.
  * libRDFox.dylib (on Mac OS X), libRDFox.so (on Linux), and libRDFox.dll (on Windows)
    are dynamic libraries that implement the native methods of JRDFox.

To use JRDFox in your project, simply add JRDFox.jar to your classpath,
and make sure that the path to the dynamic library is correctly specified  when starting
your program using the following JVM option:

    -Djava.library.path=<path>

To set up an Eclipse project that uses JRDFox, you can specify the path to the dynamic libraries
by specifying the "Native library location" for the JRDFox.jar library in the "Java Build
Path -> Libraries" part of the project properties page.
