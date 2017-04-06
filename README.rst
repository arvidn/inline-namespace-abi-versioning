
using inline namespaces for ABI versioning
==========================================

This repository is an example / test of how to gradually introduce proper ABI
versioning to a library.

The problem
-----------

ABI comptibility (Application Binary Interface) is the collection of everything
that a caller and a callee need to agree on for a function call to work correctly
from a client into a library.

It includes things like:

* calling convention
* structure padding
* type alignments
* which heap memory is returned to in ``delete`` or ``free``.
* the types and return values of functions
* structure layout

As libraries evolve, fix bugs and add features, some of these things are likely
to change, especially if one does not pay attention.

If the ABI of a function changes, but its name doesn't change, a client built
against an older version of the library will call it incorrectly, and subtle
memory errors ensues.

Note that a function may remain API compatible, but still change its ABI. For
example, adding a new private member to a class but otherwise leave all member
functions unchanged will make it possible for clients to use it the same way as
before, at the API level. However, every member function now takes a slightly
different version of the underlying class member layout.

The solution
------------

The solution to the ABI compatibility problem is to change the name of symbols
as their ABI changes. We want to change the names while maintaining the same
name at the API level. That is, we want clients to refer to the names just as they
always have.

inline namespaces were introduced in C++11 which lets us do this. For example:

.. code:: c++

	namespace test {
	int test_function() { return 0; }
	inline namespace v1 {
		int test_function() { return 1; }
	}
	}

This defines two functions, one with the old name and one with the new name. Old
clients were built against the old header:

.. code:: c++

	namespace test {
	int test_function() { return 0; }
	inline namespace v1 {
		int test_function() { return 1; }
	}
	}

And when linked will refer to the old name (``test::test_function``). Whereas
new clients will be built against the new header:

.. code:: c++

	namespace test {
	inline namespace v1 {
		int test_function();
	}
	}

And will refer to the new name when linked. The new name being ``test::v1::test_function``.

Existing symbols are declared directly in the ``test`` namespace. The assumption
is that this is where they are already declared, and they are left alone to
minimize the impact on clients.

When a new version of the function is introduced, it is placed in the *inline*,
versioned, namespace. At that point, the old declaration is removed from the
header, but remains in the .cpp file. This ensures that the symbol will still be
present under the old name, with the old ABI.

This provides backwards ABI compatibility while making future clients transparently
use the new version when built.

The caveat
----------

The caveat is that clients cannot forward declare any names from the library
namespace. There does not seem to be any way around this. Introducing an
unversioned name will necessarily break the versioning of the inline namespaces.

The upside is that forward declaring will be a link error, avoiding subtle ABI
issues.

On a theoretical level, clients forward declaring names of shared libraries will
probably not be correct in general either way. Since some platforms and ABIs
require attributes on symbols that are imported from a shared library.

Specifically, on windows, the name mangling of functions depend on whether it
has been marked with ``__declspec(dllimport)`` or not.

