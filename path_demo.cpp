#include <iostream>
#include "filesystem/path.h"
#include "filesystem/resolver.h"

using namespace std;
using namespace filesystem;

// TAP-compliant test
int failed_nr = 0;
int test_nr = 0;
#define DIAG(out) cout << "# " << out << endl
#define _OK(cond, has_diag, diag) \
    if (cond) \
        cout << "ok " << (++test_nr) << endl; \
    else { \
        failed_nr++; \
        cout << "not ok " << (++test_nr) << endl; \
        DIAG("  Failed test at " << __FILE__ << " line " << __LINE__ << "."); \
        if (has_diag) DIAG(diag); \
    }

#define OK(cond)        _OK((cond),     0, "")
#define NOK(cond)       _OK(!(cond),    0, "")
#define IS(a, b)        _OK((a) == (b), 1, (a))
#define ISNT(a, b)      _OK((a) != (b), 1, (a))

#define DONE_TESTING() \
    cout << "1.." << test_nr << endl; \
    if (failed_nr == 0) { \
        return 0; \
    } else { \
        DIAG("Looks like you failed " << failed_nr << " test" << \
             (failed_nr > 1 ? "s" : "") << " of " << test_nr << "."); \
        return 1; \
    }

// Platform specifics
#if !defined(WIN32)
#define ROOT    "/"
#define SEP     "/"
#else
#define ROOT    "C:\\"
#define SEP     "\\"
#endif

int main(int argc, char **argv) {
    path path1(ROOT "dir 1" SEP "dir 2" SEP);
    path path2("dir 3");
    path p;

    // string
    NOK(path1.exists());
    IS(path1, ROOT "dir 1" SEP "dir 2");
    IS(path2, "dir 3");

    // concatenate
    IS(path1/path2, ROOT "dir 1" SEP "dir 2" SEP "dir 3");

    // parent
    IS((path1/path2).parent_path(), ROOT "dir 1" SEP "dir 2");
    IS((path1/path2).parent_path().parent_path(), ROOT "dir 1");
    IS((path1/path2).parent_path().parent_path().parent_path(), ROOT);
    IS((path1/path2).parent_path().parent_path().parent_path().parent_path(), ROOT);
    IS((path1/path2).parent_path().parent_path().parent_path().parent_path().parent_path(), ROOT);

    // is_absolute
    OK(path1.is_absolute());
    NOK(path2.is_absolute());

    // test conditions for str(), is_absolute(), parent_path(), filename() from boost::filesystem
    p = path("");
    IS(p.str(), "");
    NOK(p.is_absolute());
    IS(p.parent_path(), "");
    IS(p.filename(), "");

    p = path(".");
    IS(p.str(), ".");
    NOK(p.is_absolute());
    IS(p.parent_path(), "");
    IS(p.filename(), ".");

    p = path("..");
    IS(p.str(), "..");
    NOK(p.is_absolute());
    IS(p.parent_path(), "");
    IS(p.filename(), "..");

    p = path("foo");
    IS(p.str(), "foo");
    NOK(p.is_absolute());
    IS(p.parent_path(), "");
    IS(p.filename(), "foo");

    p = path("/");
    IS(p.str(), SEP);
#if defined(WIN32)
    NOK(p.is_absolute());
#else
    OK(p.is_absolute());
#endif
    IS(p.parent_path(), "");
    IS(p.filename(), SEP);

    p = path("/foo");
    IS(p.str(), SEP "foo");
#if defined(WIN32)
    NOK(p.is_absolute());
#else
    OK(p.is_absolute());
#endif
    IS(p.parent_path(), SEP);
    IS(p.filename(), "foo");

    p = path("foo/");
    IS(p.str(), "foo" SEP);
    NOK(p.is_absolute());
    IS(p.parent_path(), "foo");
    IS(p.filename(), ".");

    p = path("/foo/");
    IS(p.str(), SEP "foo" SEP);
#if defined(WIN32)
    NOK(p.is_absolute());
#else
    OK(p.is_absolute());
#endif
    IS(p.parent_path(), SEP "foo");
    IS(p.filename(), ".");

    p = path("foo/bar");
    IS(p.str(), "foo" SEP "bar");
    NOK(p.is_absolute());
    IS(p.parent_path(), "foo");
    IS(p.filename(), "bar");

    p = path("/foo/bar");
    IS(p.str(), SEP "foo" SEP "bar");
#if defined(WIN32)
    NOK(p.is_absolute());
#else
    OK(p.is_absolute());
#endif
    IS(p.parent_path(), SEP "foo");
    IS(p.filename(), "bar");

    p = path("/.");
    IS(p.str(), SEP ".");
#if defined(WIN32)
    NOK(p.is_absolute());
#else
    OK(p.is_absolute());
#endif
    IS(p.parent_path(), SEP);
    IS(p.filename(), ".");

    p = path("./");
    IS(p.str(), "." SEP);
    NOK(p.is_absolute());
    IS(p.parent_path(), ".");
    IS(p.filename(), ".");

    p = path("/..");
    IS(p.str(), SEP "..");
#if defined(WIN32)
    NOK(p.is_absolute());
#else
    OK(p.is_absolute());
#endif
    IS(p.parent_path(), SEP);
    IS(p.filename(), "..");

    p = path("../");
    IS(p.str(), ".." SEP);
    NOK(p.is_absolute());
    IS(p.parent_path(), "..");
    IS(p.filename(), ".");

    p = path("foo/.");
    IS(p.str(), "foo" SEP ".");
    NOK(p.is_absolute());
    IS(p.parent_path(), "foo");
    IS(p.filename(), ".");

    p = path("foo/..");
    IS(p.str(), "foo" SEP "..");
    NOK(p.is_absolute());
    IS(p.parent_path(), "foo");
    IS(p.filename(), "..");

    p = path("foo/./");
    IS(p.str(), "foo" SEP "." SEP);
    NOK(p.is_absolute());
    IS(p.parent_path(), "foo" SEP ".");
    IS(p.filename(), ".");

    p = path("foo/./bar");
    IS(p.str(), "foo" SEP "." SEP "bar");
    NOK(p.is_absolute());
    IS(p.parent_path(), "foo" SEP ".");
    IS(p.filename(), "bar");

    p = path("foo/..");
    IS(p.str(), "foo" SEP "..");
    NOK(p.is_absolute());
    IS(p.parent_path(), "foo");
    IS(p.filename(), "..");

    p = path("foo/../");
    IS(p.str(), "foo" SEP ".." SEP);
    NOK(p.is_absolute());
    IS(p.parent_path(), "foo" SEP "..");
    IS(p.filename(), ".");

    p = path("foo/../bar");
    IS(p.str(), "foo" SEP ".." SEP "bar");
    NOK(p.is_absolute());
    IS(p.parent_path(), "foo" SEP "..");
    IS(p.filename(), "bar");

#if defined(WIN32)
    p = path("\\foo");
    IS(p.str(), SEP "foo");
    NOK(p.is_absolute());
    IS(p.parent_path(), SEP);
    IS(p.filename(), "foo");

    p = path("c:");
    IS(p.str(), "c:");
    NOK(p.is_absolute());
    IS(p.parent_path(), "");
    IS(p.filename(), "c:");

    p = path("c:/");
    IS(p.str(), "c:" SEP);
    OK(p.is_absolute());
    IS(p.parent_path(), "c:");
    IS(p.filename(), SEP);

    p = path("c:foo");
    IS(p.str(), "c:foo");
    NOK(p.is_absolute());
    IS(p.parent_path(), "c:");
    IS(p.filename(), "foo");

    p = path("c:/foo");
    IS(p.str(), "c:" SEP "foo");
    OK(p.is_absolute());
    IS(p.parent_path(), "c:" SEP);
    IS(p.filename(), "foo");

    p = path("c:foo/");
    IS(p.str(), "c:foo" SEP);
    NOK(p.is_absolute());
    IS(p.parent_path(), "c:foo");
    IS(p.filename(), ".");

    p = path("c:/foo/");
    IS(p.str(), "c:" SEP "foo" SEP);
    OK(p.is_absolute());
    IS(p.parent_path(), "c:" SEP "foo");
    IS(p.filename(), ".");

    p = path("c:/foo/bar");
    IS(p.str(), "c:" SEP "foo" SEP "bar");
    OK(p.is_absolute());
    IS(p.parent_path(), "c:" SEP "foo");
    IS(p.filename(), "bar");

    p = path("c:\\");
    IS(p.str(), "c:" SEP);
    OK(p.is_absolute());
    IS(p.parent_path(), "c:");
    IS(p.filename(), SEP);

    p = path("c:\\foo");
    IS(p.str(), "c:" SEP "foo");
    OK(p.is_absolute());
    IS(p.parent_path(), "c:" SEP);
    IS(p.filename(), "foo");

    p = path("c:foo\\");
    IS(p.str(), "c:foo" SEP);
    NOK(p.is_absolute());
    IS(p.parent_path(), "c:foo");
    IS(p.filename(), ".");

    p = path("c:\\foo\\");
    IS(p.str(), "c:" SEP "foo" SEP);
    OK(p.is_absolute());
    IS(p.parent_path(), "c:" SEP "foo");
    IS(p.filename(), ".");

    p = path("c:\\foo/");
    IS(p.str(), "c:" SEP "foo" SEP);
    OK(p.is_absolute());
    IS(p.parent_path(), "c:" SEP "foo");
    IS(p.filename(), ".");

    p = path("c:/foo\\bar");
    IS(p.str(), "c:" SEP "foo" SEP "bar");
    OK(p.is_absolute());
    IS(p.parent_path(), "c:" SEP "foo");
    IS(p.filename(), "bar");
#endif

    // operator==()
    OK( path("some/path.ext") == path("some/path.ext"));
    NOK(path("some/path.ext") == path("other/path.ext"));

    // operator!=()
    OK( path("some/path.ext") != path("other/path.ext"));
    NOK(path("some/path.ext") != path("some/path.ext"));

    // exists, is_file, is_directory
    NOK(path("nonexistant").exists());
    NOK(path("nonexistant").is_file());
    NOK(path("nonexistant").is_directory());

    OK( path("../filesystem").exists());
    NOK(path("../filesystem").is_file());
    OK( path("../filesystem").is_directory());

    OK( path("filesystem").exists());
    NOK(path("filesystem").is_file());
    OK( path("filesystem").is_directory());

    OK( path("filesystem/path.h").exists());
    OK( path("filesystem/path.h").is_file());
    NOK(path("filesystem/path.h").is_directory());

    // filename, extension
    p = path(".");
    IS(p.filename(), ".");
    IS(p.extension(), "");

    p = path("..");
    IS(p.filename(), "..");
    IS(p.extension(), "");

    p = path(".exrc");
    IS(p.filename(), ".exrc");
    IS(p.extension(), "");

    p = path("yy.tab.h");
    IS(p.filename(), "yy.tab.h");
    IS(p.extension(), ".h");

    p = path("nonexistant");
    IS(p.filename(), "nonexistant");
    IS(p.extension(), "");

    p = path("filesystem/path.h");
    IS(p.filename(), "path.h");
    IS(p.extension(), ".h");

    p = path("../filesystem");
    IS(p.filename(), "filesystem");
    IS(p.extension(), "");

    // make_absolute
    p = path("filesystem/path.h");
    NOK(p.is_absolute());
    OK(p.make_absolute().is_absolute());
    DIAG(p.make_absolute());

    p = path("../filesystem");
    NOK(p.is_absolute());
    OK(p.make_absolute().is_absolute());
    DIAG(p.make_absolute());

    // resolve
    IS(resolver().resolve("filesystem/path.h"), path("filesystem/path.h").make_absolute());
    IS(resolver().resolve("nonexistant"), "nonexistant");

    DONE_TESTING();
}
